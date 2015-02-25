#include "bamFilter.h"
// for easy understang the program ,I didn't design it for best efficiency

BamFilter::BamFilter(){
    unique=false;
    softClipping=0xffff;
    hardClipping=0xffff;
    lastCigar="";
    lastSeq="";
}
BamFilter::~BamFilter(){


}

void BamFilter::processTag(){
    processedFilterStrs.clear();
    for(unsigned int i=0;i<tagFilterStrs.size();++i){
        string::size_type indexxi=0,indexda=0,indexde=0;
        if(std::string::npos!=(indexxi=tagFilterStrs.at(i).find('<')))
            processedFilterStrs.push_back(boost::make_tuple(tagFilterStrs.at(i).substr(0,indexxi),'<',tagFilterStrs.at(i).substr(indexxi+1,tagFilterStrs.at(i).length()-indexxi-1)));

        if(std::string::npos!=(indexda=tagFilterStrs.at(i).find('>')))
            processedFilterStrs.push_back(boost::make_tuple(tagFilterStrs.at(i).substr(0,indexda),'>',tagFilterStrs.at(i).substr(indexda+1,tagFilterStrs.at(i).length()-indexxi-1)));

        if(std::string::npos!=(indexde=tagFilterStrs.at(i).find('=')))
            processedFilterStrs.push_back(boost::make_tuple(tagFilterStrs.at(i).substr(0,indexde),'=',tagFilterStrs.at(i).substr(indexde+1,tagFilterStrs.at(i).length()-indexxi-1)));


    }

}

bool BamFilter::operator()(bam1_t *b){
    string curCigar;

    if(softClipping>0){
        int ns=0,nh=0;int nOper;
        for (size_t i = 0; i < b->core.n_cigar; ++i){

            nOper=bam1_cigar(b)[i]>>BAM_CIGAR_SHIFT;
            uint32_t *cigar = bam1_cigar(b);
            while(nOper-->0){
                //cigarArray<<bam_cigar_opchr(cigar[i]);
                char curOper=bam_cigar_opchr(cigar[i]);
                //int backnDeleteInsert=nDeleteInsert;
                switch(curOper){
                case 'I': break;
                case 'D': break;
                case 'S':ns++;break;
                case 'N':break;
                case 'M': break;
                case 'H':nh++;break;
                }
                //curCigar.append(1,curOper);

            }
        }
        if(ns>=softClipping)
            return false;
    }

    if(unique==true){
        string curSeq;

        if(b->core.pos==lastPos){

            for(int i=0;i<b->core.l_qseq;++i)
                curSeq.append(1,bam_nt16_rev_table[bam1_seqi(bam1_seq(b),i)]);

            if(seqTable.find(curSeq)==seqTable.end()){
                seqTable[curSeq]=1;

            }else{
                return false;
            }

        }
        else{
            seqTable.clear();
            lastPos=b->core.pos;

        }

    }

    for(unsigned int i=0;i<processedFilterStrs.size();++i){
        char* t=(char*)(processedFilterStrs.at(i).get<0>().c_str());

        //char type;
        unsigned char *data=bam_aux_get(b,t);
        if(data==NULL)
            continue;

        switch(static_cast<unsigned char>(*data)){
        case 'i':
                switch(processedFilterStrs.at(i).get<1>()){
                case '=':
                        if(bam_aux2i(data+1)==atoi(processedFilterStrs.at(i).get<2>().c_str())) return false;
                case '<':
                        if(bam_aux2i(data+1)<atoi(processedFilterStrs.at(i).get<2>().c_str())) return false;
                case '>':
                        if(bam_aux2i(data+1)>atoi(processedFilterStrs.at(i).get<2>().c_str())) return false;
                }
                break;

        case 'I':
                switch(processedFilterStrs.at(i).get<1>()){
                case '=':
                        if(bam_aux2i(data+1)==atoi(processedFilterStrs.at(i).get<2>().c_str())) return false;
                case '<':
                        if(bam_aux2i(data+1)<atoi(processedFilterStrs.at(i).get<2>().c_str())) return false;
                case '>':
                        if(bam_aux2i(data+1)>atoi(processedFilterStrs.at(i).get<2>().c_str())) return false;
                }
                break;

        case 's':
                break;

        case 'S':
                break;

        case 'c':


                switch(processedFilterStrs.at(i).get<1>()){

                case '=':
                        if(bam_aux2i(data+1)==atoi(processedFilterStrs.at(i).get<2>().c_str())) return false;
                case '<':
                        if(bam_aux2i(data+1)<atoi(processedFilterStrs.at(i).get<2>().c_str())) return false;
                case '>':
                        int y=atoi(processedFilterStrs.at(i).get<2>().c_str());
                        int x=bam_aux2i(data);
                        //int tt=bam_aux2i(data);
                        if(bam_aux2i(data+1)>atoi(processedFilterStrs.at(i).get<2>().c_str())) return false;
                }
                break;
        case 'C':
                switch(processedFilterStrs.at(i).get<1>()){
                case '=':
                        if(bam_aux2i(data+1)==atoi(processedFilterStrs.at(i).get<2>().c_str())) return false;
                case '<':
                        if(bam_aux2i(data+1)<atoi(processedFilterStrs.at(i).get<2>().c_str())) return false;
                case '>':
                        if(bam_aux2i(data+1)>atoi(processedFilterStrs.at(i).get<2>().c_str())) return false;
                }
                break;

        case 'd':
                switch(processedFilterStrs.at(i).get<1>()){
                case '=':
                        if(bam_aux2d(data+1)==atoi(processedFilterStrs.at(i).get<2>().c_str())) return false;
                case '<':
                        if(bam_aux2d(data+1)<atoi(processedFilterStrs.at(i).get<2>().c_str())) return false;
                case '>':
                        if(bam_aux2d(data+1)>atoi(processedFilterStrs.at(i).get<2>().c_str())) return false;
                }
                break;

        case 'f':
                switch(processedFilterStrs.at(i).get<1>()){
                case '=':
                        if(bam_aux2f(data+1)==atof(processedFilterStrs.at(i).get<2>().c_str())) return false;
                case '<':
                        if(bam_aux2f(data+1)<atof(processedFilterStrs.at(i).get<2>().c_str())) return false;
                case '>':
                        if(bam_aux2f(data+1)>atof(processedFilterStrs.at(i).get<2>().c_str())) return false;
                }
                break;
        case 'A':
                switch(processedFilterStrs.at(i).get<1>()){
                case '=':
                        if(bam_aux2A(data+1)==atoi(processedFilterStrs.at(i).get<2>().c_str())) return false;
                case '<':
                        if(bam_aux2A(data+1)<atoi(processedFilterStrs.at(i).get<2>().c_str())) return false;
                case '>':
                        if(bam_aux2A(data+1)>atoi(processedFilterStrs.at(i).get<2>().c_str())) return false;
                }
                break;
        case 'Z':
                switch(processedFilterStrs.at(i).get<1>()){
                case '=':
                        if(bam_aux2Z(data+1)==(processedFilterStrs.at(i).get<2>().c_str())) return false;
                case '<':
                        if(bam_aux2Z(data+1)<(processedFilterStrs.at(i).get<2>().c_str())) return false;
                case '>':
                        if(bam_aux2Z(data+1)>(processedFilterStrs.at(i).get<2>().c_str())) return false;
                }
                break;

        }


    }


return true;

}

