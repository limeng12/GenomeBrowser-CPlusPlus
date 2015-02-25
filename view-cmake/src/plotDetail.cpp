#include "plotDetail.h"
#include <wx/msgdlg.h>

PlotDetail::PlotDetail(DataFactory* tdata,BamData* tbdata,wxSize tcavanseSize,std::vector<bool>* tvector)
:PlotBase(tdata,tbdata,tcavanseSize,tvector){

    //posNulciedTable=new char[200];//
    //200是一行染色体可能的最多的数目
    //convert talbe,convert 'A' to 0,'C' to 1,'G' to 2,'T' to 3
    //convertTable=new int[100];
    convertTable.resize(200);//vector
    convertTable.at('A')=0; convertTable.at('C')=1; convertTable.at('G')=2; convertTable.at('T')=3;
    convertTable.at('a')=0; convertTable.at('c')=1; convertTable.at('g')=2; convertTable.at('t')=3;
    //frequencyTable=new int*[4];
    //for(int i=0;i<4;++i)
    //    frequencyTable[i]=new int[cavansNucliced];
    plotNucliedSize=10;
    cavansNucliced=cavanseSize.x/plotNucliedSize+1;
    frequencyTable.resize(cavansNucliced*4);

    //freQuenctSumTable=new float[200];
    freQuenctSumTable.resize(cavansNucliced);
    height=plotNucliedSize;
}

PlotDetail::~PlotDetail(){


}

void PlotDetail::NucliedSizeChange(){

}

void PlotDetail::canvasSizeChange(wxSize changedSize){
    cavanseSize=changedSize;
    cavansNucliced=cavanseSize.x/plotNucliedSize+1;
    frequencyTable.resize(cavansNucliced*4);

    //freQuenctSumTable=new float[200];
    freQuenctSumTable.resize(cavansNucliced);
    height=plotNucliedSize;

}

void PlotDetail::plotMain(wxDC& dc,std::string chr,int chrPos,int leftPos,int rightPos,int upPos,int downPos){

    if(chr.empty()){

        //wxMessageBox(_T("chromsome can't be empty"));
        return;
    }

    //string tmpChr=chr;//for debug
    //int tmpLenth=mdata->chrAndLength[chr];
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

    //cavansNucliced=(cavanseSize.x)/plotNucliedSize+1;
    toZero(freQuenctSumTable);toZero(frequencyTable);

    pairendTable.clear();
    pipeLine=0;posMessageTable.clear();bamMessageTable.clear();unsigned int i=0;//index of all the file
    pipeLine+=plotCoor(dc,chr,chrPos,begPos,endPos,0);

    if(mdata->getFastaFile()!=wxEmptyString){
        if(viewVector->at(i++)){
            pipeLine+=plotChr(dc,chr,chrPos,begPos,endPos,0);

            viewFaFlag=true;
        }else
        viewFaFlag=false;

    }
    dc.SetFont(wscut.pen9Pix);

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
            col.clear();
            pipeLine+=plotVcf(dc,chr,chrPos,begPos,endPos,j);

        }
    }

    dc.SetFont(wscut.pen2010Pix);

    if(viewVector->at(i)){
        pipeLine+=20;
        bamPipeline=pipeLine;
        plotBam(dc,chr,chrPos,begPos,endPos,0);
        pipeLine-=20;
        plotSNP(dc,chr,chrPos,begPos,endPos,0);
    }
}

