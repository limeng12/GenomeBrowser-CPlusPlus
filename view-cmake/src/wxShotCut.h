#ifndef WXSHOTCUT_H
#define WXSHOTCUT_H

#include <vector>

#include <wx/font.h>
#include <wx/colour.h>
#include <wx/pen.h>

class WxShotCut{
    public:
        WxShotCut();

    ~WxShotCut();
    wxFont pen10Pix;
    wxFont pen5Pix;
    wxFont pen7Pix;
    wxFont pen710Pix;
    wxFont pen9Pix;
    wxFont pen15Pix;
    wxFont pen20Pix;
    wxFont pen2010Pix;
    wxFont pen13Pix;
    wxFont pen12Pix;
    wxFont pen11Pix;

    wxPen *transparentPen;
    std::vector<wxColour> wxColourVector;
    std::vector<wxColour> wxColourVectorChr;
    inline wxColour GetSNPColour(int i,unsigned char value){
        if(i==0)
            return wxColour(0,0,value);
        if(i==1)
            return wxColour(0,value,0);
        if(i==2)
            return wxColour(0,0,value);
        if(i==3)
            return wxColour(value,value,value);
}

};

#endif // WXHOTCUT_H
