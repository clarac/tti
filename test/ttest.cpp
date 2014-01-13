#include "TTI.h"
	#include "SimpleDriver.h"

	int main(){
		
		// creating a driver object
		SimpleDriver *d = new SimpleDriver();

		// vector of drivers to be tested
		std::vector<TTIWrapperDriver *> dv;
		
		// just one driver is used in this example, but up to 10 are allowed
		dv.push_back(d);
	  
	  	// creating the TTI object runs initial configurations
		TTI t(dv);
		
		// getting the results of a simulation is as simple as a single method call
		// returns, for each driver, data corresponding to the time it took to complete,
		// how much damage it suffered and for how long (in game ticks) it was outiside the track
		std::vector<raceData> rd = t.race();
		
		cout << rd[0];   
	}