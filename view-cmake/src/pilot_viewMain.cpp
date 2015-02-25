/***************************************************************
 * Name:      pilot_viewMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    limeng (s6228483@yahoo.com.cn)
 * Created:   2013-05-05
 * Copyright: limeng ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
//#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

// for
#ifdef _UNICODE
#undef _UNICODE
#endif

#include "pilot_viewMain.h"
#include <fstream>
#include <wx/choice.h>
#include <wx/progdlg.h>

    #include "art/fileopen.xpm"
    #include "art/filesave.xpm"
    //#include "art/htmfoldr.xpm"
    //#include "art/folder.xpm"
    //#include "art/htmpage.xpm"
    //#include "art/home.xpm"
    #include "art/pilot_frame_.xpm"
    #include "art/hongqi2.xpm"
    //#include "art/FA.xpm"
    //#include "art/BED.xpm"
    //#include "art/GFF.xpm"
    //#include "art/BAM.xpm"
    //#include "art/mHome_folder.xpm"
   //#include "art/VCF.xpm"

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

BEGIN_EVENT_TABLE(pilot_viewFrame, wxFrame)
    EVT_TREE_ITEM_ACTIVATED(wxID_LEFT_TREE_CTRL,pilot_viewFrame::OnViewBam)

    EVT_CLOSE(pilot_viewFrame::OnClose)
    EVT_MENU(idMenuQuit, pilot_viewFrame::OnQuit)
    EVT_MENU(idMenuAbout, pilot_viewFrame::OnAbout)
    EVT_MENU(ID_OPEN,pilot_viewFrame::OnOpen)
    EVT_MENU(ID_SAVE,pilot_viewFrame::OnSave)

    EVT_MENU(ID_FILTER,pilot_viewFrame::OnFilter)
    EVT_BOOKCTRL_PAGE_CHANGED(wxID_NOTE_CHANGED,pilot_viewFrame::OnBookChange)
    EVT_SIZE(pilot_viewFrame::OnSize)
    //EVT_MENU(idNaviGateMenu,pilot_viewFrame::OnNaviGate)
    EVT_TEXT_ENTER(wxID_POSITION,pilot_viewFrame::OnNaviGate)
    EVT_TEXT_ENTER(wxID_SNPPOSITION,pilot_viewFrame::OnPlotSnp)

    EVT_IDLE(pilot_viewFrame::OnIdle)
//    EVT_PAINT(pilot_viewFrame::OnPaint)

    EVT_MENU(ID_DO_STATIC,pilot_viewFrame::OnDoStatistic)
    EVT_CHAR_HOOK(pilot_viewFrame::OnViewRightLeft)//这里EVT_CHAR不好用，所以改用EVT_CHAR_HOOK

END_EVENT_TABLE()

pilot_viewFrame::pilot_viewFrame(wxFrame *frame, const wxString& title)
    : wxFrame(frame, -1, title)
{
    finished=false;
    isBackUp=false;
    programOverFlag=true;
    // create a menu bar
    wxMenuBar* mbar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu(_T(""));
    fileMenu->Append(ID_OPEN,_("&Open\tCtrl-o"),_("Open an file"));

    fileMenu->Append(ID_SAVE,_("&Save\tCtrl-s"),_("Save a file or image"));
    mbar->Append(fileMenu, _("&File"));
    fileMenu->AppendSeparator();

    fileMenu->Append(idMenuQuit, _("&Quit\tAlt-F4"), _("Quit the application"));

    wxMenu* naviGateMenu=new wxMenu(_T(""));
    naviGateMenu->Append(ID_FILTER,_("&Filter\t"),_("go to a positon"));
    mbar->Append(naviGateMenu,_("&Filter"));

    wxMenu* statisticMenuBed=new wxMenu(_T(""));
    statisticMenuBed->Append(ID_DO_STATIC,_("&Do statistic\tCtrl-t"),_("do static on this bamFile"));
    mbar->Append(statisticMenuBed,_("&StaticBed"));

    wxMenu* helpMenu = new wxMenu(_T(""));
    helpMenu->Append(idMenuAbout, _("&About\tF1"), _("Hrbin Engineering University"));
    mbar->Append(helpMenu, _("&Help"));

    SetMenuBar(mbar);
    //menu end

    //toolbar begin
    mtoolBar=new wxToolBar(this,wxID_TOOL_BAR,wxDefaultPosition,wxDefaultSize,wxTB_HORIZONTAL);//|wxNO_BORDER);
    wxBitmap bmpOpen(fileopen_xpm);
    wxBitmap bmpSave(filesave_xpm);
    mtoolBar->AddTool(ID_OPEN,_T("Open"),bmpOpen,_T("open a file"), wxITEM_NORMAL);
    mtoolBar->AddTool(ID_SAVE,_T("Save"),bmpSave,_T("save an image"), wxITEM_NORMAL);
    mtoolBar->AddSeparator();

    //chrChoice=new wxChoice(mtoolBar,wxID_CHR_CHOICE,wxDefaultPosition,wxSize(100,30),choiceStrings);
    chromosomeChoice=new wxComboCtrl(mtoolBar, wxID_CHR_CHOICE,wxEmptyString,wxPoint(70,0),wxDefaultSize,wxLB_SINGLE|wxLB_NEEDED_SB,wxDefaultValidator, wxComboBoxNameStr);
    //chromosomeChoice->SetPopupMaxHeight(5);
    popupCtrl=new ChrosomeChoice();
    chromosomeChoice->SetPopupControl(popupCtrl);

    this->Connect(wxID_CHR_CHOICE,wxEVT_COMMAND_TEXT_UPDATED,wxCommandEventHandler(pilot_viewFrame::OnSelectChr));

    mtoolBar->AddControl(chromosomeChoice);

    mtoolBar->AddControl(new wxStaticText(mtoolBar,wxID_ANY,"Position"));
    mtoolBar->AddControl(mPosition=new wxTextCtrl(mtoolBar,wxID_POSITION,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB));

    mtoolBar->AddControl(new wxStaticText(mtoolBar,wxID_ANY,"SNP point position"));
	mtoolBar->AddControl(mSNPPos=new wxTextCtrl(mtoolBar,wxID_SNPPOSITION,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB));


    mtoolBar->Realize();
    SetToolBar(mtoolBar);
    //toolbar end

    splitter=new wxSplitterWindow(this,wxID_SPLITTER_WINDOW,wxPoint(0,0),GetClientSize(),wxSP_3D);
    //leftWindow design
    wxSplitterWindow* leftWindow=new wxSplitterWindow(splitter,wxLEFT_SCROLL_WINDOW,wxDefaultPosition,wxDefaultSize,wxSP_3D);
    //leftWindow->SetScrollbars(20,20,100,100);


    //wxBoxSizer *leftTopSizer=new wxBoxSizer(wxVERTICAL);
    fileCtrl=new MyTreeCtrl(leftWindow,wxID_LEFT_TREE_CTRL,wxPoint(0,0),wxSize(100,100),wxTR_DEFAULT_STYLE|wxTR_HAS_BUTTONS|wxTR_SINGLE);
    //this->Connect(ID_TREE,wxEVT_COMMAND_TREE_ITEM_ACTIVATED,wxTreeEventHandler(MyTreeCtrl::test));

    //wxImageList* imageList=new wxImageList();//;(16,16);
    //imageList->Add(wxIcon(folder_xpm));

    //imageList->Add(wxIcon(mHome_folder_xpm));
    //imageList->Add(wxIcon(htmpage_xpm));
    //imageList->Add(wxIcon(bam_xpm));
    //imageList->Add(wxIcon(gff_xpm));
    //imageList->Add(wxIcon(bed_xpm));
    //imageList->Add(wxIcon(vcf_xpm));
    //imageList->Add(wxIcon(fa_xpm));

    //imageList->Add(wxIcon(my_bam));

    //fileCtrl->AssignImageList(imageList);
    wxTreeItemId root =fileCtrl->AddRoot(_("Vew"),0,0,new MyTreeItemData(_("")));
    rootIdBam=fileCtrl->AppendItem(root,_("Bam files"),0,0,new MyTreeItemData(_("")));
    rootIdGtf=fileCtrl->AppendItem(root,_("GTF files"),0,0,new MyTreeItemData(_("")));
    rootIdGff=fileCtrl->AppendItem(root,_("GFF files"),0,0,new MyTreeItemData(_("")));
    rootIdBed=fileCtrl->AppendItem(root,_("Bed files"),0,0,new MyTreeItemData(_("")));
    rootIdVcf=fileCtrl->AppendItem(root,_("Vcf files"),0,0,new MyTreeItemData(_("")));
    rootIdFasta=fileCtrl->AppendItem(root,_("Fasta files"),0,0,new MyTreeItemData(_("")));


        //wxImageList* imageListNoteBook=new wxImageList();
        //imageListNoteBook->Add(wxIcon(home_xpm));
        //imageListNoteBook->Add(wxIcon(my_bam));

        noteStatistic=new wxNotebook(leftWindow,wxID_NOTE_STATIC,wxDefaultPosition,wxSize(200,400));
            //noteStatistic->SetImageList(imageListNoteBook);
            m_plots.push_back(new MMpwindow( noteStatistic, -1, wxPoint(0,0), wxSize(200,400), wxSUNKEN_BORDER |wxWANTS_CHARS|wxTAB_TRAVERSAL));
            m_plots.push_back(new MMpwindow( noteStatistic, -1, wxPoint(0,0), wxSize(200,400), wxSUNKEN_BORDER |wxWANTS_CHARS|wxTAB_TRAVERSAL));
            m_plots.push_back(new MMpwindow( noteStatistic, -1, wxPoint(0,0), wxSize(200,400), wxSUNKEN_BORDER |wxWANTS_CHARS|wxTAB_TRAVERSAL));
            m_plots.push_back(new MMpwindow( noteStatistic, -1, wxPoint(0,0), wxSize(200,400), wxSUNKEN_BORDER |wxWANTS_CHARS|wxTAB_TRAVERSAL));
           // m_plots.push_back(new MMpwindow( noteStatistic, -1, wxPoint(0,0), wxSize(200,400), wxSUNKEN_BORDER |wxWANTS_CHARS|wxTAB_TRAVERSAL));
            //m_plots.push_back(new MMpwindow( noteStatistic, -1, wxPoint(0,0), wxSize(200,400), wxSUNKEN_BORDER |wxWANTS_CHARS|wxTAB_TRAVERSAL));


            noteStatistic->AddPage(m_plots.at(0),wxT("length plot"),true,NO_IMAGE);
            noteStatistic->AddPage(m_plots.at(1),wxT("quality plot"),true,NO_IMAGE);
            noteStatistic->AddPage(m_plots.at(2),wxT("edit distance plot"),true,NO_IMAGE);
            //noteStatistic->AddPage(m_plots.at(4),wxT("coverage plot"),true,NO_IMAGE);
            noteStatistic->AddPage(m_plots.at(3),wxT("soft clip plot"),true,NO_IMAGE);
            //noteStatistic->AddPage(m_plots.at(5),wxT("region count plot"),true,NO_IMAGE);

    leftWindow->SetMinimumPaneSize(20);

    leftWindow->SplitHorizontally(fileCtrl, noteStatistic);

    //leftWindow design end

    //rightWindow design begin
    rightWindow=new MRightWin(splitter,wxRIGHT_SCROLL_WINDOW,wxDefaultPosition,wxDefaultSize,wxBORDER_NONE);

    wxBoxSizer *rightWindowSizer=new wxBoxSizer(wxVERTICAL);
        //wxImageList* imageListNoteBookPlot=new wxImageList();
        //imageListNoteBookPlot->Add(wxIcon(home_xpm));////////can't compile on windows

        plotViewNotebook=new wxNotebook(rightWindow,wxID_NOTE_CHANGED,wxDefaultPosition,wxDefaultSize,wxBORDER_NONE);
        //plotViewNotebook->SetImageList(imageListNoteBookPlot);

    //backup button begin
    //wxSize btnpos=rightWindow->GetSize();

    backupBtn=new wxButton(rightWindow,wxID_BACKUP,wxT("OK"),wxDefaultPosition,wxDefaultSize);
    this->Connect(wxID_BACKUP,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(pilot_viewFrame::readWorkspace));
    backupBtn->SetLabel(_T("Hit me to restore last WorkSpace"));
    //backup button end

    rightWindowSizer->Add(plotViewNotebook,0,wxEXPAND,0);
    rightWindowSizer->Add(backupBtn,1,wxEXPAND,0);

    rightWindow->SetSizer(rightWindowSizer);
    rightWindowSizer->SetSizeHints(rightWindow);

    splitter->SetMinimumPaneSize(20);
    leftWindow->Fit();
    rightWindow->Show(true);
    leftWindow->Show(true);
    splitter->SplitVertically( leftWindow, rightWindow );

    wxBoxSizer *rootSizer=new wxBoxSizer(wxVERTICAL);
    rootSizer->Add(splitter,1,wxEXPAND,0);
    SetSizer(rootSizer);

    Maximize();
    SetTitle(_("light-viewer"));
    SetIcon(wxIcon(pilot_frame_xpm));
    //wxSize tmp=GetSize();


    //variabel init
    pageIndex=0;
    mdataFactory=new DataFactory();
    fDialog=new FilterDialog(this,wxID_ANY,wxT("Filter view set"),wxDefaultPosition,wxDefaultSize,wxDEFAULT_DIALOG_STYLE);
    //statistic thread create
    myProcess.finishFlag=false;
    processt =new ProcessThread(*this);
    if(processt->Create()!=wxTHREAD_NO_ERROR){
        wxLogError(wxT("process thread can't be create"));

    }
    finished=true;

    //statistic thread end

    //event connection
    //Connect( wxEVT_CHAR,wxKeyEventHandler(pilot_viewFrame::OnViewRightLeft))

#if wxUSE_STATUSBAR
    // create a status bar with some information about the used wxWidgets version
    CreateStatusBar(4);
    wxStatusBar *sbar=GetStatusBar();
    wxStaticBitmap *bitMapStatus;

    SetStatusText(_("welcome!"),1);
    SetStatusText(_("Harbin Engeerning University"),2);
    SetStatusText(_(""),3);

    int widths[]={40,100,200,-1};
    sbar->SetStatusWidths(4,widths);
    bitMapStatus=new wxStaticBitmap(sbar, -1, wxBitmap(wxImage(hongqi_),wxBITMAP_TYPE_BMP ));


    //wxwidget2.9.4
#endif // wxUSE_STATUSBAR

}

pilot_viewFrame::~pilot_viewFrame()
{
    //delete mdataFactory;//add this will cause a bug,the best thing for the wxProgram is never destroy the last thing,operater system will take care of it
    //delete processt;// pilot_viewFrame will destroy it

}

void pilot_viewFrame::OnClose(wxCloseEvent &event)
{
    programOverFlag=true;
    backUpWorkspace();

    //for(itePlot=plotView.begin();itePlot!=plotView.end();++itePlot){
    //    itePlot->second->Destroy();
//
   // }


    Destroy();
}

void pilot_viewFrame::OnQuit(wxCommandEvent &event)
{
    programOverFlag=true;
    backUpWorkspace();
   // for(itePlot=plotView.begin();itePlot!=plotView.end();++itePlot){
   //     itePlot->second->Destroy();

   // }

    Destroy();
}

void pilot_viewFrame::OnAbout(wxCommandEvent &event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("hrbeu"));
}

void pilot_viewFrame::OnSave(wxCommandEvent& event){
/*
    if(noteStatistic->HasFocus()){
        wxFileDialog saveImage(this,_("Save image"),"","","jpeg file(*.jpeg)|*.jpeg|bmp file(*.bmp)|*.bmp",wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
        if(saveImage.ShowModal()==wxID_OK){
            wxString path=saveImage.GetPath();
            //wxMessageBox(path);
            MMpwindow* curMpWindow=dynamic_cast<MMpwindow*>(noteStatistic->GetCurrentPage());
            if(saveImage.GetFilterIndex()==0)
                curMpWindow->SaveScreenshot(path,wxBITMAP_TYPE_BMP);
            if(saveImage.GetFilterIndex()==1)
                curMpWindow->SaveScreenshot(path,wxBITMAP_TYPE_JPEG);
        }
    }
*/

    PlotWindow* curWin=dynamic_cast<PlotWindow*>(plotViewNotebook->GetCurrentPage());
    if(curWin==NULL)
    	return;

	wxFileDialog saveImage(this,_("Save image"),"","","bmp file(*.bmp)|*.bmp",wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
       if(saveImage.ShowModal()==wxID_OK){
		    wxString path=saveImage.GetPath();
			//wxImage::AddHandler(wxJPEGHandler);
			//wxImage plotShot=curWin->saveBitmap.ConvertToImage();

            curWin->saveBitmap.SaveFile(path,wxBITMAP_TYPE_BMP);
	}
}

