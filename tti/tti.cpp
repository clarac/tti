
#include "tti.h"




tti::tti(std::vector<WrapperBaseDriver*> dv,std::string config, std::string folder){
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




void tti::callRace(bool callTorcs){
	int size = drivers.size();


	std::thread torcs;

	if(callTorcs){
		std::string cmd = "torcs -r " + races.getNextXML();
		torcs=std::thread(popen, cmd.c_str(), "r");	
	}		
	std::vector<std::thread*> tds;
	for(int i=0;i<size;i++){
		drivers[i]->clear();
		std::thread *cl = new std::thread(client::connect,3001+i, "localhost","SCR",drivers[i]);
		tds.push_back(cl);
	}

	if(callTorcs){
		torcs.join();
	}
    for(int i=0;i<size;i++){
		tds[i]->join();

	} 

	raceIndex=(raceIndex+1)%race_qty;
	
	if(raceIndex==0)
		round++;

}     


std::vector<raceData> tti::race(){
	raceData rd;
	std::vector<raceData> ret;

	
	if(ready&&!drivers.empty()){
		tti::callRace();
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


int tti::getRound(){
	return round;
}

void tti::restartRound(){
	raceIndex=0;
	races.restartRound();
}
