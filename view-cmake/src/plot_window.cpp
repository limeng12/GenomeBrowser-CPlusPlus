#include "plot_window.h"
#include <wx/dcclient.h>
#include "faidx.h"
#include <wx/dcbuffer.h>
#include <wx/dcmemory.h>
#include "MRightWin.h"

#ifdef __WXMSW__
#include "windows.h"
#undef GetClassInfo
#endif


PlotWindow::PlotWindow(wxWindow *parent,wxWindowID id,const wxPoint &pos,const wxSize &winsize, long style,wxString bam,DataFactory* tdata,int pageIndex)
:wxScrolledCanvas(parent,id,pos,winsize,style,wxPanelNameStr),mdata(tdata),page(pageIndex){

    if(bam==wxEmptyString){
        wxLogError("bamfile can't be empty");
        return;
    }
    change=0;
    bamdata=new BamData();
    bamdata->bamFileName=bam;
    bamdata->init();

    //localCurChr=std::string(toCString(bamdata->header.sequenceInfos[0].i1));

    oldPos=wxPoint(0,0);
    //m_timer.Start(100000000000000);

    //RefreshTable();

    if(localCurChr.empty()==0){

        boost::unordered_map<string,int>::iterator ite;

        ite=mdata->chrAndLength.begin();

        localCurChr=ite->first;
        localCurChrPos=ite->second/2;
    }

    viewMenu=new wxMenu(_T("View track set"));
    viewCheckVector=new std::vector<bool>();

    message=new MessagePopupWin(this->GetGrandParent());
    //message=new MessagePopupWin();

    //SetCursor(wxCURSOR_BLANK);
    //SetCursor(wxCURSOR_SIZENS);
    //SetCursor(*wxCROSS_CURSOR);
    //SetCursor(wxCursor_);
    SetBackgroundColour(*wxWHITE);
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
    SetScrollbars(0,20,0,50);
    SetVirtualSize(winsize.x,3000);
    cavansSize=wxSize(winsize.x,3000);

    mplotDeatil=new PlotDetail(mdata,bamdata,cavansSize,viewCheckVector);
    mplotNormal=new PlotNormal(mdata,bamdata,cavansSize,viewCheckVector);
    mplotProfile=new PlotProfile(mdata,bamdata,cavansSize,viewCheckVector);
    for(int i=0;i<16;++i){
        if(i<=5){
            zoomVector.push_back(std::make_pair(mplotProfile,pow(0.5,5-i)));

        }
        if(i>5&&i<15)
            zoomVector.push_back(std::make_pair(mplotNormal,(i-5)));
        if(i==15)
            zoomVector.push_back(std::make_pair(mplotDeatil,(i-5)));

    }
    curNucliedSize=15;
    //mActive=zoomVector[curNucliedSize].first;
    mActive=zoomVector[15].first;

    refreshPopupMenu();

    //myHelp=new wxSimpleHelpProvider();
    //myHelp->AddHelp(this,_T("afafff"));
    //SetHelpText(_T("dafafa"));
    t=new wxTimer(this);
    initConnection();


}

PlotWindow::~PlotWindow(){
    disConnection();
    delete mplotDeatil;
    delete mplotNormal;
    delete mplotProfile;

    delete t;
    delete viewMenu;
    delete bamdata;
    delete viewCheckVector;
}

wxString PlotWindow::getBamFileName(){
    return bamdata->bamFileName;
}

void PlotWindow::plot(wxDC& dc){
    dc.Clear();//wxSize(10,10)
    //这里要选择20，因为画时，字体会被缩小一倍

    wxSize s= GetVirtualSize();
    mActive->plotMain(dc,localCurChr,localCurChrPos,0,s.x,0,s.y);
}

void PlotWindow::onZoom(bool zoom){
    if(zoom==true){//zoom in
            curNucliedSize++;
            if(curNucliedSize>=zoomVector.size())
            curNucliedSize=zoomVector.size()-1;
            mActive=zoomVector.at(curNucliedSize).first;
            mActive->SetNucliedSize(zoomVector.at(curNucliedSize).second);

            Refresh();
    }else{//zoom out
        curNucliedSize--;
        if(curNucliedSize<0)
            curNucliedSize=0;

        mActive=zoomVector.at(curNucliedSize).first;
        mActive->SetNucliedSize(zoomVector.at(curNucliedSize).second);
        Refresh();

    }

}

