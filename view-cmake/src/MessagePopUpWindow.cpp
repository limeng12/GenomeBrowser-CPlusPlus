#include "MessagePopUpWindow.h"
MessagePopupWin::MessagePopupWin(wxWindow *parent,wxPoint pos,wxSize size):wxPopupTransientWindow(parent){
    SetBackgroundColour(*wxYELLOW);
    m_mouseText=new wxStaticText(this,MESSAGE_ID,_("---"),pos,size);

    Connect( wxEVT_MOTION,wxMouseEventHandler(MessagePopupWin::OnMouseView));
    popFlag=false;
}

MessagePopupWin::~MessagePopupWin(){


}

void MessagePopupWin::OnMouseView(wxMouseEvent& event){
    //int a=0;
    //int b=0;
   // a=b;
   this->Dismiss();

}

void MessagePopupWin::Popup(wxWindow *focus){
    popFlag=true;
    wxPopupTransientWindow::Popup(focus);

}

void MessagePopupWin::Dismiss(){
    popFlag=false;
    wxPopupTransientWindow::Dismiss();
}

bool MessagePopupWin::isPopup(){
    return popFlag;
}
