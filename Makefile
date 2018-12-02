CXX		 = clang++
CXXFLAGS = -Wall -Wextra -Wreturn-type -std=c++11 -g -O2

writeGo: extractPfam.cpp
	${CXX} ${CXXFLAGS} extractPfam.cpp -o writeGo

writeSmaller: smallFiles.cpp
	${CXX} ${CXXFLAGS} smallFiles.cpp -o writeSmaller

clean: 
	rm *.o
