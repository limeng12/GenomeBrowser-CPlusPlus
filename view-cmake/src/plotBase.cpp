#include "plotBase.h"
#include "wx/msgdlg.h"
#ifdef __WXMSW__
#include "windows.h"
#undef DrawText
#endif

PlotBase::PlotBase(DataFactory* tdata,BamData* tbdata,wxSize tcavanseSize,std::vector<bool>* tvector)
:mdata(tdata),mbamdata(tbdata),cavanseSize(tcavanseSize),viewVector(tvector){
    //viewVector=tvector;
    height=plotNucliedSize;
    minHeight=0;
    plotNucliedSize=10;
    plotFontSize=plotNucliedSize*2;
    numberToNucliedTable[0]=_T("+A");
    numberToNucliedTable[1]=_T("+C");
    numberToNucliedTable[2]=_T("+G");
    numberToNucliedTable[3]=_T("+T");

    numberToNucliedTable[4]=_T("-A");
    numberToNucliedTable[5]=_T("-C");
    numberToNucliedTable[6]=_T("-G");
    numberToNucliedTable[7]=_T("-T");

    int cavansWidth=cavanseSize.x;
    cavansNucliced=(cavansWidth+1)/(plotNucliedSize);
    posNulciedTable.resize(cavansNucliced);
    pipeLine=0;
    //gffLine=0;
    //curEndPos=new int[100];
    curEndPos.resize(100);
}

PlotBase::~PlotBase(){


}

void PlotBase::SetNucliedSize(double t){
    plotNucliedSize=t;
    NucliedSizeChange();
}

void PlotBase::NucliedSizeChange(){

}

double PlotBase::GetNucliedSize(){
    return plotNucliedSize;
}

int PlotBase::plotCoor(wxDC& dc,std::string chr,int chrPos,int begPos,int endPos,int index){

#ifdef __WXMSW__
    dc.SetFont(wscut.pen710Pix);
#else
    dc.SetFont(wscut.pen10Pix);
#endif
    dc.DrawLine(0,pipeLine+30,(endPos-begPos)*plotNucliedSize,pipeLine+30);

    int beginPos=begPos;
    int change=((int)(beginPos/(100.0/plotNucliedSize)+0.5))*(100.0/plotNucliedSize)-beginPos;
    int coorPos=0;
    //dc.SetFont(wxFont(wxSize(0,plotFontSize-5),wxFONTFAMILY_TELETYPE ,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,wxEmptyString,wxFONTENCODING_DEFAULT));
    //wxDCFontChanger(dc,wscut.pen20Pix);
    while(coorPos<(int)(endPos-begPos)+1){
        dc.DrawLine((coorPos+change)*plotNucliedSize,pipeLine+20,(coorPos+change)*plotNucliedSize,pipeLine+30);
        dc.DrawText(wxString::Format("%d",beginPos+coorPos+change+1),(coorPos+change)*plotNucliedSize,pipeLine);
        coorPos=coorPos+100.0/plotNucliedSize;
    }//+1 conver to 1 based coordinate

    return 32;
}

