#include "filterDialog.h"

BEGIN_EVENT_TABLE(FilterDialog,wxDialog)

    EVT_BUTTON(wxID_ADDBUTTON,FilterDialog::OnAddButton)

END_EVENT_TABLE()


FilterDialog::FilterDialog():wxDialog(){
//filter=new BamFilter();

}

FilterDialog::FilterDialog(wxWindow* parent,wxWindowID id,const wxString& caption,const wxPoint& pos,const wxSize& size,long style):wxDialog(parent,id,caption,pos,size,style){

            createControl();

};



bool FilterDialog::Create(wxWindow* parent,wxWindowID id,
                const wxString& caption,
                const wxPoint& pos,
                const wxSize& size,
                long style){

                    wxDialog::Create(parent,id,caption,pos,size,style);
                    //SetMinSize(wxSize(500,500));

                    createControl();
return true;
}

void FilterDialog::createControl(){
    command=new wxPanel(this,wxID_ANY);
        wxBoxSizer* commandSizer=new wxBoxSizer(wxHORIZONTAL);
            commandSizer->Add(cancel=new wxButton(command,wxID_CANCEL,wxT("Cancel")),0,wxEXPAND,5);
            commandSizer->Add(ok=new wxButton(command,wxID_OK,wxT("OK")),1,wxEXPAND,5);
        command->SetSizer(commandSizer);

    rootSizer=new wxBoxSizer(wxVERTICAL);

        wxPanel* scPanel=new wxPanel(this,wxID_SCPANEL);
            wxBoxSizer* scPanelSizer=new wxBoxSizer(wxHORIZONTAL);
                scPanelSizer->Add(new wxStaticText(scPanel,wxID_ANY,wxT("softclipping<")),0,wxEXPAND,5);
                scPanelSizer->Add(scTextCtrl=new wxTextCtrl(scPanel,wxID_ANY,wxT("")),1,wxEXPAND,5);
        scPanel->SetSizer(scPanelSizer);
    rootSizer->Add(scPanel,1,wxEXPAND,5);
/*
    wxPanel* hdPanel=new wxPanel(this,wxID_HDPANEL);
        wxBoxSizer* hdPanelSizer=new wxBoxSizer(wxHORIZONTAL);
            hdPanelSizer->Add(new wxStaticText(hdPanel,wxID_ANY,wxT("hardClipping<")),0,wxEXPAND,5);
            hdPanelSizer->Add(hdTextCtrl=new wxTextCtrl(hdPanel,wxID_ANY,wxT("")),1,wxEXPAND,5);
        hdPanel->SetSizer(hdPanelSizer);
    rootSizer->Add(hdPanel,1,wxEXPAND,5);
*/
    rootSizer->Add(uniqueCtrl=new wxCheckBox(this,wxID_UNIQUE,wxT("Unique Bams")));


    rootSizer->AddSpacer(5);
    wxStaticText *optionField=new wxStaticText(this,wxID_ANY,wxT("input the tag filter\neg:If you want to shor NM<=2 input: NM>2\nThree operators '<' '=' '>' are support"));
    rootSizer->Add(optionField,1,wxEXPAND,5);
    rootSizer->AddSpacer(5);

    wxPanel* c1Panel=new wxPanel(this,wxID_ANY);
        wxBoxSizer* c1Sizer=new wxBoxSizer(wxHORIZONTAL);
            wxTextCtrl *c1Ctrl;
            c1Sizer->Add(c1Ctrl=new wxTextCtrl(c1Panel,wxID_ANY,wxT("")),1,wxEXPAND,5);
            c1Sizer->Add(addButton=new wxButton(c1Panel,wxID_ADDBUTTON,wxT("Add")),1,wxEXPAND,5);
    c1Panel->SetSizer(c1Sizer);
    rootSizer->Add(c1Panel,1,wxEXPAND,5);
    filterStringCtrl.push_back(boost::make_tuple(c1Panel,c1Sizer,c1Ctrl));

    rootSizer->Add(command,1,wxEXPAND,5);
    //rootSizer->SetMinSize(200,200);
    SetSizer(rootSizer);
    Fit();
}

void FilterDialog::OnAddButton(wxCommandEvent& event){
    rootSizer->AddSpacer(5);
    rootSizer->Detach(command);

    int lastIndex=filterStringCtrl.size();
    filterStringCtrl.at(lastIndex-1).get<1>()->Detach(addButton);
    delete addButton;

    wxPanel* c1Panel=new wxPanel(this,wxID_ANY);
        wxBoxSizer* c1Sizer=new wxBoxSizer(wxHORIZONTAL);
            wxTextCtrl *c1Ctrl;
            c1Sizer->Add(c1Ctrl=new wxTextCtrl(c1Panel,wxID_ANY,wxT("")),1,wxEXPAND,5);
            c1Sizer->Add(addButton=new wxButton(c1Panel,wxID_ADDBUTTON,wxT("Add")),1,wxEXPAND,5);
        c1Panel->SetSizer(c1Sizer);

    filterStringCtrl.push_back(boost::make_tuple(c1Panel,c1Sizer,c1Ctrl));
    size_t index=rootSizer->GetItemCount();
    rootSizer->Add(c1Panel,1,wxEXPAND,5);
    rootSizer->Add(command,1,wxEXPAND,5);
    Fit();
}

bool FilterDialog::TransferDataFromWindow(){

    sc=atoi(scTextCtrl->GetValue().c_str());
    //hd=atoi(hdTextCtrl->GetValue().c_str());
    unique=uniqueCtrl->IsChecked();
    filterStrings.clear();
    for(unsigned int i=0;i<filterStringCtrl.size();++i){
        wxString tfilter=filterStringCtrl.at(i).get<2>()->GetValue();
        if(tfilter.length()>0)
            filterStrings.push_back(tfilter.ToStdString());

    }
    EndModal(wxID_OK);
    //SetReturnCode(wxID_OK);
    //this->Show(false);
    return true;
}
