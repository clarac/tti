/***************************************************************************
 
    file                 : TTIWrapperDriver.h
    copyright            : (C) 2007 Daniele Loiacono
                           (C) 2013 Clara Caldeira
 
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

#ifndef TTIWrapperDRIVER_H_
#define TTIWrapperDRIVER_H_

#include "scr/CarState.h"
#include "scr/CarControl.h"
#include "scr/WrapperBaseDriver.h"
#include <cmath>
#include <cstdlib>
#include <vector>


/** Wrapper function for drivers */
class TTIWrapperDriver : public WrapperBaseDriver
{
public:
	
	TTIWrapperDriver() {clear();}
	/** the drive function with string input and output */
	virtual string drive(string sensors);
	
	/** drive function that exploits the CarState and CarControl wrappers as input and output. */
	virtual CarControl wDrive(CarState cs)=0;

	/** resets race specific attributes, should be called at the end of every race */
	void clear(){
		lastLap=0;
		lapTimes.clear();
		outside=0;
		curTick=0;
		lastCheck=0;
		lastDist=0;
		zombie=false;
		
	}

	bool isDead(){
		if(md<=0||mt<=0)
			return false;

		if(curTick-lastCheck<mt)
			return false;

		
		if(current.getDistRaced()-lastDist<md)
			return true;

		lastCheck=curTick;
		lastDist=current.getDistRaced();

		return false;

	}

	/** maintains the last received CarState */
	CarState current;

	/** keeps the time to complete the previous lap */
	float lastLap;

	/** holds times for previous laps */
	std::vector<float> lapTimes;

	/** keeps the amount of ticks the car spent outside the track */
	int outside;

	/** true if driver is waiting for the race to finish */
	bool zombie;

	/** counts ticks */
	long long curTick;

	/** early termination parameters (min distance / max ticks) */
	double md,mt;

	/** early termination vars, checks periodically */
	double lastCheck, lastDist;

};

#endif /*TTIWrapperDRIVER_H_*/
