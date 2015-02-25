/***************************************************************
 * Name:      pilot_viewMain.h
 * Purpose:   Defines Application Frame
 * Author:    limeng (s6228483@yahoo.com.cn)
 * Created:   2013-05-05
 * Copyright: limeng ()
 * License:
 **************************************************************/

#ifndef PILOT_VIEWMAIN_H
#define PILOT_VIEWMAIN_H
//#include "wx_pch.h"
//#ifndef WX_PRECOMP
//    #include <wx/wx.h>
//#endif

    #include "pilot_viewApp.h"
    #include <wx/wx.h>
    #include <wx/panel.h>
    #include <wx/thread.h>
    #include <wx/numdlg.h>
    #include <wx/splitter.h>
    #include <wx/treectrl.h>
    #include <wx/imaglist.h>
    #include <wx/vscroll.h>
    #include <wx/filename.h>
    #include <wx/notebook.h>
    #include <wx/textfile.h>
    #include <wx/combo.h>
    #include <wx/toolbar.h>
    #include <wx/filedlg.h>

    #include <vector>
    #include <map>
    #include <faidx.h>
    #include <tabix.h>
    #include <utility>

    //#include "art/xpm/my_bam.xpm"
    #include "pilot_workspace.h"
    #include "process.h"
    #include "stl_help.h"
    #include "statistic_plot.h"
    #include "filterDialog.h"
    #include "plot_window.h"
    #include "MMpwindow.h"
    #include "MRightWin.h"
    #include "ChrosomeChoice.h"
    #include "dataFactory.h"
    #include "snpPosDialog.h"


//#include "wx_pch.h"

//class ChrosomeChoice;

class pilot_viewFrame: public wxFrame
{
    public:
        pilot_viewFrame(wxFrame *frame, const wxString& title);
        ~pilot_viewFrame();
    private:
        enum
        {
            //menu1
            idMenuQuit = 1000,
            idMenuAbout,
            //menu2
            ID_OPEN,
            ID_SAVE,
            //navigate menu
            idNaviGateMenu,
            //filter
            ID_FILTER,
            //toolbar begin
            wxID_TOOL_BAR,
            wxID_CHR_CHOICE,
            wxID_POSITION,
            wxID_SNPPOSITION,
            //tool bar end

            //menu statistic begin
            ID_DO_STATIC,
            //menu statistic end

            wxID_BACKUP,
            wxID_NOTE_CHANGED,
            wxID_SPLITTER_WINDOW,
                wxLEFT_SCROLL_WINDOW,//this window is not scrolled now
                    wxID_LEFT_TREE_CTRL,
                    wxID_NOTE_STATIC,

                wxRIGHT_SCROLL_WINDOW,//this window is vertical scrolled

            //upDate event
            wxID_UI_UPDATE,// doesn't work

            NO_IMAGE=-1
        };

        //control and window// small control's class are inner class
        class MyTreeCtrl:public wxTreeCtrl{

        public:
            MyTreeCtrl(wxWindow *parent, const wxWindowID id,const wxPoint& pos, const wxSize& size,long style);

        };

        class MyTreeItemData:public wxTreeItemData{
        private:
            wxString fileName;
        public:
            MyTreeItemData(wxString f):wxTreeItemData(),fileName(f){}

            wxString GetDesc() const {
            return fileName;
            }
        };

        //control variable
        wxToolBar* mtoolBar;
        wxTextCtrl* mPosition;
        wxTextCtrl* mSNPPos;

        wxSplitterWindow* splitter;
        wxNotebook* noteStatistic;
        wxNotebook* plotViewNotebook;
        MyTreeCtrl* fileCtrl;
        MRightWin* rightWindow;
        //wxChoice* chrChoice;
        wxComboCtrl* chromosomeChoice;
        ChrosomeChoice* popupCtrl;
        wxButton *backupBtn;

        //the statistic window
        //- Mouse Move+CTRL: Pan (Move)
        //- Mouse Wheel: Vertical scroll
        //- Mouse Wheel+SHIFT: Horizontal scroll
        //- Mouse Wheel UP+CTRL: Zoom in
        //- Mouse Wheel DOWN+CTRL: Zoom out
        //data filesnames

        DataFactory* mdataFactory;
        FilterDialog* fDialog;

        std::vector<MMpwindow*> m_plots;//内存清理时，这里会不会出现问题//mpWindow
        std::map<wxString,PlotWindow*> plotView;
        std::map<wxString,PlotWindow*>::iterator itePlot;

        int pageIndex;

        //thread statistic plot
        Process myProcess;
        class ProcessThread:public wxThread{
        public:
            pilot_viewFrame& parent;
            ~ProcessThread(){}
            ProcessThread(pilot_viewFrame& t):parent(t),wxThread(wxTHREAD_DETACHED){

            };
            virtual void * Entry(){
                //wxFileName testBedFile=wxFileName(parent.bedFile);

                //if(testBedFile.Exists()){
                //    parent.myProcess.getInteRegion(parent.bedFile.ToStdString());
                //    parent.myProcess.processBam(parent.activeBamFile.ToStdString(),true);
                //}
              //  else{
                    parent.myProcess.processBam(parent.mdataFactory->getActiveBamFile().ToStdString(),false);
               // }
                parent.myProcess.finishFlag=true;
                return NULL;
            }
        };
        ProcessThread* processt;
        //thread statistic plot end


        bool programOverFlag;
        bool finished;
        //leftWindow design
        wxTreeItemId rootIdBam;
        wxTreeItemId rootIdGtf;
        wxTreeItemId rootIdGff;
        wxTreeItemId rootIdBed;
        wxTreeItemId rootIdVcf;
        wxTreeItemId rootIdFasta;
        std::vector<MyTreeItemData*> treeItems;


        //gui function
        void viewVectorRefresh();
        void filesViewRefresh();
        void backUpWorkspace();
        bool isBackUp;
        void readWorkspace(wxCommandEvent& event);

        //event function
        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnOpen(wxCommandEvent& event);
        void OnSave(wxCommandEvent& event);


        void OnNaviGate(wxCommandEvent& event);
		void OnPlotSnp(wxCommandEvent& event);

        void OnDoStatistic(wxCommandEvent& event);
        void OnViewBam(wxTreeEvent& event);

        void OnFilter(wxCommandEvent& event);

        void OnPaint(wxPaintEvent & event);
        void OnIdle(wxIdleEvent &event);
        void OnSize(wxSizeEvent& event);

        void OnViewRightLeft(wxKeyEvent& event);

        void OnBookChange(wxBookCtrlBaseEvent& event);
        void OnTreeBook(wxTreeEvent& event);


public:
        void OnSelectChr(wxCommandEvent& event);

        DECLARE_EVENT_TABLE()

};


#endif // PILOT_VIEWMAIN_H
