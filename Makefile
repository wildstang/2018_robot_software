##
##  2018 Yearly robot code
##

EXE = wildstang

ROBOT = 10.1.11.2
USER = lvuser

##
##  must set WPILIB_PATH in your environment
##  must set TARGET in your environment
##
##  Options for TARGET:
##    simulation
##    frc_crio_wpilib
##

FILES = subsystems/drive/CheesyDriveHelper.cpp \
        subsystems/drive/WsDrive.cpp \
        subsystems/drive/Util.cpp \
        subsystems/drive/DrivePidIO.cpp \
        subsystems/drive/TurnPidIO.cpp \
        subsystems/drive/DriveHeadingPidIO.cpp \
        subsystems/drive/ArcAnglePidIO.cpp \
        subsystems/drive/ArcDrivePidIO.cpp \
        subsystems/Drive.cpp \
        subsystems/Claw.cpp \
        subsystems/Monitor.cpp \
        subsystems/Arm.cpp \
        subsystems/Lift.cpp\
		subsystems/Manipulator.cpp \
		subsystems/LED.cpp \
        subsystems/AutoChooser.cpp \
		auto/WsAutoStepFactory.cpp \
        auto/steps/DriveSetBrakeModeStep.cpp \
        auto/steps/DriveStraightStep.cpp \
		auto/steps/DriveStraightQuickStep.cpp \
		auto/steps/DriveTurnStep.cpp \
        auto/steps/DriveQuickTurnStep.cpp \
        auto/steps/ArcDriveStep.cpp \
		auto/steps/LiftStep.cpp \
		auto/steps/ManipulatorStep.cpp \
		auto/steps/LiftFinishedStep.cpp \
		auto/steps/ClawStep.cpp \
		auto/steps/ArmStep.cpp \
		auto/steps/ArmFinishedStep.cpp \
        auto/steps/IntakeCubeStep.cpp \
        auto/steps/ScoreStep.cpp \
        auto/steps/StartConfigStep.cpp \
        robot/WsRobot.cpp

CXXFLAGS = -O2 -Wall --std=c++11 -pthread -g -rdynamic
LDFLAGS = -lm
CROSS = dummy-
CXX = g++
CC = gcc
AR = ar
ECHO = /bin/echo
RM = rm
TR = tr
MKDIR = mkdir
MAKE = make
SSHPASS = sshpass
SSH = ssh
SCP = scp
CP = cp

INCLUDES = -I../libwscore/src \
           -I../libwshardware/src/common \
		   -I../libwsutil/src \
           -Isrc
LIBS = -lwshardware \
       -lwscore \
	   -lwsutil

TARGET_LOWER = $(shell echo $(TARGET) | tr A-Z a-z)

HARDWARE_LIB = ../libwshardware/lib/$(TARGET_LOWER)/libwshardware.a
CORE_LIB = ../libwscore/lib/$(TARGET_LOWER)/libwscore.a
UTIL_LIB = ../libwsutil/lib/$(TARGET_LOWER)/libwsutil.a
NAVX_LIB = ../libwsnavx/lib/$(TARGET_LOWER)/libwsnavx.a

LIB_DEPS = $(HARDWARE_LIB) $(CORE_LIB) $(UTIL_LIB)

# check targets and apply
ifeq ($(TARGET_LOWER),frc_crio_wpilib)
    CXXFLAGS += -DTARGET_FRC_CRIO_WPILIB -DNAMESPACED_WPILIB
    CROSS = arm-frc-linux-gnueabi-
    INCLUDES += -I$(WPILIB_PATH)/cpp/current/include \
                -I$(WPILIB_PATH)/cpp/current/include/networktables \
                -I$(WPILIB_PATH)/cpp/current/include/HAL \
                -I$(WPILIB_PATH)/user/cpp/include \
				-I../libwsnavx/src
    LDFLAGS += -Wl,-rpath,/opt/GenICam_v3_0_NI/bin/Linux32_ARM,-rpath,/usr/local/frc/lib
    LDFLAGS += -L$(WPILIB_PATH)/common/current/lib/linux/athena/shared \
               -L$(WPILIB_PATH)/cpp/current/reflib/linux/athena/shared \
               -L$(WPILIB_PATH)/user/cpp/lib \
			   -L../libwsnavx/lib/$(TARGET_LOWER)
    LIBS += -lwsnavx \
			-lCTRE_Phoenix \
            -lCTRE_PhoenixCCI \
            -lwpi
    LIB_DEPS += $(NAVX_LIB)
