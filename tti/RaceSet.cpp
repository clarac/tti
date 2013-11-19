/***************************************************************************
 
    file                 : RaceSet.cpp
    copyright            : (C) 2013 Clara Caldeira
 
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

#include "RaceSet.h"
#include <sys/stat.h>

RaceSet::RaceSet(std::vector<Race> races, std::string folder, int carQty){
	this->races=races;	
	this->folder=folder;
	this->carQty=carQty;

	if(carQty<1||carQty>10){		
		std::cout<<"RaceSet construction failed: car quantity outside limits [1,10]"<<std::endl;
		throw 1;
	} else
		init();	
}

RaceSet::RaceSet(std::string configXML, int carQty, std::string folder) {

	this->carQty=carQty;
	xmlOut=folder;
	if(carQty<1||carQty>10){
		std::cout<<"RaceSet construction failed: car quantity outside limits [1,10]"<<std::endl;
		throw 1;
	}
	else 
		readConfig(configXML);
	init();		
}



void RaceSet::init(){
	qty=races.size();
	next=0;
	rounds=0;
	if(qty==0){
		std::cout<<"RaceSet construction failed: empty set of races"<<std::endl;
		throw 1;
	}	
	genXML();
	
}

void RaceSet::genXML(){
	std::string path = xmlFolder+"quickrace"+std::to_string(carQty)+".xml";
	void *parmHandle = GfParmReadFile(path.c_str(), GFPARM_RMODE_CREAT);
	int ret =mkdir(folder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	if(ret<0&&errno!=EEXIST){
		std::cout<<"Race failed: Unable to access xml directory"<<std::endl;
		throw 1;
	}

	for(int i=0;i<qty;i++){		
		GfParmSetNum(parmHandle, "Quick Race", "laps", NULL, races[i].laps);
		GfParmSetNum(parmHandle, "Quick Race", "distance", NULL, races[i].distance);
		GfParmSetStr(parmHandle, "Tracks/1", "name", races[i].track.c_str());
		GfParmSetStr(parmHandle, "Tracks/1", "category", races[i].cat.c_str());
		GfParmWriteFile((folder+"r"+std::to_string(i)+".xml").c_str(), parmHandle, NULL);
	}
}

void RaceSet::readConfig(std::string configXML){
	
	
	void *parmHandle = GfParmReadFile(configXML.c_str(), GFPARM_RMODE_STD);

	std::string id=GfParmGetStr(parmHandle, "Header", "id", "default");
	folder=xmlOut+id+"/";

	md=GfParmGetNum(parmHandle, "Header", "mindist", NULL, 0);
	mt=GfParmGetNum(parmHandle, "Header", "maxticks", NULL, 0);


	Race r;
	std::string path;

	std::string track = GfParmGetStr(parmHandle, "Races/1", "track", "not found");

	


	path="Races/"+std::to_string(1);
	

	int i;
	for(i=1;track!="not found";i++){
		r.track=track;
		
		r.distance=GfParmGetNum(parmHandle, path.c_str(), "distance", NULL, 0);
		r.laps=GfParmGetNum(parmHandle, path.c_str(), "laps", NULL, 0);
		r.cat= GfParmGetStr(parmHandle, path.c_str(), "category", "road");
		if(r.distance<=0&&r.laps<=0){
			std::cout<<"RaceSet construction failed: invalid race length for race #"<<i<<std::endl;
			throw 1;
		}

		races.push_back(r);

		path="Races/"+std::to_string(i+1);

		track = GfParmGetStr(parmHandle, path.c_str(), "track", "not found");
	}


	if(i==1){
		std::cout<<"RaceSet construction failed: empty set of races"<<std::endl;
		throw 1;
	}
	
	
}


int RaceSet::getRaceQty(){
	return qty;

}

std::string RaceSet::getNextXML(){
	std::string num=std::to_string(next);
	next=(next+1)%qty;
	return folder+"r"+num+".xml";	
}

void RaceSet::restartRound(){
	next=0;
}