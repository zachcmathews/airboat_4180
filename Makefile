all: project.o serial.o servo.o motor.o simpleio.o
	g++ -Wall -pthread project.o serial.o servo.o motor.o simpleio.o -o project -lpigpio -lrt

clean:
	rm -rfv *.o project

project.o: project.cpp serial.h servo.h motor.h simpleio.h
	g++ -Wall -c project.cpp -o project.o

serial.o: serial.cpp serial.h
	g++ -Wall -c serial.cpp -o serial.o

servo.o: servo.cpp servo.h
	g++ -Wall -c servo.cpp -o servo.o

motor.o: motor.cpp motor.h simpleio.h
	g++ -Wall -c motor.cpp -o motor.o

simpleio.o: simpleio.cpp simpleio.h
	g++ -Wall -c simpleio.cpp -o simpleio.o
