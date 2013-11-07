/***************************************************************************
 
    file                 : TTI.cpp
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

#include "TTI.h"




TTI::TTI(std::vector<WrapperBaseDriver*> dv,std::string config, std::string folder){
	drivers=dv;
	car_qty=drivers.size();
	round=0;
	raceIndex=0;
	try {
		races=RaceSet(config,car_qty,folder);
		ready=true;
		race_qty=races.getRaceQty();
	} catch(int e){
		ready=false;
	}
}




void TTI::callRace(bool callTorcs){
	FILE *p=NULL;
	int size = drivers.size();
	

	std::thread torcs;
	
	if(callTorcs){
		std::string cmd = "torcs -r " + races.getNextXML();
		p = popen(cmd.c_str(),"r");	
	}		
	std::vector<std::thread*> tds;
	for(int i=0;i<size;i++){
		drivers[i]->clear();
		std::thread *cl = new std::thread(client::connect,3001+i, "localhost","SCR",drivers[i]);
		tds.push_back(cl);
	}

	char output[500];
    if(p != NULL) {
       	while(fgets(output, sizeof(output), p) != NULL) {
       		//cout<<output<<endl;
       	}
       pclose(p);
     }


	for(int i=0;i<size;i++){
		tds[i]->join();

	} 


	
	
    

	raceIndex=(raceIndex+1)%race_qty;
	
	if(raceIndex==0)
		round++;

}     


std::vector<raceData> TTI::race(){
	raceData rd;
	std::vector<raceData> ret;

	
	
	if(ready&&!drivers.empty()){
		
		TTI::callRace();

		int size=drivers.size();
		for(int i=0;i<size;i++){
			rd.damage=drivers[i]->current.getDamage();
			rd.ticksOutside=drivers[i]->outside;
			rd.timeList=drivers[i]->lapTimes;
			rd.totalTime=getTotal(rd.timeList);
			rd.avgTime=rd.totalTime/rd.timeList.size();
			ret.push_back(rd);
		}	
	}
	
	return ret;
}


int TTI::getRound(){
	return round;
}

void TTI::restartRound(){
	raceIndex=0;
	races.restartRound();
}
