#include "stl_help.h"
#include <iterator>
#include <string.h>
using std::vector;
using std::string;


string getId(string &str,const char* s){
    string::iterator strite;
    size_t n=str.find(s);
    size_t l=strlen(s);
    size_t end=str.find(';',n);
    return str.substr(n+l+1,end-(n+l)-1);
}

void split(const string& dataLine,char cut,vector<string> & arrayString){


    arrayString.clear();
    string tmpString;
    string::const_iterator it;
    for(it=dataLine.begin(); it!=dataLine.end(); it++)
    {
        if(*it!=cut)
            tmpString+=*it;
        else
        {
            arrayString.push_back(tmpString);
            tmpString.clear();
        }

    }
    arrayString.push_back(tmpString);
    // cout<<arrayString.at(0)<<endl;
    //return arrayString;
}

