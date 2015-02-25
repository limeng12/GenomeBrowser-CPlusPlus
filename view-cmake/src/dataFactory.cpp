#include "dataFactory.h"
//#include <wx/log.h>
#include <fstream>
//#include <wx/msgdlg.h>
//there is a bug here declartion doesn't declairtiong doesn't declair anything

DataFactory::DataFactory(){

    fastaFile=wxEmptyString;
    vcfTableInitFlag=false;
    gffTableInitFlag=false;
    bedTableInitFlag=false;
    gtfTableInitFlag=false;
    filesChanged=false;
    fai=NULL;
    bamfilter=new BamFilter();

}

DataFactory::~DataFactory(){
    //if(fai!=NULL)
    //    fai_destroy(fai);
    //faiTable
    for(int i=0;i<openFais.size();++i)
        fai_destroy(openFais[i]);

    /*
    boost::unordered_map<std::string,faidx_t*>::iterator ite;
    for(ite=faiTable.begin();ite!=faiTable.end();++ite)
    {
        if(ite->second!=NULL){
            fai_destroy(ite->second);
            ite->second=NULL;
        }
    }
    /*

    /*

    for(size_t i=0;i<vcfIndex.size();++i)
        ti_close(vcfIndex.at(i));
    for(size_t i=0;i<gffIndex.size();++i)
        ti_close(gffIndex.at(i));
    for(size_t i=0;i<bedIndex.size();++i)
        ti_close(bedIndex.at(i));
    for(size_t i=0;i<gtfIndex.size();++i)
        ti_close(gtfIndex.at(i));
    */

    delete bamfilter;

}


void DataFactory::addBamFiles(wxString tbam){
    bamFiles.push_back(tbam);
}

void DataFactory::addFastaFile(wxString tfa){
    filesChanged=true;
    fastaFile=tfa;

    for(size_t i=0;i<fastaFiles.size();++i){
        if(fastaFiles[i]==tfa)
            return;

    }

    fastaFiles.push_back(tfa);
    wxFileName testFaFai=wxFileName(fastaFile+_(".fai"));
    std::string fa=fastaFile.ToStdString();////fiexed me
    if(testFaFai.Exists()==false)
        {

            if (fai_build((char*)fa.c_str()) != 0){  // try to build
                //wxMessageBox("fastaIndex can't be build");  // Error.
                //fixed me
                return;
            }
            fai=fai_load((char*)fa.c_str());
        }
        else{
            fai=fai_load((char*)fa.c_str());

        }

        openFais.push_back(fai);
        //fais.push_back(fai);
                    //file.Close();

        std::ifstream rfai;
        rfai.open((char*)(fa+".fai").c_str());
        if(rfai.fail()){
            //wxMessageBox("can't open fasta index file");
            //fixed me
            return;
        }

        std::vector<std::string> arrStr;
        std::string buff;
        int i=0;
        //chrAndLength.clear();
        while(!rfai.eof()){

            std::getline(rfai,buff,'\n');
            if(buff.length()==0)
                    break;

            split(buff,'\t',arrStr);
            //int tmp=atoi((char*)arrStr[1].c_str());

            chrAndLength.insert(std::make_pair<std::string,int>(arrStr[0],atoi((char*)arrStr[1].c_str())));
            faiTable[arrStr[0]]=openFais.size()-1;
            //chromosomeChoice->SetString(i,wxString(arrStr[0]));
            //chromosomeChoice->AppendText (wxString(arrStr[0]));
            //popupCtrl->InsertItem(popupCtrl->GetItemCount(),wxString(arrStr[0]));
            i++;
        }


        rfai.close();


}

void DataFactory::addGtfFiles(wxString tgff){
    gtfTableInitFlag=false;
    filesChanged=true;

    for(size_t i=0;i<gtfFiles.size();++i){
        if(gtfFiles[i]==tgff)
            return;
    }


    gtfFiles.push_back(tgff);
    gtfDatas.push_back(TabRead(0,3,4));
    gtfDatas[gtfDatas.size()-1].readTab(tgff.ToStdString());

/*
    wxFileName testTbi=tgff+".tbi";
    if(!testTbi.Exists()){
        wxASSERT_MSG(-1!=ti_index_build((char*)(tgff.ToStdString().c_str()),&ti_conf_gff),_T("gtf Index can't be build,is it sort?"));
    }
    tabix_t *ttabix=ti_open((char*)(tgff.ToStdString().c_str()),(char*)((tgff+".tbi").ToStdString().c_str()));
    gtfIndex.push_back(ttabix);
    ti_lazy_index_load(ttabix);
*/
}

