# Generated automatically from Makefile.in by configure.
CXX = c++
VERSION = 0.9.3
PG_INCL_FLAGS = 
PG_LIB_FLAGS = 
GPS_LIB_FLAGS = -L./gpsd
GPSD_INCL_FLAGS = -I./gpsd
GPSD_DAEMON = ./gpsd/gpsd

LIBS = -lgtkembedmoz  -L/usr/local/mozilla -L./lib -lpgDBManager -lpthread
GPS_LIBS = $(GPS_LIB_FLAGS) -lgpsd
GTK_LIBS = `gtk-config --libs`

OBJECTS = appFactory.o gtkApp.o main.o gtkMenu.o engine.o gtkToolbar.o position.o mgd77Extractor.o gdkCanvas.o colorDefaultAlgorithm.o spline.o panner.o gtkPreferences.o rcPersistence.o timer.o spline_forwarddifference.o parametricSurface.o contourGraphic.o latLongGrid.o gtkLatLongLine.o latLongLine.o navTool.o sal55NavTool.o eventSource.o command.o commandPan.o commandApplication.o commandZoom.o commandDraw.o commandGrid.o commandCursor.o commandTrack.o gtkCommandHandler.o gps.o gtkCurrentPositionIndicator.o commandAutoCenter.o contourGraphicContainer.o gtkGPSDisplay.o gtkSplash.o contourDBManager.o gecko.o cursor.o cursorCrosshair.o cursorDrawTrack.o cursorNull.o effort.o effortDraw.o effortDecoration.o effortDecorationCollection.o

FLAGS = -g -O2 -I/usr/lib/glib/include -I/usr/X11R6/include -D _DEBUG

#GTK_INCLUDES = `gtk-config --cflags` 

all: $(GPSD_DAEMON) lib/libpgDBManager contour 

contour: $(OBJECTS)
	$(CXX) -o contour $(OBJECTS) $(LIBS) $(GPS_LIBS) $(GTK_LIBS)

lib/libpgDBManager: postgres/pgDBManager.h postgres/pgDBManager.cpp
	cd postgres && $(MAKE)

# only compile this if its the local copy
$(GPSD_DAEMON): 
ifeq ($(GPSD_DAEMON),./gpsd/gpsd)
	cd gpsd && $(MAKE)
endif

clean: 
	rm -f core *.o $(PROGNAME) nohup.out

main.o: main.cpp
	$(CXX) $(FLAGS) -c main.cpp

appFactory.o: appFactory.h appFactory.cpp
	$(CXX) $(FLAGS) -c appFactory.cpp

gtkApp.o: bathApp.h gtkApp.h gtkApp.cpp position.o appFactory.o gtkPreferences.o rcPersistence.o gtkLatLongLine.h gtkToolbar.h gtkMenu.h contourGraphic.h gtkSplash.h
	$(CXX) $(FLAGS) -c gtkApp.cpp

gtkMenu.o: gtkMenu.h gtkMenu.cpp commandPan.h commandZoom.h commandDraw.h commandGrid.o commandAutoCenter.o eventSource.h eventSubscriber.h
	$(CXX) $(FLAGS) -c gtkMenu.cpp
	
gtkToolbar.o: gtkMenu.h gtkToolbar.h gtkToolbar.cpp
	$(CXX) $(FLAGS) -c gtkToolbar.cpp

position.o: position.h position.cpp
	$(CXX) $(FLAGS) -c position.cpp

mgd77Extractor.o: position.h position.cpp extractor.h mgd77Extractor.h mgd77Extractor.cpp
	$(CXX) $(FLAGS) -c mgd77Extractor.cpp

engine.o: position.h engine.h engine.cpp exception.h
	$(CXX) $(FLAGS) -c engine.cpp

splineEngine.o: position.h engine.h engine.cpp splineEngine.h splineEngine.cpp extractor.h
	$(CXX) $(FLAGS) -c splineEngine.cpp

parametricSurface.o: position.h engine.h engine.cpp parametricSurface.h parametricSurface.cpp extractor.h exception.h
	$(CXX) $(FLAGS) -c parametricSurface.cpp

gdkCanvas.o: canvas.h gdkCanvas.h gdkCanvas.cpp
	$(CXX) $(FLAGS) -c gdkCanvas.cpp
	
colorDefaultAlgorithm.o: colorAlgorithm.h colorDefaultAlgorithm.h colorDefaultAlgorithm.cpp
	$(CXX) $(FLAGS) -c colorDefaultAlgorithm.cpp

spline.o: position.h engine.h engine.cpp spline.h spline.cpp
	$(CXX) $(FLAGS) -c spline.cpp

spline_forwarddifference.o: spline.h spline.cpp spline_forwarddifference.h spline_forwarddifference.cpp
	$(CXX) $(FLAGS) -c spline_forwarddifference.cpp

panner.o: engine.h canvas.h panner.h panner.cpp
	$(CXX) $(FLAGS) -c panner.cpp

gtkPreferences.o: engine.h canvas.h preferences.h gtkPreferences.h gtkPreferences.cpp
	$(CXX) $(FLAGS) -c gtkPreferences.cpp

rcPersistence.o: persistence.h  rcPersistence.cpp  rcPersistence.h
	$(CXX) $(FLAGS) -c rcPersistence.cpp

timer.o: timer.h  timer.cpp
	$(CXX) $(FLAGS) -c timer.cpp

