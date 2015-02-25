#ifndef PLOTPROFILE
#define PLOTPROFILE

#include <faidx.h>
#include <bam.h>

#include <tabix.h>
#include "wxShotCut.h"
#include "wx/dc.h"
#include "dataFactory.h"
#include "bamData.h"
#include "plotBase.h"

#include <string>
#include <cmath>
#include <vector>
#include <map>
#include <boost/unordered_map.hpp>

class PlotProfile:public PlotBase{

protected:
    std::vector<int> frequencyPosTable;
    std::vector<int> frequencyNegTable;
    std::vector<int> frequencySumTable;
    std::vector<int> frequencySumPosTable;
    std::vector<int> frequencySumNegTable;

    std::vector<int> convertTable;

    int plotInterval;
    vector<int> maxNuclied;
    int maxSumNuclied;
    int maxSumPosNuclied;
    int maxSumNegNuclied;
    int maxDepth;//最大深度

    int maxPlotValue;//一点最多能画多高
    int readNumInWindow;//一个窗口中最多有多少个读数

public:


    PlotProfile(DataFactory* tdata,BamData *tbdata,wxSize tcavanseSize,std::vector<bool>* tcheckV);

    ~PlotProfile();

public:

    int GetFontSize(){
        return plotFontSize;
    }
    int GetNucliedSize(){
        return plotNucliedSize;
    }

    void NucliedSizeChange();

    void canvasSizeChange(wxSize changed);

    void plotMain(wxDC& dc,std::string chr,int chrPos,int leftPos,int rightPos,int upPos,int downPos);

    //int plotVcf(wxDC& dc,std::string chr,int chrPos,int begPos,int endPos,int index);

    int MbamHelpNOFa(bam1_t* b,wxDC& dc,int beg, int end,int &maxLine,int &lastPos,int &lastLine);

    int MbamHelpPairEnd(bam1_t* b,wxDC& dc,int beg, int end,int& maxLine,int& lastPos,int &lastLine);

    int MbamHelpSingleEnd(bam1_t* b,wxDC& dc,int beg, int end,int& maxLine,int& lastPos,int &lastLine);

    int MbamHelpProfile(bam1_t* b,wxDC& dc,int beg, int end,int& maxLine,int& lastPos,int &lastLine);

    int plotFrequency(wxDC& dc,std::string chr,int chrPos,int begPos,int endPos);

    int plotFrequency(wxDC& dc,std::string chr,int chrPos,int begPos,int endPos,int vi);

};


#endif
