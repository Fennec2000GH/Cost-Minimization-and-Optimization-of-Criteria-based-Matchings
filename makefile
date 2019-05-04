test1: test1.o
	g++ -std=c++11 -o test1 test1.o

test1.o: test1.cpp
	g++ -std=c++11 -c test1.cpp

test2: test2.o
	g++ -std=c++11 -o test2 test2.o 

test2.o: test2.cpp 
	g++ -std=c++11 -c test2.cpp 
 
clean: 
	rm *.o test1
