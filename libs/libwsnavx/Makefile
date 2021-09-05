##
##  WSCORE Robot framework code
##

LIB = libwsnavx

##
##  must set WPILIB_PATH in your environment
##  must set TARGET in your environment
##
##  Options for TARGET:
##    simulation
##    frc_crio_wpilib
##

FILES = ahrs/OffsetTracker.cpp \
		ahrs/SerialIO.cpp \
		ahrs/RegisterIOSPI.cpp \
		ahrs/AHRS.cpp \
		ahrs/ContinuousAngleTracker.cpp \
		ahrs/RegisterIO.cpp \
		ahrs/RegisterIOI2C.cpp \
		ahrs/InertialDataIntegrator.cpp \
        ws_navx.cpp

# CXXFLAGS = -O2 -Wall --std=c++11 -DNAMESPACED_WPILIB -g -rdynamic
CXXFLAGS = -O2 -Wall --std=c++11 -g -rdynamic
CROSS = dummy-
CXX = g++
CC = gcc
AR = ar
ECHO = /bin/echo
RM = rm
TR = tr
MKDIR = mkdir
SED = sed

INCLUDES = -I$(WPILIB_PATH)/cpp/current/include \
	       -I$(WPILIB_PATH)/cpp/current/include/networktables \
           -I$(WPILIB_PATH)/cpp/current/include/HAL \
           -I$(WPILIB_PATH)/user/cpp/include \
           -I../libwscore/src \
           -Isrc \
           -Isrc/ahrs \
           -Isrc/ahrs_include

TARGET_LOWER = $(shell echo $(TARGET) | tr A-Z a-z)

# check targets and apply
ifeq ($(TARGET_LOWER),frc_crio_wpilib)
    CXXFLAGS += -DTARGET_FRC_CRIO_WPILIB
    CROSS = arm-frc-linux-gnueabi-
endif
ifeq ($(TARGET_LOWER),simulation)
    CXXFLAGS += -DTARGET_SIMULATION
    CROSS =
endif

OBJECTS = $(addprefix obj/$(TARGET_LOWER)/, $(FILES:.cpp=.o))
DEPS = $(OBJECTS:.o=.d)
LIB_FILE = lib/$(TARGET_LOWER)/$(LIB).a

ifeq ($(DEBUG),1)
    CXXFLAGS += -DWS_DEBUG
endif

.PHONY: clean
#depend

all: $(LIB_FILE)

#depend: .depend

#.depend: $(addprefix src/, $(FILES))
#	@$(RM) -f ./.depend
#	@$(CROSS)$(CXX) $(INCLUDES) $(CXXFLAGS) -MM $^ >> ./.depend;
#	@$(SED) -Ei 's#^(.*\.o: *)src/(.*/)?(.*\.cpp)#obj/$(TARGET_LOWER)/\2\1src/\2\3#' $@

#include .depend

$(LIB_FILE): $(OBJECTS)
	@$(ECHO) -e "\x1b[32m[AR]\x1b[0m $@"
	@$(MKDIR) -p $(@D)
	@$(CROSS)$(AR) rcs $@ $^

obj/$(TARGET_LOWER)/%.o: src/%.cpp
	@$(ECHO) -e "\x1b[33m[CC]\x1b[0m \x1b[93m[libwsnavx]\x1b[0m $@"
	@$(MKDIR) -p $(@D)
	@$(CROSS)$(CXX) $(INCLUDES) $(CXXFLAGS) -c $^ -o $@

clean:
	@$(RM) -rf obj
	@$(RM) -rf lib
	@$(RM) -f ./.depend

