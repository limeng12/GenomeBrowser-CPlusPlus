//for future upgrade
//my plot window used to plot more beautiful figure than mpWindow
#ifndef MMPWINDOW_H
#define MMPWINDOW_H

#include "mathplot.h"

class MMpwindow:public mpWindow{
public:
    MMpwindow();
    MMpwindow(wxWindow *parent, wxWindowID id,
                     const wxPoint &pos = wxDefaultPosition,
                     const wxSize &size = wxDefaultSize,
                     long flags = 0);

virtual ~MMpwindow();


};
#endif // MMPWINDOW_H