int PlotBase::plotGTFExon(wxDC& dc,std::string chr,int chrPos,int begPos,int endPos,int vi){
    //if(mdata->gtfTable.at(vi).find(chr)==mdata->gtfTable.at(vi).end())
    //    return 0;
    std::string mchr;
    if(mdata->gtfDatas[vi].nameTable.find(chr)!=mdata->gtfDatas[vi].nameTable.end())
         mchr=chr;
    else if(mdata->gtfDatas[vi].nameChrTable.find(chr)!=mdata->gtfDatas[vi].nameChrTable.end())
        mchr=chr.substr(3,chr.length()-3);
    else
        return 0;

    //int index=(mdata->gtfTable).at(vi)[chr];
    std::list<std::string> resultStr=mdata->gtfDatas[vi].getRegion(mchr,begPos,endPos);

    //ti_iter_t miter=ti_queryi(mdata->getGtfIx(vi),index, begPos, endPos);
    //int len;
    // std::string tmpStr;
    size_t curEnd[100];
    for(size_t i=0;i<100;++i)
        curEnd[i]=0;

    std::vector<std::string> arrStr;
    std::string oldGeneid;
    //dc.SetTextForeground(*wxBLACK);
    int maxLine=0;
    boost::unordered_map<std::string,int> transcripTable;
    //boost::unordered_map<std::string,int>::iterator transIte;
    int transcipLine=0;
    //int begPos=(int)(chrPos-rightPos/plotNucliedSize/2);
    //int endPos=(int)(chrPos+rightPos/plotNucliedSize/2);
    std::list<std::string>::iterator ite;
    for(ite=resultStr.begin();ite!=resultStr.end();++ite){
        //const char *str=ti_read(mdata->getGtfIx(vi),miter,&len);
        //dc.DrawText(wxString(str),0,300);

        //if(str==NULL)
        //    break;

        std::string tmpStr(*ite);
        split(tmpStr,'\t',arrStr);

        int readbeg=atoi((char*)arrStr[3].c_str());
        int readend=atoi((char*)arrStr[4].c_str());
        int rectangleBegin=readbeg>begPos?readbeg:begPos;
        int rectangelEnd=readend>endPos?endPos:readend;

        unsigned int relativeBeg=readbeg>begPos?readbeg-begPos:0;
        int relativeEnd=readend<endPos?readend-begPos:endPos-begPos;

        string transid=getId(arrStr[8],"transcript_id");
        string geneid=getId(arrStr[8],"gene_id");

        if(oldGeneid!=geneid){
            transcripTable.clear();
            oldGeneid=geneid;

            transcipLine=0;

        }

        std::string seqName=arrStr[2];
        if(seqName!="exon")
            continue;
        std::string geneName=getId(arrStr[8],"gene_name");
        if(arrStr[6]=="+")
            dc.SetBrush(*wxYELLOW_BRUSH);
        else
            dc.SetBrush(*wxBLUE_BRUSH);

        //transIte=transcripTable.find(transid);
        if(transcripTable.find(transid)==transcripTable.end()){
            //transcripTable.insert(std::make_pair(transid,transcipLine++));
            for(int i=transcipLine+1;i<100;++i){
                if(relativeBeg>=curEnd[i]){
                    dc.DrawRectangle((rectangleBegin-begPos)*(plotNucliedSize),pipeLine+i*height,(rectangelEnd-rectangleBegin+1)*(plotNucliedSize),height);

                    if(plotNucliedSize>1)
                    dc.DrawText(wxString(arrStr[1]+" "+arrStr[2]+" "+geneName),(rectangleBegin-begPos)*(plotNucliedSize),pipeLine+i*height);

                    //free(str);//fixed me!!!!!!!!!!!!!!!!!!!!!!!!!!!!//这个内存不许要释放，这个内存是由miter申请的


                    curEnd[i]+=relativeEnd;
                    transcipLine=i;
                    break;
                }


            }
            transcripTable[transid]=transcipLine;

            if(transcipLine>maxLine)
                maxLine=transcipLine;

            continue;
        }

        else{

            dc.DrawRectangle((rectangleBegin-begPos)*(plotNucliedSize),pipeLine+transcripTable[transid]*height,(rectangelEnd-rectangleBegin+1)*(plotNucliedSize),height);

            if(plotNucliedSize>1)
            dc.DrawText(wxString(arrStr[1]+" "+arrStr[2]+" "+geneName),(rectangleBegin-begPos)*(plotNucliedSize),pipeLine+transcripTable[transid]*height);

            //free(str);//fixed me!!!!!!!!!!!!!!!!!!!!!!!!!!!!//这个内存不许要释放，这个内存是由miter申请的
           if(transcipLine>maxLine)
                maxLine=transcipLine;
        }

    }



    //ti_iter_destroy(miter);
    return (maxLine)*(height);
}

