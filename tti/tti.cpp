
#include "tti.h"
#include <tgf.h>


tti::tti() {

}

tti::~tti() {
}


FILE * tti::callT(int cars, int laps, std::string track, int dind, std::string driver){
	void *parmHandle = GfParmReadFile("/home/clara/.torcs/results/practice.xml", GFPARM_RMODE_STD);
	GfParmSetNum(parmHandle, "Practice", "laps", NULL, laps);
	GfParmSetStr(parmHandle, "Tracks/1", "name", track.c_str());
	GfParmSetNum(parmHandle, "Drivers/1", "idx", NULL, dind);
	GfParmSetStr(parmHandle, "Drivers/1", "module", driver.c_str());
	GfParmWriteFile (NULL, parmHandle, NULL);
	return popen("torcs -r /home/clara/.torcs/results/practice.xml", "r");
}         


std::vector<float> tti::getSingleTimeList(int laps, std::string track, int dind, std::string driver){
	std::vector<float> lapTimes, empty;
	float last=0.0f, aux;
	if(laps>0){
		char output[100], subs[12];
		float val;
		int i;
		FILE *p = callT(1,laps,track, dind, driver);
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
	float val=0.0f;
	unsigned int i;
	if(tempos.empty()){
		return -1.0f;
	}
	else if(ret==tti_AVG){
		for(i=0;i<tempos.size();i++){

			val+=tempos.at(i);
		}
		return val/tempos.size();
	} else if(ret==tti_AVG1&&tempos.size()>1){
		for(i=1;i<tempos.size();i++){
			val+=tempos.at(i);
		}
		return val/(tempos.size()-1);
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
		FILE *p = callT(1,laps,track, dind, driver);
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
