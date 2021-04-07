CC    = g++
FLAGS        = -std=c++03 #chack if it is ok
CFLAGS       = -fPIC -g
CPPFLAGS  =  -I ../../inc/
CPPFLAGS  +=  -I ../../../inc/
LDFLAGS      = -shared

DEBUGFLAGS   = -O0 -D _DEBUG
RELEASEFLAGS = -O2 -D NDEBUG -combine -fwhole-program

TARGET  = sensor_agent_fire_safty.so

SOURCES = sensor_agent_fire_safty.cpp
SOURCES += ../../src/sensor_agent_base.cpp
SOURCES += ../../src/location.cpp
SOURCES += ../../src/event.cpp

HEADERS = sensor_agent_fire_safty.hpp
HEADERS += ../../../inc/common_exc.hpp
HEADERS += ../../inc/i_agent.hpp
HEADERS += ../../inc/sensor_agent_base.hpp
HEADERS += ../../inc/parser_file.hpp
HEADERS += ../../inc/location.hpp
HEADERS += ../../inc/event.hpp
HEADERS += ../../inc/i_event_receiver.hpp
HEADERS += ../../../inc/atomic_number.hpp

OBJECTS = $(SOURCES:.c=.o)


all: $(TARGET)

$(TARGET): $(OBJECTS)
		$(CC) $(FLAGS) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(DEBUGFLAGS) -o $(TARGET) $(OBJECTS)

