CC    = g++
FLAGS        = # -std=gnu99 -Iinclude
CFLAGS       = -fPIC -g
CPPFLAGS  =  -I ../../inc/
CPPFLAGS  +=  -I ../../../inc/
LDFLAGS      = -shared

DEBUGFLAGS   = -O0 -D _DEBUG
RELEASEFLAGS = -O2 -D NDEBUG -combine -fwhole-program

TARGET  = controller_agent_sprinkler.so

SOURCES = controller_agent_sprinkler.cpp
SOURCES += ../../src/controller_agent_base.cpp
HEADERS = controller_agent_sprinkler.hpp
HEADERS += ../../../inc/common_exc.hpp
HEADERS += ../../inc/i_agent.hpp
HEADERS += ../../inc/controller_agent_base.hpp
HEADERS += ../../inc/parser_file.hpp
HEADERS += ../../inc/i_event_receiver.hpp
HEADERS += ../../../inc/atomic_number.hpp

OBJECTS = $(SOURCES:.c=.o)


all: $(TARGET)

$(TARGET): $(OBJECTS)
		$(CC) $(FLAGS) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(DEBUGFLAGS) -o $(TARGET) $(OBJECTS)

