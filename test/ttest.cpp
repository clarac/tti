#include "TTI.h"
#include "SimpleDriver.h"

int main(){
	
	// in order to test a driver, first create a pointer to an object
	// this can be any subclass of TTIWrapperDriver
	SimpleDriver *d = new SimpleDriver();

	// create a vector of drivers to race
	std::vector<TTIWrapperDriver *> dv;
	
	// just one driver is used in this example, but up to 10 are allowed
	// the order of the race is determined by the order of the array
	dv.push_back(d);
  
  	// path to the configuration file, config.xml is given as an example
  	// this file contains a list of races to be performed by the drivers
	std::string configFile = std::string(getenv("TORCS_BASE"))+ "/src/tti/setups/config.xml";

  	// creating the TTI object runs initial configurations
	TTI t(dv,configFile);
	
	// getting the results of a simulation is as simple as a single method call
	// which returns, for each driver, data corresponding to the time it took to complete,
	// how much damage it suffered and for how long (in game ticks) it was outside the track
	std::vector<raceData> rd = t.race();
	
	// printing the result of the first driver
	cout << rd[0];   

	// to run the second race in the config file, just call race() again

	// rd = t.race();
	// cout << rd[0];

	// a third call would run the first race a second time, since only two are specified
}