int PlotDetail::plotChr(wxDC& dc,std::string chr,int chrPos,int begPos,int endPos,int index){
    dc.SetFont(wscut.pen2010Pix);
    int len;
    if(mdata->faiTable.find(chr)==mdata->faiTable.end()){
        wxMessageBox(wxString("don't have this chr=")+wxString(chr));
        return 0;
    }
    //wxMessageBox(wxString("chr=")+wxString(chr)+wxString("begPos=")+wxString::FromDouble(begPos)+wxString("endPos=")+wxString::FromDouble(endPos));

    char* tmp=faidx_fetch_seq(mdata->getFai(chr), (char*)chr.c_str(), begPos,endPos,&len);
    //char* tmp=faidx_fetch_seq(mdata->fai, (char*)chr.c_str(), begPos,endPos,&len);

    if(tmp==NULL)
        return 0;

    //wxMessageBox(wxString(tmp));

    wxString printString(tmp);

    for(unsigned int i=0;i<printString.length();++i){
        char curChar;
        printString[i].GetAsChar(&curChar);
        switch(curChar){

        case 'A':
                dc.SetTextForeground(wscut.wxColourVectorChr[0]);
                posNulciedTable[i]=curChar;
                dc.DrawText(printString[i],i*10,pipeLine);
                break;

        case 'a':
                dc.SetTextForeground(wscut.wxColourVectorChr[0]);
                posNulciedTable[i]=curChar;
                dc.DrawText(printString[i],i*10,pipeLine);
                break;

        case 'C':
                dc.SetTextForeground(wscut.wxColourVectorChr[1]);
                posNulciedTable[i]=curChar;
                dc.DrawText(printString[i],i*10,pipeLine);
                break;


        case 'c':
                dc.SetTextForeground(wscut.wxColourVectorChr[1]);
                posNulciedTable[i]=curChar;
                dc.DrawText(printString[i],i*10,pipeLine);
                break;


        case 'G':
                dc.SetTextForeground(wscut.wxColourVectorChr[2]);
                posNulciedTable[i]=curChar;
                dc.DrawText(printString[i],i*10,pipeLine);
                break;


        case 'g':
                dc.SetTextForeground(wscut.wxColourVectorChr[2]);
                posNulciedTable[i]=curChar;
                dc.DrawText(printString[i],i*10,pipeLine);
                break;

        case 'T':
                dc.SetTextForeground(wscut.wxColourVectorChr[3]);
                posNulciedTable[i]=curChar;
                dc.DrawText(printString[i],i*10,pipeLine);
                break;


        case 't':
                dc.SetTextForeground(wscut.wxColourVectorChr[3]);
                posNulciedTable[i]=curChar;
                dc.DrawText(printString[i],i*10,pipeLine);
                break;


        case 'N':
                dc.SetTextForeground(wscut.wxColourVectorChr[4]);
                posNulciedTable[i]=curChar;
                dc.DrawText(printString[i],i*10,pipeLine);
                break;
            }

        }


    free(tmp);

    return 30;
}

