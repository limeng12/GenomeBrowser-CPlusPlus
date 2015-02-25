#ifndef BAMFILTER_H
#define BAMFILTER_H

#include <stdint.h>
#include <vector>
#include <string>
#include <boost/tuple/tuple.hpp>
#include <boost/unordered_map.hpp>

#include <bam.h>
#include <wx/panel.h>
#include "stl_help.h"
//this file is for bam file

using std::vector;
using std::string;

class BamFilter{
public:

    BamFilter();
    ~BamFilter();

    int32_t softClipping;
    int32_t hardClipping;
    int32_t lastPos;
    boost::unordered_map<string,int> seqTable;


    bool unique;
    vector<string> tagFilterStrs;
    string lastCigar;
    string lastSeq;
    vector<boost::tuple<string,char, string> > processedFilterStrs;

public:
    bool operator()(bam1_t *b);
    void processTag();

};

#endif // BAMFILTER_H