void PlotWindow::OnKeyView(bool right){

    //bool flag=IsFocusable ();

    if(right==false){
        //plot(localCurChr,localCurChrPos-1);
        localCurChrPos=localCurChrPos-10.0/mActive->GetNucliedSize();

        localCurChrPos=localCurChrPos<mActive->cavansNucliced/2?mActive->cavansNucliced/2:localCurChrPos;
        localCurChrPos=localCurChrPos<mdata->chrAndLength[localCurChr]-mActive->cavansNucliced/2?localCurChrPos:mdata->chrAndLength[localCurChr]-mActive->cavansNucliced/2;

        Refresh();
    }
    else if(right==true){

        localCurChrPos=localCurChrPos+10.0/mActive->GetNucliedSize();

        localCurChrPos=localCurChrPos<mActive->cavansNucliced/2?mActive->cavansNucliced/2:localCurChrPos;
        localCurChrPos=localCurChrPos<mdata->chrAndLength[localCurChr]-mActive->cavansNucliced/2?localCurChrPos:mdata->chrAndLength[localCurChr]-mActive->cavansNucliced/2;

        Refresh();
    }

}
//this is function is unused
void PlotWindow::OnKeyView(wxKeyEvent& event){

    //bool flag=IsFocusable ();
    if(event.GetKeyCode()==WXK_LEFT)
        OnKeyView(false);
    else if(event.GetKeyCode()==WXK_RIGHT)
        OnKeyView(true);
    else if(event.GetKeyCode()==WXK_UP)
        onZoom(true);//fang da zoom in
    else if(event.GetKeyCode()==WXK_DOWN)
        onZoom(false);//
    else
    event.Skip();

}

void PlotWindow::OnMouseView(wxMouseEvent& event){
    if(t->IsRunning())
        t->Stop();

    nIdleTimes=0;
    if(message->isPopup())
        message->Dismiss();

    if(event.m_leftDown){

        double tchange=oldPos.x-event.GetX();

        //oldPos.x=event.GetX();
        if(fabs(change+tchange)>20){

            double t=mActive->GetNucliedSize();

            localCurChrPos=localCurChrPos+(change+tchange)/t;
            localCurChrPos=localCurChrPos<mActive->cavansNucliced/2?mActive->cavansNucliced/2:localCurChrPos;
            localCurChrPos=localCurChrPos<mdata->chrAndLength[localCurChr]-mActive->cavansNucliced/2?localCurChrPos:mdata->chrAndLength[localCurChr]-mActive->cavansNucliced/2;

            Refresh();
            oldPos.x=event.GetX();

            //change=(int)(change+tchange)%t;// 补偿
        }
    }
    else{
        wxClientDC dc2(this);
        wxBitmap tmpbitmap;
     	wxBufferedDC dc(&dc2,tmpbitmap);
        dc.Clear();
        wxSize tmp=dc.GetSize();

        if(saveBitmap.IsOk())
            dc.DrawBitmap(saveBitmap,0,0);
        //saveBitmap.SaveFile(_T("/home/lm/ajflajg.bmp"),wxBITMAP_TYPE_BMP);
        dc.DrawLine(event.GetX(),0,event.GetX(),tmp.y);
        dc.DrawLine(0,event.GetY(),tmp.x,event.GetY());
        //dc2.DrawBitmap(saveBitmap,0,0);

    }


}

void PlotWindow::OnMouseLeftDown(wxMouseEvent& event){
    if(t->IsRunning())
        t->Stop();

    //f(!this->HasCapture())
    //    this->CaptureMouse();

    if(!this->HasFocus())
        this->SetFocus();

    nIdleTimes=0;

    oldPos=event.GetPosition();

}

void PlotWindow::onMouseRigthDown(wxMouseEvent& event){
    flag=false;
    //if(this->HasCapture())
    //    this->ReleaseMouse();

    PopupMenu(viewMenu);
    for(size_t i=0;i<viewCheckVector->size();++i)
        viewCheckVector->at(i)=viewMenu->IsChecked(i);

    //viewCheckVector
    Refresh();

}