void pilot_viewFrame::OnOpen(wxCommandEvent &event){
    wxString caption=wxT("Choose files");
    //wxFileName oldPath(wxGetCwd()+"/ini");
    //wxString curDir;
    //wxTextFile file;

/*
    if(!oldPath.Exists()){
        file.Create(oldPath.GetFullPath());
        //file.Open(oldPath.GetFullPath());
        curDir=wxGetCwd();
    }else{
        file.Open(oldPath.GetFullPath());
        curDir=file.GetFirstLine();
    }
*/

    wxString wildcard=wxT("BAM file(*.bam)|*.bam|FASTA files(*.fa)|*.fa|BED file(*.bed.gz)|*.bed.gz|VCF file(*.vcf.gz)|*.vcf.gz|gff file(*.gff.gz)|*.gff.gz|gtf file(*.gtf.gz)|*.gtf.gz|BED file(*.bed)|*.bed|VCF file(*.vcf)|*.vcf|gff file(*.gff)|*.gff|gtf file(*.gtf)|*.gtf|ALL files(*.*)|*.*");
    wxFileDialog dialog(this,caption,"","",wildcard,wxFD_OPEN|wxFD_MULTIPLE);
    if(dialog.ShowModal()==wxID_OK){

        wxProgressDialog prossing(_T("Processing"), _T("Read and Processing data,please wait"), 100, this,wxPD_APP_MODAL|wxPD_AUTO_HIDE);

        wxArrayString tmpAS;
        dialog.GetPaths(tmpAS);
        for(size_t i=0;i<tmpAS.GetCount();++i){
            if(tmpAS[i].EndsWith(_(".bam"))){
                    mdataFactory->addBamFiles(wxString(tmpAS[i]));
            }

            if(tmpAS[i].EndsWith(_(".fa"))||tmpAS[i].EndsWith(".fasta")){
                    //file.Clear();//备份路径
                    //file.AddLine(dialog.GetDirectory());
                    //file.Write();

                    //fastaFile=wxString(tmpAS[i]);
                    mdataFactory->addFastaFile(tmpAS[i]);
                    //boost::unordered_map<std::string,int>::iterator it;
                    popupCtrl->ClearAll();
                    /*
                    for(ite=mdataFactory->chrAndLength.begin();ite!=mdataFactory->chrAndLength.end();++ite){
                        //chromosomeChoice->SetString(i,wxString(ite->first));
                        //chromosomeChoice->AppendText (wxString(ite->first));
                        popupCtrl->InsertItem(popupCtrl->GetItemCount(),wxString(ite->first));

                    }
                    */
                    //popupCtrl->SetStringValue(wxString(mdataFactory->chrAndLength.begin()->first));
                    //if need build fastaIndex
                    std::list<std::string> chrNames=mdataFactory->getChrNames();
                    std::list<std::string>::iterator ite;

                    for(ite=chrNames.begin();ite!=chrNames.end();++ite){
                        popupCtrl->InsertItem(popupCtrl->GetItemCount(),wxString(*ite));

                    }

            }

            if(tmpAS[i].EndsWith(_(".gff.gz"))||tmpAS[i].EndsWith(_(".gff"))){
                mdataFactory->addGffFiles(tmpAS[i]);
                mdataFactory->setGffRefreshFlag(false);

            }

            if(tmpAS[i].EndsWith(_(".gtf.gz"))||tmpAS[i].EndsWith(_(".gtf"))){
                mdataFactory->addGtfFiles(tmpAS[i]);
                mdataFactory->setGtfRefreshFlag(false);
            }

            if(tmpAS[i].EndsWith(_(".bed.gz"))||tmpAS[i].EndsWith(_(".bed"))){
                    mdataFactory->addBedFiles(tmpAS[i]);
                    mdataFactory->setBedRefreshFlag(false);
            }

            if(tmpAS[i].EndsWith(_(".vcf.gz"))||tmpAS[i].EndsWith(_(".vcf"))){
                //vcfFiles.Add(wxString(tmpAS[i]));
                mdataFactory->addVcfFiles(tmpAS[i]);
                mdataFactory->setVcfRefreshFlag(false);

            }


        }

        if(backupBtn!=NULL){
            delete backupBtn;
            backupBtn=NULL;
        }

        filesViewRefresh();
        viewVectorRefresh();
        //mdataFactory->refreshTable();

        fileCtrl->ExpandAll();
    }


}

