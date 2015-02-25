#ifndef CHROSOMECHOICE_H
#define CHROSOMECHOICE_H

#include <wx/combo.h>
#include <wx/listctrl.h>


class ChrosomeChoice:public wxListView,public wxComboPopup{
private:
   // pilot_viewFrame& root;
public:
    ChrosomeChoice(/*pilot_viewFrame& parent*/);

    virtual void Init();
    virtual bool Create(wxWindow* parent);
    virtual wxWindow *GetControl();
    virtual void SetStringValue(const wxString& s);
    virtual wxString GetStringValue() const;
    virtual void OnComboDoubleClick ();
    void OnDoubleClick(wxMouseEvent& event);

protected:
    int m_value;

private:

    wxDECLARE_EVENT_TABLE();
};


#endif // CHROSOMECHOICE_H
