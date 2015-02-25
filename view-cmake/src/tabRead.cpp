#include "tabRead.h"
#include <algorithm>
#include <fstream>
#include <cstdlib>

#include <boost/bind.hpp>
#include <boost/unordered_set.hpp>
#include <boost/lexical_cast.hpp>

#include <wx/debug.h>
#include <wx/msgdlg.h>

//int TabRead::times=0;
TabRead::TabRead(){
    isGz=false;
}

long TabRead::GetFileSize(std::string filename)
{
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}


void TabRead::split(std::string& dataLine,char cut,std::vector<std::string> & arrayString,int n){


    //arrayString.clear();
    std::string tmpString;
    std::string::const_iterator it;
    int t=0;
    int index=0;
    for(it=dataLine.begin(); it!=dataLine.end(); it++,index++)
    {
        if(*it!=cut)
            tmpString+=*it;
        else
        {
            if(t==begPosCol||t==endPosCol){// becarefull here
                int x=atoi((char*)tmpString.c_str());
                tmpString=boost::lexical_cast<std::string>(x-1);
                for(int i=0;i<tmpString.length();i++){
                    dataLine[index+i-tmpString.length()]=tmpString[i];
                }

            }
            /*
            if(t==endPosCol){
                int x=atoi((char*)tmpString.c_str());
                tmpString=boost::lexical_cast<std::string>(x-1);
                for(int i=0;i<tmpString.length();i++){
                    dataLine[index+i-tmpString.length()]=tmpString[i];
                }

            }
            */

            arrayString[t]=tmpString;
            tmpString.clear();
            if(++t>=n)
                break;
        }

    }

}

TabRead::TabRead(int tchrCol,int tbegCol,int tendCol){
    chrCol=tchrCol;
    begPosCol=tbegCol;
    endPosCol=tendCol;
    //times=0;
    int maxValue=tchrCol>tbegCol?tchrCol:tbegCol;
    maxValue=maxValue>tendCol?maxValue:tendCol;

    //strArr1.resize(5);
    //strArr2.resize(5);
    strArr.resize(maxValue+1);
    maxCol=maxValue+1;
    isGz=false;
}

TabRead::~TabRead(){

    if(isGz==true)
        ti_close(ttabix);

}

std::list<std::string> TabRead::getRegionFlat(std::string chr,int beg,int end){

    int indexPos=static_cast<uint32_t>(beg/100);
    uint32_t region=(indexPos-1>0?indexPos-1:indexPos)*100;//索引向前移动一百，防止遗漏。尝试而已

    std::list<std::string> resultList;
    std::list<uint32_t>::iterator it;
    if(nameTable.find(chr)==nameTable.end())
        return resultList;

    boost::unordered_set<uint32_t> ensureOnce;
    while(region<=end){
        if(gffIndex[nameTable[chr]].find(region)!=gffIndex[nameTable[chr]].end()){
            for(it=gffIndex[nameTable[chr]][region].begin();it!=gffIndex[nameTable[chr]][region].end();++it){
                //int x=data[*it].endPos;
                //int y=data[*it].beginPos;
                if(ensureOnce.find(*it)!=ensureOnce.end())
                    continue;
                else
                   ensureOnce.insert(*it);

                if((data[*it].endPos)>beg&&(data[*it].beginPos<end))

                    resultList.push_back(data[*it].mdata);
            }
        }

        region+=100;

    }
    return resultList;


}

std::list<std::string> TabRead::getRegionIndex(std::string chr,int beg,int end){
    int index=nameTable[chr];
    ti_iter_t miter=ti_queryi(ttabix,index, beg, end);
    int len;
    // std::string tmpStr;
    std::list<std::string> resultStr;
    //std::string oldGeneid;
    //dc.SetTextForeground(*wxBLACK);
    //int maxLine=0;
    //boost::unordered_map<std::string,int> transcripTable;
    //boost::unordered_map<std::string,int>::iterator transIte;
    //int transcipLine=0;
    //int begPos=(int)(chrPos-rightPos/plotNucliedSize/2);
    //int endPos=(int)(chrPos+rightPos/plotNucliedSize/2);

    while(true){
        const char *str=ti_read(ttabix,miter,&len);
        if(str==NULL)
            break;

        resultStr.push_back(str);

    }

    ti_iter_destroy(miter);

    return resultStr;
}

std::list<std::string> TabRead::getRegion(std::string chr,int beg,int end){
    if(isGz==true)
       return getRegionIndex(chr,beg,end);
    else
        return getRegionFlat(chr,beg,end);

}

bool TabRead::readTab(std::string tabFile){

    fileName=tabFile;
    if(std::string::npos==tabFile.find(".gz")){
        isGz=false;
		long fileSize=GetFileSize(tabFile);
        if(fileSize>700000000||fileSize==-1){

			wxMessageBox(wxString(_T("too larget file: "))+wxString::Format(_T("%ld"),fileSize)+wxString(_T("bytes >200MB please use tabix index gz instead")) );

			return false;
		}
        readFlat(tabFile);
    }

    else{



        readidx(tabFile);
        isGz=true;
    }
return true;

}

bool TabRead::testSort(std::string tabGzFile){

    //BGZF* gzf=bgzf_open((char*)tabGzFile.c_str(), "r");
	//kstring_t *str;

	//str = calloc(1, sizeof(kstring_t));
    //while ((ret = ti_readline(fp, str)) >= 0) {


    //bgzf_close(gzf);
}


