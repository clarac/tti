
#include "tti.h"


tti::tti() {
}

tti::tti(std::vector<WrapperBaseDriver*> dv){
	drivers=dv;
}


FILE * tti::callTL(int laps, std::string track, int dind, std::string driver){
	std::string path = std::string(getenv("HOME"))+"/.tti/setups/practice.xml";
	std::string cmd = "torcs -r " + path;
	void *parmHandle = GfParmReadFile(path.c_str(), GFPARM_RMODE_STD);
	GfParmSetNum(parmHandle, "Practice", "laps", NULL, laps);
	GfParmSetStr(parmHandle, "Tracks/1", "name", track.c_str());
	GfParmSetNum(parmHandle, "Drivers/1", "idx", NULL, dind);
	GfParmSetStr(parmHandle, "Drivers/1", "module", driver.c_str());
	GfParmWriteFile (NULL, parmHandle, NULL);
	return popen(cmd.c_str(), "r"); 
} 

FILE * tti::callT(int laps, std::string track, WrapperBaseDriver *driver){
	std::string path = std::string(getenv("HOME"))+"/.tti/setups/practice_server.xml";
	std::string cmd = "torcs -r " + path;
	void *parmHandle = GfParmReadFile(path.c_str(), GFPARM_RMODE_STD);
	GfParmSetNum(parmHandle, "Practice", "laps", NULL, laps);
	GfParmSetStr(parmHandle, "Tracks/1", "name", track.c_str());
	GfParmWriteFile (NULL, parmHandle, NULL);

	std::thread cl(client::connect,3001, "localhost","SCR",driver);
	
	auto future = std::async(popen, cmd.c_str(), "r");
    FILE* ret = future.get();
    cl.join();

	return ret; 
}     

void tti::callTMult(int laps, std::string track){
	int size = drivers.size();

	if(size>10)
		size=10;
	if(size<1)
		return;
	std::string sz=std::to_string(size);
	std::string path = std::string(getenv("HOME"))+"/.tti/setups/quickrace"+sz+".xml";
	//std::cout << path<< std::endl;
	std::string cmd = "torcs -r " + path;
	void *parmHandle = GfParmReadFile(path.c_str(), GFPARM_RMODE_STD);
	GfParmSetNum(parmHandle, "Quick Race", "laps", NULL, laps);
	GfParmSetStr(parmHandle, "Tracks/1", "name", track.c_str());
	GfParmWriteFile (NULL, parmHandle, NULL);

	std::vector<std::thread*> tds;
	for(int i=0;i<size;i++){
		drivers[i]->clear();
		std::thread *cl = new std::thread(client::connect,3001+i, "localhost","SCR",drivers[i]);
		tds.push_back(cl);
	}

	std::thread tc(popen, cmd.c_str(), "r");
	tc.join();
    for(int i=size-1;i>=0;i--){
		tds[i]->join();
		tds.pop_back();
	} 
}     

void tti::callTGUI(){
	int size = drivers.size();

	if(size>10)
		size=10;
	if(size<1)
		return;

	std::string cmd = "torcs";

	std::vector<std::thread*> tds;
	for(int i=0;i<size;i++){
		drivers[i]->clear();
		std::thread *cl = new std::thread(client::connect,3001+i, "localhost","SCR",drivers[i]);
		tds.push_back(cl);
	}

	//std::thread tc(popen, cmd.c_str(), "r");
    
    
    for(int i=size-1;i>=0;i--){
		tds[i]->join();
		tds.pop_back();
	}
	//tc.join();

} 


std::vector<float> tti::getSingleTimeList(int laps, std::string track, int dind, std::string driver){
	std::vector<float> lapTimes, empty;
	float last=0.0f, aux;
	if(laps>0){
		char output[100], subs[12];
		float val;
		int i;
		FILE *p = callTL(laps,track, dind, driver);
		if(p != NULL) {
			while(fgets(output, sizeof(output), p) != NULL) {
				if(strncmp(output,"Sim Time:",9)==0){
					for(i=9;i<21 && output[i]!='\0' && output[i]!='\n' && output[i]!='[';i++){
						subs[i-9]=output[i];
					}
					subs[i-9]='\0';
					aux=last;
					last=atof (subs);
					val= last-aux;

					//printf("%f\n",val);
					//printf("%f (%f)\n",last,val);
					lapTimes.push_back(val);
				}
			}
			pclose(p);
			return lapTimes;
		} else
			return empty;
	} else
		return empty;
}


float tti::getSingleTime(int laps, std::string track, int ret, int dind, std::string driver){
	std::vector<float> tempos = getSingleTimeList(laps,track,dind,driver);
	if(tempos.empty()){
		return -1.0f;
	}
	else if(ret==tti_AVG){
		return getAvg(tempos);
	} else if(ret==tti_AVG1&&tempos.size()>1){
		return getAvg(tempos,1);
	} else if(ret>0 && (unsigned int)ret <= tempos.size()){
		return tempos.at(ret-1);
	} else
		return -1.0f;
}

std::vector<float> tti::getSingleDistanceList(int laps, std::string track, int dif, int dind, std::string driver){
	std::vector<float> lapTimes, empty;
	float last=0.0f, aux;
	if(laps>0){
		char output[100], subs[100];
		float val;
		int i;
		FILE *p = callTL(laps,track, dind, driver);
		if(p != NULL) {
			while(fgets(output, sizeof(output), p) != NULL) {
				for(i=59;i<71 && output[i]!='\0' && output[i]!='\n' && output[i]!='[';i++){
					subs[i-59]=output[i];
					//printf("(%c)",output[i]);
				}
				subs[i-59]='\0';
				val=atof(subs);
				if(dif>0){
					aux=last;
					last=val;
					val=last-aux;
				}
				lapTimes.push_back(val);
			}
			pclose(p);
			return lapTimes;
		} else
			return empty;
	} else
		return empty;
}


std::vector<float> tti::getSingleTimeListS(int laps, std::string track, WrapperBaseDriver *driver){
	std::vector<float> lapTimes, empty;
	float last=0.0f, aux;
	if(laps>0){
		char output[100], subs[12];
		float val;
		int i;
		FILE *p = callT(laps,track,driver);
		if(p != NULL) {
			while(fgets(output, sizeof(output), p) != NULL) {
				if(strncmp(output,"Sim Time:",9)==0){
					for(i=9;i<21 && output[i]!='\0' && output[i]!='\n' && output[i]!='[';i++){
						subs[i-9]=output[i];
					}
					subs[i-9]='\0';
					aux=last;
					last=atof (subs);
					val= last-aux;

					//printf("%f\n",val);
					//printf("%f (%f)\n",last,val);
					lapTimes.push_back(val);
				}
			}
			pclose(p);
			return lapTimes;
		} else
			return empty;
	} else
		return empty;
}



std::vector<raceData> tti::race(int laps, std::string track){
	raceData rd;
	std::vector<raceData> ret;

	if(!drivers.empty()){
		tti::callTMult(laps, track);
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

std::vector<raceData> tti::raceGUI(){
	raceData rd;
	std::vector<raceData> ret;

	if(!drivers.empty()){
		tti::callTGUI();
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

float tti::getAvg(std::vector<float> values, int from, int to){
	if(to==-1)
		to=values.size();
	if(to<=from)
		return 0;
	else
		return getTotal(values, from, to)/(to-from);
}

float tti::getTotal(std::vector<float> values, int from, int to){
	int total=0;
	if(to==-1 && !values.empty())
		to=values.size();
	for(int i=from;i<to;i++){
		total+=values[i];
	}
	return total;
}