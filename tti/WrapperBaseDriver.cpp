/***************************************************************************
 
    file                 : WrapperBaseDriver.cpp
    copyright            : (C) 2007 Daniele Loiacono
 
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "WrapperBaseDriver.h"

string 
WrapperBaseDriver::drive(string sensors)
{
	CarState cs(sensors);
	current=cs;
	float lap = current.getLastLapTime();
	if(lap!=lastLap){
		lapTimes.push_back(lap);
		lastLap=lap;
	}
	if(current.getTrackPos()>=1||current.getTrackPos()<=-1){
		outside++;
	}
	CarControl cc = wDrive(cs);
	return cc.toString();	
}


