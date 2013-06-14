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

/// normal linear average
#define tti_AVG -1

/// linear average ignoring first value
#define tti_AVG1 -2

/// location of xml race congif files
#define xmlFolder std::string(getenv("TORCS_BASE"))+ "/src/tti/setups/"


/**
* Keeps basic race info
*/
struct raceData{
	int damage, ticksOutside;
	float totalTime, avgTime;
	std::vector<float> timeList;
};

class tti {
public:

	/**
	* Empty constructor, use with local driver mode
	*/
	tti();

	/**
	* Client-server mode constructor: keeps array of drivers for later use
	* 
	* @param	dv 	array of drivers (should contain between 1 and 10 of them)
	*/
	tti(std::vector<WrapperBaseDriver*> dv);

	/**
	* Returns the linear average for a section in a vector of float values. All of it by default.
	* @param	values	the array
	* @param	from	first position of the interval, defaults to 0
	* @param	to 		last position of the interval, defaults to -1 (last position)
	*/
	float getAvg(std::vector<float> values, int from=0, int to=-1);

	/**
	* Returns the sum of a section in a vector of float values. All of it by default.
	* @param	values	the array
	* @param	from	first position of the interval, defaults to 0
	* @param	to 		last position of the interval, defaults to -1 (last position)
	*/
	float getTotal(std::vector<float> values, int from=0, int to=-1);

	// local mode: inform driver (module) name and index

	/**
	* Returns a vector of floats corresponding to the time of each lap. Used with local drivers.
	* <em>Important</em>: the array returned might cointain less items than the laps specified if
	* the driver is taken out of the race early
	* @param	laps	number of laps to race
	* @param	track	name of the track (see list of tracks and drivers)
	* @param	dind	index of driver
	* @param	driver  name of the driver module 
	*/
	std::vector<float> getSingleTimeList(int laps, std::string track, int dind, std::string driver);

	/**
	* Returns an average corresponding to the time of each lap. Used with local drivers.
	* @param	laps	number of laps to race
	* @param	track	name of the track (see list of tracks and drivers)
	* @param	ret 	return type: tti_AVG, tti_AVG1 or returns time of lap ret
	* @param	dind	index of driver
	* @param	driver  name of the driver module 
	*/
	float getSingleTime(int laps, std::string track, int ret, int dind, std::string driver);

	/**
	* Returns a vector of floats corresponding to the distance of each lap. Used with local drivers.
	* <em>Important</em>: the array returned might cointain less items than the laps specified if
	* the driver is taken out of the race early
	* @param	laps	number of laps to race
	* @param	track	name of the track (see list of tracks and drivers)
	* @param	dif		cumulative values if > 0, separate otherwise 
	* @param	dind	index of driver
	* @param	driver  name of the driver module 
	*/
	std::vector<float> getSingleDistanceList(int laps, std::string track, int dif, int dind, std::string driver);

	// client-server mode for a single driver (returns time array only)
	// more info publicly accessible in the driver (after the race)


	/**
	* Returns a vector of floats corresponding to the time of each lap. Used for a single client driver.
	* <em>Important</em>: the array returned might cointain less items than the laps specified if
	* the driver is taken out of the race early
	* @param	laps	number of laps to race
	* @param	track	name of the track (see list of tracks and drivers)
	* @param	driver  pointer to driver 
	*/	
	std::vector<float> getSingleTimeListS(int laps, std::string track, WrapperBaseDriver *driver);

	// must have set driver array to use the next two methods

	// torcs in text-mode, faster

	/**
	* Returns a vector of raceData, each corresponding to the driver of same index. Used with array of client drivers.
	* @param	laps	number of laps to race
	* @param	track	name of the track (see list of tracks and drivers)
	*/	
	std::vector<raceData> race(int laps, std::string track);

	// GUI: configure race manually

	/**
	* Returns a vector of raceData, each corresponding to the driver of same index. Used with array of client drivers.
	* Used for GUI mode, race config must be done manually. Does \e not invoke TORCS.
	*/	
	std::vector<raceData> raceGUI();

private:


	/**
	* Launches TORCS in practice mode for a race with a single driver.
	* @param	laps	number of laps to complete
	* @param	track 	name of track  (see list of tracks and drivers)
	* @param	dind	index number of driver
	* @param	driver 	name of driver module
	* @return 	text output given by TORCS
	*/
	FILE * callTL(int laps, std::string track, int dind, std::string driver);

	// (practice)

	/**
	* Launches TORCS in practice mode for a race with a single client driver.
	* @param	laps	number of laps to complete
	* @param	track 	name of track  (see list of tracks and drivers)
	* @param	driver 	pointer to driver
	* @return 	text output given by TORCS
	*/
	FILE * callT(int laps, std::string track, WrapperBaseDriver *driver);

	// must have used constructor with a driver array (up to 10)
	// above one is (or should be) faster for a single driver
	
	/**
	* Launches TORCS in practice mode for a race with array of client drivers.
	* @param	laps	number of laps to complete
	* @param	track 	name of track  (see list of tracks and drivers)
	*/
	void callTMult(int laps, std::string track);

	/**
	* Launches array of client drivers only, does \e not invoke TORCS.
	*/
	void callTGUI();

	/// array of pointers to client drivers: subclasses of \e modified WrapperBaseDriver 
	std::vector<WrapperBaseDriver*> drivers;
};

#endif /* tti_H_ */
