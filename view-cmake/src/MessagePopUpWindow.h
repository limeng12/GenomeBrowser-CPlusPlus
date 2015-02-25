#ifndef MESSAGEPOPUPWINDOW_H
#define MESSAGEPOPUPWINDOW_H

#include "wx/popupwin.h"
#include <wx/stattext.h>

class MessagePopupWin:public wxPopupTransientWindow{
public:
    MessagePopupWin(wxWindow *parent,wxPoint pos=wxDefaultPosition,wxSize size=wxDefaultSize);
    virtual ~MessagePopupWin();
    wxStaticText *m_mouseText;
    bool popFlag;

    void Popup(wxWindow *focus=NULL);

    void Dismiss();

    bool isPopup();

private:
    enum{
        MESSAGE_ID=1111
    };
    void OnMouseView(wxMouseEvent &event);


};

#endif // MESSAGEPOPUPWINDOW_H
