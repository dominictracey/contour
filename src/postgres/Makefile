# Generated automatically from Makefile.in by configure.
FLAGS = -g -D _DEBUG
INCLUDES =  
#INCLUDES = -I/usr/include/pgsql -I/usr/local/lib/glib/include
LIBS =  -lpq 
 
CC = g++

all: pgDBManager.o pgDBEffort.o
	rm -f ../lib/libpgDB*
	$(CC) -shared $(LIBS) -Wl,-soname,libpgDBManager.so.1 -o ../lib/libpgDBManager.so.1.0 pgDBManager.o pgDBEffort.o
	ln -s ../lib/libpgDBManager.so.1.0 ../lib/libpgDBManager.so.1
	ln -s ../lib/libpgDBManager.so.1 ../lib/libpgDBManager.so

pgDBManager.o: pgDBManager.cpp
	$(CC) -fPIC $(FLAGS) $(INCLUDES) -c pgDBManager.cpp

pgDBEffort.o: pgDBEffort.cpp
	$(CC) -fPIC $(FLAGS) $(INCLUDES) -c pgDBEffort.cpp

clean:
	rm f ../lib/libpgDB* *.o
