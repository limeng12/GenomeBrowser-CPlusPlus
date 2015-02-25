#include "snp_plot_window.h"
#include <wx/msgdlg.h>

//             SNPPlotWindow(wxWindow *parent, long style,wxWindowID id=wxID_ANY,const wxPoint &pos=wxDefaultPosition,const wxSize &winsize=wxDefaultSize);

SNPPlotWindow::SNPPlotWindow(wxWindow *parent, long style,wxWindowID id,const wxPoint& pos,const wxSize &winsize)
:wxScrolledCanvas(parent,id,pos,winsize,style),flag(false){

    Connect(wxEVT_PAINT,wxPaintEventHandler(SNPPlotWindow::onPaint));
    Connect(wxEVT_SIZE,wxSizeEventHandler(SNPPlotWindow::onSize));


    SetScrollbars(0,20,0,50);
    SetVirtualSize(winsize.x,5000);

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

}

void SNPPlotWindow::onPaint(wxPaintEvent& event){
    wxAutoBufferedPaintDC dc(this);
    //wxPaintDC dc(this);
    dc.SetBackground(*wxWHITE_BRUSH);
    DoPrepareDC(dc);

    dc.Clear();


    wxSize size=GetVirtualSize();
	//wxMessageBox(wxString::Format("%d\t%d",size.x,size.y));

    int plotNucliedSize=mplot->GetNucliedSize();

	int rightPos=size.x;
	int begPos=(int)(curPos-rightPos/plotNucliedSize/2);

    int endPos=(int)(curPos+rightPos/plotNucliedSize/2);
	mplot->plotMain(dc,chr,curPos,begPos,endPos,0,0);

    //mplot->plot(dc);
    //dc.Clear();

}

void SNPPlotWindow::onSize(wxSizeEvent& event){
	if(flag==true)
	mplot->canvasSizeChange(GetVirtualSize());

}


void SNPPlotWindow::setPlot(){
	flag=true;
    mplot=new PlotSnp( mdata,bamdata,chr,curPos,wxSize(1000,5000));
}

void SNPPlotWindow::setDataFactory(DataFactory * tdata){
    mdata=tdata;

}

void SNPPlotWindow::setBamData(BamData* tbamData){
    bamdata=tbamData;

}

void SNPPlotWindow::setChr(string tchr){
    chr=tchr;
}

void SNPPlotWindow::setPos(int tpos){
    curPos=tpos;
}

SNPPlotWindow::~SNPPlotWindow(){
    delete mplot;
}
