#include "plotNormal.h"

PlotNormal::PlotNormal(DataFactory* tdata,BamData* tbdata,wxSize tcavanseSize,std::vector<bool>* tvector)
:PlotBase(tdata,tbdata,tcavanseSize,tvector){

    //posNulciedTable=new char[200];//
    //200是一行染色体可能的最多的数目
    minHeight=3;

}

PlotNormal::~PlotNormal(){


}

void PlotNormal::NucliedSizeChange(){

}

void PlotNormal::canvasSizeChange(wxSize changedSize){

}


void PlotNormal::plotMain(wxDC& dc,std::string chr,int chrPos,int leftPos,int rightPos,int upPos,int downPos){
    //for snp frequence drawing

    int begPos=(int)(chrPos-rightPos/plotNucliedSize/2);
    int endPos=(int)(chrPos+rightPos/plotNucliedSize/2);
    if(begPos<0){
        begPos=0;
        endPos=begPos+rightPos/plotNucliedSize;
    }
    if(endPos>mdata->chrAndLength[chr]){
        endPos=mdata->chrAndLength[chr];
        begPos=endPos-rightPos/plotNucliedSize;
    }


    height=(plotNucliedSize>minHeight?plotNucliedSize:minHeight);

    pairendTable.clear();
    pipeLine=0;unsigned int i=0;
    pipeLine+=plotCoor(dc,chr,chrPos,begPos,endPos,0);
    dc.SetFont(wxFont(1*(height-1),wxFONTFAMILY_TELETYPE,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL));

    if(mdata->getFastaFile()!=wxEmptyString){
        if(viewVector->at(i++)){
//            pipeLine+=plotChr(dc,chr,chrPos,begPos,endPos,0);

            viewFaFlag=true;
        }else
        viewFaFlag=false;

    }
    //dc.SetFont(wscut.pen9Pix);

    for(size_t j=0;j<mdata->getGtfCount();++j,++i){
        if(viewVector->at(i)){
            //gffTable.clear();
            dc.SetTextForeground(*wxBLACK);

            pipeLine+=plotGTFExon(dc,chr,chrPos,begPos,endPos,j);///also use plot gff//fixed me
            pipeLine+=plotGTFCds(dc,chr,chrPos,begPos,endPos,j);///also use plot gff


        }
    }

    for(size_t j=0;j<mdata->getGffCount();++j,++i){
        if(viewVector->at(i)){
            //gffTable.clear();
            dc.SetTextForeground(*wxBLACK);
            dc.SetBrush(*wxGREEN_BRUSH);

            pipeLine+=plotGff(dc,chr,chrPos,begPos,endPos,j);///also use plot gff

        }
    }

    for(size_t j=0;j<mdata->getBedCount();++j,++i){
        if(viewVector->at(i)){
            dc.SetTextForeground(*wxBLACK);
            pipeLine+=plotBed(dc,chr,chrPos,begPos,endPos,j);///also use plot gff
           //fixed me

        }
    }

    for(size_t j=0;j<mdata->getVcfCount();++j,++i){
        if(viewVector->at(i)){
            dc.SetTextForeground(wscut.wxColourVector[j]);
            pipeLine+=plotVcf(dc,chr,chrPos,begPos,endPos,j);

        }
    }

    if(viewVector->at(i)){
        pipeLine+=plotNucliedSize*2;
        plotBam(dc,chr,chrPos,begPos,endPos,0);
//        pipeLine-=20;
//       plotSNP(dc,chr,chrPos,begPos,endPos,0);
    }


}

int PlotNormal::MbamHelp(bam1_t* b,wxDC& dc,int beg, int end,int &maxLine,int& lastPos,int &lastLine){
    int tlastLine=0;
    if(maxLine>=99)
    	return maxLine;

    switch(b->core.flag&0x0001){
    case 0:
        tlastLine=MbamHelpSingleEnd(b,dc,beg,end,maxLine,lastPos,lastLine);
        break;
    case 1:
        tlastLine=MbamHelpPairEnd(b,dc,beg,end,maxLine,lastPos,lastLine);
        break;

    }

    if(tlastLine>maxLine)
        maxLine=tlastLine;
    return maxLine;

}