int PlotDetail::MbamHelp(bam1_t* b,wxDC& dc,int beg, int end,int &maxLine,int& lastPos,int &lastLine){
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

int PlotDetail::MbamHelpSingleEnd(bam1_t* b,wxDC& dc,int beg, int end,int& maxLine,int& lastPos,int &lastLine){
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
                int indexRelativePos=b->core.pos+sum-beg;
                int seqPos=sum-backnDeleteInsert;
                if(seqPos>=length)
                    goto End;
                char tstr=bam_nt16_rev_table[bam1_seqi(bam1_seq(b),seqPos)];

                switch(curOper){
                    case 'I':
                        /*drawSnp.push_back(std::make_pair(indexRelativePos,'I'));*/ ;break;
                    case 'D':
                        drawSnp.push_back(std::make_pair(indexRelativePos,'D')); break;
                    case 'S':
                        drawSnp.push_back(std::make_pair(indexRelativePos,'S'));break;
                    case 'N':
                        drawSnp.push_back(std::make_pair(indexRelativePos,'N')); break;
                    case 'X':

                        drawSnp.push_back(std::make_pair(indexRelativePos,'X'));break;
                    case 'M':
                        wxASSERT_MSG(indexRelativePos<cavansNucliced,_T("index overflow"));
                        frequencyTable.at(convertTable.at(tstr)*cavansNucliced+indexRelativePos)++;
                        freQuenctSumTable[indexRelativePos]++;
                        if(tstr!=posNulciedTable[indexRelativePos]&&tstr!=posNulciedTable[indexRelativePos]-32&&tstr!=posNulciedTable[indexRelativePos]+32){
                            drawSnp.push_back(std::make_pair(indexRelativePos,tstr));

                        }

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
            dc.DrawRectangle(relativePos*plotNucliedSize,i*plotNucliedSize*2+pipeLine,(length-ttmp)*(plotNucliedSize),plotNucliedSize*2.005);

            for(size_t j=0;j<drawSnp.size();++j)
                dc.DrawText(drawSnp[j].second,(drawSnp[j].first)*(plotNucliedSize),i*plotNucliedSize*2+pipeLine);

            curEndPos[i]=b->core.pos+length-beg;

            lastPos=(b)->core.pos;
            lastLine=i;

            break;
        }

        //if(bamMessageTable.find(lastLine)!=bamMessageTable.end()){
        bamRect tbamr;
        tbamr.leftPos=b->core.pos;
        tbamr.leftRect=relativePos;
        tbamr.rightRect=((b->core.pos+length)>end?end:b->core.pos+length)-beg;
        tbamr.sqname=std::string(bam1_qname(b));
        bamMessageTable[lastLine].push_back(tbamr);

   /* }else{
        bamRect tbamr;
        tbamr.leftPos=b->core.pos;
        tbamr.leftRect=relativePos;
        tbamr.rightRect=((b->core.pos+length)>end?end:b->core.pos+length)-beg;
        tbamr.sqname=bam1_qname(b);

        std::vector<bamRect> tv;tv.push_back(tbamr);
        bamMessageTable[lastLine]=tv;

    }
*/

    return lastLine;
}

int PlotDetail::MbamHelpPairEnd(bam1_t* b,wxDC& dc,int beg, int end,int& maxLine,int& lastPos,int &lastLine){
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

            if(b->core.pos+sum>=end)///这里李梦改了，要注意
                goto End;
            if(b->core.pos+sum>=beg){
                int indexRelativePos=b->core.pos+sum-beg;
                int seqPos=sum-backnDeleteInsert;
                if(seqPos>=length)
                    goto End;
                char tstr=bam_nt16_rev_table[bam1_seqi(bam1_seq(b),seqPos)];

                switch(curOper){
                    case 'I':
                        /*drawSnp.push_back(std::make_pair(indexRelativePos,'I'));*/ ;break;
                    case 'D':
                        drawSnp.push_back(std::make_pair(indexRelativePos,'D')); break;
                    case 'S':
                        drawSnp.push_back(std::make_pair(indexRelativePos,'S'));break;
                    case 'N':
                        drawSnp.push_back(std::make_pair(indexRelativePos,'N')); break;
                    case 'X':

                        drawSnp.push_back(std::make_pair(indexRelativePos,'X'));break;
                    case 'M':
wxASSERT_MSG(indexRelativePos<cavansNucliced,wxString::FromDouble(indexRelativePos)+wxString(_T("\t"))+wxString::FromDouble(cavansNucliced)+wxString(_T("index overflow")));

                        frequencyTable.at(convertTable.at(tstr)*cavansNucliced+indexRelativePos)++;
                        freQuenctSumTable[indexRelativePos]++;
                        if(tstr!=posNulciedTable[indexRelativePos]&&tstr!=posNulciedTable[indexRelativePos]-32&&tstr!=posNulciedTable[indexRelativePos]+32){
                            drawSnp.push_back(std::make_pair(indexRelativePos,tstr));

                        }

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

    int mateSize=b->core.isize;
    int matePos=b->core.mpos;//+mateSize;
    int line=0;
    if(pairendTable.find(std::string(bam1_qname(b)))!=pairendTable.end()){
        line=pairendTable[std::string(bam1_qname(b))];
        if(b->core.pos==matePos)
            return lastLine;
        int ttmp=(beg)>((b)->core.pos)?(beg-(b)->core.pos):0;//the rectangle length
        dc.DrawRectangle(relativePos*plotNucliedSize,line*plotNucliedSize*2+pipeLine,(length-ttmp)*(plotNucliedSize),plotNucliedSize*2.005);

        for(size_t j=0;j<drawSnp.size();++j)
            dc.DrawText(drawSnp[j].second,(drawSnp[j].first)*(plotNucliedSize),line*plotNucliedSize*2+pipeLine);

        if(matePos+b->core.l_qseq-beg<relativePos)//这里使用了当前序列长度来代替mate序列长度
            dc.DrawLine((matePos+b->core.l_qseq-beg)*plotNucliedSize,line*plotNucliedSize*2+pipeLine+10,relativePos*plotNucliedSize,line*plotNucliedSize*2+pipeLine+10);//这里使用了当前序列长度来代替mate序列长度

        curEndPos[line]=b->core.pos+length-beg;//>end-beg?end-beg:relativePos+length;

        lastLine=line;
        bamRect tbamr;
        tbamr.leftPos=b->core.pos;
        tbamr.leftRect=relativePos;
        tbamr.rightRect=(((b->core.pos+length)>end?end:b->core.pos+length)-beg);
        tbamr.sqname=std::string(bam1_qname(b));
        bamMessageTable[lastLine].push_back(tbamr);
        return lastLine;
    }


    if(mateSize<0){
        maxLine++;
        pairendTable[std::string(bam1_qname(b))]=maxLine;

        int ttmp=(beg)>((b)->core.pos)?(beg-(b)->core.pos):0;//the rectangle length
        dc.DrawRectangle(relativePos*plotNucliedSize,maxLine*plotNucliedSize*2+pipeLine,(length-ttmp)*(plotNucliedSize),plotNucliedSize*2.005);
        //dc.DrawText(wxString(bam1_qname(b)),relativePos*plotNucliedSize,maxLine*plotNucliedSize*2+pipeLine);


        for(size_t j=0;j<drawSnp.size();++j)
            dc.DrawText(drawSnp[j].second,(drawSnp[j].first)*(plotNucliedSize),maxLine*plotNucliedSize*2+pipeLine);

        if(beg<b->core.pos){
            int lineBegin=beg>matePos+b->core.l_qseq?beg:matePos+b->core.l_qseq;//这里使用了当前序列长度来代替mate序列长度
            dc.DrawLine((lineBegin-beg)*plotNucliedSize,maxLine*plotNucliedSize*2+pipeLine+10,relativePos*plotNucliedSize,maxLine*plotNucliedSize*2+pipeLine+10);
        }

        curEndPos[maxLine]=b->core.pos+length-beg;
        lastLine=maxLine;
        lastPos=b->core.pos;

        bamRect tbamr;
        tbamr.leftPos=b->core.pos;
        tbamr.leftRect=relativePos;
        tbamr.rightRect=(((b->core.pos+length)>end?end:b->core.pos+length)-beg);
        tbamr.sqname=std::string(bam1_qname(b));
        bamMessageTable[lastLine].push_back(tbamr);
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
            dc.DrawRectangle(relativePos*plotNucliedSize,i*plotNucliedSize*2+pipeLine,(length-ttmp)*(plotNucliedSize),plotNucliedSize*2.005);
            //dc.DrawText(wxString(bam1_qname(b)),relativePos*plotNucliedSize,i*plotNucliedSize*2+pipeLine);

            if(mateSize>0){
                if(end>b->core.pos+length){

                    if(b->core.pos+length-beg<matePos-beg)
                    {

                        dc.DrawLine((b->core.pos+length-beg)*plotNucliedSize,i*plotNucliedSize*2+pipeLine+10,(matePos-beg)*plotNucliedSize,i*plotNucliedSize*2+pipeLine+10);
                    }

                }
                if(matePos-beg+b->core.l_qseq<=b->core.pos+length-beg){//这里使用了当前序列长度来代替mate序列长度
                    curEndPos[i]=b->core.pos+length-beg;

                }else{

                    curEndPos[i]=matePos-beg+b->core.l_qseq;//这里使用了当前序列长度来代替mate序列长度
                }
            }
            else if(mateSize==0){

                curEndPos[i]=b->core.pos+length-beg;

            }

            for(size_t j=0;j<drawSnp.size();++j)
                dc.DrawText(drawSnp[j].second,(drawSnp[j].first)*(plotNucliedSize),i*plotNucliedSize*2+pipeLine);

            lastPos=(b)->core.pos;
            lastLine=i;

            break;
        }

    bamRect tbamr;
    tbamr.leftPos=b->core.pos;
    tbamr.leftRect=relativePos;
    tbamr.rightRect=(((b->core.pos+length)>end?end:b->core.pos+length)-beg);
    tbamr.sqname=std::string(bam1_qname(b));
    bamMessageTable[lastLine].push_back(tbamr);

    return lastLine;
}

int PlotDetail::MbamHelpNOFa(bam1_t* b,wxDC& dc,int beg, int end,int& maxLine,int& lastPos,int &lastLine){
    int relativePos=(b)->core.pos-beg>0?(b)->core.pos-beg:0;
    unsigned int length=(b)->core.l_qseq;

 	if(((b)->core.flag)&0x10)
        dc.SetBrush(*wxBLUE_BRUSH);//complentary
    else
         dc.SetBrush(*wxYELLOW_BRUSH);

    int sum=0;
    int nOper=0;
    int nDeleteInsert=0;//xulie de chang du
    wxString printString;

    uint32_t *cigar = bam1_cigar(b);
    for (size_t i = 0; i < b->core.n_cigar; ++i) {

        nOper=bam1_cigar(b)[i]>>BAM_CIGAR_SHIFT;
        while(nOper-->0){
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
                int indexRelativePos=b->core.pos+sum-beg;
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

                        wxASSERT_MSG(indexRelativePos<cavansNucliced,_T("index overflow"));
                        frequencyTable.at(convertTable.at(tstr)*cavansNucliced+indexRelativePos)++;
                        freQuenctSumTable[indexRelativePos]++;
                        printString<<tstr;

                        break;
                }

            }
            sum++;

        }
    }
    End:
    length+=nDeleteInsert;

    if(lastLine>=99)
        return lastLine;

    if(lastPos==(b)->core.pos){
        lastLine++;

        int ttmp=(beg)>((b)->core.pos)?(beg-(b)->core.pos):0;//the rectangle length
        dc.DrawRectangle(relativePos*plotNucliedSize,lastLine*plotNucliedSize*2+pipeLine,(length-ttmp)*(plotNucliedSize),plotNucliedSize*2.005);
        dc.DrawText(printString,relativePos*plotNucliedSize,lastLine*plotNucliedSize*2+pipeLine);

    }

    for(int i=0;i<100;++i)


        if(curEndPos[i]<=relativePos){

            int ttmp=(beg)>((b)->core.pos)?(beg-(b)->core.pos):0;//the rectangle length
            dc.DrawRectangle(relativePos*plotNucliedSize,i*plotNucliedSize*2+pipeLine,(length-ttmp)*(plotNucliedSize),plotNucliedSize*2.005);
            dc.DrawText(printString,relativePos*plotNucliedSize,i*plotNucliedSize*2+pipeLine);

            curEndPos[i]+=length-(beg-(b)->core.pos);

            lastPos=(b)->core.pos;
            lastLine=i;



            break;
        }

    return lastLine;
}

wxString PlotDetail::bamSearch(std::string chr,int chrPos,std::string qname){///zhei buyong yinyong chuandi jiuhui chuxian wenti made
    wxString mes;


    int chrIndex;
    if(mbamdata->bamMap.find(chr)!=mbamdata->bamMap.end())
        chrIndex=mbamdata->bamMap[chr];
    else if(mbamdata->bamMapChr.find(chr)!=mbamdata->bamMapChr.end())
        chrIndex=mbamdata->bamMapChr[chr];
    else{
        wxMessageBox(_T("bam search hase not this chr=")+wxString(chr));
        return mes;
    }

	int ret;
	bam_iter_t iter;

	bam1_t *b;
	b = bam_init1();

	iter = bam_iter_query(mbamdata->bamIndex, chrIndex, chrPos,chrPos+1);
	while ((ret = bam_iter_read(mbamdata->bamf, iter, b)) >= 0){
        if(!strcmp(bam1_qname(b),qname.c_str())){
            //mes=_T("affffffffffffffffff");
            /*mes<<_T("seq name=")<<bam1_qname(b)<<_T("\n");
            mes<<_T("chr=")<<wxString::FromDouble(double(b->core.tid))<<_T("\n");
            mes<<_T("pos=")<<wxString::FromDouble(double(b->core.pos))<<_T("\n");
            mes<<_T("seq=");

            //char *seq=new char[b->core.l_qseq];
            for(int j=0;j<b->core.l_qseq;++j)
                mes<<bam_nt16_rev_table[bam1_seqi(bam1_seq(b),j)];
            mes<<_T("\n");
            mes<<_T("cigar=");


            uint32_t *cigar = bam1_cigar(b);
            //std::string cigarStr;
            //char tchar[10];
            int nOper;
            for (size_t i = 0; i < b->core.n_cigar; ++i){

                nOper=bam1_cigar(b)[i]>>BAM_CIGAR_SHIFT;
                char curOper=bam_cigar_opchr(cigar[i]);

                mes<<wxString::FromDouble(double(nOper))<<curOper;
                //sprintf(tchar,"%d",nOper);
                //cigarStr+=tchar;
                //cigarStr+=curOper;
            }
            mes<<_T("\n");
*/
            mes=bam_format1(mbamdata->header,b);//<<_T("\n");
            mes.Replace("\t","\n");
            mes<<_T("\n");


            //unsigned char *qualStr=bam1_qual(b);
            //unsigned char *auxStr=bam1_aux(b);
            //mes<<_T("qual=")<<bam1_qual(b)<<_T("\n");
            //mes<<_T("aux=")<<bam1_aux(b)<<_T("\n");
            //mes<<_T("qual=")<<reinterpret_cast<char*>(bam1_qual(b))<<_T("\n");
            //mes<<_T("aux data=")<<reinterpret_cast<char*>(bam1_aux(b))<<_T("\n");

/*
            char *qual=new char[b->core.l_qseq];
            char *aux=new char[b->l_aux];

            for(int i=0;i<b->core.l_qseq;++i){
                qual[i]=static_cast<char>(static_cast<int>(qualStr[i])-127);
            }

            for(int i=0;i<b->l_aux;++i){
                aux[i]=static_cast<char>(static_cast<int>(auxStr[i])-127);
            }


            //sprintf(t,"chr=%d\tpos=%d\nmateTid=%d\tmatePos=%d\nseqname=%s\nseq=%s\ncigar=%s\nqual=%s\naux=%s\n",b->core.tid,b->core.pos,b->core.mtid,b->core.mpos,bam1_qname(b),seq,cigarStr.c_str(),qual,aux);

            //mes=t;

            delete[] aux;
            delete[] qual;
            delete[] seq;
            */
            break;
        }

    }


	bam_iter_destroy(iter);
	bam_destroy1(b);

    return mes;
	//return (ret == -1)? 0 : ret;
	//return lastLine+1;

}

int PlotDetail::plotSNP(wxDC& dc,std::string chr,int chrPos,int begPos,int endPos,int index){
    dc.SetFont(wscut.pen5Pix);
    unsigned char tcolour;//color for snp table

    wxDCPenChanger penChange(dc,*(wscut.transparentPen));
    snpTableLine=pipeLine;
    int maxCol=-1,maxRow=-1;
    for(int j=0;j<cavansNucliced;++j){
        maxCol=-1;
        for(int k=0;k<4;++k){
	    if(maxCol<frequencyTable.at(k*cavansNucliced+j)){
		maxCol=frequencyTable.at(k*cavansNucliced+j);
		maxRow=k;
		}

	}
        //add 0.000001 is because frequenctsumtable is usuall equal to zero
        tcolour=static_cast<unsigned char>(255-maxCol/(freQuenctSumTable[j]+0.000001)*255);
        if(convertTable[posNulciedTable[j]]!=maxRow&&freQuenctSumTable[j]!=0)
		tcolour=20;
	dc.SetBrush(wxColour(tcolour,tcolour,tcolour));

        dc.DrawRectangle(j*plotNucliedSize,pipeLine,plotNucliedSize,20);
    }

    return 20;
}

int PlotDetail::plotVcf(wxDC& dc,std::string chr,int chrPos,int begPos,int endPos,int vi){
    std::string mchr;
    if(mdata->vcfDatas[vi].nameTable.find(std::string(chr))!=mdata->vcfDatas[vi].nameTable.end())
        mchr=chr;
    else if(mdata->vcfDatas[vi].nameChrTable.find(chr)!=mdata->vcfDatas[vi].nameChrTable.end())
        mchr=chr.substr(3,chr.length()-3);
    else
        return 0;

    //int index=(mdata->gtfTable).at(vi)[chr];
    std::list<std::string> resultStr=mdata->vcfDatas[vi].getRegion(mchr,begPos,endPos);


    //int len;
    std::string tmpStr;
    std::vector<std::string> arrStr;

    //dc.SetTextForeground(*wxBLACK);

    std::list<std::string>::iterator ite;
    for(ite=resultStr.begin();ite!=resultStr.end();++ite){
    //while(true){
        //const char *str=ti_read(mdata->getVcfIx(vi),miter,&len);
        //if(str==NULL)
        //    break;

        tmpStr=*ite;
        split(tmpStr,'\t',arrStr);
        int w=(atoi((char*)arrStr[1].c_str())-begPos)*plotNucliedSize;
        col[(int)(w/30)]=arrStr[3]+"-"+arrStr[4]+"\t"+arrStr[7];
        dc.DrawLine(w,pipeLine+10,w,pipeLine+20);
        dc.DrawText(wxString(arrStr[2]),w,pipeLine);
        //free((char*)str);//fixed me!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }

    //ti_iter_destroy(miter);
    posMessageTable[(int)(pipeLine/30)]=col;
    return 3*10;
}