void PlotWindow::OnPaint(wxPaintEvent &event){
    //if(mdata->IsFileChanged()){
    //    refreshPopupMenu();
        //RefreshTable();
    //    mdata->setFileUnchanged();
    //}
    //this {} must be exist ensured wxBuffered funciton
    //if()

 	{
        wxBufferedPaintDC dc(this);
        DoPrepareDC(dc);
        this->plot(dc);
        saveBitmap=dc.GetAsBitmap();
 	}

    wxPaintDC dc2(this);

    wxPoint posScreen=wxGetMousePosition();
    wxPoint posDia=ScreenToClient(posScreen);

    cavansSize=GetVirtualSize();

    dc2.DrawLine(posDia.x,0,posDia.x,cavansSize.y);
    dc2.DrawLine(0,posDia.y,cavansSize.x,posDia.y);

}

void PlotWindow::refreshPopupMenu(){
    //viewMenu->Destroy();///////////////////////////////////////fixed memememem
    viewCheckVector->clear();
    viewMenu->~wxMenu();
    viewMenu=new wxMenu(_T("View track set"));

    unsigned int i=0;
    if(mdata->getFastaFile()!=wxEmptyString){
        viewMenu->AppendCheckItem(i,_T("chr Fasta File"),mdata->getFastaFile());
        viewCheckVector->push_back(true);
        viewMenu->Check(i++,true);

    }

    //gffCount=gffFiles.GetCount();
    for(size_t j=0;j<mdata->getGtfCount();++j,++i){
        viewMenu->AppendCheckItem(i,wxFileName(mdata->getGtfFile(j)).GetName(),mdata->getGtfFile(j));
        viewCheckVector->push_back(true);
        viewMenu->Check(i,true);

    }

    for(size_t j=0;j<mdata->getGffCount();++j,++i){
        viewMenu->AppendCheckItem(i,wxFileName(mdata->getGffFile(j)).GetName(),mdata->getGffFile(j));
        viewCheckVector->push_back(true);
        viewMenu->Check(i,true);

    }

    for(size_t j=0;j<mdata->getBedCount();++j,++i){
        viewMenu->AppendCheckItem(i,wxFileName(mdata->getBedFile(j)).GetName(),mdata->getBedFile(j));
        viewCheckVector->push_back(true);
        viewMenu->Check(i,true);

    }

    //vcfCount=vcfFiles.GetCount();

    for(size_t j=0;j<mdata->getVcfCount();++j,++i){
        viewMenu->AppendCheckItem(i,wxFileName(mdata->getVcfFile(j)).GetName(),mdata->getVcfFile(j));
        viewCheckVector->push_back(true);

        viewMenu->Check(i,true);
    }

    viewMenu->AppendCheckItem(i,_T("bam"),bamdata->bamFileName);
    viewCheckVector->push_back(true);

    viewMenu->Check(i++,true);
}

void PlotWindow::OnMouseDLeftClick(wxMouseEvent& event){

//OnViewDetail(event.GetX(),event.GetY());

}

