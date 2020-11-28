CXX=g++
CFLAGS= -Wall -std=c++11
EXEC=run

all: run

debug: CFLAGS += -DDEBUG
debug: clean
debug: run

run: mailDB.o 
	$(CXX) $(CFLAGS) main.cpp mailDB.o -o $(EXEC)

mailDB.o: mailDB.cpp
	$(CXX) -c $(CFLAGS) mailDB.cpp

clean:
	rm -f $(EXEC) *.o