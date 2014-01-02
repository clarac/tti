ttest: ttest.o SimpleDriver.o
	g++ -L${TORCS_BASE}/export/lib -L/usr/lib  -o ttest ttest.o SimpleDriver.o -ltti -ltgf -ltxml

ttest.o: ttest.cpp 
	g++ -I${TORCS_BASE}/export/include -I${TORCS_BASE} -I. -std=c++11 -pthread -c ttest.cpp 

SimpleDriver.o: SimpleDriver.h SimpleDriver.cpp 
	g++ -I${TORCS_BASE}/export/include -I${TORCS_BASE} -I. -c SimpleDriver.cpp 