int PlotBase::plotGTFCds(wxDC& dc,std::string chr,int chrPos,int begPos,int endPos,int vi){///also use plot gff
    std::string mchr;
    if(mdata->gtfDatas[vi].nameTable.find(chr)!=mdata->gtfDatas[vi].nameTable.end())
        mchr=chr;
    else if(mdata->gtfDatas[vi].nameChrTable.find(chr)!=mdata->gtfDatas[vi].nameChrTable.end())
        mchr=chr.substr(3,chr.length()-3);
    else
        return 0;

    //int index=(mdata->gtfTable).at(vi)[chr];
    std::list<std::string> resultStr=mdata->gtfDatas[vi].getRegion(mchr,begPos,endPos);

    //size_t curEnd[100];
    //for(size_t i=0;i<100;++i)
    //    curEnd[i]=0;

    //int len;
    dc.SetPen(*wxBLACK_PEN);
    //std::string tmpStr;
    std::vector<std::string> arrStr;
    std::string oldGeneid;
    //dc.SetTextForeground(*wxBLACK);
    int maxLine=0;
    boost::unordered_map<std::string,int> transcripTable;
    //std::map<std::string,int>::iterator transIte;
    int transcipLine=0;
    //int begPos=(int)(chrPos-rightPos/plotNucliedSize/2);
    //int endPos=(int)(chrPos+rightPos/plotNucliedSize/2);
    std::list<std::string>::iterator ite;
    for(ite=resultStr.begin();ite!=resultStr.end();++ite){
        //const char *str=ti_read(mdata->getGtfIx(vi),miter,&len);
        //dc.DrawText(wxString(str),0,200);

        //if(str==NULL)
        //    break;

        std::string tmpStr(*ite);
        split(tmpStr,'\t',arrStr);

        int readbeg=atoi((char*)arrStr[3].c_str());
        int readend=atoi((char*)arrStr[4].c_str());
        int rectangleBegin=readbeg>begPos?readbeg:begPos;
        int rectangelEnd=readend>endPos?endPos:readend;
        string transid=getId(arrStr[8],"transcript_id");
        string geneid=getId(arrStr[8],"gene_id");
        if(oldGeneid!=geneid){
            transcripTable.clear();
            oldGeneid=geneid;
            transcipLine=0;
        }

        std::string seqName=arrStr[2];
        if(seqName=="CDS"){
            //transIte=transcripTable.find(transid);
            if(transcripTable.find(transid)==transcripTable.end()){
                transcripTable[transid]=transcipLine++;

            }
           //transcripTable[transid]++;

            std::string geneName=getId(arrStr[8],"gene_name");
            int codonFrame=atoi((char*)arrStr[7].c_str());

            if(arrStr[6]=="+"){
                dc.SetBrush(*wxYELLOW_BRUSH);
            }
            else{
                codonFrame=(readend-readbeg-codonFrame)%3;
                dc.SetBrush(*wxBLUE_BRUSH);
            }
            //int x=(rectangleBegin-begPos)*plotFontSize/2;

            dc.DrawRectangle((rectangleBegin-begPos)*plotNucliedSize,pipeLine+transcripTable[transid]*height+height,(rectangelEnd-rectangleBegin+1)*plotNucliedSize,height);


            int initCodonLine;
            if(begPos>readbeg)
                initCodonLine=3-(begPos-readbeg-codonFrame)%3;
            else
                initCodonLine=readbeg-begPos+codonFrame;


            if(plotNucliedSize>1){
                if(arrStr[6]=="+")
                    for(int i=0;i<(rectangelEnd-rectangleBegin+1)/3;i++)
                        dc.DrawLine((initCodonLine+i*3)*plotNucliedSize,pipeLine+transcripTable[transid]*height+plotNucliedSize,(initCodonLine+i*3)*plotNucliedSize,pipeLine+(transcripTable[transid]+1)*plotNucliedSize+plotNucliedSize);
                else
                    for(int i=0;i<(rectangelEnd-rectangleBegin+1)/3;i++)
                        dc.DrawLine((initCodonLine+i*3)*plotNucliedSize,pipeLine+transcripTable[transid]*height+plotNucliedSize,(initCodonLine+i*3)*plotNucliedSize,pipeLine+(transcripTable[transid]+1)*plotNucliedSize+plotNucliedSize);


                dc.DrawText(wxString(arrStr[1]+" "+arrStr[2]+" "+geneName),(rectangleBegin-begPos)*plotNucliedSize,plotNucliedSize+pipeLine+transcripTable[transid]*height);
            }

            if(transcipLine>maxLine)
                maxLine=transcipLine;
        }
        if(seqName=="start_codon"){
            //wxDCBrushChanger(dc,*wxRED_BRUSH);
            //wxDCFontChanger(dc,wscut.pen10Pix);

            dc.SetBrush(*wxRED_BRUSH);
            //if(plotNucliedSize==10)
            //    dc.SetFont(wscut.pen15Pix);
            dc.DrawRectangle((rectangleBegin-begPos)*plotNucliedSize,pipeLine,(rectangelEnd-rectangleBegin+1)*plotNucliedSize,height);
            if(plotNucliedSize>1)
                dc.DrawText(_T("start"),(rectangleBegin-begPos)*plotNucliedSize,pipeLine);
        }

        if(seqName=="stop_codon"){
            dc.SetBrush(*wxRED_BRUSH);
            //if(plotNucliedSize==10)
                //dc.SetFont(wscut.pen15Pix);
            //wxDCBrushChanger(dc,*wxBLUE_BRUSH);
            //wxDCFontChanger(dc,wscut.pen5Pix);

            dc.DrawRectangle((rectangleBegin-begPos)*plotNucliedSize,pipeLine,(rectangelEnd-rectangleBegin+1)*plotNucliedSize,height);
            if(plotNucliedSize>1)
                dc.DrawText(_T("stop"),(rectangleBegin-begPos)*plotNucliedSize,pipeLine);

        }
        //free(str);//fixed me!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }

    //ti_iter_destroy(miter);

    return (maxLine+1)*(height);

}

int PlotBase::plotGff(wxDC& dc,std::string chr,int chrPos,int begPos,int endPos,int vi){
    std::string mchr;
    if(mdata->gffDatas[vi].nameTable.find(chr)!=mdata->gffDatas[vi].nameTable.end())
         mchr=chr;
    else if(mdata->gffDatas[vi].nameChrTable.find(chr)!=mdata->gffDatas[vi].nameChrTable.end())
        mchr=chr.substr(3,chr.length()-3);
    else
        return 0;

    //int index=(mdata->gtfTable).at(vi)[chr];
    std::list<std::string> resultStr=mdata->gffDatas[vi].getRegion(mchr,begPos,endPos);

    size_t curEnd[100];
    for(size_t i=0;i<100;++i)
        curEnd[i]=0;
    std::vector<std::string> arrStr;

    int len;
    unsigned int maxLine=0;
    //int begPos=(int)(chrPos-rightPos/plotNucliedSize/2);
    //int endPos=(int)(chrPos+rightPos/plotNucliedSize/2);

    std::list<std::string>::iterator ite;
    for(ite=resultStr.begin();ite!=resultStr.end();++ite){
    //while(true){
        //const char *str=ti_read(mdata->getGffIx(vi),miter,&len);
        //if(str==NULL)
        //    break;

        std::string tstr(*ite);
        split(tstr,'\t',arrStr);
        int readBeg=atoi((char*)arrStr[3].c_str());
        int readEnd=atoi((char*)arrStr[4].c_str());
        unsigned int relativeBeg=readBeg>begPos?readBeg-begPos:0;
        int relativeEnd=readEnd<endPos?readEnd-begPos:endPos-begPos;
        size_t i=0;
        for(;i<100;++i){
            if(relativeBeg>=curEnd[i]){
                dc.DrawRectangle(relativeBeg*plotNucliedSize,pipeLine+i*height,(relativeEnd-relativeBeg+1)*plotNucliedSize,height);

                if(plotNucliedSize>1){
                    wxString name=wxString(arrStr[1]+_("\t")+arrStr[2]);
                    dc.DrawText(name,relativeBeg*plotNucliedSize,pipeLine+i*height);
                }
                curEnd[i]+=relativeEnd;
            break;
            }
        }
        if(i>maxLine)
            maxLine=i;

    }
    //ti_iter_destroy(miter);

    return (maxLine+1)*(height);
}

int PlotBase::plotBed(wxDC& dc,std::string chr,int chrPos,int begPos,int endPos,int vi){
    std::string mchr;
    if(mdata->bedDatas[vi].nameTable.find(chr)!=mdata->bedDatas[vi].nameTable.end())
         mchr=chr;
    else if(mdata->bedDatas[vi].nameChrTable.find(chr)!=mdata->bedDatas[vi].nameChrTable.end())
        mchr=chr.substr(3,chr.length()-3);
    else
        return 0;

    //int index=(mdata->gtfTable).at(vi)[chr];
    std::list<std::string> resultStr=mdata->bedDatas[vi].getRegion(mchr,begPos,endPos);

    size_t curEnd[100];
    for(size_t i=0;i<100;++i)
        curEnd[i]=0;
    std::vector<std::string> arrStr;

    //int len;
    int maxLine=0;
    //int begPos=(int)(chrPos-rightPos/plotNucliedSize/2);
    //int endPos=(int)(chrPos+rightPos/plotNucliedSize/2);

    std::list<std::string>::iterator ite;
    for(ite=resultStr.begin();ite!=resultStr.end();++ite){
    //while(true){
        //const char *str=ti_read(mdata->getBedIx(vi),miter,&len);
        //if(str==NULL)
        //    break;
        std::string tstr(*ite);
        split(tstr,'\t',arrStr);
        int readBeg=atoi((char*)arrStr[1].c_str());
        int readEnd=atoi((char*)arrStr[2].c_str());
        int relativeBeg=readBeg>begPos?readBeg-begPos:0;
        int relativeEnd=readEnd<endPos?readEnd-begPos:endPos-begPos;
        size_t i=0;
        for(;i<100;++i){
            if(relativeBeg>=curEnd[i]){
                dc.DrawRectangle(relativeBeg*plotNucliedSize,pipeLine+i*height,(relativeEnd-relativeBeg+1)*plotNucliedSize,height);
                if(plotNucliedSize>1)
                    dc.DrawText(wxString(arrStr[3]+_("\t")+arrStr[5]),relativeBeg*plotNucliedSize,pipeLine+i*height);
                curEnd[i]+=relativeEnd;
            break;
            }
        }
        if(i>maxLine)
            maxLine=i;

    }
    //ti_iter_destroy(miter);

    return (maxLine+1)*(height);
}

int PlotBase::plotVcf(wxDC& dc,std::string chr,int chrPos,int begPos,int endPos,int vi){
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
        //col[(int)(w/30)]=arrStr[3]+"-"+arrStr[4]+"\t"+arrStr[7];
        dc.DrawLine(w,pipeLine+10,w,pipeLine+20);
        dc.DrawText(wxString(arrStr[2]),w,pipeLine);
        //free((char*)str);//fixed me!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }

    //ti_iter_destroy(miter);
    //posMessageTable[(int)(pipeLine/30)]=col;
    return 3*10;
}

int PlotBase::plotBam(wxDC& dc,std::string chr,int chrPos,int begPos,int endPos,int index){

    //int chrIndex=mbamdata->bamMap[chr]>mbamdata->bamMapChr[chr]?mbamdata->bamMap[chr]:mbamdata->bamMapChr[chr];
    int chrIndex;
    if(mbamdata->bamMap.find(chr)!=mbamdata->bamMap.end())
        chrIndex=mbamdata->bamMap[chr];
    else if(mbamdata->bamMapChr.find(chr)!=mbamdata->bamMapChr.end())
        chrIndex=mbamdata->bamMapChr[chr];
    else{
        wxMessageBox(_T("bam hase not this chr=")+wxString(chr));
        return 0;
    }
    dc.SetBrush(*wxYELLOW_BRUSH);
    dc.SetTextForeground(*wxRED);
    dc.SetFont((wscut.pen2010Pix));
    int maxLine=Mbam_fetch(mbamdata->bamIndex,chrIndex,begPos,endPos,dc);

    dc.DrawLine(0,pipeLine+height*2*100,(endPos-begPos)*plotNucliedSize,pipeLine+(height)*2*100);
    return maxLine*20;

}

int PlotBase::MbamHelp(bam1_t* b,wxDC& dc,int beg, int end,int &maxLine,int& lastPos,int &lastLine){
    int tlastLine=0;

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

int PlotBase::Mbam_fetch(const bam_index_t *idx, int tid, int beg,int end,wxDC& dc){

	for(int i=0;i<100;++i) curEndPos[i]=0;

	int lastPos=0;//this variable is for speed
	int maxLine=0;
	int lastLine=0;

	int ret;
	bam_iter_t iter;

	bam1_t *b;
	b = bam_init1();
	iter = bam_iter_query(idx, tid, beg, end);
	while ((ret = bam_iter_read(mbamdata->bamf, iter, b)) >= 0)
        if(mdata->getFastaFile()!=wxEmptyString&&viewFaFlag==true)
            MbamHelp(b,dc,beg,end,maxLine,lastPos,lastLine);
        else
            MbamHelpNOFa(b,dc,beg,end,maxLine,lastPos,lastLine);


	bam_iter_destroy(iter);
	bam_destroy1(b);


	//return (ret == -1)? 0 : ret;
	return lastLine+1;
}
