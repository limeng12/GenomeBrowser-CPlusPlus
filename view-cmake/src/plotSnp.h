#ifndef PLOTSNP_H_INCLUDED
#define PLOTSNP_H_INCLUDED
#include "DataFactory.h"
#include "BamData.h"
#include "wxShotCut.h"
#include "wx/dc.h"
#include "plotBase.h"
#include <boost/bind.hpp>

class PlotSnp:public PlotBase{

public:
    PlotSnp(DataFactory* tdata,BamData* tbamdata,string tchr,int tcurPos,wxSize size);
    ~PlotSnp();

private:

    DataFactory* mdata;
    BamData* bamdata;
    PlotSnp* mplot;
    vector<bam1_t*> reads;
    string chr;
    int curPos;
    int begPosition;
    int endPosition;
    vector<int> convertTable;
    bam_iter_t iter;


public:
    void plot(wxDC &dc,string tchr,int chrPos,int begPos,int endPos);


    int plotChr(wxDC& dc,std::string tchr,int chrPos,int begPos,int endPos,int index);

    wxString seartchAPositon();

    int plotBam(wxDC& dc,std::string chr,int chrPos,int begPos,int endPos,int index);

    //bool compareTwoRead(bam1_t* b1,bam1_t* b2);

    void canvasSizeChange(wxSize changedSize);

    bool compareTwoRead(bam1_t *b1,bam1_t* b2);

	bool getIfMismatch(bam1_t* b);

    void plotMain(wxDC& dc,std::string chr,int chrPos,int leftPos,int rightPos,int upPos,int downPos);

    int MbamHelpNOFa(bam1_t* b,wxDC& dc,int beg, int end,int &maxLine,int &lastPos,int &lastLine);

    int MbamHelpPairEnd(bam1_t* b,wxDC& dc,int beg, int end,int& maxLine,int& lastPos,int &lastLine);

    int MbamHelpSingleEnd(bam1_t* b,wxDC& dc,int beg, int end,int& maxLine,int& lastPos,int &lastLine);




};



#endif // PLOTSNP_H_INCLUDED
