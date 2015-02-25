/***************************************************************
 * Name:      pilot_viewApp.cpp
 * Purpose:   Code for Application Class
 * Author:    limeng (s6228483@yahoo.com.cn)
 * Created:   2013-05-05
 * Copyright: limeng ()
 * License:
 **************************************************************/
#ifdef __WXGTK__
#include <cstdlib>
int x=putenv("LD_LIBRARY_PATH=./lib");
#endif


#ifdef WX_PRECOMP
//#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "pilot_viewApp.h"
#include "pilot_viewMain.h"

IMPLEMENT_APP(pilot_viewApp);

bool pilot_viewApp::OnInit()
{
    pilot_viewFrame* frame = new pilot_viewFrame(0L, _("wxWidgets Application Template"));

    frame->Show();

    return true;
}
