##
##  WSHARDWARE crio drivers
##

LIB = libwshardware

##
##  must set WPILIB_PATH in your environment
##  must set TARGET in your environment
##
##  Options for TARGET:
##    simulation
##    frc_crio_wpilib
##

CRIO_FILES = outputs/WsServo.cpp \
             outputs/RemoteDigitalOutput.cpp \
			 outputs/WsSolenoid.cpp \
			 outputs/WsDigitalOutput.cpp \
			 outputs/WsDoubleSolenoid.cpp \
			 outputs/WsRelay.cpp \
			 outputs/WsI2COutput.cpp \
			 outputs/WsTalon.cpp \
			 outputs/WsVictor.cpp \
			 outputs/RemoteAnalogOutput.cpp \
			 outputs/WsCanTalon.cpp \
             outputs/WsCanTalonCtreMagEncoder.cpp \
			 outputs/WsCanVictor.cpp \
             outputs/WsPhoenixController.cpp \
			 inputs/WsI2CInput.cpp \
			 inputs/WsJoystickAxis.cpp \
			 inputs/WsDigitalInput.cpp \
			 inputs/WsAnalogGyro.cpp \
			 inputs/WsAbsoluteEncoder.cpp \
			 inputs/WsCompassInput.cpp \
			 inputs/RemoteDigitalInput.cpp \
			 inputs/WsJoystickButton.cpp \
			 inputs/WsLIDAR.cpp \
			 inputs/WsHallEffectInput.cpp \
			 inputs/RemoteAnalogInput.cpp \
			 inputs/WsDPadButton.cpp \
			 inputs/WsDSAnalogInput.cpp \
			 inputs/WsDSDigitalInput.cpp \
			 inputs/WsAnalogInput.cpp \
             inputs/WsNavxInput.cpp \
			 LidarSensor.cpp \
			 CrioHardware.cpp \
			 WsCrioSmartDashboard.cpp \
             WsIterativeRobot.cpp \
             CrioIOFactory.cpp \
             crio_utils.cpp

SIMULATION_FILES = inputs/WsSimulatedInput.cpp \
                   outputs/WsSimulatedOutput.cpp \
                   SimulationHardware.cpp \
                   SimulationSmartDashboard.cpp \
                   WsSimulation.cpp \
                   SimulationIOFactory.cpp

CXXFLAGS = -O2 -Wall --std=c++11 -DNAMESPACED_WPILIB -g -rdynamic
CROSS = dummy-
CXX = g++
CC = gcc
AR = ar
ECHO = /bin/echo
RM = rm
TR = tr
MKDIR = mkdir

INCLUDES = -I$(WPILIB_PATH)/cpp/current/include \
	       -I$(WPILIB_PATH)/cpp/current/include/networktables \
           -I$(WPILIB_PATH)/cpp/current/include/HAL \
           -I$(WPILIB_PATH)/user/cpp/include \
           -I../libwscore/src \
           -I../libwsutil/src \
           -Isrc/common

TARGET_LOWER = $(shell echo $(TARGET) | tr A-Z a-z)

# check targets and apply
ifeq ($(TARGET_LOWER),frc_crio_wpilib)
    CXXFLAGS += -DTARGET_FRC_CRIO_WPILIB
    CROSS = arm-frc-linux-gnueabi-
    FILES = $(CRIO_FILES)
    INCLUDES += -I../libwsnavx/src
endif
ifeq ($(TARGET_LOWER),simulation)
    CXXFLAGS += -DTARGET_SIMULATION
    CROSS =
    FILES = $(SIMULATION_FILES)
endif

OBJECTS = $(addprefix obj/$(TARGET_LOWER)/, $(FILES:.cpp=.o))
LIB_FILE += lib/$(TARGET_LOWER)/$(LIB).a
INCLUDES += -Isrc/$(TARGET_LOWER)

ifeq ($(DEBUG),1)
    CXXFLAGS += -DWS_DEBUG
endif

.PHONY: clean

all: $(LIB_FILE)

$(LIB_FILE): $(OBJECTS)
	@$(ECHO) -e "\x1b[32m[AR]\x1b[0m $@"
	@$(MKDIR) -p $(@D)
	@$(CROSS)$(AR) rcs $@ $^

obj/$(TARGET_LOWER)/%.o: src/$(TARGET_LOWER)/%.cpp
	@$(ECHO) -e "\x1b[33m[CC]\x1b[0m \x1b[36m[libwshardware]\x1b[0m $@"
	@$(MKDIR) -p $(@D)
	@$(CROSS)$(CXX) $(INCLUDES) $(CXXFLAGS) -c $^ -o $@

clean:
	@$(RM) -rf obj
	@$(RM) -rf lib
