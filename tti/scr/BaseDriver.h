/***************************************************************************
 
    file                 : BaseDriver.h
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

#ifndef BASEDRIVER_H_
#define BASEDRIVER_H_

#include<iostream>

using namespace std;

class BaseDriver
{
public:
	
	/** Possible race stages */
	typedef enum{WARMUP,QUALIFYING,RACE,UNKNOWN} tstage;

	/** Holds current stage */
	tstage stage;

	/** Name of the track */
	char trackName[100];

	/** Default Constructor */
	BaseDriver(){};
	
	/** Default Destructor; */
	virtual ~BaseDriver(){};
	
	/** Initialization of the desired angles for the rangefinders */
	virtual void init(float *angles){
		for (int i = 0; i < 19; ++i)
			angles[i]=-90+i*10;
	};

	/** The main function: 
	*     - the input variable sensors represents the current world sate
	*     - it returns a string representing the controlling action to perform    */
	virtual string drive(string sensors)=0;

	/** Callback function called at shutdown */
	virtual void onShutdown(){};
	
	/** Callback function called at server restart */
	virtual void onRestart(){};

};
#endif /*BASEDRIVER_H_*/
