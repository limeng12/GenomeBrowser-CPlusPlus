#include "plotProfile.h"

PlotProfile::PlotProfile(DataFactory* tdata,BamData *tbdata,wxSize tcavanseSize,std::vector<bool>* tcheckV)
:PlotBase(tdata,tbdata,tcavanseSize,tcheckV){
    cavansNucliced=(cavanseSize.x)/plotNucliedSize+1;

    frequencyNegTable.resize(cavansNucliced*4);
    frequencyPosTable.resize(cavansNucliced*4);

    frequencySumTable.resize(cavansNucliced);
    frequencySumNegTable.resize(cavansNucliced);
    frequencySumPosTable.resize(cavansNucliced);

    minHeight=5;
    plotInterval=5;
    maxDepth=30;
    maxPlotValue=30;
    readNumInWindow=0;

    convertTable.resize(200);//vector
    convertTable.at('A')=0; convertTable.at('C')=1; convertTable.at('G')=2; convertTable.at('T')=3;
    convertTable.at('a')=0; convertTable.at('c')=1; convertTable.at('g')=2; convertTable.at('t')=3;
}

PlotProfile::~PlotProfile(){

}

void PlotProfile::canvasSizeChange(wxSize changedSize){
    cavanseSize=changedSize;
    cavansNucliced=(cavanseSize.x)/plotNucliedSize+1;
    //toZero(frequencySumTable);
    frequencySumTable.clear();frequencySumTable.resize(cavansNucliced);
    //int t=frequencySumTable.at(200);
    frequencySumNegTable.clear();frequencySumNegTable.resize(cavansNucliced);
    frequencySumPosTable.clear();frequencySumPosTable.resize(cavansNucliced);
    frequencyNegTable.clear();frequencyNegTable.resize(cavansNucliced*4);
    frequencyPosTable.clear();frequencyPosTable.resize(cavansNucliced*4);
}

void PlotProfile::NucliedSizeChange(){
    cavansNucliced=(cavanseSize.x)/plotNucliedSize+1;
    //toZero(frequencySumTable);
    frequencySumTable.clear();frequencySumTable.resize(cavansNucliced);
    //int t=frequencySumTable.at(200);
    frequencySumNegTable.clear();frequencySumNegTable.resize(cavansNucliced);
    frequencySumPosTable.clear();frequencySumPosTable.resize(cavansNucliced);
    frequencyNegTable.clear();frequencyNegTable.resize(cavansNucliced*4);
    frequencyPosTable.clear();frequencyPosTable.resize(cavansNucliced*4);

}

void PlotProfile::plotMain(wxDC& dc,std::string chr,int chrPos,int leftPos,int rightPos,int upPos,int downPos){
    //for snp frequence drawing

    int begPos=(int)(chrPos-rightPos/plotNucliedSize/2);
    int endPos=(int)(chrPos+rightPos/plotNucliedSize/2);

    if(begPos<0){
        begPos=0;
        endPos=begPos+rightPos/plotNucliedSize;
    }

    if(mdata->chrAndLength.find(chr)==mdata->chrAndLength.end())
        return;

    if(endPos>mdata->chrAndLength[chr]){
        endPos=mdata->chrAndLength[chr];
        begPos=endPos-rightPos/plotNucliedSize;
    }

    toZero(frequencySumNegTable);toZero(frequencySumPosTable);
    toZero(frequencySumTable);toZero(frequencyNegTable);toZero(frequencyPosTable);

    height=(plotNucliedSize>minHeight?plotNucliedSize:minHeight);

    maxNuclied.clear();maxNuclied.resize(8);
    maxSumNuclied=0;maxSumNegNuclied=0;maxSumPosNuclied=0;


    pairendTable.clear();
    pipeLine=0;unsigned int i=0;
    pipeLine+=plotCoor(dc,chr,chrPos,begPos,endPos,0);
    dc.SetFont(wxFont(1*(height),wxFONTFAMILY_TELETYPE,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL));

    if(mdata->getFastaFile()!=wxEmptyString){//this is line is unnessary becaue chrAndLenght has test the chr
        if(viewVector->at(i++)){
            //pipeLine+=plotChr(dc,chr,chrPos,begPos,endPos,0);

            viewFaFlag=true;
        }else
        viewFaFlag=false;

    }
    //dc.SetFont(wscut.pen15Pix);

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
//            col.clear();
            pipeLine+=plotVcf(dc,chr,chrPos,begPos,endPos,j);

        }
    }

    dc.SetPen(*wxRED_PEN);
    if(viewVector->at(i)){
        pipeLine+=20;
        plotBam(dc,chr,chrPos,begPos,endPos,0);
    }
    plotFrequency(dc,chr,chrPos,begPos,endPos,0);

}

