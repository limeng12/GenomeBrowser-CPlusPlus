#include "MRightWin.h"
MRightWin::MRightWin(wxWindow *parent,
                wxWindowID id,
                const wxPoint& pos ,
                const wxSize& size ,
                long style ,
                const wxString& name ):wxWindow(parent,id,pos,size,style,name){


    //Connect( wxEVT_MOTION,wxMouseEventHandler(MRightWin::OnMouseView));
    //Connect(wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEvent());
}

void MRightWin::OnMouseView(wxMouseEvent& event){
    //wxClientDC dc(this);
   // dc.Clear();
    //wxSize tmp=dc.GetSize();
    //wxScreenDC dc2;
    //dc.DrawLine(event.GetX(),0,event.GetX(),tmp.y);
    //dc.DrawLine(0,event.GetY(),tmp.x,event.GetY());
}

void MRightWin::draw(wxSize m){
    //wxClientDC dc(this);
    //dc.Clear();
   // wxSize tmp=dc.GetSize();
    //wxScreenDC dc2;
    //dc2.Clear();
    //dc2.DrawLine(m.GetX(),0,m.GetX(),tmp.y);
    //dc2.DrawLine(0,m.GetY(),tmp.x,m.GetY());

}