void pilot_viewFrame::viewVectorRefresh(){
    itePlot=plotView.begin();

    for(;itePlot!=plotView.end();++itePlot){
        itePlot->second->refreshPopupMenu();

    }

}


void pilot_viewFrame::filesViewRefresh(){
    fileCtrl->DeleteChildren(rootIdBam);
    fileCtrl->DeleteChildren(rootIdGff);
    fileCtrl->DeleteChildren(rootIdGtf);
    fileCtrl->DeleteChildren(rootIdBed);
    fileCtrl->DeleteChildren(rootIdVcf);
    fileCtrl->DeleteChildren(rootIdFasta);
    //for(int i=0;i<treeItems.size();++i)
    //    treeItems[i]->Destroy();
    treeItems.clear();

    int k=0;
    for(size_t i=0;i<mdataFactory->getBamCount();++i){
        wxFileName bamFile=wxFileName(mdataFactory->getBamFile(i));
        treeItems.push_back(new MyTreeItemData(bamFile.GetFullPath()));
        /*wxTreeItemId bamtmpID=*/fileCtrl->AppendItem(rootIdBam,bamFile.GetName(),1,1,treeItems[k++]);
        //treeItems.push_back(bamtmpID);
    }

    if(mdataFactory->getFastaFile()!=wxEmptyString){
        for(unsigned i=0;i<mdataFactory->getFastaCount();++i){

            wxFileName faFile=wxFileName(mdataFactory->getFastaFile(i));
            treeItems.push_back(new MyTreeItemData(faFile.GetFullPath()));
            /*wxTreeItemId fatmpID=*/fileCtrl->AppendItem(rootIdFasta,faFile.GetName(),5,5,treeItems[k++]);
            //treeItems.push_back(fatmpID);
        }
    }

    if(mdataFactory->getGffCount()!=0){
        for(unsigned i=0;i<mdataFactory->getGffCount();++i){
            wxFileName gffFile=wxFileName(mdataFactory->getGffFile(i));
            treeItems.push_back(new MyTreeItemData(gffFile.GetFullPath()));
            /*wxTreeItemId vcftmpID=*/fileCtrl->AppendItem(rootIdGff,gffFile.GetName(),2,2,treeItems[k++]);
 //           treeItems.push_back(vcftmpID);

        }
    }

    if(mdataFactory->getGtfCount()!=0){
        for(unsigned i=0;i<mdataFactory->getGtfCount();++i){
            wxFileName gtfFile=wxFileName(mdataFactory->getGtfFile(i));
            treeItems.push_back(new MyTreeItemData(gtfFile.GetFullPath()));
            /*wxTreeItemId vcftmpID=*/fileCtrl->AppendItem(rootIdGtf,gtfFile.GetName(),2,2,treeItems[k++]);
 //           treeItems.push_back(vcftmpID);

        }
    }


   // if(bedFile!=wxEmptyString){
    if(mdataFactory->getBedCount()!=0){
        for(size_t i=0;i<mdataFactory->getBedCount();++i){
            wxFileName beFile=wxFileName(mdataFactory->getBedFile(i));
            treeItems.push_back(new MyTreeItemData(beFile.GetFullPath()));
            /*wxTreeItemId bedtmpID=*/fileCtrl->AppendItem(rootIdBed,beFile.GetName(),3,3,treeItems[k++]);
        //    treeItems.push_back(bedtmpID);
        }
    }

    if(mdataFactory->getVcfCount()!=0){
        for(unsigned i=0;i<mdataFactory->getVcfCount();++i){
            wxFileName vcFile=wxFileName(mdataFactory->getVcfFile(i));
            treeItems.push_back(new MyTreeItemData(vcFile.GetFullPath()));
            /*wxTreeItemId vcftmpID=*/fileCtrl->AppendItem(rootIdVcf,vcFile.GetName(),4,4,treeItems[k++]);
 //           treeItems.push_back(vcftmpID);
        }
    }

}

