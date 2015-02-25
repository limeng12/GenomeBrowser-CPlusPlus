//for future upgrade
// on the top of plotWindow is transparent,not used now ,because gtk<2.9 doesn't support the
//transparent background
#ifndef MOUSEWIN_H
#define MOUSEWIN_H

#include <wx/window.h>
#include <wx/dcclient.h>
#include <wx/dcscreen.h>

class MouseWin:public wxWindow{
public:
    MouseWin(wxWindow *parent,
                wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& name = wxPanelNameStr);


    void OnMouseView(wxMouseEvent &event);
    void draw(wxSize m);
};

#endif // MOUSEWIN_H
