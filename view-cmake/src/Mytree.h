#ifndef MYTREECTRL_H
#define MYTREECTRL_H

class MyTreeCtrl:public wxTreeCtrl{

public:
    MyTreeCtrl(wxWindow *parent, const wxWindowID id,const wxPoint& pos, const wxSize& size,long style): wxTreeCtrl(parent, id, pos, size, style){}
    void test(wxTreeEvent& event){
            wxMessageBox(_T("heihei"));
        }
private:
    enum{
        ID_TREE=1000;
    };


};


class MyTreeItemData:public wxTreeItemData{

};

#endif // MYTREECTRL_H