int PlotNormal::MbamHelpSingleEnd(bam1_t* b,wxDC& dc,int beg, int end,int& maxLine,int& lastPos,int &lastLine){
    if(!mdata->bamfilter->operator()(b))
        return lastLine;

    int relativePos=(b)->core.pos-beg>0?(b)->core.pos-beg:0;
    unsigned int length=(b)->core.l_qseq;

    //if(relativePos>2)
    //{
    //    int xx=1201;
    //    relativePos=relativePos;

    //}

 	if(((b)->core.flag)&0x10)
        dc.SetBrush(*wxBLUE_BRUSH);//complentary
    else
         dc.SetBrush(*wxYELLOW_BRUSH);

    std::vector<std::pair<int,char> > drawSnp;


    int sum=0;
    int nOper=0;
    int nDeleteInsert=0;//xulie de chang du

    uint32_t *cigar = bam1_cigar(b);
    for (size_t i = 0; i < b->core.n_cigar; ++i) {

        nOper=bam1_cigar(b)[i]>>BAM_CIGAR_SHIFT;
        while(nOper-->0){
            //cigarArray<<bam_cigar_opchr(cigar[i]);
            char curOper=bam_cigar_opchr(cigar[i]);
            int backnDeleteInsert=nDeleteInsert;
            switch(curOper){
            case 'I': nDeleteInsert--;break;
            case 'D': nDeleteInsert++;break;
            case 'S':break;
            case 'N':nDeleteInsert++;break;
            case 'M': break;

            }

            if(b->core.pos+sum>end)
                goto End;
            if(b->core.pos+sum>=beg){
                //int indexRelativePos=b->core.pos+sum-beg;
                int seqPos=sum-backnDeleteInsert;
                if(seqPos>=length)
                    goto End;
                //char tstr=bam_nt16_rev_table[bam1_seqi(bam1_seq(b),seqPos)];

                switch(curOper){
                    case 'I':
                        //drawSnp.push_back(std::make_pair(indexRelativePos,'I')); ;break;
                    case 'D':
                        //drawSnp.push_back(std::make_pair(indexRelativePos,'D')); break;
                    case 'S':
                        //drawSnp.push_back(std::make_pair(indexRelativePos,'S'));break;
                    case 'N':
                        //drawSnp.push_back(std::make_pair(indexRelativePos,'N')); break;
                    case 'X':

                        //drawSnp.push_back(std::make_pair(indexRelativePos,'X'));break;
                    case 'M':

//                        frequencyTable.at(convertTable.at(tstr)*cavansNucliced+indexRelativePos)++;
//                       freQuenctSumTable[indexRelativePos]++;
//                        if(tstr!=posNulciedTable[indexRelativePos]&&tstr!=posNulciedTable[indexRelativePos]-32&&tstr!=posNulciedTable[indexRelativePos]+32){
//                            drawSnp.push_back(std::make_pair(indexRelativePos,tstr));

  //                      }

                        break;
                }

            }
            sum++;

        }
    }
    End:
    length+=nDeleteInsert;

    //if(lastLine>=99)
    //    return lastLine;

/*
    if(lastPos==(b)->core.pos){
        lastLine++;

        int ttmp=(beg)>((b)->core.pos)?(beg-(b)->core.pos):0;//the rectangle length
        dc.DrawRectangle(relativePos*plotNucliedSize,lastLine*plotNucliedSize*2+pipeLine,(length-ttmp)*(plotNucliedSize),plotNucliedSize*2.005);

        for(size_t j=0;j<drawSnp.size();++j)
            dc.DrawText(drawSnp[j].second,(drawSnp[j].first)*(plotNucliedSize),lastLine*plotNucliedSize*2+pipeLine);

        curEndPos[lastLine]=b->core.pos+length-beg;

        return lastLine;

    }
*/

    for(int i=1;i<100;++i)


        if(curEndPos[i]<=relativePos){

            int ttmp=(beg)>((b)->core.pos)?(beg-(b)->core.pos):0;//the rectangle length
            dc.DrawRectangle(relativePos*plotNucliedSize,i*height*2+pipeLine,(length-ttmp)*(plotNucliedSize),(height)*2.005);



//            for(size_t j=0;j<drawSnp.size();++j)
//                dc.DrawText(drawSnp[j].second,(drawSnp[j].first)*(plotNucliedSize),i*plotNucliedSize*2+pipeLine);

            curEndPos[i]=b->core.pos+length-beg;

            //if(i==99)
            //    overPos=curEndPos[i];

            lastPos=(b)->core.pos;
            lastLine=i;

            break;
        }

    return lastLine;
}

