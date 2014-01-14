/***************************************************************************
 
    file                 : CarState.h
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

#ifndef CARSTATE_H_
#define CARSTATE_H_

#define FOCUS_SENSORS_NUM 5
#define TRACK_SENSORS_NUM 19
#define OPPONENTS_SENSORS_NUM 36

#include <iostream>
#include <sstream>
#include <cstring>
#include <cassert>
#include "SimpleParser.h"

using namespace std;


 /** Wrapper class for input of data from the car */
class CarState
{

private:
        /** angle sensor */
        float angle;

        /** curLapTime sensor */
        float curLapTime;

        /** damage sensor */
        float damage;

        /** distFromStart sensor */
        float distFromStart;

        /** distRaced sensor */
        float distRaced;

        /** focus sensors */
        float focus[FOCUS_SENSORS_NUM];

        /** fuel sensor */
        float fuel;

        /** gear sensor */
        int   gear;

        /** lastLapTime sensor */
        float lastLapTime;

        /** opponents sensors */
        float opponents[OPPONENTS_SENSORS_NUM];

        /** racePos sensor */
        int   racePos;

        /** rpm sensor */
        int   rpm;

        /** speedX sensor */
        float speedX;

        /** speedY sensor */
        float speedY;

        /** speedZ sensor */
        float speedZ;

        /** track sensors */
        float track[TRACK_SENSORS_NUM];

        /** trackPos sensor */
        float trackPos;

        /** wheelSpinVel sensors */
        float wheelSpinVel[4];

        /** z sensor */
        float z;
        

public:
	
        /** Empty constructor */
		CarState(){};

        /** Constructor receiving a string corresponding to the sensors */
        CarState(string sensors);

        /** Returns the string corresponding to the object's attributes */
        string toString();

        /* Getter and setter methods */
        
        /** Returns the value of angle */
        float getAngle();
        
        /** Changes the value of angle */
        void setAngle(float angle);
        
        /** Returns the value of curLapTime */
        float getCurLapTime();
        
        /** Changes the value of curLapTime */
        void setCurLapTime(float curLapTime);
        
        /** Returns the value of damage */
        float getDamage();
        
        /** Changes the value of damage */
        void setDamage(float damage);
        
        /** Returns the value of distFromStart */
        float getDistFromStart();
        
        /** Changes the value of distFromStart */
        void setDistFromStart(float distFromStart);
        
        /** Returns the value of distRaced */
        float getDistRaced();
        
        /** Changes the value of distRaced */
        void setDistRaced(float distRaced);

        /** Returns the value of focus[i] */
        float getFocus(int i);

        /** Changes the value of focus[i] */
        void setFocus(int i, float value);

        /** Returns the value of fuel */
        float getFuel();
        
        /** Changes the value of fuel */
        void setFuel(float fuel);
        
        /** Returns the value of gear */
        int getGear();
        
        /** Changes the value of gear */
        void setGear(int gear);
        
        /** Returns the value of lastLapTime */
        float getLastLapTime();
        
        /** Changes the value of lastLapTime */
        void setLastLapTime(float lastLapTime);
        
        /** Returns the value of opponents[i] */
        float getOpponents(int i);
        
        /** Changes the value of opponents[i] */
        void setOpponents(int i, float value);
        
        /** Returns the value of racePos */
        int getRacePos();
        
        /** Changes the value of racePos */
        void setRacePos(int racePos);
        
        /** Returns the value of rpm */
        int getRpm();
        
        /** Changes the value of rpm */
        void setRpm(int rpm);
        
        /** Returns the value of speedX */
        float getSpeedX();
        
        /** Changes the value of speedX */
        void setSpeedX(float speedX);
        
        /** Returns the value of speedY */
        float getSpeedY();
        
        /** Changes the value of speedY */
        void setSpeedY(float speedY);
        
        /** Returns the value of speedZ */
        float getSpeedZ();

        /** Changes the value of speedZ */
        void setSpeedZ(float speedZ);

        /** Returns the value of track[i] */
        float getTrack(int i);
        
        /** Changes the value of track[i] */
        void setTrack(int i, float value);
        
        /** Returns the value of trackPos */
        float getTrackPos();
        
        /** Changes the value of trackPos */
        void setTrackPos(float trackPos);
        
        /** Returns the value of wheelSpinVel[i] */
        float getWheelSpinVel(int i);
        
        /** Changes the value of wheelSpinVel[i] */
        void setWheelSpinVel(int i, float value);

        /** Returns the value of z */
        float getZ();

        /** Changes the value of z */
        void setZ(float z);


};

#endif /*CARSTATE_H_*/
