#ifndef tti_H_
#define tti_H_

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <vector>
#include "Client.h"
#include <tgf.h>
#include <wordexp.h>
#include <thread>
#include <future>


#define tti_AVG 0
#define tti_AVG1 -1

struct raceData{
	int damage, ticksOutside;
	float totalTime, avgTime;
	std::vector<float> timeList;
};

class tti {
public:

	tti();
	tti(std::vector<WrapperBaseDriver*> dv);

	float getAvg(std::vector<float> values, int from=0, int to=-1);
	float getTotal(std::vector<float> values, int from=0, int to=-1);

	// local mode: inform driver (module) name and index

	std::vector<float> getSingleTimeList(int laps, std::string track, int dind, std::string driver);
	float getSingleTime(int laps, std::string track, int ret, int dind, std::string driver);
	std::vector<float> getSingleDistanceList(int laps, std::string track, int dif, int dind, std::string driver);

	// client-server mode for a single driver (returns time array only)
	// more info publicly accessible in the driver (after the race)
	
	std::vector<float> getSingleTimeListS(int laps, std::string track, WrapperBaseDriver *driver);

	// must have set driver array to use the next two methods

	// torcs in text-mode, faster
	
	std::vector<raceData> race(int laps, std::string track);

	// GUI: configure race manually

	std::vector<raceData> raceGUI();

private:

	// inform driver name and index (practice)

	FILE * callTL(int laps, std::string track, int dind, std::string driver);

	// (practice)

	FILE * callT(int laps, std::string track, WrapperBaseDriver *driver);

	// must have used constructor with a driver array (up to 10)
	// above one is (or should be) faster for a single driver
	
	void callTMult(int laps, std::string track);
	void callTGUI();

	std::vector<WrapperBaseDriver*> drivers;
};

#endif /* tti_H_ */