int PlotProfile::MbamHelpSingleEnd(bam1_t* b,wxDC& dc,int beg, int end,int& maxLine,int& lastPos,int &lastLine){

    return MbamHelpProfile(b,dc,beg,end,maxLine,lastPos,lastLine);

}

int PlotProfile::MbamHelpPairEnd(bam1_t* b,wxDC& dc,int beg, int end,int& maxLine,int& lastPos,int &lastLine){

    return MbamHelpProfile(b,dc,beg,end,maxLine,lastPos,lastLine);

}

int PlotProfile::MbamHelpNOFa(bam1_t* b,wxDC& dc,int beg, int end,int& maxLine,int& lastPos,int &lastLine){

    return MbamHelpProfile(b,dc,beg,end,maxLine,lastPos,lastLine);

}

int PlotProfile::MbamHelpProfile(bam1_t* b,wxDC& dc,int beg, int end,int& maxLine,int& lastPos,int &lastLine){
    //if(readNumInWindow++>10000)
    //    return lastLine;

    if(!mdata->bamfilter->operator()(b))
        return lastLine;

    //int relativePos=(b)->core.pos-beg>0?(b)->core.pos-beg:0;
    unsigned int length=(b)->core.l_qseq;

 	if(((b)->core.flag)&0x10)
        dc.SetBrush(*wxBLUE_BRUSH);//complentary
    else
         dc.SetBrush(*wxYELLOW_BRUSH);

    //std::vector<std::pair<int,char> > drawSnp;


    int sum=0;
    int nOper=0;
    int nDeleteInsert=0;//xulie de chang du
    //readNumInWindow=0;

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
                int indexRelativePos=b->core.pos+sum-beg;
                int seqPos=sum-backnDeleteInsert;
                if(seqPos>=length)
                    goto End;
                char tstr=bam_nt16_rev_table[bam1_seqi(bam1_seq(b),seqPos)];

                switch(curOper){
                    case 'I':
                        /*drawSnp.push_back(std::make_pair(indexRelativePos,'I'));*/ ;break;
                    case 'D':
                        //drawSnp.push_back(std::make_pair(indexRelativePos,'D'));
                        break;
                    case 'S':
                        //drawSnp.push_back(std::make_pair(indexRelativePos,'S'));
                        break;
                    case 'N':
                        //drawSnp.push_back(std::make_pair(indexRelativePos,'N'));
                        break;
                    case 'X':

                        //drawSnp.push_back(std::make_pair(indexRelativePos,'X'));
                        break;
                    case 'M':

                        wxASSERT_MSG(indexRelativePos<cavansNucliced,_T("index overfolw"));
                        frequencySumTable[indexRelativePos]++;
                        if(frequencySumTable[indexRelativePos]>maxSumNuclied)
                            maxSumNuclied=frequencySumTable[indexRelativePos];

                        if(b->core.flag&0x10){
                            frequencySumNegTable[indexRelativePos]++;
                            if(frequencySumNegTable[indexRelativePos]>maxSumNegNuclied)
                                maxSumNegNuclied=frequencySumTable[indexRelativePos];

                        int tx=frequencyNegTable[convertTable[tstr]*cavansNucliced+indexRelativePos]++;
                        if(tx>maxNuclied[4+convertTable[tstr]])
                            maxNuclied[4+convertTable[tstr]]=tx;

                        }
                        else{
                            frequencySumPosTable[indexRelativePos]++;
                            if(frequencySumPosTable[indexRelativePos]>maxSumPosNuclied)
                                maxSumPosNuclied=frequencySumTable[indexRelativePos];

                        int ty=frequencyPosTable[convertTable[tstr]*cavansNucliced+indexRelativePos]++;
                        if(ty>maxNuclied[convertTable[tstr]])
                            maxNuclied[convertTable[tstr]]=ty;
                        }

                        break;
                }

            }
            sum++;

        }
    }
    End:
    //length+=nDeleteInsert;


    return lastLine;

}

