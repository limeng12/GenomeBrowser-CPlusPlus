#ifndef BAMDATA_H
#define BAMDATA_H

#include <bam.h>

#include <wx/string.h>
#include <wx/filename.h>
#include <wx/debug.h>

#include <string>
#include <boost/unordered/unordered_map.hpp>

class BamData{
public:
    wxString bamFileName;
    bam_index_t *bamIndex;
    bamFile bamf;
    bam_header_t* header;

    //bam染色体的文件名和它的索引的关系
    boost::unordered_map<std::string,int> bamMap;
    boost::unordered_map<std::string,int> bamMapChr;
    BamData();
    ~BamData();
    void init();


};

#endif // BAMDATA_H

