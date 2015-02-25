#include "wxShotCut.h"

WxShotCut::WxShotCut(){

    pen5Pix=wxFont(wxSize(5,5),wxFONTFAMILY_TELETYPE ,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,wxEmptyString,wxFONTENCODING_DEFAULT);

    pen7Pix=wxFont(wxSize(7,7),wxFONTFAMILY_TELETYPE ,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,wxEmptyString,wxFONTENCODING_DEFAULT);

    pen710Pix=wxFont(wxSize(7,10),wxFONTFAMILY_TELETYPE ,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,wxEmptyString,wxFONTENCODING_DEFAULT);

    pen9Pix=wxFont(wxSize(9,9),wxFONTFAMILY_TELETYPE ,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,wxEmptyString,wxFONTENCODING_DEFAULT);

    pen10Pix=wxFont(wxSize(10,10),wxFONTFAMILY_TELETYPE ,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,wxEmptyString,wxFONTENCODING_DEFAULT);

    pen15Pix=wxFont(wxSize(15,15),wxFONTFAMILY_TELETYPE ,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,wxEmptyString,wxFONTENCODING_DEFAULT);

    pen20Pix=wxFont(wxSize(20,20),wxFONTFAMILY_TELETYPE ,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,wxEmptyString,wxFONTENCODING_DEFAULT);

    pen11Pix=wxFont(wxSize(11,11),wxFONTFAMILY_TELETYPE ,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,wxEmptyString,wxFONTENCODING_DEFAULT);

    pen12Pix=wxFont(wxSize(12,12),wxFONTFAMILY_TELETYPE ,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,wxEmptyString,wxFONTENCODING_DEFAULT);

    pen13Pix=wxFont(wxSize(13,13),wxFONTFAMILY_TELETYPE ,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,wxEmptyString,wxFONTENCODING_DEFAULT);

    pen2010Pix=wxFont(wxSize(10,20),wxFONTFAMILY_TELETYPE ,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,wxEmptyString,wxFONTENCODING_DEFAULT);


    //pen2010Pix=wxFont(wxSize(10,20),wxFONTFAMILY_TELETYPE ,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,wxEmptyString,wxFONTENCODING_DEFAULT);

    wxColourVector.push_back(*wxGREEN);
    wxColourVector.push_back(*wxRED);
    wxColourVector.push_back(*wxBLUE);
    wxColourVector.push_back(*wxYELLOW);
    wxColourVector.push_back(*wxCYAN);
    wxColourVector.push_back(*wxBLACK);
    wxColourVector.push_back(*wxLIGHT_GREY);
    wxColourVector.push_back(*wxBLACK);
    wxColourVector.push_back(wxColour(111,222,72));
    wxColourVector.push_back(wxColour(131,122,132));
    wxColourVector.push_back(wxColour(213,62,121));

    wxColourVectorChr.push_back(wxColour(0,0,255));
    wxColourVectorChr.push_back(wxColour(0,255,0));
    wxColourVectorChr.push_back(wxColour(255,0,0));
    wxColourVectorChr.push_back(wxColour(111,111,111));
    wxColourVectorChr.push_back(wxColour(222,222,222));

    transparentPen =new wxPen(*wxBLACK,1,wxPENSTYLE_TRANSPARENT);


}

WxShotCut::~WxShotCut(){
    delete transparentPen;
}