int PlotProfile::plotFrequency(wxDC& dc,std::string chr,int chrPos,int begPos,int endPos,int vi){

    dc.SetFont(wscut.pen15Pix);
    dc.SetPen(*wxBLACK_PEN);

    pipeLine+=(maxSumNuclied>maxDepth?maxDepth:maxSumNuclied)*plotInterval;
    //if(>plotInterval)
    int value1,value2;

    for(int i=1;i<cavanseSize.x;++i){
        value1=frequencySumTable.at((i-1)/plotNucliedSize);
        value1=value1>maxPlotValue?maxPlotValue:value1;
        value2=frequencySumTable.at(i/plotNucliedSize);
        value2=value2>maxPlotValue?maxPlotValue:value2;
        dc.DrawLine((i-1),pipeLine-value1*plotInterval,i,pipeLine-value2*plotInterval);

    }

    dc.DrawText(_T("Profile"),0,pipeLine);

    dc.SetPen(*wxRED_PEN);

    pipeLine+=(maxSumPosNuclied>maxDepth?maxDepth:maxSumPosNuclied)*plotInterval;

    for(int i=1;i<cavanseSize.x;++i){
        value1=frequencySumPosTable.at((i-1)/plotNucliedSize);
        value1=value1>maxPlotValue?maxPlotValue:value1;
        value2=frequencySumPosTable.at(i/plotNucliedSize);
        value2=value2>maxPlotValue?maxPlotValue:value2;

        dc.DrawLine(i-1,pipeLine-value1*plotInterval,i,pipeLine-value2*plotInterval);

    }
    dc.DrawText(_T("Positive Profile"),0,pipeLine);
    pipeLine+=10;


    dc.SetPen(*wxBLUE_PEN);
    pipeLine+=maxSumNegNuclied*plotInterval>maxDepth*plotInterval?maxDepth*plotInterval:maxSumNegNuclied*plotInterval;

    for(int i=1;i<cavanseSize.x;++i){
        value1=frequencySumNegTable.at((i-1)/plotNucliedSize);
        value1=value1>maxPlotValue?maxPlotValue:value1;

        value2=frequencySumNegTable.at(i/plotNucliedSize);
        value2=value2>maxPlotValue?maxPlotValue:value2;
        dc.DrawLine(i-1,pipeLine-value1*plotInterval,i,pipeLine-value2*plotInterval);

    }
    dc.DrawText(_T("Negative Profile"),0,pipeLine);
    pipeLine+=10;

//pos
    dc.SetPen(*wxRED_PEN);
    for(int i=0;i<4;i++){
        pipeLine+=maxNuclied[i]*plotInterval>maxDepth*plotInterval?maxDepth*plotInterval:maxNuclied[i]*plotInterval;
        dc.DrawText(numberToNucliedTable[i],0,pipeLine);
        for(int j=1;j<cavanseSize.x;++j){
            value1=frequencyPosTable[i*cavansNucliced+(j-1)/plotNucliedSize];
            value1=value1>maxPlotValue?maxPlotValue:value1;

            value2=frequencyPosTable[i*cavansNucliced+j/plotNucliedSize];
            value2=value2>maxPlotValue?maxPlotValue:value2;
            dc.DrawLine(j-1,pipeLine-value1*plotInterval,j,pipeLine-value2*plotInterval);

        }
        pipeLine+=10;
    }
//neg
    dc.SetPen(*wxBLUE_PEN);

    for(int i=0;i<4;i++){
        pipeLine+=maxNuclied[i+4]*plotInterval>maxDepth*plotInterval?maxDepth*plotInterval:maxNuclied[i+4]*plotInterval;
        dc.DrawText(numberToNucliedTable[i+4],0,pipeLine);

        for(int j=1;j<cavanseSize.x;++j){
            value1=frequencyNegTable[i*cavansNucliced+(j-1)/plotNucliedSize];
            value1=value1>maxPlotValue?maxPlotValue:value1;
            value2=frequencyNegTable[i*cavansNucliced+j/plotNucliedSize];
            value2=value2>maxPlotValue?maxPlotValue:value2;
            dc.DrawLine(j-1,pipeLine-value1*plotInterval,j,pipeLine-value2*plotInterval);

        }
        pipeLine+=10;

    }

}