void PlotWindow::OnViewDetail(int x,int y){

    std::string str=mplotDeatil->posMessageTable[(int)(y/30)][(int)(x/30)];///fixed mem !!!!!!!!

    if(!str.empty()){
        //message->Position(wxPoint(x+50,y-40),wxSize(200,200));
#ifdef __WXGTK__
        message->Position(ClientToScreen(wxPoint(x,y)),wxDefaultSize);
#else
        message->Position(wxPoint(x,y),wxDefaultSize);

#endif
        message->m_mouseText->SetLabel(wxString(str));
        message->Fit();
        message->Popup();
        //if(!this->HasCapture())
         //   this->CaptureMouse();
        return;
    }

    if(y>mplotDeatil->snpTableLine&&y<mplotDeatil->snpTableLine+20){
        wxString showStr;
        int fre;
        int index=0*(mplotDeatil->cavansNucliced)+x/(mplotDeatil->GetNucliedSize());
        fre=mplotDeatil->frequencyTable.at(index);
        showStr<<"Frequency A="<<fre<<"\n";

        index=1*(mplotDeatil->cavansNucliced)+x/(mplotDeatil->GetNucliedSize());
        fre=mplotDeatil->frequencyTable.at(index);
        showStr<<"Frequency C="<<fre<<"\n";

        index=2*(mplotDeatil->cavansNucliced)+x/(mplotDeatil->GetNucliedSize());
        fre=mplotDeatil->frequencyTable.at(index);
        showStr<<"Frequency G="<<fre<<"\n";

        index=3*(mplotDeatil->cavansNucliced)+x/(mplotDeatil->GetNucliedSize());
        fre=mplotDeatil->frequencyTable.at(index);
        showStr<<"Frequency T="<<fre<<"\n";

        //myHelp->ShowHelpAtPoint(this,wxPoint(0,20),wxHelpEvent::Origin_Unknown);
        //message->Position(wxPoint(x+50,y-40),wxSize(200,200));
#ifdef __WXGTK__
        message->Position(ClientToScreen(wxPoint(x+10,y)),wxDefaultSize);
#else
        message->Position(wxPoint(x+10,y),wxDefaultSize);

#endif
        message->m_mouseText->SetLabel(wxString(showStr));
        message->Fit();
        message->Popup();
       // if(!this->HasCapture())
       //     this->CaptureMouse();

        return;
    }

    int piexline=y-mplotDeatil->bamPipeline;
    int line=piexline/20;
    int col=(x+5)/10;
    if(mplotDeatil->bamMessageTable.find(line)!=mplotDeatil->bamMessageTable.end()){
        std::vector<PlotDetail::bamRect> tbamr=mplotDeatil->bamMessageTable[line];
        for(int i=0;i<tbamr.size();++i){
            int left=tbamr[i].leftRect;
            int right=tbamr[i].rightRect;

            if(tbamr[i].leftRect<col&&col<tbamr[i].rightRect){
                wxString msgStr=mplotDeatil->bamSearch(localCurChr,tbamr[i].leftPos,tbamr[i].sqname);
                //wxString msgStr=_T("afaf");
                //message->Position(wxPoint(x+50,y-40),wxSize(200,200));
#ifdef __WXGTK__
                //message->Position(ClientToScreen(wxPoint(x,y)),wxDefaultSize);
                message->Position(wxDefaultPosition,wxDefaultSize);
#else
                //message->Position(wxPoint(x,y),wxDefaultSize);
                message->Position(wxDefaultPosition,wxDefaultSize);
#endif
                message->m_mouseText->SetLabel(wxString(msgStr));
                message->Fit();
                message->Popup();
                //if(!this->HasCapture())
                //    this->CaptureMouse();
                break;
            }

        }
    }

}

void PlotWindow::disConnection(){
            Disconnect(wxEVT_LEFT_DCLICK,wxMouseEventHandler(PlotWindow::OnMouseDLeftClick));
            Disconnect(wxEVT_LEFT_DOWN,wxMouseEventHandler(PlotWindow::OnMouseLeftDown));
            Disconnect(wxEVT_MOTION,wxMouseEventHandler(PlotWindow::OnMouseView));
            Disconnect( wxEVT_KEY_DOWN,wxKeyEventHandler(PlotWindow::OnKeyView));//这个不好用，需要修改
            Disconnect(wxEVT_RIGHT_DOWN,wxMouseEventHandler(PlotWindow::onMouseRigthDown));
            //key event 必须由主窗口链接？？？
            Disconnect(wxEVT_IDLE,wxIdleEventHandler(PlotWindow::OnIdle));
            Disconnect(wxEVT_SIZE,wxSizeEventHandler(PlotWindow::OnSize));
            Disconnect(wxEVT_TIMER,wxTimerEventHandler(PlotWindow::OnTimer));

            Disconnect(wxEVT_PAINT,wxPaintEventHandler(PlotWindow::OnPaint));
            Disconnect(wxEVT_MOUSEWHEEL,wxMouseEventHandler(PlotWindow::OnScroll));
            Disconnect(wxEVT_LEAVE_WINDOW,wxMouseEventHandler(PlotWindow::OnLeave));
            Disconnect(wxEVT_MOUSE_CAPTURE_LOST,wxMouseEventHandler(PlotWindow::OnLoseMouse));
}

