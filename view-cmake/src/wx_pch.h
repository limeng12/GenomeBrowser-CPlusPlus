/***************************************************************
 * Name:      wx_pch.h
 * Purpose:   Header to create Pre-Compiled Header (PCH)
 * Author:    limeng (s6228483@yahoo.com.cn)
 * Created:   2013-05-05
 * Copyright: limeng ()
 * License:
 **************************************************************/

#ifndef WX_PCH_H_INCLUDED
#define WX_PCH_H_INCLUDED
#define WX_PRECOMP

// basic wxWidgets headers
//#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif

#ifdef WX_PRECOMP
    // put here all your rarely-changing header files
    #include <wx/numdlg.h>
    #include <wx/splitter.h>
    #include <wx/treectrl.h>
    #include <wx/imaglist.h>
    #include <wx/vscroll.h>
    #include <wx/filename.h>
    #include <wx/notebook.h>
    #include <wx/textfile.h>
    //#include <wx/choice.h>
    #include <wx/combo.h>

//    #include "bam_filter.h"
    #include "process.h"
    #include "stl_help.h"
    #include "statistic_plot.h"
    #include "plot_window.h"

    //#include <mathplot.h>
    #include <utility>

    //#include "art/xpm/my_bam.xpm"

    #include "art/fileopen.xpm"
    #include "art/filesave.xpm"
    #include "art/htmfoldr.xpm"
    #include "art/folder.xpm"
    #include "art/htmpage.xpm"
    #include "art/home.xpm"

#endif // WX_PRECOMP

#endif // WX_PCH_H_INCLUDED
