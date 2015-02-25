#ifndef SNP_PLOT_WINDOW_H_INCLUDED
#define SNP_PLOT_WINDOW_H_INCLUDED

#include <wx/scrolwin.h>
#include "plotSnp.h"
#include "bamData.h"
#include "DataFactory.h"

#include <wx/dcbuffer.h>


class SNPPlotWindow:public wxScrolledCanvas{
public:
    SNPPlotWindow(wxWindow *parent, long style,wxWindowID id=wxID_ANY,const wxPoint &pos=wxDefaultPosition,const wxSize &winsize=wxSize(1000,500));
    //below three terms must be set properly
    ~SNPPlotWindow();

    void onPaint(wxPaintEvent& event);

    void setDataFactory(DataFactory * tdata);

    void setBamData(BamData* tbamData);

private:
    wxString bamFileName;
    DataFactory* mdata;
    BamData* bamdata;
    PlotSnp* mplot;

    string chr;
    int curPos;
    bool flag;

public:

	void onSize(wxSizeEvent& event);

    void setChr(string tchr);
    void setPos(int tpos);
    void setPlot();

};



#endif // SNP_PLOT_WINDOW_H_INCLUDED
