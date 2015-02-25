#include "pilot_workspace.h"
#include <wx/msgdlg.h>
#ifdef __WXGTK__
#include <cstdlib>
#endif

void Pilot_WorkSpace::writeWorkSpace(){
#ifdef __WXMSW__
    wxString olds=_T("\\");
    wxString news=_T("\\");
#else
    wxString olds=_T("/");
    wxString news=_T("/");

#endif
    //wxXmlNode* root2=new wxXmlNode(wxXML_ELEMENT_NODE,_T("workspace"));

    wxXmlNode* root=new wxXmlNode(wxXML_ELEMENT_NODE,_T("workspace"));

    wxXmlNode* fasta=new wxXmlNode(root, wxXML_ELEMENT_NODE,_T("Fasta"));

    fasta->AddAttribute(_T("number"),wxString::FromDouble(fastaFiles.size()));
    for(unsigned int i=0;i<fastaFiles.size();++i){
        fastaFiles[i].Replace(olds,news,true);
        //wxMessageBox(fastaFiles[0]);
        fasta->AddAttribute(_T("Property")+wxString::FromDouble(i),fastaFiles[i]);
    }

    wxXmlNode* bam=new wxXmlNode(root, wxXML_ELEMENT_NODE,_T("Bam"));
    bam->AddAttribute(_T("number"),wxString::FromDouble(bamFiles.size()));
    for(size_t i=0;i<bamFiles.size();++i){
        bamFiles[i].Replace(olds,news,true);
        bam->AddAttribute(_T("Property")+wxString::FromDouble(i),bamFiles[i]);
    }

    wxXmlNode* bed=new wxXmlNode(root, wxXML_ELEMENT_NODE,_T("Bed"));
    bed->AddAttribute(_T("number"),wxString::FromDouble(bedFiles.size()));
    for(size_t i=0;i<bedFiles.size();++i){
        bedFiles[i].Replace(olds,news,true);
        bed->AddAttribute(_T("Property")+wxString::FromDouble(i),bedFiles[i]);
    }

    wxXmlNode* gff=new wxXmlNode(root, wxXML_ELEMENT_NODE,_T("Gff"));
    gff->AddAttribute(_T("number"),wxString::FromDouble(gffFiles.size()));
    for(size_t i=0;i<gffFiles.size();++i){
        gffFiles[i].Replace(olds,news,true);
        gff->AddAttribute(_T("Property")+wxString::FromDouble(i),gffFiles[i]);
    }

    wxXmlNode* gtf=new wxXmlNode(root, wxXML_ELEMENT_NODE,_T("Gtf"));
    gtf->AddAttribute(_T("number"),wxString::FromDouble(gtfFiles.size()));
    for(size_t i=0;i<gtfFiles.size();++i){
        gtfFiles[i].Replace(olds,news,true);
        gtf->AddAttribute(_T("Property")+wxString::FromDouble(i),gtfFiles[i]);
    }

    wxXmlNode* vcf=new wxXmlNode(root, wxXML_ELEMENT_NODE,_T("Vcf"));
    vcf->AddAttribute(_T("number"),wxString::FromDouble(vcfFiles.size()));
    for(size_t i=0;i<vcfFiles.size();++i){
        vcfFiles[i].Replace(olds,news,true);

        vcf->AddAttribute(_T("Property")+wxString::FromDouble(i),vcfFiles[i]);
    }
    char str[10];
    wxString tt;
    for(size_t i=0;i<windowProperty.size();++i){
        wxXmlNode* pw=new wxXmlNode(root, wxXML_ELEMENT_NODE,_T("PlotWindow"));
            for(int j=0;j<4;++j){
                sprintf(str,"%d",j);
                switch(j){
                    case 0:
                        pw->AddAttribute(_T("Property")+wxString(str),wxString(windowProperty[i].chr));
                        break;
                    case 1:

                        tt=wxString(windowProperty[i].bam);
                        tt.Replace(olds,news,true);
                        pw->AddAttribute(_T("Property")+wxString(str),tt);

                        break;
                    case 2:
                        pw->AddAttribute(_T("Property")+wxString(str),wxString::Format(wxT("%ld"),windowProperty[i].pos));
                        break;
                    case 3:
                        pw->AddAttribute(_T("Property")+wxString(str),wxString::Format(wxT("%d"),windowProperty[i].page));
                        break;

                }
            }
    }
   // workSpace.SetDocumentNode(root);

    workSpace.SetRoot(root);
    wxString xmlPath=_T(".viewWorkSpace.xml");
#ifdef __WXGTK__
xmlPath=wxString(getenv("HOME"))+wxString(_T("/"))+xmlPath;

#endif    

    workSpace.Save(xmlPath);
}

