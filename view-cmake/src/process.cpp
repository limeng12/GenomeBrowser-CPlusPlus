#include "process.h"
#include <wx/log.h>
std::map<std::string,float>::iterator ites;

Process::Process(){
    finishFlag=false;
	inteRegion.resize(0);
	coverage.resize(0);

}

Process::~Process(){
	inteRegion.clear();
	regionCount.clear();
	lengthDis.clear();
	mapQuality.clear();
	nm.clear();
	sc.clear();
	coverage.clear();
	codis.clear();
}

void Process::split(const std::string& dataLine,char cut,std::vector<std::string> & arrayString){


    arrayString.clear();
    std::string tmpString;
    std::string::const_iterator it;
    for(it=dataLine.begin(); it!=dataLine.end(); it++)
    {
        if(*it!=cut)
            tmpString+=*it;
        else
        {
            arrayString.push_back(tmpString);
            tmpString.clear();
        }

    }
    arrayString.push_back(tmpString);
    // cout<<arrayString.at(0)<<endl;
    //return arrayString;
}

void Process::getInteRegion(std::string filename){
	std::ifstream fileconn;
	fileconn.open((char*)filename.c_str());
	std::string tmpString;
	std::vector<std::string> arrayString;
	if(fileconn.fail()){
		wxLogError(_T("can't open the file"));
		return ;
	}

	getline(fileconn,tmpString,'\n');//读入文件头
	while(!fileconn.eof()){
		getline(fileconn,tmpString,'\n');
		if(tmpString.length()==0)
			continue;
		split(tmpString,'\t',arrayString);
		int chrn=atoi((arrayString[0].substr(3,arrayString[0].length()-1)).c_str())-1;//获取第几个染色体
		//std::cout<<chrn<<std::endl;
		inteRegion[chrn].insert(std::make_pair<int,int>(atoi(arrayString[1].c_str()),atoi(arrayString[2].c_str())));
#ifdef DEBUG
		//std::cout<<std::atoi(arrayString[1].c_str())<<std::endl;//<<"\t"<<std::atoi(arrayString[2].c_str())<<std::endl;
#endif
	}


	fileconn.close();
}

bool Process::testRegion(int chrn,int position){
	std::map<int,int>::iterator ite;
	char buff[50];
	unsigned int l=0;
	for(ite=inteRegion[chrn].begin();ite!=inteRegion[chrn].end();++ite){
		if((ite->first<=position)&&(ite->second>=position)){
			l=ite->second-ite->first;
			sprintf(buff,"chr%d_%d_%d",chrn+1,ite->first,ite->second);
			ites=regionCount.find(std::string(buff));
			if(ites!=regionCount.end()){
				ites->second=ites->second+1.0/l;
			}
			else{
				regionCount.insert(std::make_pair<std::string,int>(std::string(buff),1.0/l));
			}


			return true;

		}
#ifdef DEBUG
	//	debugfile<<chrn<<"\t"<<ite->first<<"\t"<<position<<"\t"<<ite->second<<std::endl;
#endif
	}
	return false;

}

//测试这个read是否有可能在这个区间里面，如果是，返回true
bool Process::testRegionAppro(int chrn,int position,int length){
	std::map<int,int>::iterator ite;
	for(ite=inteRegion[chrn].begin();ite!=inteRegion[chrn].end();++ite){
		if((ite->first-length<=position)&&(ite->second>=position))
			return true;
#ifdef DEBUG
	//	debugfile<<chrn<<"\t"<<ite->first<<"\t"<<position<<"\t"<<ite->second<<std::endl;
#endif
	}
	return false;


}