void pilot_viewFrame::OnViewBam(wxTreeEvent& event){
    MyTreeItemData* testBam=dynamic_cast<MyTreeItemData*>(fileCtrl->GetItemData(fileCtrl->GetFocusedItem()));
    wxString testBamFile=testBam->GetDesc();
    //wxMessageBox(testBamFile);
    if(!testBamFile.EndsWith(wxT(".bam"))){
        return;

    }
    itePlot=plotView.find(testBamFile);


    if(itePlot==plotView.end()){//if not find{

            wxSize tt=rightWindow->GetClientSize();
            plotViewNotebook->SetSize(tt);

            plotView.insert(std::make_pair<wxString,PlotWindow*>(testBamFile,new PlotWindow( plotViewNotebook, -1, wxDefaultPosition, tt,wxBORDER_NONE|wxWANTS_CHARS|wxVSCROLL|wxTAB_TRAVERSAL,testBamFile,mdataFactory,pageIndex)));
            wxFileName tBamFile=wxFileName(testBamFile);

            plotViewNotebook->AddPage(plotView[testBamFile],tBamFile.GetName(),true,NO_IMAGE);

            pageIndex++;

            PlotWindow *curWin=plotView[testBamFile];
            //curWin->statusBar=GetStatusBar();
            curWin->SetFocus();
            //curWin->CaptureMouse();
            //curWin->ReleaseMouse();

            SetStatusText(testBamFile,3);
            //curWin->refreshPopupMenu();
            curWin->Refresh();

    }else{

        PlotWindow* curWin=plotView[testBamFile];
        //PlotWindow* curWin=dynamic_cast<PlotWindow*>(plotViewNotebook->GetCurrentPage());
        if(curWin==NULL){
            //event.Skip();
            return;
        }

        SetStatusText(testBamFile,3);

        //int index=plotViewNotebook->->FindPage(curWin);
        //plotViewNotebook->
        SetStatusText(curWin->getBamFileName(),3);
        curWin->Refresh();

    }


}

