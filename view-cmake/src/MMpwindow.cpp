#include "MMpwindow.h"

MMpwindow::MMpwindow(wxWindow *parent, wxWindowID id,
                     const wxPoint &pos ,
                     const wxSize &size,
                     long flags ):mpWindow(parent,id,pos,size,flags){

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
    EnableDoubleBuffer(true);
}

MMpwindow::MMpwindow():mpWindow(){
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

}

MMpwindow::~MMpwindow(){


}




