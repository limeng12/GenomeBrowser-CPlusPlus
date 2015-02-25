#ifndef DATAFACTORY_H
#define DATAFACTORY_H

#include <wx/string.h>
#include <wx/filefn.h>
#include <wx/filename.h>

#include <faidx.h>
#include <tabix.h>

#include <boost/unordered_map.hpp>
#include <string>
#include <vector>

#include "tabRead.h"
#include "stl_help.h"
#include "bamFilter.h"

class DataFactory{
private:

public:
    DataFactory();
    ~DataFactory();

    boost::unordered_map<std::string,int> chrAndLength;//chrosome name and length
    boost::unordered_map<std::string,int>::iterator chrite;

    std::vector<TabRead> vcfDatas;

    std::vector<TabRead> gffDatas;

    std::vector<TabRead> gtfDatas;

    std::vector<TabRead> bedDatas;

    boost::unordered_map<std::string,int> faiTable;

    std::vector<faidx_t*> openFais;

    faidx_t* fai;//active fasta index

    //bamfilter
    BamFilter* bamfilter;

private:

    //private variable
    bool filesChanged;
    bool vcfTableInitFlag;
    bool gffTableInitFlag;
    bool gtfTableInitFlag;
    bool bedTableInitFlag;

    std::vector<wxString> bamFiles;
    std::vector<wxString> bedFiles;
    std::vector<wxString> vcfFiles;
    std::vector<wxString> gtfFiles;
    std::vector<wxString> gffFiles;
    wxString fastaFile;
    std::vector<wxString> fastaFiles;

    wxString activeBamFile;
    wxString activeFastaFile;


    //std::vector<faidx_t*> fais;
    std::vector< tabix_t*> vcfIndex;
    std::vector<tabix_t*> gffIndex;
    std::vector<tabix_t*> gtfIndex;
    std::vector<tabix_t*> bedIndex;

public:
    std::vector<wxString> getBamFiles();
    std::vector<wxString> getBedFiles();
    std::vector<wxString> getGffFiles();
    std::vector<wxString> getGtfFiles();
    std::vector<wxString> getVcfFiles();

    void addBamFiles(wxString tbam);
    void addFastaFile(wxString tfa);
    void addGffFiles(wxString tgff);
    void addGtfFiles(wxString tgff);
    void addBedFiles(wxString tbed);
    void addVcfFiles(wxString tvcf);

    size_t getFastaCount();
    size_t getBamCount();
    size_t getGffCount();
    size_t getGtfCount();
    size_t getBedCount();
    size_t getVcfCount();

    wxString getActiveBamFile();
    void setActiveBamFile(wxString tbam);

    wxString getBamFile(size_t i);
    wxString getBedFile(size_t i);
    wxString getGffFile(size_t i);
    wxString getGtfFile(size_t i);
    wxString getFastaFile();
    std::vector<wxString> getFastaFiles();
    wxString getFastaFile(size_t i);
    wxString getVcfFile(size_t i);

    bool getVcfRefreshFlag();
    bool getBedRefreshFlag();
    bool getGffRefreshFlag();
    bool getGtfRefreshFlag();

    void setBedRefreshFlag(bool flag);
    void setGtfRefreshFlag(bool flag);
    void setVcfRefreshFlag(bool flag);
    void setGffRefreshFlag(bool flag);

    size_t getChrLengthSize();
    void setChrLength(boost::unordered_map<std::string,int> tchrLen);
    std::list<std::string> getChrNames();

    //void refreshTable();
    //void refreshBEDTable();
    //void refreshSNPTable();
    //void refreshGFFTable();
    //void refreshGTFTable();

    faidx_t* getFai();
    faidx_t* getFai(std::string chr);
    tabix_t* getVcfIx(size_t i);
    tabix_t* getGffIx(size_t i);
    tabix_t* getGtfIx(size_t i);
    tabix_t* getBedIx(size_t i);

    bool IsFileChanged();
    void setFileUnchanged();

    void setFastaFile(wxString fa);
    void setFastaFiles(std::vector<wxString> fas);
    void setBedFiles(std::vector<wxString> beds);
    void setBamFiles(std::vector<wxString> bams);
    void setGffFiles(std::vector<wxString> gffs);
    void setGtfFiles(std::vector<wxString> gtfs);
    void setVcfFiles(std::vector<wxString> vcfs);

};

#endif // DATAFACTORY_H