void pilot_viewFrame::OnDoStatistic(wxCommandEvent& event){

    MyTreeItemData* testBam=dynamic_cast<MyTreeItemData*>(fileCtrl->GetItemData(fileCtrl->GetFocusedItem()));
    wxString testBamFile=testBam->GetDesc();

    if((myProcess.finishFlag==false)&&testBamFile.EndsWith(".bam")){
        mdataFactory->setActiveBamFile(testBamFile);
        processt->Run();
        SetStatusText(wxT("busy"),1);
    }

}

void pilot_viewFrame::OnIdle(wxIdleEvent &event){

    if(myProcess.finishFlag==true){
        mpLayer *lpen;
        for(size_t i=0;i<m_plots.size();++i){
            m_plots[i]->DelAllLayers(true);
            m_plots[i]->AddLayer(     new mpScaleX( wxT("length")));
            m_plots[i]->AddLayer(     new mpScaleY( wxT("number of read")));
            if(i==0){
                m_plots[i]->AddLayer(     new mpScaleX( wxT("length")));
                m_plots[i]->AddLayer(     new mpScaleY( wxT("number of read")));
                m_plots[i]->AddLayer(lpen=new LenDis(myProcess.lengthDis));

            }if(i==1){
                m_plots[i]->AddLayer(     new mpScaleX( wxT("quality")));
                m_plots[i]->AddLayer(     new mpScaleY( wxT("number of read")));
                m_plots[i]->AddLayer(lpen=new QuaDis(myProcess.mapQuality));
            }if(i==2){
                m_plots[i]->AddLayer(     new mpScaleX( wxT("edit distance")));
                m_plots[i]->AddLayer(     new mpScaleY( wxT("number of read")));
                m_plots[i]->AddLayer(lpen=new NM(myProcess.nm));
            }if(i==3){
                m_plots[i]->AddLayer(     new mpScaleX( wxT("soft clipping")));
                m_plots[i]->AddLayer(     new mpScaleY( wxT("number of read")));
                m_plots[i]->AddLayer(lpen=new SoftC(myProcess.sc));
            }/*if(i==4){
                m_plots[i]->AddLayer(     new mpScaleX( wxT("coverage")));
                m_plots[i]->AddLayer(     new mpScaleY( wxT("number of bases")));
                m_plots[i]->AddLayer(lpen=new Coverage(myProcess.codis));

                */

            //}//if(i==5&&bedFile!=wxEmptyString){
              //  m_plots[i]->AddLayer(     new mpScaleX( wxT("region")));
              //  m_plots[i]->AddLayer(     new mpScaleY( wxT("average coverage of base")));
              //  m_plots[i]->AddLayer(lpen=new RegionCount(myProcess.regionCount));

           // }
            lpen->SetPen(wxPen(*wxRED,3,wxSOLID));
            lpen->SetContinuity(true);
            //lpen->
            m_plots[i]->Fit();

            SetStatusText(wxT("welcome"),0);
            //delete processt;

        }
        myProcess.finishFlag=false;

    }else{
        //myProcess.finishFlag=false;
        event.Skip();
    }

}

