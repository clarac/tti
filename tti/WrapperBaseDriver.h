/***************************************************************************
 
    file                 : WrapperBaseDriver.h
    copyright            : (C) 2007 Daniele Loiacono
                           (C) 2013 Clara Caldeira
 
 ***************************************************************************/

/***************************************************************************

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.  

 ***************************************************************************/

#ifndef WRAPPERBASEDRIVER_H_
#define WRAPPERBASEDRIVER_H_

#include "CarState.h"
#include "CarControl.h"
#include "BaseDriver.h"
#include <cmath>
#include <cstdlib>
#include <vector>


/** Wrapper function for drivers */
class WrapperBaseDriver : public BaseDriver
{
public:
	
	/** the drive function with string input and output */
	virtual string drive(string sensors);
	
	/** drive function that exploits the CarState and CarControl wrappers as input and output. */
	virtual CarControl wDrive(CarState cs)=0;

	/** resets race specific attributes, should be called at the end of every race */
	void clear(){
		lastLap=0;
		lapTimes.clear();
		outside=0;
	}

	/** maintains the last received CarState */
	CarState current;

	/** keeps the time to complete the previous lap */
	float lastLap=0;

	/** holds times for previous laps */
	std::vector<float> lapTimes;

	/** keeps the amount of ticks the car spent outside the track */
	int outside=0;
};

#endif /*WRAPPERBASEDRIVER_H_*/