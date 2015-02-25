#include "MouseWin.h"
MouseWin::MouseWin(wxWindow *parent,
                wxWindowID id,
                const wxPoint& pos ,
                const wxSize& size ,
                long style ,
                const wxString& name ):wxWindow(){



                //SetBackgroundStyle(wxBG_STYLE_TRANSPARENT);
                Create(parent,id,pos,size,style,name);
                //bool f=	IsTransparentBackgroundSupported();
                //SetForegroundColour(wxTransparentColour );
 //   Connect( wxEVT_MOTION,wxMouseEventHandler(MouseWin::OnMouseView));
}

void MouseWin::OnMouseView(wxMouseEvent& event){
  //  wxClientDC dc(this);
  //  dc.Clear();
  //  dc.SetBackgroundMode( wxTRANSPARENT);
  //  wxSize tmp=dc.GetSize();
    //wxScreenDC dc2;
   // dc.SetBrush(*wxTRANSPARENT_BRUSH);

  //  dc.DrawLine(event.GetX(),0,event.GetX(),tmp.y);
  //  dc.DrawLine(0,event.GetY(),tmp.x,event.GetY());
}

void MouseWin::draw(wxSize m){
    //wxClientDC dc(this);
    //dc.Clear();
   // wxSize tmp=dc.GetSize();
    //wxScreenDC dc2;
    //dc2.Clear();
    //dc2.DrawLine(m.GetX(),0,m.GetX(),tmp.y);
    //dc2.DrawLine(0,m.GetY(),tmp.x,m.GetY());

}



