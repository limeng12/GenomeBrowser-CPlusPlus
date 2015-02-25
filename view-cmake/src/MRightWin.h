//for future upgrade
#ifndef MRIGHTWIN_H
#define MRIGHTWIN_H

#include <wx/window.h>
#include <wx/dcclient.h>
#include <wx/dcscreen.h>
#include <wx/button.h>

class MRightWin:public wxWindow{
public:
    MRightWin(wxWindow *parent,
                wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& name = wxPanelNameStr);


    void OnMouseView(wxMouseEvent &event);
    void draw(wxSize m);
   // wxButton* loadLastWorkspace;
};

#endif // MRIGHTWIN_H
