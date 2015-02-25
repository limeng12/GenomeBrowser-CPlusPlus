#ifndef TabRead_H
#define TabRead_H

#include <vector>
#include <list>
#include <string>
#include <boost/unordered_map.hpp>
#include <boost/tuple/tuple.hpp>

#include <tabix.h>

class TabRead{

public:
    int chrCol;
    int begPosCol;
    int endPosCol;
    int maxCol;

private:
    //
    bool isGz;
    tabix_t *ttabix;
    std::string fileName;

    //read and sort begin
    //static int times;

    struct DataCom{
        std::string chr;
        uint32_t beginPos;
        uint32_t endPos;
        std::string mdata;


    };

    std::vector<DataCom> data;
    std::list<DataCom> dataCompare;

    std::string gffFile;
    std::vector<std::string> strArr;
    //read and sort end
    //index begin

	long GetFileSize(std::string filename);

    std::vector<boost::unordered_map<uint32_t,std::list<uint32_t> > > gffIndex;

    std::vector<boost::unordered_map<uint32_t,std::list<uint32_t> > > gffIndexBegin;

public:
    boost::unordered_map<std::string,int> nameTable;

    boost::unordered_map<std::string,int> nameChrTable;

    //index begin
    //std::vector<std::string> strArr2;
public:

    TabRead();

    TabRead(int tchrCol,int tbegCol,int tendCol);

    ~TabRead();

    bool testSort(std::string tabGzFile);

    void split(std::string& dataLine,char cut,std::vector<std::string> & arrayString,int n);

    bool readTab(std::string tabFile);

    bool readidx(std::string tabFile);

    bool readFlat(std::string fileName);

    inline bool coorLesser(const DataCom& data1,const DataCom& data2){

    //std::cout<<times++<<std::endl;

    if(data1.chr<data2.chr)
        return true;

    if(data1.chr>data2.chr)
        return false;

    if(data1.beginPos<data2.beginPos)
        return true;

    if(data1.beginPos>data2.beginPos)
        return false;


    return false;

    }

    std::list<std::string> getRegion(std::string chr,int beg,int end);

    std::list<std::string> getRegionIndex(std::string chr,int beg,int end);

    std::list<std::string> getRegionFlat(std::string chr,int begPos,int endPos);

};

#endif // TabRead_H
