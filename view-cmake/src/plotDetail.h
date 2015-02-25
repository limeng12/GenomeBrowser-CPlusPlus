#ifndef PLOTDETAIL_H
#define PLOTDETAIL_H

#include <faidx.h>
#include <bam.h>
#include <tabix.h>

#include <string>
#include <cmath>
#include <vector>
#include <map>
#include <boost/unordered_map.hpp>

#include "wxShotCut.h"
#include "wx/dc.h"
#include <wx/debug.h>

#include "dataFactory.h"
#include "bamData.h"
#include "plotBase.h"



class PlotDetail:public PlotBase{
public:
    std::vector<int> convertTable;

    std::vector<float> freQuenctSumTable;

    std::vector<int> frequencyTable;//[4][2000];//500 highest number of bases in the window

    boost::unordered_map<int,string> col;

    boost::unordered_map<int,boost::unordered_map<int,string> > posMessageTable;

    int bamPipeline;

    struct bamRect{
        int leftPos;
        std::string sqname;
        int leftRect;
        int rightRect;
    };

    boost::unordered_map<int,std::vector<bamRect> > bamMessageTable;//the value in the pair is leftmost coor and sequence name;

    PlotDetail(DataFactory* tdata,BamData *tbdata,wxSize tcavanseSize,std::vector<bool>* tcheckV);

    ~PlotDetail();

public:

    int GetFontSize(){
        return plotFontSize;
    }
    int GetNucliedSize(){
        return plotNucliedSize;
    }

    void NucliedSizeChange();


    wxString bamSearch(std::string chr,int chrPos,std::string qname);

    void plotMain(wxDC& dc,std::string chr,int chrPos,int leftPos,int rightPos,int upPos,int downPos);

    void canvasSizeChange(wxSize);

    int plotChr(wxDC& dc,std::string chr,int chrPos,int begPos,int endPos,int index);

    int MbamHelp(bam1_t* b,wxDC& dc,int beg, int end,int &maxLine,int& lastPos,int &lastLine);

    int MbamHelpNOFa(bam1_t* b,wxDC& dc,int beg, int end,int &maxLine,int &lastPos,int &lastLine);

    int MbamHelpPairEnd(bam1_t* b,wxDC& dc,int beg, int end,int& maxLine,int& lastPos,int &lastLine);

    int MbamHelpSingleEnd(bam1_t* b,wxDC& dc,int beg, int end,int& maxLine,int& lastPos,int &lastLine);

    int plotSNP(wxDC& dc,std::string chr,int chrPos,int begPos,int endPos,int index);

    int plotVcf(wxDC& dc,std::string chr,int chrPos,int begPos,int endPos,int vi);
};


#endif // PLOTDETAIL_H