///refresh the layout of the child window
void pilot_viewFrame::OnPaint(wxPaintEvent & event){
/*
    wxPaintDC dc(this);
    if(wxNOT_FOUND!=plotViewNotebook->GetSelection()){
        PlotWindow* curWin=dynamic_cast<PlotWindow*>(plotViewNotebook->GetCurrentPage());//->plotMain(dc,500,);
//        wxSize tmp=GetSize();
        wxSize tt=rightWindow->GetClientSize();
        plotViewNotebook->SetSize(tt);
        curWin->SetSize(tt);

    }

*/
}

void pilot_viewFrame::OnNaviGate(wxCommandEvent& event){
    //wxNumberEntryDialog dialog(this,wxT(""),wxT("to go a position"),wxT("navigation"),500,0,999999999);
    //if(dialog.ShowModal()!=wxID_OK)
    //    return;


    PlotWindow* curWin=dynamic_cast<PlotWindow*>(plotViewNotebook->GetCurrentPage());
    if(curWin==NULL)
        return;

    //curChrPos[curWin]=dialog.GetValue();
    //int tcurPos=dialog.GetValue();
    wxString valueStr=mPosition->GetValue();
    for(size_t i=0;i<valueStr.length();++i){
        wxUniChar tc=valueStr.at(i);
        char t;tc.GetAsChar(&t);
        if(t>57||t<48)
            return;
    }

    long tcurPos=atol(valueStr.c_str());
//    int maxValue=mdataFactory->chrAndLength[curWin->localCurChr];
   // if(mdataFactory->getFastaFile()!=wxEmptyString){
//        if(tcurPos>maxValue)
//            tcurPos=maxValue-100;

//        if(tcurPos<0)
//            tcurPos=100;
  //  }

    curWin->localCurChrPos=tcurPos;
    curWin->Refresh();

    //curWin->plot(curChr[curWin],curChrPos[curWin]);


}