void DataFactory::addGffFiles(wxString tgff){

    for(size_t i=0;i<gffFiles.size();++i){
        if(gffFiles[i]==tgff)
            return;
    }

    gffTableInitFlag=false;
    filesChanged=true;
    gffFiles.push_back(tgff);
    gffDatas.push_back(TabRead(0,3,4));
    gffDatas[gffDatas.size()-1].readTab(tgff.ToStdString());

/*
    wxFileName testTbi=tgff+".tbi";
    if(!testTbi.Exists()){
         wxASSERT_MSG(-1!=ti_index_build((char*)(tgff.ToStdString().c_str()),&ti_conf_gff),_T("gtf Index can't be build,is it sort?"));
    }
    tabix_t *ttabix=ti_open((char*)(tgff.ToStdString().c_str()),(char*)((tgff+".tbi").ToStdString().c_str()));
    gffIndex.push_back(ttabix);
    ti_lazy_index_load(ttabix);
*/
}

void DataFactory::addBedFiles(wxString tbed){
    for(size_t i=0;i<bedFiles.size();++i){
        if(bedFiles[i]==tbed)
            return;
    }

    bedTableInitFlag=false;
    filesChanged=true;
    bedFiles.push_back(tbed);

    bedDatas.push_back(TabRead(0,1,2));
    bedDatas[bedDatas.size()-1].readTab(tbed.ToStdString());

    /*
    wxFileName testTbi=tbed+".tbi";
    if(!testTbi.Exists()){
        wxASSERT_MSG(-1!=ti_index_build((char*)(tbed.ToStdString().c_str()),&ti_conf_bed),_T("gtf Index can't be build,is it sort?"));
    }
    tabix_t *ttabix=ti_open((char*)(tbed.ToStdString().c_str()),(char*)((tbed+".tbi").ToStdString().c_str()));
    bedIndex.push_back(ttabix);
    ti_lazy_index_load(ttabix);
*/
}

void DataFactory::addVcfFiles(wxString tvcf){
    for(size_t i=0;i<vcfFiles.size();++i){
        if(vcfFiles[i]==tvcf)
            return;
    }

    vcfTableInitFlag=false;
    filesChanged=true;

    vcfFiles.push_back(tvcf);
    //int x=gffDatas.size();

    vcfDatas.push_back(TabRead(0,1,1));
    vcfDatas[vcfDatas.size()-1].readTab(tvcf.ToStdString());

    /*
    wxFileName testTbi=wxString(tvcf)+".tbi";
    if(!testTbi.Exists()){
        wxASSERT_MSG(-1!=ti_index_build((char*)(wxString(tvcf).ToStdString().c_str()),&ti_conf_vcf),_T("gtf Index can't be build,is it sort?"));

    }

    tabix_t *ttabix=ti_open((char*)(tvcf.ToStdString().c_str()),(char*)((tvcf+".tbi").ToStdString().c_str()));
    vcfIndex.push_back(ttabix);
    ti_lazy_index_load(ttabix);
*/
}

size_t DataFactory::getBamCount(){
    return bamFiles.size();
}

size_t DataFactory::getFastaCount(){
    return fastaFiles.size();
}

size_t DataFactory::getGtfCount(){
    return gtfFiles.size();

}

size_t DataFactory::getGffCount(){
    return gffFiles.size();

}

size_t DataFactory::getVcfCount(){
    return vcfFiles.size();

}

size_t DataFactory::getBedCount(){
    return bedFiles.size();

}

wxString DataFactory::getActiveBamFile(){
return activeBamFile;
}

void DataFactory::setActiveBamFile(wxString tbam){
    activeBamFile=tbam;
}

wxString DataFactory::getBamFile(size_t i){
    return bamFiles[i];
}

wxString DataFactory::getFastaFile(){
    return fastaFile;

}

wxString DataFactory::getFastaFile(size_t i){
    return fastaFiles[i];
}

std::vector<wxString> DataFactory::getFastaFiles(){
    return fastaFiles;

}

