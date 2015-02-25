

#include "statistic_plot.h"
bool LenDis::GetNextXY(double & x,double &y){
if(ite!=len.end()){
        x=ite->first;
        y=ite->second;
        ++ite;
        return true;
    }
    else
        return false;
}

bool QuaDis::GetNextXY(double & x,double &y){
if(ite!=len.end()){
        x=ite->first;
        y=ite->second;
        ++ite;
        return true;
    }
    else
        return false;
}

bool NM::GetNextXY(double & x,double &y){
if(ite!=len.end()){
        x=ite->first;
        y=ite->second;
        ++ite;
        return true;
    }
    else
        return false;
}

bool Coverage::GetNextXY(double & x,double &y){
if(ite!=len.end()){
        x=ite->first;
        y=ite->second;
        ++ite;
        return true;
    }
    else
        return false;
}

bool SoftC::GetNextXY(double & x,double &y){
if(ite!=len.end()){
        x=ite->first;
        y=ite->second;
        ++ite;
        return true;
    }
    else
        return false;
}

bool RegionCount::GetNextXY(double & x,double &y){
if(ite!=len.end()){
        //x=ite->first;
        x=id++;
        y=ite->second;
        ++ite;
        return true;
    }
    else
        return false;
}
