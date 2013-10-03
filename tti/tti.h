#ifndef tti_H_
#define tti_H_

#include "Client.h"

#include "RaceSet.h"



/**
* Returns the sum of a section in a vector of float values.
* @param	values	the array
* @param	from	first position of the interval, defaults to 0
* @param	to 		last position of the interval, defaults to -1 (last position of the vector)
*/  
float getTotal(std::vector<float> values, int from=0, int to=-1){
	float total=0.0;
	if(values.empty())
		return 0;
	if(to==-1)
		to=values.size();
	for(int i=from;i<to;i++){
		total+=values[i];
	}
	return total;
}


/**
* Returns the linear average for a section in a vector of float values.
* @param	values	the array
* @param	from	first position of the interval, defaults to 0
* @param	to 		last position of the interval, defaults to -1 (last position of the vector)
*/
float getAvg(std::vector<float> values, int from=0, int to=-1){
	if(values.empty())
		return 0;
	if(to==-1)
		to=values.size()-1;
	if(to<from||to>=(int)values.size())
		return 0;
	else
		return getTotal(values, from, to)/(1+to-from);
}



/**
* Keeps basic race data, returned by simulations for each driver
*/
struct raceData{
	int damage, ticksOutside;
	float totalTime, avgTime;
	std::vector<float> timeList;
};

/**
* Main class, deals with drivers and data collected from races
*/

class tti {
public:

	/**
	* Constructor: initializes array of drivers and races (set up based on the configuration)
	* 
	* @param	dv 		array of drivers (should contain between 1 and 10 of them)
	* @param	config	path to the race configuration xml, defaults to tti/setups/config.xml
	* @param 	folder 	path in which individual xmls will be stored, defaults to tti/setups
	*/
	tti(std::vector<WrapperBaseDriver*> dv, std::string config = xmlFolder+"config.xml", std::string folder=xmlFolder);


	/**
	* Executes the next race in the list, starting with the first and restarting after reaching the end. 
	*
	* @return 	vector of raceData, in which every element corresponds to the driver of same index and contains its data obtained from the race.
	* 
	*/	
	std::vector<raceData> race();

	/**
	* Returns the number of the current round (starting at 0, incremented every time the last race in the list is completed)
	*
	* @return 0-based round number or quantity of finished rounds
	*/
	int getRound();

	/**
	* Makes the next race to be the first one, does not increment the number of rounds 
	*
	*/
	void restartRound();

private:

	
	int car_qty, race_qty, round, raceIndex;	
	std::string id;
	std::vector<std::string> tracks;
	bool ready = false;
	/**
	* Launches TORCS for the next race along with a thread for every driver.
	*
	* @param	callTorcs defaults to true, should be set to false when working with the graphic interface.
	* @return	returns false in case of error.
	*/
	void callRace(bool callTorcs=true);

	/// array of pointers to client drivers: subclasses of \e modified WrapperBaseDriver 
	std::vector<WrapperBaseDriver*> drivers;

	/// object keeps track of the configuration and xml files for the races
	RaceSet races;
};

#endif /* tti_H_ */