void pilot_viewFrame::OnPlotSnp(wxCommandEvent& event){

    PlotWindow* curWin=dynamic_cast<PlotWindow*>(plotViewNotebook->GetCurrentPage());
    if(curWin==NULL)
        return;

	int pos=0;

    //wxString valueStr=mPosition->GetValue();

	wxString valueStr=mSNPPos->GetValue().Trim();
	if(valueStr.Length()!=0)
		pos=atol(valueStr.c_str());
	else
		pos=curWin->getPos();
	//int curPos=curWin->getPos();

    SNPDialog *snpdia=new SNPDialog(this,wxID_ANY,_T("snp view"),wxDefaultPosition,wxSize(1000,500));
    snpdia->setData(mdataFactory,curWin->getBamdata(),curWin->getChr(),pos);

    if(snpdia->Show()==wxID_OK){
        //wxMessageBox(_T("haha"));
    }

    //wxMessageBox(_T("heihei"));

}

//this function is unused
void pilot_viewFrame::OnViewRightLeft(wxKeyEvent& event){

    //PlotWindow* curWin=dynamic_cast<PlotWindow*>(plotViewNotebook->GetCurrentPage());
    //if(curWin==NULL){
    //    event.Skip();
    //    return;
    //}

    //if(event.GetKeyCode()==WXK_LEFT)
    //    curWin->OnKeyView(false);
    //else if(event.GetKeyCode()==WXK_RIGHT)
     //   curWin->OnKeyView(true);
    //else if(event.GetKeyCode()==WXK_UP)
    //    curWin->onZoom(true);//fang da zoom in
   // else if(event.GetKeyCode()==WXK_DOWN)
   //     curWin->onZoom(false);//
    //else
        event.Skip();
}

void pilot_viewFrame::OnSelectChr(wxCommandEvent& event){
    plotViewNotebook->GetSelection();
    plotViewNotebook->GetCurrentPage();
    PlotWindow* curWin=dynamic_cast<PlotWindow*>(plotViewNotebook->GetCurrentPage());
    if(curWin==NULL){
        //event.Skip();
        return;
    }

    wxString chr=popupCtrl->GetStringValue();
    if(chr.length()==0)
        return;

    curWin->localCurChr=chr.ToStdString();
    curWin->Refresh();
}

void pilot_viewFrame::OnSize(wxSizeEvent& event){
    if(this->finished){
        this->Layout();
        //Refresh();
        splitter->SetSashPosition(200);
    }
}

pilot_viewFrame::MyTreeCtrl::MyTreeCtrl(wxWindow *parent, const wxWindowID id,const wxPoint& pos, const wxSize& size,long style): wxTreeCtrl(parent, id, pos, size, style){

}

