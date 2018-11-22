CXX		 = clang++
CXXFLAGS = -Wall -Wextra -Wreturn-type -std=c++11 -g -O2

writeSmaller: smallFiles.cpp
	${CXX} ${CXXFLAGS} smallFiles.cpp -o writeSmaller

clean: 
	rm *.o
