#ifndef STL_HELP_H
#define STL_HELP_H

#include <map>
#include <vector>
#include <string>
#include <iterator>

using std::vector;
using std::string;
using std::map;

double ld(const double x);
template<typename T1,typename T2>
T1 maxFirst(map<T1,T2> &tmap){
    typename map<T1,T2>::iterator ite;
    ite=tmap.begin();
    T1 max=ite->first;
    for(ite=tmap.begin();ite!=tmap.end();++ite)
        if(ite->first>max)
            max=ite->first;
    return max;
}

template<typename T1,typename T2>
T2 maxSecond(map<T1,T2> &tmap){
    typename map<T1,T2>::iterator ite;
    ite=tmap.begin();
    T2 max=ite->second;
    for(ite=tmap.begin();ite!=tmap.end();++ite)
        if(ite->second>max)
            max=ite->second;
    return max;
}


template<typename T1,typename T2>
T1 minFirst(map<T1,T2> &tmap){
    typename map<T1,T2>::iterator ite;
    ite=tmap.begin();
    T1 min=ite->first;
    for(ite=tmap.begin();ite!=tmap.end();++ite)
        if(ite->first<min)
            min=ite->first;
    return min;
}

template<typename T1,typename T2>
T2 minSecond(map<T1,T2> &tmap){
    typename map<T1,T2>::iterator ite;
    ite=tmap.begin();
    T2 min=ite->second;
    for(ite=tmap.begin();ite!=tmap.end();++ite)
        if(ite->second<min)
            min=ite->second;
    return min;
}
class strOder{
public:
    bool operator ()(string str1,string str2){
        if(str1.length()<str2.length())
            return true;
        else if(str1.length()>str2.length())
            return false;

        else if(str1<str2)
            return true;

        return false;
    }

};

template<class miterator>
void toZero(miterator beg,miterator end){
    miterator iter;
    for(iter=beg;iter!=end;++iter){
        *iter=0;
    }
}
template<class Type>
void toZero(Type &t){
    typename Type::iterator iter;
    for(iter=t.begin();iter!=t.end();++iter){
        *iter=0;

    }

}

string getId(string &str,const char* s);

void split(const string& dataLine,char cut,vector<string> & arrayString);

int ksplit_core(char *s, int delimiter, int *_max, int **_offsets);

inline int *split(const char *s, int delimiter, int *n);
/*
namespace __gnu_cxx
{
        template<> struct hash< std::string >
        {
                size_t operator()( const std::string& s ) const
                 {
                     unsigned long __h = 0;
                        for (unsigned i = 0;i < s.size();++i)
                        __h ^= (( __h << 5) + (__h >> 2) + s[i]);
                        return size_t(__h);

                }


        };
}

*/



#endif // HELP_H