void Process::processBam(std::string filename,bool hasbed){
    // Open input stream, BamStream can read SAM and BAM files.
    bamFile bamf=bam_open((char*)filename.c_str(),"r");
    bam_header_t *h=bam_header_init();
    h=bam_header_read(bamf);

    bam_header_destroy(h);
    int maxLength=0;double aveLength=0;
    //std::ofstream resultconn("result");
    unsigned int numberOfRead=0;
    unsigned int numberOfUnmap=0;
    unsigned int numberOfReverseMap=0;
    unsigned int numberOfMultipleMap=0;

    std::map<int,int>::iterator ite;

    std::map<int,int>::iterator iteq;

    std::map<int,int>::iterator itenm;

    std::map<int,int>::iterator itesc;

    //获取每一个点的覆盖的次数
    std::map<long int,int>::iterator itec;

    //std::ofstream test("DEBUG");
    // Copy header.  The header is automatically written out before
    // the first record.
    //bamStreamOut.header = bamStreamIn.header;

    bam1_t *b=bam_init1();

    while (bam_read1(bamf,b)>0){

        if(b->core.flag&0x4)
            numberOfUnmap++;//错配多少个

        if(b->core.flag&0x10)
            numberOfReverseMap++;//比对到反链有多少个

        //if(hasFlagMultiple(record))
        //numberOfMultipleMap++;//多重比对有多少个

        numberOfRead++;//总共有多少个read

        tmpLength=b->core.l_qseq;
        aveLength+=tmpLength;
        if(tmpLength>maxLength)
        maxLength=tmpLength;

        ite=lengthDis.find(tmpLength);
        //length distribution
        if(ite!=lengthDis.end()){
            ite->second++;
        }
        else{
            lengthDis.insert(std::make_pair<int,int>(tmpLength,1));

        }
        if(b->core.flag&0x4)
        continue;

        //quality
        tmpQuality=static_cast<int>(b->core.qual);//std::cout<<tmpQuality<<std::endl;
        iteq=mapQuality.find(tmpQuality);
        if(iteq!=mapQuality.end()){
            iteq->second++;
        }
        else{
            mapQuality.insert(std::make_pair<int,int>(tmpQuality,1));
        }
        //nm
        //seqan::BamTagsDict tagsDict(record.tags);
        //unsigned index=0;
        //bool key=seqan::findTagKey(index,tagsDict,"NM");
        uint8_t* tnm=bam_aux_get(b,"NM");
        tmpNm=bam_aux2i(tnm);

        itenm=nm.find(tmpNm);
        if(nm.end()!=itenm){
                itenm->second++;
        }
        else{
                nm.insert(std::make_pair<int,int>(tmpNm,1));
        }


        //soft clip
        int tmpSc=0;int nOper;
        for (size_t i = 0; i < b->core.n_cigar; ++i) {

            nOper=bam1_cigar(b)[i]>>BAM_CIGAR_SHIFT;
            uint32_t *cigar = bam1_cigar(b);
            while(nOper-->0){
                //cigarArray<<bam_cigar_opchr(cigar[i]);
                char curOper=bam_cigar_opchr(cigar[i]);
                switch(curOper){
                    case 'S':
                    tmpSc++;break;


                }

            }
        }

        itesc=sc.find(tmpSc);
        if(itesc!=sc.end()){
            itesc->second++;
        }

        else{
            sc.insert(std::make_pair<int,int>(tmpSc,1));
        }


        //std::cout<<record.rId<<std::endl;
        //coverage
        //std::cout<<record.rId<<std::endl;
        //cout<<b->core.tid<<endl;
        //if(b->core.tid>24)
         //   continue;

/*
        for(unsigned int i=b->core.pos;i<b->core.pos+bam_cigar2qlen(&(b->core),bam1_cigar(b));++i){

    #ifdef DEBUG
        //		std::cout<<flag<<std::endl;
    #endif
            while(coverage.size()<=b->core.tid)
                coverage.push_back(std::map<long int,int>());

            itec=coverage[b->core.tid].find(i);
            if(itec!=coverage[b->core.tid].end()){
                itec->second++;
            }
            else{
                coverage[b->core.tid].insert(std::make_pair<long int,int>(i,1));
            }

        }
*/
    }

    bam_destroy1(b);
    bam_close(bamf);
/*
    std::cout<<"caculate coverage distribution"<<std::endl;
    //coverage distribution
    std::map<int,int> codis;int tmpCd;
    std::map<int,int>::iterator itecd;

    for(unsigned int i=0;i<coverage.size();++i){

        for(itec=coverage[i].begin();itec!=coverage[i].end();++itec){
            tmpCd=itec->second;
            itecd=codis.find(tmpCd);
            if(itecd!=codis.end()){
                itecd->second++;
            }
            else{
                codis.insert(std::make_pair<int,int>(tmpCd,1));
            }

        }

    }
*/
}

