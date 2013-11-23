#include "TTI.h"
#include "SimpleDriver.cpp"


/*
	This code is an example of how to use TTI. You can run it to make sure it is working
	as make your own modifications.
*/





int main(){
	// vector of drivers to be tested (up to 10 are allowed). 
	std::vector<WrapperBaseDriver *> pilots(1, new SimpleDriver());
  
	// Training setup.
	TTI tti(pilots);	// a second argument may be passed with the *absolute* path 
						// of an xml configuration file (see test_config.xml)
						// defaults to tti/setups/config.xml
	
	// Execution
	while(!tti.getRound())
		for(auto result : tti.race())
			std::cout << result << std::endl;	

	return 0;    
}
