#ifndef PLOT_WINDOW_H
#define PLOT_WINDOW_H

#include <wx/window.h>
#include <wx/menu.h>
#include <wx/string.h>
#include <wx/log.h>
#include <wx/msgdlg.h>
#include <wx/filename.h>
#include <wx/dcclient.h>
#include <wx/infobar.h>
#include <wx/panel.h>
#include <wx/statusbr.h>

#include <wx/utils.h>
//#include <wx/help.h>
#include <wx/tooltip.h>
#include <wx/popupwin.h>
#include <wx/scrolwin.h>
#include <wx/timer.h>


//#include <wx/cshelp.h>
//#include <wx/event.h>
#include <faidx.h>
#include <bam.h>
#include <string>
#include <cmath>
#include <tabix.h>
#include <cmath>
#include <vector>

#include "MessagePopUpWindow.h"
#include "wxShotCut.h"
#include "MouseWin.h"

#include "dataFactory.h"
#include "bamData.h"
#include "stl_help.h"
#include "plotDetail.h"
#include "plotNormal.h"
#include "plotProfile.h"


class PlotWindow:public wxScrolledCanvas{
public:
    //friend int bamPlotHelper(const bam1_t* b,void *data);
    friend class PlotDetail;
    friend class PlotNormal;
    friend class PlotProfile;

    int page;
    //wxStatusBar* statusBar;
    std::string localCurChr;


    long localCurChrPos;
    //int localCurChrMinPos;
    //int localCurChrMaxPos;

    //wxString bamFileName;
    wxBitmap saveBitmap;

private:
    //__gnu_cxx::hash_map<int,char> posNulciedTable;
    //bool menuFirstRunFlag;
    //画布的大小，通过dc.Getvirtual size得到
    DataFactory* mdata;
    BamData* bamdata;//=new BamData();

    std::vector<bool> *viewCheckVector;

    wxSize cavansSize;

    wxPoint oldPos;

    std::vector<std::pair<PlotBase*,double> > zoomVector;

    wxMenu* viewMenu;
    MessagePopupWin *message;
    //wxSimpleHelpProvider *myHelp;

    WxShotCut wscut;
    PlotDetail *mplotDeatil;
    PlotNormal *mplotNormal;
    PlotProfile *mplotProfile;
    PlotBase *mActive;
    int curNucliedSize;//can't be unsigned int ,may be <0

    wxTimer *t;

    int nIdleTimes;

    bool flag;//fixed me mouse rightclick flag

    //wxTimer m_timer;
    double change;//鼠标移动时的变化

enum{
    wxID_MOUSE_WIN=1000
};

public:
    ~PlotWindow();

    PlotWindow(wxWindow *parent,wxWindowID id,const wxPoint &pos=wxDefaultPosition,
                const wxSize &winsize=wxDefaultSize, long style=0,
                wxString bam=wxEmptyString,DataFactory* tdata=NULL,int pageIndex=0);
    //
    void initConnection();
    void disConnection();
    void refreshPopupMenu();

    void RefreshVcfTable();
    void RefreshGffTable();
    void RefreshBedTable();
    void RefreshGtfTable();
    void RefreshTable();

    void plot(wxDC& dc);

    wxString getBamFileName();

    BamData* getBamdata(){
	    return bamdata;
	}

	string getChr(){
	    return localCurChr;
	}

	int getPos(){
	    return localCurChrPos;
    }

    //wxString GetBam(){return bamFileName;}
    void onZoom(bool in);

    void OnKeyView(bool right);

    void OnMouseDLeftClick(wxMouseEvent& event);

    wxString GetHelpTextAtPoint(const wxPoint &point, wxHelpEvent::Origin origin) const;

    void OnKeyView(wxKeyEvent& event);

    void OnMouseView(wxMouseEvent& event);

    void OnMouseLeftDown(wxMouseEvent& event);

    void OnScroll(wxMouseEvent& event);

    void onMouseRigthDown(wxMouseEvent& event);

    void OnPaint(wxPaintEvent &event);

    void OnSize(wxSizeEvent& event);

    void OnIdle(wxIdleEvent& event);

    void OnTimer(wxTimerEvent& event);

    void OnViewDetail(int x,int y);

    void OnLeave(wxMouseEvent& event);

    void OnLoseMouse(wxMouseEvent &event);

};


#endif // PLOT_WINDOW_H
