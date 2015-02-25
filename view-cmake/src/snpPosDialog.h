#ifndef SNPPOSDIALOG_H_INCLUDED
#define SNPPOSDIALOG_H_INCLUDED

#include <wx/dialog.h>
#include "snp_plot_window.h"
#include <wx/sizer.h>


class SNPDialog:public wxDialog{
public:
    SNPDialog();
    SNPDialog(wxWindow* parent,wxWindowID id=wxID_ANY,
              const wxString& caption=wxT("snp view"),
              const wxPoint& pos=wxDefaultPosition,
              const wxSize& size=wxDefaultSize,
              long style=wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxMAXIMIZE_BOX);
            //long style=wxCAPTION);

private:
    wxBoxSizer* topSizer;
public:
    SNPPlotWindow* plotWin;

    SNPPlotWindow* getWindow();

    void setData(DataFactory* tdata,BamData* tbamdata,string tchr,int tpos);

};


#endif // SNPPOSDIALOG_H_INCLUDED
