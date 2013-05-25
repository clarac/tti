#ifndef tti_H_
#define tti_H_

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <vector>

#define tti_AVG 0
#define tti_AVG1 -1

class tti {
public:
	tti();
	virtual ~tti();
	FILE * callT(int cars, int laps, std::string track, int dind, std::string driver);
	std::vector<float> getSingleTimeList(int laps, std::string track, int dind, std::string driver);
	float getSingleTime(int laps, std::string track, int ret, int dind, std::string driver);
	std::vector<float> getSingleDistanceList(int laps, std::string track, int dif, int dind, std::string driver);
	
};

#endif /* tti_H_ */
