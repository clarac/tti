#ifndef RACESET_H_
#define RACESET_H_

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <vector>

#include <tgf.h>
#include <wordexp.h>
#include <thread>
#include <future>

/// location of xml files
static std::string xmlFolder = std::string(getenv("TORCS_BASE"))+ "/src/tti/setups/";


/**
* Basic settings for races: track and distance/laps
*/
struct Race
{
	std::string track;
	int laps, distance;
};

/**
* This class is responsible for reading the configuration, generating individual xmls and invoking TORCS.
* @todo add information about the configuration here
*/
class RaceSet {
public:

	RaceSet(){
	}

	/**
	* Standard constructor, reads the settings and configures its attributes accordingly.
	*
	* @param	configXML 	path to the configuration file
	* @param	carQty	the quantity of drivers
	*/
	RaceSet(std::string configXML, int carQty);

	/**
	* Constructor that receives a vector of races instead of a configuration file.
	*
	* @param	races 	contains race configurations
	* @param	folder 	in which to read individual xmls
	* @param	carQty	the quantity of drivers
	*/
	RaceSet(std::vector<Race> races, std::string folder, int carQty);
	
	int getRaceQty();

	std::string getNextXML();
		

private:

	/**
	* Performs initializations common to both constructors
	*/
	void init();

	/**
	* Generates individual xml files 
	*/
	void genXML();


	std::string folder;
	std::vector<Race> races;
	int next, qty, rounds, carQty;
	
	bool ready;

	/**
	* Reads the configuration file and fills the vector races accordingly
	*/
	void readConfig(std::string configXML);
};


#endif /* RACESET_H_ */