int PlotNormal::MbamHelpPairEnd(bam1_t* b,wxDC& dc,int beg, int end,int& maxLine,int& lastPos,int &lastLine){
    if(!mdata->bamfilter->operator()(b))
        return lastLine;

    int relativePos=(b)->core.pos-beg>0?(b)->core.pos-beg:0;
    unsigned int length=(b)->core.l_qseq;

 	if(((b)->core.flag)&0x10)
        dc.SetBrush(*wxBLUE_BRUSH);//complentary
    else
         dc.SetBrush(*wxYELLOW_BRUSH);

    std::vector<std::pair<int,char> > drawSnp;


    int sum=0;
    int nOper=0;
    int nDeleteInsert=0;//xulie de chang du

    uint32_t *cigar = bam1_cigar(b);
    for (size_t i = 0; i < b->core.n_cigar; ++i) {

        nOper=bam1_cigar(b)[i]>>BAM_CIGAR_SHIFT;
        while(nOper-->0){
            //cigarArray<<bam_cigar_opchr(cigar[i]);
            char curOper=bam_cigar_opchr(cigar[i]);
            int backnDeleteInsert=nDeleteInsert;
            switch(curOper){
            case 'I': nDeleteInsert--;break;
            case 'D': nDeleteInsert++;break;
            case 'S':break;
            case 'N':nDeleteInsert++;break;
            case 'M': break;

            }

            if(b->core.pos+sum>end)
                goto End;
            if(b->core.pos+sum>=beg){
                //int indexRelativePos=b->core.pos+sum-beg;
                int seqPos=sum-backnDeleteInsert;
                if(seqPos>=length)
                    goto End;
                //char tstr=bam_nt16_rev_table[bam1_seqi(bam1_seq(b),seqPos)];

                switch(curOper){
                    case 'I':
                        //drawSnp.push_back(std::make_pair(indexRelativePos,'I')) ;break;
                    case 'D':
 //                       drawSnp.push_back(std::make_pair(indexRelativePos,'D')); break;
                    case 'S':
 //                       drawSnp.push_back(std::make_pair(indexRelativePos,'S'));break;
                    case 'N':
 //                       drawSnp.push_back(std::make_pair(indexRelativePos,'N')); break;
                    case 'X':

  //                      drawSnp.push_back(std::make_pair(indexRelativePos,'X'));break;
                    case 'M':break;

//                        frequencyTable.at(convertTable.at(tstr)*cavansNucliced+indexRelativePos)++;
//                       freQuenctSumTable[indexRelativePos]++;
 //                       if(tstr!=posNulciedTable[indexRelativePos]&&tstr!=posNulciedTable[indexRelativePos]-32&&tstr!=posNulciedTable[indexRelativePos]+32){
 //                           drawSnp.push_back(std::make_pair(indexRelativePos,tstr));

 //                       }

//                        break;
                }

            }
            sum++;

        }
    }
    End:
    length+=nDeleteInsert;

    //if(lastLine>=99)
    //    return lastLine;

    int mateSize=b->core.isize;
    int matePos=b->core.mpos;//+mateSize;
    int line=0;
    if(pairendTable.find(std::string(bam1_qname(b)))!=pairendTable.end()){
       line=pairendTable[std::string(bam1_qname(b))];
        if(b->core.pos==matePos)
            return lastLine;
        int ttmp=(beg)>((b)->core.pos)?(beg-(b)->core.pos):0;//the rectangle length
        dc.DrawRectangle(relativePos*plotNucliedSize,line*height*2+pipeLine,(length-ttmp)*(plotNucliedSize),(height)*2.005);

        //dc.DrawText(wxString(bam1_qname(b)),relativePos*plotNucliedSize,line*plotNucliedSize*2+pipeLine);
        //for(size_t j=0;j<drawSnp.size();++j)
        //    dc.DrawText(drawSnp[j].second,(drawSnp[j].first)*(plotNucliedSize),line*plotNucliedSize*2+pipeLine);

        //dc.DrawLine(curEndPos[line]*plotNucliedSize,line*plotNucliedSize*2+pipeLine+10,relativePos*plotNucliedSize,line*plotNucliedSize*2+pipeLine+10);
        //dc.DrawLine(((b->core.pos-beg)>0?b->core.pos-beg:0)*plotNucliedSize,line*plotNucliedSize*2+pipeLine+10,relativePos*plotNucliedSize,line*plotNucliedSize*2+pipeLine+10);
        if(matePos+b->core.l_qseq-beg<relativePos)//这里使用了当前序列长度来代替mate序列长度
            dc.DrawLine((matePos+b->core.l_qseq-beg)*plotNucliedSize,line*height*2+pipeLine+height,relativePos*plotNucliedSize,line*height*2+pipeLine+height);//这里使用了当前序列长度来代替mate序列长度


        curEndPos[line]=b->core.pos+length-beg;//>end-beg?end-beg:relativePos+length;

        return lastLine;
    }


//这里设计的有问题，妈的
    if(mateSize<0){
        maxLine++;
        if(maxLine>100)
            return maxLine;

        pairendTable[std::string(bam1_qname(b))]=maxLine;

        int ttmp=(beg)>((b)->core.pos)?(beg-(b)->core.pos):0;//the rectangle length
        dc.DrawRectangle(relativePos*plotNucliedSize,maxLine*height*2+pipeLine,(length-ttmp)*(plotNucliedSize),(height)*2.005);

        //for(size_t j=0;j<drawSnp.size();++j)
        //    dc.DrawText(drawSnp[j].second,(drawSnp[j].first)*(plotNucliedSize),line*plotNucliedSize*2+pipeLine);


        if(beg<b->core.pos){
            int lineBegin=beg>matePos+b->core.l_qseq?beg:matePos+b->core.l_qseq;//这里使用了当前序列长度来代替mate序列长度
            //if(lineBegin-beg<relativePos)
            dc.DrawLine((lineBegin-beg)*plotNucliedSize,maxLine*height*2+pipeLine+height,relativePos*plotNucliedSize,maxLine*height*2+pipeLine+height);

                //curEndPos[lastLine]+=length-(beg-(b)->core.pos);
        }

        curEndPos[maxLine]=b->core.pos+length-beg;
        lastLine=maxLine;
        lastPos=b->core.pos;
        return lastLine;

    }


/*
    if(lastPos==(b)->core.pos){
        lastLine++;
        pairendTable[std::string(bam1_qname(b))]=lastLine;

        int ttmp=(beg)>((b)->core.pos)?(beg-(b)->core.pos):0;//the rectangle length
        dc.DrawRectangle(relativePos*plotNucliedSize,lastLine*plotNucliedSize*2+pipeLine,(length-ttmp)*(plotNucliedSize),plotNucliedSize*2.005);

        if(mateSize>0){
            if(end>b->core.pos+length){
                //int lineEnd=end<matePos?end:matePos;
                //if(relativePos+length<lineEnd-beg)
                dc.DrawLine((b->core.pos+length-beg)*plotNucliedSize,lastLine*plotNucliedSize*2+pipeLine+10,(matePos-beg)*plotNucliedSize,lastLine*plotNucliedSize*2+pipeLine+10);
                    //if(lineEnd-beg+101>b->core.pos+length-beg)
                curEndPos[lastLine]=matePos-beg+101;
                    //else
                    //    curEndPos[lastLine]=b->core.pos+length-beg;
            }
        }

        else if(mateSize==0){
                //curEndPos[lastLine]+=length-(beg-(b)->core.pos);
                curEndPos[lastLine]=b->core.pos+length-beg;

            }

        for(size_t j=0;j<drawSnp.size();++j)
            dc.DrawText(drawSnp[j].second,(drawSnp[j].first)*(plotNucliedSize),lastLine*plotNucliedSize*2+pipeLine);

        return lastLine;

    }
*/


    for(int i=1;i<100;++i)


        if(curEndPos[i]<=relativePos){
            pairendTable[std::string(bam1_qname(b))]=i;

            int ttmp=(beg)>((b)->core.pos)?(beg-(b)->core.pos):0;//the rectangle length easy erro here be carefull
            dc.DrawRectangle(relativePos*plotNucliedSize,i*height*2+pipeLine,(length-ttmp)*(plotNucliedSize),(height)*2.005);
            //dc.DrawText(wxString(bam1_qname(b)),relativePos*plotNucliedSize,i*plotNucliedSize*2+pipeLine);


            if(mateSize>0){
                if(end>b->core.pos+length){
                    //int lineEnd=end<matePos?end:matePos;
                    //if(relativePos+length<lineEnd-beg)

                    if(b->core.pos+length-beg<matePos-beg)
                    {



                        dc.DrawLine((b->core.pos+length-beg)*plotNucliedSize,i*height*2+pipeLine+height,(matePos-beg)*plotNucliedSize,i*height*2+pipeLine+height);
                    }
                        //if(lineEnd+101-beg>b->core.pos+length-beg)

                        //else
                        //    curEndPos[i]=b->core.pos+length-beg;

                }
                if(matePos-beg+length<=b->core.pos+length-beg){//这里使用了当前序列长度来代替mate序列长度
                    curEndPos[i]=b->core.pos+length-beg;

                }else{

                    curEndPos[i]=matePos-beg+length;//这里使用了当前序列长度来代替mate序列长度
                    //curEndPos[i]=b->core.pos+length-beg;
                }
            }
            else if(mateSize==0){

                curEndPos[i]=b->core.pos+length-beg;

            }else
                curEndPos[i]=b->core.pos+length-beg;


            //for(size_t j=0;j<drawSnp.size();++j)
             //   dc.DrawText(drawSnp[j].second,(drawSnp[j].first)*(plotNucliedSize),i*plotNucliedSize*2+pipeLine);



            lastPos=(b)->core.pos;
            lastLine=i;

            break;
        }

    return lastLine;
}

