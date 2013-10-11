/***************************************************************************
 
    file                 : TTITestDriver.h
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


#ifndef TTITestDriver_H_
#define TTITestDriver_H_

#include <iostream>
#include <cmath>
#include "BaseDriver.h"
#include "CarState.h"
#include "CarControl.h"
#include "SimpleParser.h"
#include "WrapperBaseDriver.h"

#define _PI 3.14159265

using namespace std;

class TTITestDriver : public WrapperBaseDriver
{
public:
	
	// Constructor
	TTITestDriver(){stuck=0;clutch=0.0;};

	// TTITestDriver implements a simple and heuristic controller for driving
	virtual CarControl wDrive(CarState cs);

	// Print a shutdown message 
	virtual void onShutdown();
	
	// Print a restart message 
	virtual void onRestart();

	// Initialization of the desired angles for the rangefinders
	virtual void init(float *angles);


	
	/* Gear Changing Constants*/
	
	// RPM values to change gear 
	static  int gearUp[6];
	static  int gearDown[6];
		
	/* Stuck ants*/
	
	// How many time steps the controller wait before recovering from a stuck position
	static  int stuckTime;
	// When car angle w.r.t. track axis is grather tan stuckAngle, the car is probably stuck
	static  float stuckAngle;
	
	/* Steering ants*/
	
	// Angle associated to a full steer command
	static  float steerLock;	
	// Min speed to reduce steering command 
	static  float steerSensitivityOffset;
	// Coefficient to reduce steering command at high speed (to avoid loosing the control)
	static  float wheelSensitivityCoeff;
	
	/* Accel and Brake ants*/
	
	// max speed allowed
	static  float maxSpeed;
	// Min distance from track border to drive at  max speed
	static  float maxSpeedDist;
	// pre-computed sin5
	static  float sin5;
	// pre-computed cos5
	static  float cos5;
	
	/* ABS Filter ants */
	
	// Radius of the 4 wheels of the car
	static  float wheelRadius[4];
	// min slip to prevent ABS
	static  float absSlip;						
	// range to normalize the ABS effect on the brake
	static  float absRange;
	// min speed to activate ABS
	static  float absMinSpeed;

	/* Clutch ants */
	static  float clutchMax;
	static  float clutchDelta;
	static  float clutchRange;
	static  float clutchDeltaTime;
	static  float clutchDeltaRaced;
	static  float clutchDec;
	static  float clutchMaxModifier;
	static  float clutchMaxTime;

private:

	// counter of stuck steps
	int stuck;
	
	// current clutch
	float clutch;

	// Solves the gear changing subproblems
	int getGear(CarState &cs);

	// Solves the steering subproblems
	float getSteer(CarState &cs);
	
	// Solves the gear changing subproblems
	float getAccel(CarState &cs);
	
	// Apply an ABS filter to brake command
	float filterABS(CarState &cs,float brake);

	// Solves the clucthing subproblems
	void clutching(CarState &cs, float &clutch);
};

#endif /*TTITestDriver_H_*/
