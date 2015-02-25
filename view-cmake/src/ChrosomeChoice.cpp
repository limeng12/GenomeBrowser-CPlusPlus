#include "ChrosomeChoice.h"

wxBEGIN_EVENT_TABLE(ChrosomeChoice, wxListView)

EVT_LEFT_UP(ChrosomeChoice::OnDoubleClick)

wxEND_EVENT_TABLE()

ChrosomeChoice::ChrosomeChoice(/*pilot_viewFrame& parent):root(parent*/){

}

void ChrosomeChoice::Init(){

}

bool ChrosomeChoice::Create(wxWindow* parent){
    return wxListView::Create(parent,1,wxDefaultPosition,wxDefaultSize);

}

wxWindow* ChrosomeChoice::GetControl(){
return this;
}

void ChrosomeChoice::SetStringValue(const wxString& s){
    int n = wxListView::FindItem(-1,s);
    if ( n >= 0 && n < wxListView::GetItemCount() )
    wxListView::Select(n);

}

wxString ChrosomeChoice::GetStringValue() const{
 if ( m_value >= 0 )
    return wxListView::GetItemText(m_value);
    return wxEmptyString;

}

void ChrosomeChoice::OnComboDoubleClick(){
    //m_value=wxListView::GetFirstSelected();
   // Dismiss();
    //root.OnSelectChr();
}

void ChrosomeChoice::OnDoubleClick(wxMouseEvent& event){
    m_value=wxListView::GetFirstSelected();
    Dismiss();

}
