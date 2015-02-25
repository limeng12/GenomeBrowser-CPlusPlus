#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <wx/dialog.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/panel.h>

#include <boost/tuple/tuple.hpp>

#include "bamFilter.h"

class FilterDialog:public wxDialog{
    //DECLARE_CLASS((FilterDialog))
    DECLARE_EVENT_TABLE()
public:
    FilterDialog();
    FilterDialog(wxWindow* parent,wxWindowID id=wxID_ANY,
                 const wxString& caption=wxT("Filter Dialog"),
                 const wxPoint& pos=wxDefaultPosition,
                 const wxSize& size=wxDefaultSize,
                 long style=wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU);
    void Init();
    bool Create(wxWindow* parent,wxWindowID id=wxID_ANY,
                const wxString& caption=wxT("Filter Dialog"),
                const wxPoint& pos=wxDefaultPosition,
                const wxSize& size=wxDefaultSize,
                long style=wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU);

    //BamFilter filter;

    void createControl();
    void OnAddButton(wxCommandEvent& event);
    bool TransferDataFromWindow();

private:
    enum{
        wxID_SCPANEL,
        wxID_HDPANEL,
        wxID_UNIQUE,
        wxID_ADDBUTTON
    };

    //vector<wxPanel*> filterStringPanel;
    //vector<wxTextCtrl*> filterStringText;
    //vector<wxBoxSizer*> filterStringSizer;
    vector<boost::tuple<wxPanel*,wxBoxSizer*,wxTextCtrl*> > filterStringCtrl;

    wxTextCtrl *scTextCtrl;
    wxTextCtrl *hdTextCtrl;
    wxCheckBox *uniqueCtrl;
public:


    int sc;
    int hd;
    bool unique;
    vector<string> filterStrings;

private:
    wxPanel* command;

    wxBoxSizer *rootSizer;
    wxButton* addButton;
    wxButton* cancel;
    wxButton* ok;

};


#endif // FILTERDIALOG_H