contourGraphic.o: contourGraphic.h  contourGraphic.cpp position.h engine.h preferences.h appFactory.h
	$(CXX) $(FLAGS) -c contourGraphic.cpp

latLongLine.o: latLongLine.h  latLongLine.cpp contourGraphic.h
	$(CXX) $(FLAGS) -c latLongLine.cpp

gtkLatLongLine.o: gtkLatLongLine.h  gtkLatLongLine.cpp latLongLine.h contourGraphic.h
	$(CXX) $(FLAGS) -c gtkLatLongLine.cpp

latLongGrid.o: latLongGrid.h  latLongGrid.cpp latLongLine.h contourGraphic.h
	$(CXX) $(FLAGS) -c latLongGrid.cpp

navTool.o: navTool.h  navTool.cpp
	$(CXX) $(FLAGS) -c navTool.cpp

sal55NavTool.o: navTool.h  softwareNavTool.h sal55NavTool.h sal55NavTool.cpp commandPan.h commandZoom.h
	$(CXX) $(FLAGS) -Ipixmaps/nav -c sal55NavTool.cpp

eventSource.o: eventSource.h eventSource.cpp eventSubscriber.h
	$(CXX) $(FLAGS) -c eventSource.cpp

command.o: command.h command.cpp
	$(CXX) $(FLAGS) -c command.cpp

commandCursor.o: command.h commandCursor.h commandCursor.cpp
	$(CXX) $(FLAGS) -c commandCursor.cpp

commandPan.o: command.h commandPan.h commandPan.cpp
	$(CXX) $(FLAGS) -c commandPan.cpp

commandApplication.o: command.h commandApplication.h commandApplication.cpp
	$(CXX) $(FLAGS) -c commandApplication.cpp

commandZoom.o: command.h commandZoom.h commandZoom.cpp
	$(CXX) $(FLAGS) -c commandZoom.cpp

commandDraw.o: command.h commandEventSource.h commandDraw.h commandDraw.cpp eventSource.h eventSubscriber.h
	$(CXX) $(FLAGS) -c commandDraw.cpp

commandGrid.o: command.h commandEventSource.h commandGrid.h commandGrid.cpp eventSource.h eventSubscriber.h
	$(CXX) $(FLAGS) -c commandGrid.cpp

commandTrack.o: command.h commandEventSource.h commandTrack.h commandTrack.cpp eventSource.h eventSubscriber.h
	$(CXX) $(FLAGS) -c commandTrack.cpp

commandAutoCenter.o: command.h commandEventSource.h commandAutoCenter.h commandAutoCenter.cpp eventSource.h eventSubscriber.h
	$(CXX) $(FLAGS) -c commandAutoCenter.cpp

gtkCommandHandler.o: gtkCommandHandler.h gtkCommandHandler.cpp
	$(CXX) $(FLAGS) -c gtkCommandHandler.cpp

gps.o: gps.h gps.cpp
	$(CXX) $(FLAGS) $(GPSD_INCL_FLAGS) -c gps.cpp

gtkCurrentPositionIndicator.o: gtkCurrentPositionIndicator.h gtkCurrentPositionIndicator.cpp
	$(CXX) $(FLAGS) -c gtkCurrentPositionIndicator.cpp

gtkGPSDisplay.o: contourGraphic.h gtkGPSDisplay.h gtkGPSDisplay.cpp
	$(CXX) $(FLAGS) -c gtkGPSDisplay.cpp

contourGraphicContainer.o: contourGraphicContainer.h contourGraphicContainer.cpp contourGraphic.h
	$(CXX) $(FLAGS) -c contourGraphicContainer.cpp

gtkSplash.o: gtkSplash.cpp gtkSplash.h
	$(CXX) $(FLAGS) -c gtkSplash.cpp

contourDBManager.o: contourDBManager.cpp contourDBManager.h
	$(CXX) $(FLAGS) $(PG_INCL_FLAGS) -c contourDBManager.cpp

contourDBEffort.o: contourDBEffort.cpp contourDBEffort.h
	$(CXX) $(FLAGS) -c contourDBEffort.cpp

# used to include a mozilla include file here but now we package the .h here
gecko.o: gecko.cpp gecko.h
	$(CXX) $(FLAGS) -c gecko.cpp

cursor.o: cursor.cpp cursor.h
	$(CXX) $(FLAGS) -c cursor.cpp

cursorNull.o: cursorNull.cpp cursorNull.h cursor.h contourGraphic.h
	$(CXX) $(FLAGS) -c cursorNull.cpp

cursorCrosshair.o: cursorCrosshair.cpp cursorCrosshair.h cursor.h contourGraphic.h
	$(CXX) $(FLAGS) -c cursorCrosshair.cpp

cursorDrawTrack.o: cursor.h cursorDrawTrack.cpp cursorDrawTrack.h contourGraphic.h
	$(CXX) $(FLAGS) -c cursorDrawTrack.cpp

effort.o: effort.cpp effort.h
	$(CXX) $(FLAGS) -c effort.cpp

effortDraw.o: effortDraw.cpp effortDraw.h effort.h contourDBEffort.h canvas.h preferences.h
	$(CXX) $(FLAGS) -c effortDraw.cpp

effortDecoration.o: effortDecoration.cpp effortDecoration.h contourGraphic.h
	$(CXX) $(FLAGS) -c effortDecoration.cpp

effortDecorationCollection.o: effortDecorationCollection.cpp contourGraphic.h effortDecorationCollection.h
	$(CXX) $(FLAGS) -c effortDecorationCollection.cpp
