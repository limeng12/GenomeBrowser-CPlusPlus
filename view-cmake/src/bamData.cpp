#include "bamData.h"
BamData::~BamData(){

    bam_header_destroy(header);
    bam_index_destroy(bamIndex);
    bam_close(bamf);
    //message->Destroy();


}

BamData::BamData(){

}

void BamData::init(){

    wxFileName testBamIndex=wxFileName(bamFileName+".bai");
    std::string bams=bamFileName.ToStdString();
    bamf=bam_open((char*)bams.c_str(),"r");

    if(testBamIndex.Exists()==false){

        wxASSERT_MSG(0==bam_index_build((char*)bams.c_str()),_T("Is it a really Bam file or not sort?"));

    }

    header=bam_header_init();

    header=bam_header_read(bamf);


    for(int i=0;i<header->n_targets;++i){

        bamMap[std::string(header->target_name[i])]=i;;
    }

    for(int i=0;i<header->n_targets;++i){

        bamMapChr[(std::string("chr")+std::string(header->target_name[i]))]=i;//fixed me
    }

    //bam_header_destroy(header);// used bu bam search,can't destroy here

    bamIndex = bam_index_load((char*)bams.c_str());


}
