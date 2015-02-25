#include "plotSnp.h"
#include <wx/msgdlg.h>
#include <algorithm>

bool PlotSnp::compareTwoRead(bam1_t *b1,bam1_t* b2){


    //unsigned char *data1=bam_aux_get(b1,"NM");
    //int nm1=bam_aux2i(data1+1);
    //unsigned char *data2=bam_aux_get(b2,"NM");
    //int nm2=bam_aux2i(data2+1);
    //if(nm1>nm2)
    //    return true;
	bool b1misMatch=getIfMismatch(b1);
	bool b2misMatch=getIfMismatch(b2);
	if(b1misMatch==true&&b2misMatch==false)
		return true;

	return false;
}

bool PlotSnp::getIfMismatch(bam1_t* b){
	//if(!mdata->bamfilter->operator()(b))
	//        return lastLine;

		int beg=begPosition;
		int end=endPosition;//doesn't use here
		int targetPos=curPos-1;
	    int relativePos=(b)->core.pos-beg>0?(b)->core.pos-beg:0;
	    unsigned int length=(b)->core.l_qseq;

	    int targetRelativePos=targetPos-beg;

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


	            //if(b->core.pos+sum>end)
	            //    goto End;
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
    for(int i=0;i<drawSnp.size();++i){
		if(drawSnp.at(i).first==targetRelativePos)
			return true;// have snp in this position


	}


	drawSnp.clear();

	return false;//don't have snp in this position

}


PlotSnp::PlotSnp(DataFactory* tmdata,BamData* tbamdata,string tchr,int tcurPos,wxSize size)
:PlotBase(tmdata,tbamdata,size,new std::vector<bool>()){


    chr=tchr;
    curPos=tcurPos;
    mdata=tmdata;
    bamdata=tbamdata;
    pipeLine=0;
    plotNucliedSize=10;
    wscut=WxShotCut();
    cavansNucliced=200;

    convertTable.resize(200);//vector
    convertTable.at('A')=0; convertTable.at('C')=1; convertTable.at('G')=2; convertTable.at('T')=3;
    convertTable.at('a')=0; convertTable.at('c')=1; convertTable.at('g')=2; convertTable.at('t')=3;

}

void PlotSnp::canvasSizeChange(wxSize changedSize){

	cavanseSize=changedSize;
	cavansNucliced=cavanseSize.x/plotNucliedSize+1;

}


void PlotSnp::plotMain(wxDC& dc,std::string tchr,int chrPos,int leftPos,int rightPos,int upPos,int downPos){

	cavansNucliced=cavanseSize.x/plotNucliedSize+1;

	chr=tchr;

	curPos=chrPos;

	int begPos=leftPos;

	int endPos=rightPos;

	begPosition=begPos;
    endPosition=endPos;

	plot(dc,chr,curPos,begPos,endPos);
}


void PlotSnp::plot(wxDC &dc,string tchr,int chrPos,int begPos,int endPos){
	curPos=chrPos;
	chr=tchr;

    pipeLine=0;

    int rightPos=cavanseSize.x;
    //begPos=(int)(curPos-rightPos/plotNucliedSize/2);

    //endPos=(int)(curPos+rightPos/plotNucliedSize/2);

    pipeLine+=plotCoor(dc,chr,curPos,begPos,endPos,0);

    pipeLine+=plotChr(dc,chr,curPos,begPos,endPos,0);

    seartchAPositon();

    //sort(reads.begin(),reads.end(),compareTwoRead);
    sort(reads.begin(),reads.end(),boost::bind(&PlotSnp::compareTwoRead, this, _1, _2));

    plotBam(dc,chr,curPos,begPos,endPos,0);

    for(int i=0;i<reads.size();++i)
    {
        bam_destroy1(reads.at(i));
    }

    bam_iter_destroy(iter);
    reads.clear();


}

int PlotSnp::plotChr(wxDC& dc,std::string chr,int chrPos,int begPos,int endPos,int index){
    dc.SetBrush(*wxBLACK_BRUSH);
    posNulciedTable.resize(endPos-begPos+1);
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

PlotSnp::~PlotSnp(){

}

wxString PlotSnp::seartchAPositon(){
    int chrPos=curPos;
    wxString mes;


    int chrIndex;
    if(bamdata->bamMap.find(chr)!=bamdata->bamMap.end())
        chrIndex=bamdata->bamMap[chr];
    else if(bamdata->bamMapChr.find(chr)!=bamdata->bamMapChr.end())
        chrIndex=bamdata->bamMapChr[chr];
    else{
        wxMessageBox(_T("bam search hase not this chr=")+wxString(chr));
        return mes;
    }

	int ret;

	bam1_t *b;
	b = bam_init1();

	iter = bam_iter_query(bamdata->bamIndex, chrIndex, chrPos,chrPos+1);

	while ((ret = bam_iter_read(bamdata->bamf, iter, b)) >= 0){
        //wxMessageBox(wxString::Format("%d",b->core.pos));

        reads.push_back(b);
        b = bam_init1();

	}

    //bam_iter_destroy(iter);

}

int PlotSnp::plotBam(wxDC& dc,std::string chr,int chrPos,int begPos,int endPos,int index){

    int lastLine=0;
    int lastPos=0;
    int maxLine=1000;

    for(int index=0;index<reads.size();++index){

        MbamHelpSingleEnd((reads.at(index)),dc,begPos, endPos,maxLine,lastPos,lastLine);


        pipeLine+=plotNucliedSize*2;

    }

}

int PlotSnp::MbamHelpSingleEnd(bam1_t* b,wxDC& dc,int beg, int end,int& maxLine,int& lastPos,int &lastLine){
    //wxMessageBox(wxString::Format(("%d"),(int)((b)->core.pos)));

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

    int ttmp=(beg)>((b)->core.pos)?(beg-(b)->core.pos):0;//the rectangle length
    dc.DrawRectangle(relativePos*plotNucliedSize,0*plotNucliedSize*2+pipeLine,(length-ttmp)*(plotNucliedSize),plotNucliedSize*2.005);
    //dc.DrawText(wxString::Format(("%d"),(int)((b)->core.pos)),relativePos*plotNucliedSize,0*plotNucliedSize*2+pipeLine);
    for(size_t j=0;j<drawSnp.size();++j)
        dc.DrawText(drawSnp[j].second,(drawSnp[j].first)*(plotNucliedSize),0*plotNucliedSize*2+pipeLine);

    return lastLine;
}




int PlotSnp::MbamHelpNOFa(bam1_t* b,wxDC& dc,int beg, int end,int &maxLine,int &lastPos,int &lastLine){

}

int PlotSnp::MbamHelpPairEnd(bam1_t* b,wxDC& dc,int beg, int end,int& maxLine,int& lastPos,int &lastLine){

}








