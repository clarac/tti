/***************************************************************************
 
    file                 : CarControl.h
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

#ifndef CARCONTROL_H_
#define CARCONTROL_H_

#include <iostream>
#include <sstream>
#include <cstring>
#include <cassert>
#include "SimpleParser.h"

using namespace std;


/** Wrapper class for output of commands to the car */
class CarControl
{
private:

        /** Accelerate command [0,1] */
        float accel;

        /** Brake command [0,1] */
        float brake;

        /** Gear command */
        int gear;
        
        /** Steering command [-1,1] */
        float steer;
        
        /** Clutch command [0,1] */
        float clutch;

        /** meta-command */
        int meta;

  		/** focus command [-90,90], i.e. angle of track sensor focus desired by client */
		int focus;

public:

        /** Empty constructor */
        CarControl(){};

        /** Constructor receiving a string corresponding to the controls */
        CarControl(string sensors);

        /** Constructor receiving separate values, meta defaults to zero */
        CarControl(float accel, float brake, int gear, float steer, float clutch, int focus, int meta=0);

        /** Constructor receiving separate values, not including meta, focus defaults to zero */
        CarControl(float accel, float brake, int gear, float steer, float clutch, int focus=0);

        /** Returns the string corresponding to the object's attributes */
        string toString();

        /** Derives values from a string */
        void fromString(string sensors);

        /* Getter and setter methods */

        /** Returns the value of accel */
        float getAccel() const;
        
        /** Changes the value of accel */
        void setAccel (float accel);
        
        /** Returns the value of brake */
        float getBrake() const;
        
        /** Changes the value of brake */
        void setBrake (float brake);
        
        /** Returns the value of gear */
        int getGear() const;
        
        /** Changes the value of gear */
        void setGear(int gear);
        
        /** Returns the value of steer */
        float getSteer() const;
        
        /** Changes the value of steer */
        void setSteer(float steer);  
        
        /** Returns the value of meta */
        int getMeta() const;
        
        /** Changes the value of meta */
        void setMeta(int gear);

        /** Returns the value of clutch */
        float getClutch() const;

        /** Changes the value of clutch */
        void setClutch(float clutch);

        /** Returns the value of focus */
		int getFocus();

        /** Changes the value of focus */
		void setFocus(int focus);

        /** meta-command value for race restart */
        static int META_RESTART;

};

#endif /*CARCONTROL_H_*/
