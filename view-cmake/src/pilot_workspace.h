#ifndef PILOT_WORKSPACE_H
#define PILOT_WORKSPACE_H

#include <wx/xml/xml.h>
#include <string>
#include <vector>
#include <wx/filefn.h>
#include <wx/string.h>
#include <map>

class PlotWindowProperty{
public:
    std::string chr;
    std::string bam;
    long pos;
    int page;
};


class Pilot_WorkSpace{
public:

    wxXmlDocument workSpace;

    std::vector<wxString> fastaFiles;

    std::vector<wxString> bamFiles;
    std::vector<wxString> bedFiles;
    std::vector<wxString> gffFiles;
    std::vector<wxString> gtfFiles;
    std::vector<wxString> vcfFiles;
    std::map<wxString,int> strTable;

    std::vector<PlotWindowProperty> windowProperty;
    void writeWorkSpace();
    void readWorkSpaceNew();
    void readWorkSpace();
};



#endif // PILOT_WORKSPACE_H
