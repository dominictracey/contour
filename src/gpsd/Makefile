# Generated automatically from Makefile.in by configure.
# Makefile.in
# $Modified: Thu Jan 22 11:55:32 1998 by brook $


## The C compiler

# What we use to compile C files.
# CC = cc
CC = gcc

# The flags we use to compile C files.
CFLAGS = -g -O2


## The C preprocessor.

# What we use for preprocessing.  (gcc -E -traditional-cpp)
CPP = @CPP_TRADITIONAL@

# Flags passed to the C preprocessor.  ()
CPPFLAGS = 

# Definitions to be passed to the C preprocessor.  (-DHAVE_CONFIG_H)
DEFS = -DHAVE_CONFIG_H


## The Linker.

# Flags passed to the linker.  (-g -O)
LDFLAGS = 

# Use this for building statically linked executables with GCC.
# LDFLAGS = -static 


## Local libraries

# Math library (-lm)
LIBM       = -lm
# C library (-lc)
LIBC       = -lc

# All libraries shown above
LIBS = $(LIBM) $(LIBC)

# Network libraries
LIBNSL = -lnsl
LIBSOCKET = 
NETLIBS = $(LIBNSL) $(LIBSOCKET)

## X Libraries

# Special flags for linking with X.  (-L/usr/X11R5/lib)
X_LDFLAGS =  -L/usr/X11R6/lib

# Motif library.  (-lXm)
LIBXM = 

# Use this alternative for building `semistatic' executables
# where Motif libraries are statically linked in.
# LIBXM = -Wl,-Bstatic  -Wl,-Bdynamic

# X extension library; needed for Athena and Motif >= 2.0.  (-lXext)
LIBXEXT = -lXext

# Xpm library; needed for DDD and sometimes for Motif >= 2.0.  (-lXpm)
LIBXPM = -lXpm

# Xp library; sometimes needed for Motif >= 2.1.  (-lXp)
LIBXP = -lXp

# gen library; sometimes needed for Motif >= 2.0.  (-lgen)
LIBGEN = 

# X toolkit library.  (-lXt)
LIBXT = -lXt

# Xaw library.  (-lXaw)
LIBXAW = -lXaw

# X library.  (-lSM -lICE -lX11 -lnsl -lsocket)
LIBX11 =  -lSM -lICE -lX11 

# All libraries shown above
ALL_X_LIBS = $(X_LDFLAGS) $(LIBXM) $(LIBXP) $(LIBXPM) $(LIBXEXT) \
$(LIBXT) $(LIBX11) $(LIBGEN)


## C compile commands.
COMPILE.c = $(CC) $(CPPFLAGS) $(DEFS) $(CFLAGS) -c
LINK.c = $(CC) $(LDFLAGS)
COMPILE_AND_LINK.c = \
    $(CC) $(CPPFLAGS) $(DEFS) $(CFLAGS) $(LDFLAGS)

## Where to look for X include files.  (-I/usr/X11R5/include)
X_INCLUDE =  -I/usr/X11R6/include

## Where to look for include files.
INCLUDE = -I. $(X_INCLUDE)

## Implicit rules.
.SUFFIXES: .c
.c.o:
	$(COMPILE.c) $(INCLUDE) -o $@ $<


## Libraries and object files
LIB_OBJECTS = netlib.o nmea_parse.o serial.o tm.o em.o $(MOTIF_OBJECTS)

## Programs
PROGRAMS=gpsd

## Motif dependent programs and object files
# MOTIF_PROGRAMS=gps xgpsspeed
# MOTIF_OBJECTS = display.o

XGPSOBJS=xgpsspeed.o Tachometer.o

all: $(PROGRAMS) $(MOTIF_PROGRAMS)

gpsd: gpsd.o libgpsd.a
	$(LINK.c) -o $@ gpsd.o -L. -lgpsd $(NETLIBS) $(LIBS)

gps: gps.o libgpsd.a
	$(LINK.c) -o $@ gps.o -L. -lgpsd $(ALL_X_LIBS) $(LIBS)

xgpsspeed: $(XGPSOBJS)
	$(LINK.c) -o $@ $(XGPSOBJS) -L. -lgpsd $(LIBXAW) $(ALL_X_LIBS) $(LIBS)

libgpsd.a:  $(LIB_OBJECTS)
	ar -r libgpsd.a $(LIB_OBJECTS)
	ranlib libgpsd.a

clean:
	rm -f *.o *.a gpsd gps *~ config.status

dist-clean: clean
	rm -f config.cache config.log
