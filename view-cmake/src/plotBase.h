#ifndef PLOTBASE_H
#define PLOTBASE_H
// for polymorism

#include <bam.h>
#include <tabix.h>

//#include "plot_window.h"
#include <wx/dc.h>
#include "wxShotCut.h"
#include "dataFactory.h"
#include "bamData.h"
#include "plotBase.h"

#include <string>
#include <vector>
#include <boost/unordered_map.hpp>

class PlotBase{

protected:
    DataFactory *mdata;
    BamData *mbamdata;

    std::vector<char> posNulciedTable;
    int pipeLine;//堆积到哪一列了

    WxShotCut wscut;
    std::vector<int> curEndPos;//堆积表，这个变量是为了速度
    //PlotWindow& parent;
    bool viewFaFlag;

    wxSize cavanseSize;
    std::vector<bool> *viewVector;
    //pair end
    boost::unordered_map<string,int> pairendTable;
    boost::unordered_map<int,wxString> numberToNucliedTable;

    int minHeight;
    int height;

public:
    int cavansNucliced;//一行有多少个碱基

    int snpTableLine;
    double plotFontSize;//1个碱基占的像素的二倍，这里，我设置的是二十
    double plotNucliedSize;

public:
    PlotBase(DataFactory* tdata,BamData *tbdata,wxSize tcavanseSize,std::vector<bool>* tcheckV);

    virtual ~PlotBase();

    double GetNucliedSize();

    void SetNucliedSize(double t);

    virtual void NucliedSizeChange();

    virtual void canvasSizeChange(wxSize changedSize)=0;

    virtual void plotMain(wxDC& dc,std::string chr,int chrPos,int leftPos,int rightPos,int upPos,int downPos)=0;

protected:

    //virtual int plotChr(wxDC& dc,std::string chr,int chrPos,int begPos,int endPos,int index);

    int plotCoor(wxDC& dc,std::string chr,int chrPos,int begPos,int endPos,int index);

    int plotGTFExon(wxDC& dc,std::string chr,int chrPos,int begPos,int endPos,int index);///also use plot gff

    int plotGTFCds(wxDC& dc,std::string chr,int chrPos,int begPos,int endPos,int index);///also use plot gff

    int plotGff(wxDC& dc,std::string chr,int chrPos,int begPos,int endPos,int index);

    int plotBed(wxDC& dc,std::string chr,int chrPos,int begPos,int endPos,int index);

    virtual int plotVcf(wxDC& dc,std::string chr,int chrPos,int begPos,int endPos,int index);// must be virtual

    int plotBam(wxDC& dc,std::string chr,int chrPos,int begPos,int endPos,int index);

    int Mbam_fetch(const bam_index_t *idx, int tid, int beg,int end,wxDC& dc);

    virtual int MbamHelp(bam1_t* b,wxDC& dc,int beg, int end,int &maxLine,int& lastPos,int &lastLine);

    virtual int MbamHelpNOFa(bam1_t* b,wxDC& dc,int beg, int end,int &maxLine,int &lastPos,int &lastLine)=0;

    virtual int MbamHelpPairEnd(bam1_t* b,wxDC& dc,int beg, int end,int& maxLine,int& lastPos,int &lastLine)=0;

    virtual int MbamHelpSingleEnd(bam1_t* b,wxDC& dc,int beg, int end,int& maxLine,int& lastPos,int &lastLine)=0;


};


#endif // PLOTBASE_H
