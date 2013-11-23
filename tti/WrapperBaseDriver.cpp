/***************************************************************************
 
    file                 : WrapperBaseDriver.cpp
    copyright            : (C) 2007 Daniele Loiacono
 
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "WrapperBaseDriver.h"

string 
WrapperBaseDriver::drive(string sensors)
{
	CarState cs(sensors);
	current=cs;
	curTick++;
	if(zombie){
		CarControl cc;
		cc.setMeta(2);
		return cc.toString();
	} 
	if(isDead()){
		clear();
		zombie=true;
		std::cout<<"terminated"<<std::endl;
		CarControl cc;
		cc.setMeta(2);
		return cc.toString();
	}
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


