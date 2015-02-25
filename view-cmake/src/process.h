////limeng s6228483@yahoo.com.cn
//process the bam file, and get the statistic information
#ifndef PROCESS_H
#define PROCESS_H

#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#include <bam.h>

class Process{
public:
	std::string bamfile;
	std::string bedfile;

public:
	//region//感兴趣区域//25条染色体
	std::vector<std::map<int,int> > inteRegion;
	// 统计每个区域的碱基的个数
	std::map<std::string,float> regionCount;

	//length distribution
	std::map<int,int> lengthDis;int tmpLength;

	//quality
	std::map<int,int> mapQuality;int tmpQuality;

	//edit distance
	std::map<int,int> nm;int tmpNm;
	//soft clipping
	std::map<int,int> sc;int tmpSc;
	//coverage of every base
	std::vector<std::map<long int ,int> > coverage;
	//coverage distribution
	std::map<int,int> codis;int tmpCd;

    bool finishFlag;

public:
	Process();
	~Process();
	void split(const std::string& dataLine,char cut,std::vector<std::string> & arrayString);

	void getInteRegion(std::string filename="HSMv12.1_reqions_NO_JAK2_NODUP.bed");

	bool testRegion(int chrn,int position);

	bool testRegionAppro(int chrn,int position,int length);

	void processBam(std::string filename,bool hasbed=false);

};

#endif