endif
ifeq ($(TARGET_LOWER),simulation)
    CXXFLAGS += -DTARGET_SIMULATION
    CROSS =
endif

OBJECTS = $(addprefix obj/$(TARGET_LOWER)/, $(FILES:.cpp=.o))
EXE_FILE = bin/$(TARGET_LOWER)_$(EXE)
LDFLAGS += -L../libwshardware/lib/$(TARGET_LOWER) \
           -L../libwscore/lib/$(TARGET_LOWER) \
           -L../libwsutil/lib/$(TARGET_LOWER)
INCLUDES += -I../libwshardware/src/$(TARGET_LOWER)

ifeq ($(DEBUG),1)
    CXXFLAGS += -DWS_DEBUG
endif

.PHONY: clean clean-all install simulate eclipse $(HARDWARE_LIB) $(CORE_LIB) $(UTIL_LIB) $(NAVX_LIB)

all: $(EXE_FILE)

$(EXE_FILE): $(LIB_DEPS) $(OBJECTS)
	@$(ECHO) -e "\x1b[32m[LD]\x1b[0m $@"
	@$(MKDIR) -p $(@D)
	@$(CROSS)$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

obj/$(TARGET_LOWER)/%.o: src/%.cpp
	@$(ECHO) -e "\x1b[33m[CC]\x1b[0m \x1b[94m[robot]\x1b[0m $@"
	@$(MKDIR) -p $(@D)
	@$(CROSS)$(CXX) $(INCLUDES) $(CXXFLAGS) -c $^ -o $@

# need to figure this out, the directories disappear when cleaning
#%.a: $(@D)/../../Makefile
#	@$(ECHO) "[CD] $(@D)"
#	@$(MAKE) -C $(@D)/../..

$(HARDWARE_LIB):
	@$(ECHO) -e "\x1b[33m[MK]\x1b[0m $@"
	@$(MAKE) -C ../libwshardware

$(CORE_LIB):
	@$(ECHO) -e "\x1b[33m[MK]\x1b[0m $@"
	@$(MAKE) -C ../libwscore

$(UTIL_LIB):
	@$(ECHO) -e "\x1b[33m[MK]\x1b[0m $@"
	@$(MAKE) -C ../libwsutil

$(NAVX_LIB):
	@$(ECHO) -e "\x1b[33m[MK]\x1b[0m $@"
	@$(MAKE) -C ../libwsnavx

clean:
	@$(RM) -rf obj
	@$(RM) -rf bin

clean-all: clean
	@$(MAKE) clean -C ../libwshardware
	@$(MAKE) clean -C ../libwscore
	@$(MAKE) clean -C ../libwsutil
	@$(MAKE) clean -C ../libwsnavx

simulate: $(EXE_FILE)
	@./$(EXE_FILE)

install: $(EXE_FILE)
	@$(ECHO) -e "\x1b[33m[SCP]\x1b[0m WPI_Native_Libraries.properties"
	@$(SCP) $(USER)@$(ROBOT):/usr/local/frc/lib/WPI_Native_Libraries.properties ./bin

eclipse:
	@$(ECHO) "installing project..."
	@$(RM) -rf $(ECLIPSE)/src
	@$(CP) -r src $(ECLIPSE)/
	@$(CP) -r ../libwscore/src/* $(ECLIPSE)/src
	@$(CP) -r ../libwshardware/src/frc_crio_wpilib/* $(ECLIPSE)/src
	@$(CP) -r ../libwshardware/src/common/* $(ECLIPSE)/src
	@$(CP) ../libwsnavx/src/ws_navx.* $(ECLIPSE)/src
	@$(CP) -r ../libwsnavx/src/ahrs/* $(ECLIPSE)/src
	@$(CP) -r ../libwsnavx/src/ahrs_include/* $(ECLIPSE)/src
	@$(CP) -r ../libwsutil/src/* $(ECLIPSE)/src