bool TabRead::readidx(std::string tabFile){
    std::string testTbi=tabFile+std::string(".tbi");
    std::ifstream testExit;
    testExit.open((char*)testTbi.c_str());
    if(testExit.fail()){
        testExit.close();
        if(tabFile.find("gff")||tabFile.find("gtf")){
            ti_conf_t conf=ti_conf_gff;
            conf.preset |= TI_FLAG_UCSC;
            wxASSERT_MSG(-1!=ti_index_build((char*)(tabFile.c_str()),&conf),"gtf Index can't be build,is it sort?");

        }else if(tabFile.find("vcf")){
            ti_conf_t conf=ti_conf_vcf;
            conf.preset |= TI_FLAG_UCSC;

            wxASSERT_MSG(-1!=ti_index_build((char*)(tabFile.c_str()),&conf),"gtf Index can't be build,is it sort?");

        }
        else if(tabFile.find("bed")){
            ti_conf_t conf=ti_conf_bed;
            conf.preset |= TI_FLAG_UCSC;
            wxASSERT_MSG(-1!=ti_index_build((char*)(tabFile.c_str()),&conf),"gtf Index can't be build,is it sort?");

        }


    }

    else{
        testExit.close();
    }

    ttabix=ti_open((char*)(tabFile.c_str()),(char*)(testTbi.c_str()));
    //gtfIndex.push_back(ttabix);
    ti_lazy_index_load(ttabix);

    int n;
    const char **names = ti_seqname(ttabix->idx, &n);

    for(int i=0;i<n;++i){
        nameTable[names[i]]=i;
        nameChrTable[std::string("chr")+std::string(names[i])]=i;

    }

}

bool TabRead::readFlat(std::string fileName){
    gffFile=fileName;
    std::ifstream gffConn;
    gffConn.open((char*)fileName.c_str());
    if(gffConn.fail())
        return false;

    std::string buff;
    std::vector<std::string> strVector;
    int t=0;

    while(!gffConn.eof()){
        //if(t++>100000)
        //    break;

        getline(gffConn,buff,'\n');
        if(buff.length()==0)
            continue;
        if(buff[0]=='#')
            continue;

        //split(buff,'\t',strVector);
        //data.push_back(buff);
        split(buff,'\t',strArr,maxCol);

        //convert to 0 based coornadite begin
        //boost::regex tmpStr(strArr[begPosCol]);
        //buff=boost::regex_replace(buff,tmpStr,boost::lexical_cast<std::string>(atoi(strArr[begPosCol].c_str())-1));

        //boost::regex tmpStr2(strArr[endPosCol]);
        //buff=boost::regex_replace(buff,tmpStr2,boost::lexical_cast<std::string>(atoi(strArr[endPosCol].c_str())-1));
        //convert coornadite end


        DataCom datam;
        datam.chr=strArr[chrCol];
        datam.beginPos=atoi(strArr[begPosCol].c_str());
        datam.endPos=atoi(strArr[endPosCol].c_str());
        datam.mdata=buff;

        dataCompare.push_back(datam);

    }

    dataCompare.sort(boost::bind<bool>(&TabRead::coorLesser,this,_1,_2));
    //sort(data.begin(),data.end(),std::mef(&TabRead::coorLesser));
    //std::ofstream test("output.txt");

    //std::copy(dataCompare.begin(),dataCompare.end(),std::ostream_iterator<std::string>(test,"\n"));
    std::list<DataCom>::iterator it;
    std::list<uint32_t> tmp;

    int iterTimes=0;int nameIndex=0;
    for(it=dataCompare.begin();it!=dataCompare.end();++it){
        if(nameTable.find(it->chr)==nameTable.end()){

            nameTable[it->chr]=nameIndex;
            nameChrTable[std::string("chr")+it->chr]=nameIndex;

            nameIndex++;
            gffIndex.resize(gffIndex.size()+1);
        }

        data.push_back(*it);

        int ite=it->beginPos;
        while(ite<=it->endPos){
            uint32_t key=(static_cast<uint32_t>(ite/100))*100;

            if(gffIndex[nameTable[it->chr]].find(key)==gffIndex[nameTable[it->chr]].end()){
                tmp.clear();tmp.push_back(iterTimes);
                gffIndex[nameTable[it->chr]][key]=tmp;
            }
            else{

                gffIndex[nameTable[it->chr]][key].push_back(iterTimes);

            }

            ite+=100;
        }


 /*
        uint32_t key=(static_cast<uint32_t>(it->endPos/10))*10;

        if(gffIndex[nameTable[it->chr]].find(key)==gffIndex[nameTable[it->chr]].end()){
            tmp.clear();tmp.push_back(iterTimes);
            gffIndex[nameTable[it->chr]][key]=tmp;
        }
        else{

            gffIndex[nameTable[it->chr]][key].push_back(iterTimes);

        }

        uint32_t key2=(static_cast<uint32_t>(it->beginPos/10))*10;
        if(gffIndex[nameTable[it->chr]].find(key2)==gffIndex[nameTable[it->chr]].end()){
            tmp.clear();tmp.push_back(iterTimes);
            gffIndex[nameTable[it->chr]][key2]=tmp;
        }
        else{

            gffIndex[nameTable[it->chr]][key2].push_back(iterTimes);

        }
*/


        iterTimes++;
    }

    dataCompare.clear();
    //test.close();
    gffConn.close();
    return true;
}
