/***************************************************************************
 
    file                 : TTI.h
    copyright            : (C) 2013 Clara Caldeira
 
 ***************************************************************************/

/***************************************************************************

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.  

 ***************************************************************************/


#ifndef tti_H_
#define tti_H_

#include "Client.h"

#include "RaceSet.h"



/**
\mainpage The TORCS Training Interface Documentation


Compilation will generate a shared object file and link it in ${TORCS_BASE}/export/lib.
The folder _test_ contains an example of how TTI can be used, more details can be found below.

See the <a href="https://github.com/clarac/tti">READ ME</a> for more info and installation instructions.

Example code:

\code{.cpp}

	#include "tti.h"
	#include "SimpleDriver.cpp"

	int main(){
		
		// creating a driver object
		TTITestDriver *d = new TTITestDriver();

		// vector of drivers to be tested
		std::vector<WrapperBaseDriver *> dv;
		
		// just one driver is used in this example, but up to 10 are allowed
		dv.push_back(d);
	  
	  	// creating the TTI object runs initial configurations
		TTI t(dv, "/path/to/config.xml");
		
		// getting the results of a simulation is as simple as a single method call
		// returns, for each driver, data corresponding to the time it took to complete,
		// how much damage it suffered and for how long (in game ticks) it was outiside the track
		std::vector<raceData> rd = t.race();
		   
	}

\endcode

	compile your code with flags
		-L${TORCS_BASE}/export/lib
		-I${TORCS_BASE}/export/include
		-ltti -ltgf -ltxml

	and include torcs/export/lib in your library path

	$ LD_LIBRARY_PATH=${TORCS_BASE}/export/lib:$LD_LIBRARY_PATH



Given a collection of drivers, a configuration file and a folder in which race specific xml
are to be stored, an object of class \ref tti will provide a simulation with a single method call
whose return is a collection of raceData, each corresponding to one of the drivers in order. This data includes
the damage suffered by the car, the number of game ticks it spent outside the lane throughout the 
race, total time it took, average time per lap and all the times for every lap completed. 

The example in /test only prints the data, but results are to be used in comparisons however the user
sees fit. Because references to driver objects are used, it is possible to make alterations
between races and evaluate how performances are changed as a result.

Drivers must inherit from the class WrapperBaseDriver, which is a __modified__ version of the class provided
with the available TORCS C++ client. A project using TTI should import that class from ${TORCS_BASE}/export/include
and _not_ have it as a file among other source code.

The library works by reading from a configuration file which races are to be simulated, then generating
individual xml files for each of them and, whenever prompted, running the next one on the list. The
races are run in the same order they a read and in a circular manner, i.e. the first one is considered to
be the next after the last. Running involves invoking TORCS and each of the drivers and waiting for every
one of them to finish. For average tracks, this takes about a couple of seconds per lap, but if a driver 
gets stuck along the way, its execution only stops after timeout is reached. Its results are still returned,
but only completed laps are included in time data.

Configurations are set in a xml file and include, for each race, the name of the track and its length in laps 
or distance (km). Additionally, an id is taken as the name of a folder in which generated xml files are stored. 
If not provided, the configuration defaults to tti/setups/config.xml, which is included and provides an example.

Although a few limitations on the code for a driver are introduced, e.g. its performance should not depend on 
the client file, this library provides easy testing and comparing for a group of up to 10 drivers at once
in multiple, easily configured races. After testing is no longer needed, just making a substitution of 
the SimpleDriver for the new driver class along with any created additionally to the TORCS client code 
provided should make it run normally as well as it did during development.


*/



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

class TTI {
public:

	/**
	* Constructor: initializes array of drivers and races (set up based on the configuration)
	* 
	* @param	dv 		array of drivers (should contain between 1 and 10 of them)
	* @param	config	path to the race configuration xml, defaults to tti/setups/config.xml
	* @param 	folder 	path in which individual xmls will be stored, defaults to tti/setups
	*/
	TTI(std::vector<WrapperBaseDriver*> dv, std::string config = xmlFolder+"config.xml", std::string folder=xmlFolder);


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
	bool ready;
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


