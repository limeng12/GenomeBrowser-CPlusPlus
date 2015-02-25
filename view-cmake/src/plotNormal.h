#ifndef PLOTNORMAL_H
#define PLOTNORMAL_H

#include <bam.h>
#include <tabix.h>

#include <string>
#include <cmath>
#include <vector>
#include <boost/unordered_map.hpp>

#include "wx/dc.h"
#include "wxShotCut.h"
#include "dataFactory.h"
#include "bamData.h"
#include "plotBase.h"


class PlotNormal:public PlotBase{

public:

    PlotNormal(DataFactory* tdata,BamData *tbdata,wxSize tcavanseSize,std::vector<bool>* tcheckV);

    ~PlotNormal();

public:

    void NucliedSizeChange();

    void canvasSizeChange(wxSize);

    void plotMain(wxDC& dc,std::string chr,int chrPos,int leftPos,int rightPos,int upPos,int downPos);

    //int plotVcf(wxDC& dc,std::string chr,int chrPos,int begPos,int endPos,int vi);

    int MbamHelp(bam1_t* b,wxDC& dc,int beg, int end,int &maxLine,int& lastPos,int &lastLine);

    int MbamHelpNOFa(bam1_t* b,wxDC& dc,int beg, int end,int &maxLine,int &lastPos,int &lastLine);

    int MbamHelpPairEnd(bam1_t* b,wxDC& dc,int beg, int end,int& maxLine,int& lastPos,int &lastLine);

    int MbamHelpSingleEnd(bam1_t* b,wxDC& dc,int beg, int end,int& maxLine,int& lastPos,int &lastLine);

};


#endif // PLOTNORMAL_H