void Pilot_WorkSpace::readWorkSpaceNew(){
    strTable.clear();
    strTable.insert(std::make_pair(_T("PlotWindow"),0));
    strTable.insert(std::make_pair(_T("Fasta"),1));
    strTable.insert(std::make_pair(_T("Bam"),2));
    strTable.insert(std::make_pair(_T("Bed"),3));
    strTable.insert(std::make_pair(_T("Gff"),4));
    strTable.insert(std::make_pair(_T("Gtf"),5));
    strTable.insert(std::make_pair(_T("Vcf"),6));

    wxString xmlPath=_T(".viewWorkSpace.xml");
#ifdef __WXGTK__
    xmlPath=wxString(getenv("HOME"))+wxString(_T("/"))+xmlPath;

#endif   

    workSpace.Load(xmlPath);

    if (workSpace.GetRoot()->GetName() != _T("workspace"))
        return ;

    wxXmlNode *child = workSpace.GetRoot()->GetChildren();

    wxString n;int nn;char convert[10];
    PlotWindowProperty pro;
    while(child){
        std::string tstr=child->GetName().ToStdString();
        //int tt;
        switch(strTable[child->GetName()]){

            case 0:
            for(int j=0;j<4;++j){
                sprintf(convert,"%d",j);
                wxString str=child->GetAttribute((_T("Property")+wxString(convert)));
                //std::string tmpStr=str.ToStdString();
                switch(j){
                    case 0:
                        pro.chr=str.ToStdString();
                        break;
                    case 1:
                        pro.bam=str.ToStdString();
                        //wxMessageBox(pro.bam);
                        break;
                    case 2:
                        //str.ToLong(&nn);
                        pro.pos=atol(str.c_str());
                        break;
                    case 3:
                        //str.ToLong(&nn);
                        pro.page=atoi(str.c_str());
                        break;
                    }
            }
            windowProperty.push_back(pro);
            child=child->GetNext();
            break;
            case 1:
                n=child->GetAttribute(_T("number"));
                //n.ToLong(&nn);
                nn=atoi(n.c_str());
                for(int j=0;j<nn;++j){
                    wxString fa=child->GetAttribute(_T("Property")+wxString::Format(wxT("%d"),j));
                    //wxMessageBox(fa);
                    fastaFiles.push_back(fa);//vcfFiles.Add(vcf);
                    //wxMessageBox(fastaFiles[0]);
                    //wxMessageBox(fastaFiles.FindAbsoluteValidPath(fastaFiles[0]));
                }
            child=child->GetNext();
            break;

            case 2:
                n=child->GetAttribute(_T("number"));
                //n.ToLong(&nn);
                nn=atoi(n.c_str());

                for(int j=0;j<nn;++j){
                    wxString bam=child->GetAttribute(_T("Property")+wxString::Format(wxT("%d"),j));
                    bamFiles.push_back(bam);//vcfFiles.Add(vcf);
                }
            child=child->GetNext();
            break;

            case 3:
                n=child->GetAttribute(_T("number"));
                //n.ToLong(&nn);
                nn=atoi(n.c_str());


                for(int j=0;j<nn;++j){
                    wxString bed=child->GetAttribute(_T("Property")+wxString::Format(wxT("%d"),j));
                    bedFiles.push_back(bed);//vcfFiles.Add(vcf);
                }
            child=child->GetNext();
            break;

            case 4:
                n=child->GetAttribute(_T("number"));
                //n.ToLong(&nn);
                nn=atoi(n.c_str());


                for(int j=0;j<nn;++j){
                    wxString gff=child->GetAttribute(_T("Property")+wxString::Format(wxT("%d"),j));
                    gffFiles.push_back(gff);//vcfFiles.Add(vcf);
                }
            child=child->GetNext();
            break;

            case 5:
                n=child->GetAttribute(_T("number"));
                //n.ToLong(&nn);
                nn=atoi(n.c_str());


                for(int j=0;j<nn;++j){
                    wxString gtf=child->GetAttribute(_T("Property")+wxString::Format(wxT("%d"),j));
                    gtfFiles.push_back(gtf);//vcfFiles.Add(vcf);
                }
            child=child->GetNext();
            break;

            case 6:
                n=child->GetAttribute(_T("number"));
                //n.ToLong(&nn);
                nn=atoi(n.c_str());

                for(int j=0;j<nn;++j){
                    wxString vcf=child->GetAttribute(_T("Property")+wxString::Format(wxT("%d"),j));
                    vcfFiles.push_back(vcf);//vcfFiles.Add(vcf);
                }
            child=child->GetNext();
            break;


        }

    }
}


