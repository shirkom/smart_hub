CXX = g++
CC = $(CXX)

CFLAGS += -ansi -pedantic -Wall
CFLAGS += -Werror
CFLAGS += -g
CXXFLAGS += -std=c++03 $(CFLAGS)
CPPFLAGS += -L. 
CPPFLAGS =  -I ../../inc/
CPPFLAGS +=  -I ../../../inc/
CPPFLAGS +=  -I ../../../inc/inl/
CPPFLAGS +=  -I ../../serverTCP/DS/libH/
CPPFLAGS +=  -I ../inc/
CPPFLAGS +=  -I ../hvac_controller/

#what libraries do link
LDLIBS = -lpthread -ldl  ../tcp_server_sensor/make_lib/libserver.so
#what flags to pass to the linker
LDFLAGS = -g3


INFRA_DIR = ../../src
FRAMEWORK_DIR = ../../../src
MAIN_MODULE = main
MAIN_SRC = $(MAIN_MODULE).cpp
MAIN_SRC += $(INFRA_DIR)/event_router.cpp
MAIN_SRC += $(INFRA_DIR)/controller_agent_base.cpp
MAIN_SRC += $(INFRA_DIR)/registration_manager.cpp
MAIN_SRC += $(INFRA_DIR)/event.cpp
MAIN_SRC += $(INFRA_DIR)/location.cpp
MAIN_SRC += $(INFRA_DIR)/event_receiver.cpp
MAIN_SRC += $(INFRA_DIR)/sensor_agent_base.cpp
MAIN_SRC += $(INFRA_DIR)/sensor_agent_base.cpp
MAIN_SRC += $(INFRA_DIR)/hub.cpp
MAIN_SRC += $(INFRA_DIR)/start_up.cpp
MAIN_SRC += $(INFRA_DIR)/parser_file.cpp
MAIN_SRC += $(INFRA_DIR)/tcp_server_sensor.cpp
MAIN_SRC += $(INFRA_DIR)/network.cpp
MAIN_SRC += $(INFRA_DIR)/agent_factory.cpp
MAIN_SRC += $(INFRA_DIR)/event_marshal.cpp
MAIN_SRC += $(INFRA_DIR)/http_event_unserializer.cpp
MAIN_SRC += $(FRAMEWORK_DIR)/condition.cpp
MAIN_SRC += $(FRAMEWORK_DIR)/mutex.cpp
MAIN_SRC += $(FRAMEWORK_DIR)/thread_pool.cpp
MAIN_SRC += $(FRAMEWORK_DIR)/atomicflag.cpp


$(MAIN_MODULE): $(MAIN_SRC)

hvac: 
	$(MAKE) -C ../hvac_controller/

server: 
	$(MAKE) -C ../tcp_server_sensor/make_lib/

safty: 
	$(MAKE) -C ../fire_safety_sensor/

sprinkler: 
	$(MAKE) -C ../sprinkler_controller/

temp: 
	$(MAKE) -C ../temp_sensor/

check: $(MAIN_MODULE) hvac safty sprinkler temp server
	./$(MAIN_MODULE)

recheck: clean check
clean:
	-rm $(MAIN_MODULE)
debug: $(MAIN_MODULE)
	gdb -q ./$(MAIN_MODULE)

redebug: clean debug 

release: CPPFLAGS += -DNDEBUG

release: clean $(MAIN_MODULE)
	strip $(MAIN_MODULE)

VALA_FLAGS = --leak-check=full --track-origins=yes -s
VALA_FLAGS += --log-file=Val.log

vala: $(MAIN_MODULE)
	valgrind $(VALA_FLAGS) ./$(MAIN_MODULE)

.PHONY: clean check recheck