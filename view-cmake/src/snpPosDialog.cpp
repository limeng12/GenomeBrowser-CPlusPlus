#include "snpPosDialog.h"

SNPDialog::SNPDialog(){

}

SNPDialog::SNPDialog(wxWindow* parent,wxWindowID id,
              const wxString& caption,
              const wxPoint& pos,
              const wxSize& size,
              long style):wxDialog(parent,id,caption,pos,size,style){

    plotWin=new SNPPlotWindow(this,wxBORDER_NONE|wxWANTS_CHARS|wxVSCROLL|wxTAB_TRAVERSAL);

    topSizer=new wxBoxSizer(wxVERTICAL);
    this->SetSizer(topSizer);
    topSizer->Add(plotWin,1,wxEXPAND,0);


}

SNPPlotWindow*  SNPDialog::getWindow(){
    return plotWin;
}

void SNPDialog::setData(DataFactory* tdata,BamData* tbamData,string tchr,int tpos){

    plotWin->setDataFactory(tdata);
    plotWin->setBamData(tbamData);

    plotWin->setChr(tchr);
    plotWin->setPos(tpos);
    plotWin->setPlot();
}



