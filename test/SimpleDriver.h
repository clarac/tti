/***************************************************************************
 
    file                 : SimpleDriver.h
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
#ifndef SIMPLEDRIVER_H_
#define SIMPLEDRIVER_H_

#include <iostream>
#include <cmath>
#include "BaseDriver.h"
#include "CarState.h"
#include "CarControl.h"
#include "SimpleParser.h"
#include "WrapperBaseDriver.h"




#define __PI 3.14159265

#define insideTolerance conf[0]
#define straightTolerance conf[1]
#define accelSensitivity conf[2]
#define maxSpeed conf[3]
#define steerSensitivityOffset conf[4]
#define stuckTime conf[5]
#define stuckAngle conf[6]
#define stuckSpeed conf[7]
#define maxSpeedDist conf[8]
#define wheelSensitivityCoeff conf[9]
#define absSlip conf[9]
#define absRange conf[10]
#define absMinSpeed conf[11]
#define clutchMax conf[12]
#define clutchDelta conf[13]
#define clutchRange conf[14]
#define clutchDeltaTime conf[15]
#define clutchDeltaRaced conf[16]
#define clutchDec conf[17]
#define clutchMaxModifier conf[18]
#define clutchMaxTime conf[19]
#define steerTolerance conf[20]
#define outsideAccel conf[21]
#define stuckAccel conf[22]
#define steerLock conf[23]
#define minSpeed conf[24]
#define minDistance conf[25]
#define straightTargetOffset conf[26]
#define curveTargetOffset conf[27]
#define lookaheadDistance conf[28]
#define straightProp conf[29]

#define curTrackPos (current.getTrackPos()/insideTolerance)



#define insideTrack (!outside&&fabs(curTrackPos)<=insideTolerance)
#define straightSegment (maxAngle==9||current.getTrack(9)/current.getTrack(maxAngle)>straightProp||\
 				( fabs(maxAngle*__PI/18 - __PI/2 - current.getAngle())<__PI/8))

//#define leftCurve ((current.getTrack(11+angleOffset)+current.getTrack(10+angleOffset))<(current.getTrack(8+angleOffset)+current.getTrack(7+angleOffset)))
#define leftCurve (maxAngle<9);
#define rightCurve (maxAngle>9)


#define rightCurveLookahead (current.getTrack(10)>current.getTrack(8) && current.getTrack(8)/current.getTrack(10)<straightProp)
#define leftCurveLookahead (current.getTrack(8)>current.getTrack(10) && current.getTrack(10)/current.getTrack(8)<straightProp)

#define straightLookahead ((current.getTrack(10)>=lookaheadDistance || \
	current.getTrack(9) >= lookaheadDistance || current.getTrack(8) >= lookaheadDistance) ||(\
	!leftCurveLookahead && !rightCurveLookahead))


#define isFrontLeftBlocked current.getOpponents(20)<minDistance || current.getOpponents(19)<minDistance ||\
        current.getOpponents(18)<minDistance
#define isFrontRightBlocked current.getOpponents(17)<minDistance || current.getOpponents(16)<minDistance ||\
        current.getOpponents(15)<minDistance
#define isFrontBlocked isFrontLeftBlocked || isFrontRightBlocked

#define segString(a) (a==straight? " straight ": (a==left? " left " : " right "))

using namespace std;

class SimpleDriver : public WrapperBaseDriver
{
public:
	
	// Constructor
	SimpleDriver(){
		stuck=0;
		clutch=0.0;
		insideTolerance=0.75;
		straightTolerance=60; 
		accelSensitivity=3;
		maxSpeed=220;
		minSpeed=50;
		/* Stuck constants*/
		stuckTime = 25;
		stuckAngle = .523598775; //PI/6
		stuckSpeed = 5; // m/s

		/* Accel and Brake Constants*/

		maxSpeedDist=60;

		/* Steering constants*/
		steerLock=0.366519 ;
		steerSensitivityOffset=80.0;
		wheelSensitivityCoeff=0.85;
		steerTolerance =1.0;
		/* ABS Filter Constants */
		
		absSlip=2.0;
		absRange=3.0;
		absMinSpeed=3.0;

		/* Clutch constants */
		clutchMax=0.8;
		clutchDelta=0.15;
		clutchRange=0.82;
		clutchDeltaTime=0.02;
		clutchDeltaRaced=10;
		clutchDec=0.01;
		clutchMaxModifier=1.8;
		clutchMaxTime=1.9;

		outsideAccel=0.3;
		stuckAccel=0.8;

		minDistance = 5;

		straightTargetOffset = 0.9;
		curveTargetOffset = 80;

		lastSeg = straight;
		lookaheadDistance = 180;
		straightProp=0.8;
	};

	// SimpleDriver implements a simple and heuristic controller for driving
	virtual CarControl wDrive(CarState cs);

	// Print a shutdown message 
	virtual void onShutdown();
	
	// Print a restart message 
	virtual void onRestart();

	// Initialization of the desired angles for the rangefinders
	virtual void init(float *angles);

	float filterTCL(float accel_brake);

	float conf[30];

private:
	
	/* Gear Changing Constants*/
	
	// RPM values to change gear 
	static const int gearUp[6];
	static const int gearDown[6];
		
	
	// Radius of the 4 wheels of the car
	static const float wheelRadius[4];
	static const float sin5, cos5;

	//CarState current;
	enum segment {straight, left, right} curSeg, lastSeg, lookahead;

	// counter of stuck steps
	int stuck;
	
	// current clutch
	float clutch;

	float maxAngle;

	bool outside;
	// Solves the gear changing subproblems
	int getGear();

	// Solves the steering subproblems
	float getSteer();
	
	// Solves the gear changing subproblems
	float getAccel();

	float filterOpponentsAccel(float accel);

	float filterOpponentsSteer(float steer);
	
	// Apply an ABS filter to brake command
	float filterABS(float brake);

	// Solves the clucthing subproblems
	void clutching(float &clutch);

	bool isStuck();

	void updateSeg(segment s);

	float getMax(float a,float b);
	float getMax3(float a, float b, float c);
	float getMin(float a, float b);
};

#endif /*SIMPLEDRIVER_H_*/