void DataFactory::setFastaFiles(std::vector<wxString> fas){
    for(unsigned int i=0;i<fas.size();++i)
        addFastaFile(fas[i]);
}

wxString DataFactory::getVcfFile(size_t i){
    return vcfFiles[i];

}

wxString DataFactory::getGtfFile(size_t i){
    return gtfFiles[i];
}

wxString DataFactory::getGffFile(size_t i){
    return gffFiles[i];
}

wxString DataFactory::getBedFile(size_t i){
    return bedFiles[i];
}

bool DataFactory::getVcfRefreshFlag(){
    return vcfTableInitFlag;
}

bool DataFactory::getGffRefreshFlag(){
    return gffTableInitFlag;
}

bool DataFactory::getGtfRefreshFlag(){
    return gtfTableInitFlag;
}

bool DataFactory::getBedRefreshFlag(){
    return bedTableInitFlag;
}

void DataFactory::setVcfRefreshFlag(bool flag){
    vcfTableInitFlag=flag;
}

void DataFactory::setGffRefreshFlag(bool flag){
    gffTableInitFlag=flag;
}

void DataFactory::setGtfRefreshFlag(bool flag){
    gtfTableInitFlag=flag;
}

void DataFactory::setBedRefreshFlag(bool flag){
    bedTableInitFlag=flag;
}

size_t DataFactory::getChrLengthSize(){
    return chrAndLength.size();
}

void DataFactory::setChrLength(boost::unordered_map<std::string,int> tchrLen){
    chrAndLength=tchrLen;

}

std::list<std::string> DataFactory::getChrNames(){
    std::list<std::string> chrNames;
    boost::unordered_map<std::string,int>::iterator ite;
    for(ite=chrAndLength.begin();ite!=chrAndLength.end();++ite){
        ite->first;
        chrNames.push_back(ite->first);
    }

    chrNames.sort();
    return chrNames;
}


faidx_t* DataFactory::getFai(){
    return fai;
}

faidx_t* DataFactory::getFai(std::string chr){
    return openFais[faiTable[chr]];
}

tabix_t* DataFactory::getVcfIx(size_t i){
    return vcfIndex.at(i);
}

tabix_t* DataFactory::getGffIx(size_t i){
    return gffIndex.at(i);
}

tabix_t* DataFactory::getGtfIx(size_t i){
    return gtfIndex.at(i);
}

tabix_t* DataFactory::getBedIx(size_t i){
    return bedIndex.at(i);
}

bool DataFactory::IsFileChanged(){
    return filesChanged;
}

void DataFactory::setFileUnchanged(){
    filesChanged=false;
}

std::vector<wxString> DataFactory::getBamFiles(){
return bamFiles;
}

std::vector<wxString> DataFactory::getBedFiles(){
return bedFiles;
}

std::vector<wxString> DataFactory::getGffFiles(){
return gffFiles;
}

std::vector<wxString> DataFactory::getGtfFiles(){
return gtfFiles;
}

std::vector<wxString> DataFactory::getVcfFiles(){
    return vcfFiles;
}

void DataFactory::setFastaFile(wxString fa){
    if(fa!=wxEmptyString)
        addFastaFile(fa);
}

void DataFactory::setBamFiles(std::vector<wxString> bams){
    for(unsigned int i=0;i<bams.size();++i){
        //std::string tstr=bams[i].ToStdString();
        addBamFiles(bams[i]);
    }
}

void DataFactory::setBedFiles(std::vector<wxString> beds){
    for(size_t i=0;i<beds.size();++i){
     //std::string tstr=beds[i].ToStdString();
        addBedFiles(beds[i]);
    }
}

void DataFactory::setGffFiles(std::vector<wxString> gffs){
    for(size_t i=0;i<gffs.size();++i){
       // std::string tstr=gffs[i].ToStdString();
        addGffFiles(gffs[i]);
    }
}

void DataFactory::setGtfFiles(std::vector<wxString> gtfs){
    for(size_t i=0;i<gtfs.size();++i){
     //std::string tstr=gtfs[i].ToStdString();
        addGtfFiles(gtfs[i]);
    }
}

void DataFactory::setVcfFiles(std::vector<wxString> vcfs){
    for(size_t i=0;i<vcfs.size();++i){
     //std::string tstr=vcfs[i].ToStdString();
        addVcfFiles(vcfs[i]);
    }
}
