#ifndef STATIC_PLOT_H
#define STATIC_PLOT_H

#include <map>

#include "mathplot.h"
#include "stl_help.h"
//这里我可以只使用一个类，但是我为了表达更清晰,使用了多个

class LenDis:public mpFXY{

private:
    std::map<int,int>::iterator ite;
    std::map<int,int> len;
public:
    LenDis(std::map<int,int> & tmp):mpFXY(wxT("length distribution")),len(tmp){
        ite=len.begin();
    }

    virtual bool GetNextXY(double & x,double &y);

    virtual void Rewind(){ite=len.begin();}
    virtual double GetMaxX(){return maxFirst(len);}
    virtual double GetMinX(){return minFirst(len);}
    virtual double GetMaxY(){return maxSecond(len);}
    virtual double GetMinY(){return minSecond(len);}

};

class QuaDis:public mpFXY{

private:
    std::map<int,int>::iterator ite;
    std::map<int,int> len;
public:
    QuaDis(std::map<int,int> & tmp):mpFXY(wxT("quality distribution")),len(tmp){
        ite=len.begin();
    }

    virtual bool GetNextXY(double & x,double &y);

    virtual void Rewind(){ite=len.begin();}
    virtual double GetMaxX(){return maxFirst(len);}
    virtual double GetMinX(){return minFirst(len);}
    virtual double GetMaxY(){return maxSecond(len);}
    virtual double GetMinY(){return minSecond(len);}

};

class NM:public mpFXY{

private:
    std::map<int,int>::iterator ite;
    std::map<int,int> len;
public:
    NM(std::map<int,int> & tmp):mpFXY(wxT("edit distance distribution")),len(tmp){
        ite=len.begin();
    }

    virtual bool GetNextXY(double & x,double &y);

    virtual void Rewind(){ite=len.begin();}
    virtual double GetMaxX(){return maxFirst(len);}
    virtual double GetMinX(){return minFirst(len);}
    virtual double GetMaxY(){return maxSecond(len);}
    virtual double GetMinY(){return minSecond(len);}

};

class Coverage:public mpFXY{

private:
    std::map<int,int>::iterator ite;
    std::map<int,int> len;
public:
    Coverage(std::map<int,int> & tmp):mpFXY(wxT("coverage distribution")),len(tmp){
        ite=len.begin();
    }

    virtual bool GetNextXY(double & x,double &y);

    virtual void Rewind(){ite=len.begin();}
    virtual double GetMaxX(){return maxFirst(len);}
    virtual double GetMinX(){return minFirst(len);}
    virtual double GetMaxY(){return maxSecond(len);}
    virtual double GetMinY(){return minSecond(len);}

};


class SoftC:public mpFXY{

private:
    std::map<int,int>::iterator ite;
    std::map<int,int> len;
public:
    SoftC(std::map<int,int> & tmp):mpFXY(wxT("softclipping distribution")),len(tmp){
        ite=len.begin();
    }

    virtual bool GetNextXY(double & x,double &y);

    virtual void Rewind(){ite=len.begin();}
    virtual double GetMaxX(){return maxFirst(len);}
    virtual double GetMinX(){return minFirst(len);}
    virtual double GetMaxY(){return maxSecond(len);}
    virtual double GetMinY(){return minSecond(len);}

};

class RegionCount:public mpFXY{

private:
    std::map<std::string,float>::iterator ite;
    std::map<std::string,float> len;
    int id;
public:
    RegionCount(std::map<std::string,float> & tmp):mpFXY(wxT("region count")),len(tmp){
        ite=len.begin();
        id=0;
    }

    virtual bool GetNextXY(double & x,double &y);

    virtual void Rewind(){ite=len.begin();id=0;}
    virtual double GetMaxX(){
        size_t index=0;
        for(ite=len.begin();ite!=len.end();++ite)
            index++;
        return index;
    }

    virtual double GetMinX(){return 0;}
    virtual double GetMaxY(){return maxSecond(len);}
    virtual double GetMinY(){return minSecond(len);}

};


#endif // STATIC_PLOT_H
