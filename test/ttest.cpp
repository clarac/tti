#include "tti.h"
#include "SimpleDriver.cpp"

int main(){
	  
	SimpleDriver *d = new SimpleDriver();
	std::vector<WrapperBaseDriver *> dv;
	
	dv.push_back(d);
  
	tti t(dv);
	
	while(t.getRound()==0){
		std::vector<raceData> rd = t.race();
		for(int i=0;i<rd.size();i++){
			std::cout << "\ndriver #" << i << std::endl;
			std::cout << "total time = " << rd[i].totalTime << "; avgTime = " << rd[i].avgTime;
			std::cout << "; damage points = " << rd[i].damage << "; outside for = " << rd[i].ticksOutside<< endl;
			for(int j=0;j<rd[i].timeList.size();j++){
				std::cout << rd[i].timeList[j]<<std::endl;
			}	
		}
	}	
	return 0;    
}