void PlotWindow::initConnection(){

            Connect(wxEVT_LEFT_DCLICK,wxMouseEventHandler(PlotWindow::OnMouseDLeftClick));
            Connect(wxEVT_LEFT_DOWN,wxMouseEventHandler(PlotWindow::OnMouseLeftDown));
            Connect(wxEVT_MOTION,wxMouseEventHandler(PlotWindow::OnMouseView));
            Connect( wxEVT_KEY_DOWN,wxKeyEventHandler(PlotWindow::OnKeyView));//这个不好用，需要修改
            Connect(wxEVT_RIGHT_DOWN,wxMouseEventHandler(PlotWindow::onMouseRigthDown));
            //key event 必须由主窗口链接？？？
            Connect(wxEVT_IDLE,wxIdleEventHandler(PlotWindow::OnIdle));
            Connect(wxEVT_SIZE,wxSizeEventHandler(PlotWindow::OnSize));
            Connect(wxEVT_TIMER,wxTimerEventHandler(PlotWindow::OnTimer));

            Connect(wxEVT_PAINT,wxPaintEventHandler(PlotWindow::OnPaint));
            Connect(wxEVT_MOUSEWHEEL,wxMouseEventHandler(PlotWindow::OnScroll));
            Connect(wxEVT_LEAVE_WINDOW,wxMouseEventHandler(PlotWindow::OnLeave));
            Connect(wxEVT_MOUSE_CAPTURE_LOST,wxMouseEventHandler(PlotWindow::OnLoseMouse));
}

void PlotWindow::OnSize(wxSizeEvent& event){

    wxSize s= GetVirtualSize();
    mplotDeatil->canvasSizeChange(s);
    mplotNormal->canvasSizeChange(s);
    mplotProfile->canvasSizeChange(s);

}

void PlotWindow::OnScroll(wxMouseEvent& event){
    if(event.ShiftDown()){
        //int x=event.GetWheelDelta();
        //event.GetWheelAxis();
        int x=event.GetWheelRotation();
        if(fabs(x)>10){
        //plot(localCurChr,localCurChrPos+(int)(change/10));
        localCurChrPos=localCurChrPos+(int)(x/(mActive->GetNucliedSize()));//mplotDeatil

        localCurChrPos=localCurChrPos<mActive->cavansNucliced/2?mActive->cavansNucliced/2:localCurChrPos;
        localCurChrPos=localCurChrPos<mdata->chrAndLength[localCurChr]-mActive->cavansNucliced/2?localCurChrPos:mdata->chrAndLength[localCurChr]-mActive->cavansNucliced/2;


            //if(localCurChrPos>=cavansSize.x/(mActive->GetNucliedSize())/2&&localCurChrPos<mdata->chrAndLength[localCurChr]-cavansSize.x/(mActive->GetNucliedSize())/2)
        Refresh();
        }
    }
    else if(event.ControlDown()){

        int x=event.GetWheelRotation();
        if(x>0){
            onZoom(true);
        }else{
            onZoom(false);
        }

    }
    else{
        event.Skip();
    }

}

void PlotWindow::OnIdle(wxIdleEvent& event){

    //if(this->message->HasCapture())
    //    this->CaptureMouse();

    if(mActive->GetNucliedSize()!=10)
        return;

    //if(!this->HasCapture()){
    //    event.Skip();
    //    return;

   // }
    if(!this->HasFocus()){
        event.Skip();
        return;
    }
    if(!t->IsRunning()){
        t->Start(100);
        nIdleTimes=0;
    }
    else
        event.Skip();


}

void PlotWindow::OnTimer(wxTimerEvent& event){
    nIdleTimes++;
    int x=event.GetInterval()*nIdleTimes;
    if(x>1500){
        //wxMessageDialog *msg=new wxMessageDialog(this,_T(">2s"),_T("test"));
        //msg->ShowModal();
        if(message->isPopup())
            return;
        wxPoint m=wxGetMousePosition();
        wxPoint virtualStart=this->GetViewStart();
        //if(virtualStart.y>0)
       // 	wxMessageBox(wxString::Format(_T("%d"),virtualStart.y));

        m=this->ScreenToClient(m);

		m.y=m.y+virtualStart.y*20;//for virtual size

        OnViewDetail(m.x,m.y);
        Refresh();
        nIdleTimes=0;
        t->Stop();
    }
    //t->Stop();
}

void PlotWindow::OnLeave(wxMouseEvent& event){
    //if(this->HasCapture()&&flag==true)
    //    this->ReleaseMouse();
    if(t->IsRunning())
        t->Stop();

    flag=true;
}

void PlotWindow::OnLoseMouse(wxMouseEvent& event){

    //wxMessageBox(_T("lose mouse"));

}

wxString PlotWindow::GetHelpTextAtPoint(const wxPoint &point, wxHelpEvent::Origin origin) const {

    wxMessageBox(_T("afasfd"));
    return wxString(_T("afaf"));
}