int PlotNormal::MbamHelpNOFa(bam1_t* b,wxDC& dc,int beg, int end,int& maxLine,int& lastPos,int &lastLine){
    int relativePos=(b)->core.pos-beg>0?(b)->core.pos-beg:0;
    unsigned int length=(b)->core.l_qseq;

    //if(relativePos<overPos)
      //          return ;
 	if(((b)->core.flag)&0x10)
        dc.SetBrush(*wxBLUE_BRUSH);//complentary
    else
         dc.SetBrush(*wxYELLOW_BRUSH);


    //std::string name=bam1_qname(b);

    int sum=0;
    int nOper=0;
    int nDeleteInsert=0;//xulie de chang du
    wxString printString;
    for (size_t i = 0; i < b->core.n_cigar; ++i) {

        nOper=bam1_cigar(b)[i]>>BAM_CIGAR_SHIFT;
        uint32_t *cigar = bam1_cigar(b);
        while(nOper-->0){
            //cigarArray<<bam_cigar_opchr(cigar[i]);
            char curOper=bam_cigar_opchr(cigar[i]);
            int backnDeleteInsert=nDeleteInsert;
            switch(curOper){
                case 'I': nDeleteInsert--;break;
                case 'D': nDeleteInsert++;break;
                case 'S':break;
                case 'N':nDeleteInsert++;break;
                case 'M': break;

            }
            if(b->core.pos+sum>end)
                goto End;
            if(b->core.pos+sum>=beg){
                //int indexRelativePos=b->core.pos+sum-beg;
                int seqPos=sum-backnDeleteInsert;
                if(seqPos>=length)
                    goto End;
                char tstr=bam_nt16_rev_table[bam1_seqi(bam1_seq(b),seqPos)];

                switch(curOper){
                    case 'I':
                        tstr=0;break;
                    case 'D':
                       printString<<'D';break;
                    case 'S':
                        printString<<tstr-32;break;
                    case 'N':
                        printString<<'N';break;
                    case 'X':

                        break;
                    case 'M':


//                        frequencyTable.at(convertTable.at(tstr)*cavansNucliced+indexRelativePos)++;
//                        freQuenctSumTable[indexRelativePos]++;
                        printString<<tstr;
                       // if(tstr!=posNulciedTable[indexRelativePos]){
                       //     drawSnp.push_back(std::make_pair(indexRelativePos,tstr));

                       // }

                        break;
                }

            }
            sum++;

        }
    }
    End:
    length+=nDeleteInsert;

    //printString<<" "<<bam1_qname(b);

    //if(lastLine==100)
    //    return lastLine;

    if(lastPos==(b)->core.pos){
        lastLine++;

        int ttmp=(beg)>((b)->core.pos)?(beg-(b)->core.pos):0;//the rectangle length
        dc.DrawRectangle(relativePos*plotNucliedSize,lastLine*plotNucliedSize*2+pipeLine,(length-ttmp)*(plotNucliedSize),(height)*2.005);
        //dc.DrawText(printString,relativePos*plotNucliedSize,lastLine*plotNucliedSize*2+pipeLine);

    }

    for(int i=0;i<100;++i)


        if(curEndPos[i]<=relativePos){

            int ttmp=(beg)>((b)->core.pos)?(beg-(b)->core.pos):0;//the rectangle length
            dc.DrawRectangle(relativePos*plotNucliedSize,i*plotNucliedSize*2+pipeLine,(length-ttmp)*(plotNucliedSize),height*2.005);
            //dc.DrawText(printString,relativePos*plotNucliedSize,i*plotNucliedSize*2+pipeLine);

            curEndPos[i]+=length-(beg-(b)->core.pos);

            //if(i==99)
            //    overPos=curEndPos[i];

            lastPos=(b)->core.pos;
            lastLine=i;



            break;
        }

}

