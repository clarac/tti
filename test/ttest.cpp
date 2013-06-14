#include <tti.h>
#include "SimpleDriver.cpp"

int main(){
	
	SimpleDriver *d = new SimpleDriver();
	std::vector<WrapperBaseDriver *> dv;
	
	dv.push_back(d);
	d = new SimpleDriver();
	dv.push_back(d);
	d = new SimpleDriver();
	dv.push_back(d);


	tti t(dv);

	//client-server mode test
	std::vector<raceData> rd = t.race(3, "g-track-1");

	for(int i=0;i<rd.size();i++){
		std::cout << "driver #" << i << std::endl;
		std::cout << "total time = " << rd[i].totalTime << "; avgTime = " << rd[i].avgTime;
		std::cout << "; damage points = " << rd[i].damage << "; outside for = " << rd[i].ticksOutside<< endl;
		for(int j=0;j<rd[i].timeList.size();j++){
			printf("%f\n",rd[i].timeList[j]);
		}	
	}

	std::cout << "----------------------------------------"<< std::endl;

	rd = t.race(3, "ruudskogen");
 
	for(int i=0;i<rd.size();i++){
		std::cout << "driver #" << i << std::endl;
		std::cout << "total time = " << rd[i].totalTime << "; avgTime = " << rd[i].avgTime;
		std::cout << "; damage points = " << rd[i].damage << "; outside for = " << rd[i].ticksOutside<< endl;
		for(int j=0;j<rd[i].timeList.size();j++){
			printf("%f\n",rd[i].timeList[j]);
		}	
	}


	//test for simple average (not counting 1st lap)
	//printf("%f\n",t.getSingleTime(12, "wheel-1", tti_AVG1,3,"berniw"));


	//test for 
	/*
	std::vector<float> tempos = t.getSingleTimeList(5, "wheel-1", 7, "tita");
	for(int i=0;i<tempos.size();i++){
		printf("%f\n",tempos[i]);
	}*/


	//if(t.getSingleDistanceList(5, "dirt-1", 1,7, "tita")
	//printf("%f\n",));
	
	return 0;    
}



