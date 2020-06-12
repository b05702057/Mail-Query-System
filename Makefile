run: mailDB cmdParser.cpp
	g++ cmdParser.cpp mailDB.o -o $@

mailDB: mailDB.h mailDB.cpp
	g++ mailDB.cpp -c

clean:
	rm -f mailDB.o run