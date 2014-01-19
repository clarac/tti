#include "TTI.h"
#include "SimpleDriver.h"

using namespace std;

int main(int argc, char * argv[]){
	
	// in order to test a driver, first create a pointer to an object
	// this can be any subclass of TTIWrapperDriver
	SimpleDriver *d = new SimpleDriver();

	// create a vector of drivers to race
	vector<TTIWrapperDriver *> dv;
	
	// just one driver is used in this example, but up to 10 are allowed
	// the order of the race is determined by the order of the array
	dv.push_back(d);
  
  	// the test expects a config file as a parameter
	if(argc<2){
		cout << "Error: please specify a configuration file" << endl;
		exit(1);
	}

  	// path to the configuration file, config.xml is given as an example
  	// this file contains a list of races to be performed by the drivers
	string configFile = string(argv[1]);

  	// creating the TTI object runs initial configurations
	TTI t(dv,configFile);
	
	// getting the results of a simulation is as simple as a single method call
	// which returns, for each driver, data corresponding to the time it took to complete,
	// how much damage it suffered and for how long (in game ticks) it was outside the track
	vector<raceData> rd = t.race();
	
	// printing the result of the first driver
	cout << rd[0];   

	// to run the second race in the config file, just call race() again

	rd = t.race();
	cout << rd[0];

	// a third call would run the first race a second time, since only two are specified
}