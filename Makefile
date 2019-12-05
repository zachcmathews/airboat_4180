all: project.o motor.o simpleio.o
	g++ -Wall -pthread project.o motor.o simpleio.o -o project -lpigpio -lrt

clean:
	rm -rfv *.o project

project.o: project.cpp motor.h simpleio.h
	g++ -Wall -c project.cpp -o project.o

motor.o: motor.cpp motor.h simpleio.h
	g++ -Wall -c motor.cpp -o motor.o

simpleio.o: simpleio.cpp simpleio.h
	g++ -Wall -c simpleio.cpp -o simpleio.o
