/***************************************************************************
 
    file                 : SimpleParser.h
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

#ifndef SIMPLEPARSER_H_
#define SIMPLEPARSER_H_

#include <iostream>
#include <sstream>
#include <cstring>

using namespace std;

/** Provides methods for parsing strings */
class SimpleParser
{
public:
		/** Extracts value of tag from sensors, returns false if tag was not found */
        static bool  parse(string sensors, string tag, float &value);

        /** Extracts value of tag from sensors, returns false if tag was not found */
        static bool  parse(string sensors, string tag, int &value);

        /** Extracts value of tag from sensors, returns false if tag was not found */
        static bool  parse(string sensors, string tag, float *value, int size);

        /** Returns string for tag and value */
        static string stringify(string tag, float value);

        /** Returns string for tag and value */
        static string  stringify(string tag, int value);

        /** Returns string for tag and value */
        static string  stringify(string tag, float *value, int size);

};

#endif /*SIMPLEPARSER_H_*/
