#include <tti.h>
#include "SimpleDriver.cpp"

int main(){
	
	SimpleDriver *d = new SimpleDriver();
	std::vector<WrapperBaseDriver *> dv;
	
	dv.push_back(d);
	d = new SimpleDriver();
	//dv.push_back(d);
	d = new SimpleDriver();
	//dv.push_back(d);
	d = new SimpleDriver();
	//dv.push_back(d);

	tti t(dv);

	std::vector<raceData> rd = t.raceGUI();

	for(int i=0;i<rd.size();i++){
		std::cout << "total time = " << rd[i].totalTime << "; avgTime = " << rd[i].avgTime;
		std::cout << "; damage points = " << rd[i].damage << "; outside for = " << rd[i].ticksOutside<< endl;
		for(int j=0;j<rd[i].timeList.size();j++){
			printf("%f\n",rd[i].timeList[j]);
		}	
	}


	//printf("%f\n",t.getSingleTime(12, "wheel-1", tti_AVG1));
	//std::vector<float> tempos = t.getSingleTimeList(5, "wheel-1", 7, "tita");
	/*for(int i=0;i<tempos.size();i++){
		printf("%f\n",tempos[i]);
	}*/
	//printf("%f\n",t.getSingleDistanceList(2,"",1)[0]);
	
	return 0;    
}