void pilot_viewFrame::OnBookChange(wxBookCtrlBaseEvent& event){
    //plotViewNotebook->GetSelection();
    //plotViewNotebook->GetCurrentPage();

    PlotWindow* curWin=dynamic_cast<PlotWindow*>(plotViewNotebook->GetCurrentPage());
    if(curWin==NULL){
        //event.Skip();
        return;
    }

    SetStatusText(curWin->getBamFileName(),3);

    std::string t=curWin->localCurChr;
    //popupCtrl->SetStringValue(wxString(curWin->localCurChr));
    chromosomeChoice->SetText(wxString(curWin->localCurChr));
}

void pilot_viewFrame::backUpWorkspace(){
    if(isBackUp)
        return;
    Pilot_WorkSpace curWork;
    curWork.fastaFiles=mdataFactory->getFastaFiles();
    curWork.bamFiles=mdataFactory->getBamFiles();
    curWork.bedFiles=mdataFactory->getBedFiles();
    curWork.gffFiles=mdataFactory->getGffFiles();
    curWork.gtfFiles=mdataFactory->getGtfFiles();
    curWork.vcfFiles=mdataFactory->getVcfFiles();
    PlotWindowProperty pro;
    for(itePlot=plotView.begin();itePlot!=plotView.end();++itePlot){
        pro.bam=itePlot->first;
        pro.chr=itePlot->second->localCurChr;
        pro.pos=itePlot->second->localCurChrPos;
        pro.page=itePlot->second->page;
        curWork.windowProperty.push_back(pro);
    }

    curWork.writeWorkSpace();
}

void pilot_viewFrame::readWorkspace(wxCommandEvent& event){

    wxProgressDialog prossing(_T("Processing"), _T("Read and Processing data,please wait"), 100, this,wxPD_APP_MODAL|wxPD_AUTO_HIDE);

    Pilot_WorkSpace curWork;
    curWork.readWorkSpaceNew();
    delete backupBtn;
    backupBtn=NULL;
    //wxMessageBox(curWork.fastaFiles[0]);
    //wxMessageBox(curWork.bamFiles[0]);

    mdataFactory->setFastaFiles(curWork.fastaFiles);
    mdataFactory->setBamFiles(curWork.bamFiles);
    mdataFactory->setBedFiles(curWork.bedFiles);
    mdataFactory->setGffFiles(curWork.gffFiles);
    mdataFactory->setGtfFiles(curWork.gtfFiles);
    mdataFactory->setVcfFiles(curWork.vcfFiles);

    boost::unordered_map<std::string,int>::iterator ite;
    for(ite=mdataFactory->chrAndLength.begin();ite!=mdataFactory->chrAndLength.end();++ite){
        //chromosomeChoice->SetString(i,wxString(ite->first));
        //chromosomeChoice->AppendText (wxString(ite->first));
            popupCtrl->InsertItem(popupCtrl->GetItemCount(),wxString(ite->first));

    }

    filesViewRefresh();
    //mdataFactory->refreshTable();

    fileCtrl->ExpandAll();

    for(unsigned int i=0;i<curWork.windowProperty.size();++i){

        wxString testBamFile=curWork.windowProperty[i].bam;
        std::string tbam=testBamFile.ToStdString();
        int pageIndex=curWork.windowProperty[i].page;
        std::string chr=curWork.windowProperty[i].chr;
        long chrPos=curWork.windowProperty[i].pos;

        popupCtrl->SetStringValue(chr);

        mPosition->SetValue(wxString::Format(_T("%ld"),chrPos));

        wxSize tt=rightWindow->GetClientSize();
        plotViewNotebook->SetSize(tt);

        plotView.insert(std::make_pair<wxString,PlotWindow*>(testBamFile,new PlotWindow( plotViewNotebook, -1, wxDefaultPosition, tt,wxBORDER_NONE|wxWANTS_CHARS|wxVSCROLL|wxTAB_TRAVERSAL,testBamFile,mdataFactory,pageIndex)));
        wxFileName tBamFile=wxFileName(testBamFile);

        plotViewNotebook->AddPage(plotView[testBamFile],tBamFile.GetName(),true,NO_IMAGE);

        pageIndex++;
        PlotWindow *curWin=plotView[testBamFile];
        curWin->localCurChr=chr;
        curWin->localCurChrPos=chrPos;

        curWin->refreshPopupMenu();
        curWin->Refresh();


    }



}

void pilot_viewFrame::OnFilter(wxCommandEvent& event){

    if(fDialog->ShowModal()==wxID_OK){
        mdataFactory->bamfilter->softClipping=fDialog->sc;
        //mdataFactory->bamfilter->hardClipping=fDialog->hd;
        mdataFactory->bamfilter->unique=fDialog->unique;
        mdataFactory->bamfilter->tagFilterStrs=fDialog->filterStrings;
        mdataFactory->bamfilter->processTag();

        PlotWindow* curWin=dynamic_cast<PlotWindow*>(plotViewNotebook->GetCurrentPage());
        if(curWin==NULL){
            //event.Skip();
            return;
        }

        //wxString chr=popupCtrl->GetStringValue();
        //curWin->localCurChr=chr.ToStdString();
        //curWin->Refresh();
    }

}
