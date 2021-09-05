##
##  WSCORE Robot framework code
##

LIB = libwscore

##
##  must set WPILIB_PATH in your environment
##  must set TARGET in your environment
##
##  Options for TARGET:
##    simulation
##    frc_crio_wpilib
##

FILES = io/OutputManager.cpp \
        io/routing/InputListener.cpp \
        io/routing/InputSlot.cpp \
        io/routing/OutputSource.cpp \
        io/routing/InputListenerValue.cpp \
        io/outputs/OutputTemplate.cpp \
        io/outputs/NullOutputs.cpp \
        io/outputs/OutputStubs.cpp \
        io/outputs/WsSmartOutput.cpp \
        io/outputs/Analogifier.cpp \
        io/outputs/SmartOutputPidIO.cpp \
        io/InputManager.cpp \
        io/Output.cpp \
        io/Input.cpp \
        io/InputOutputFactory.cpp \
        io/inputs/NullInputs.cpp \
        io/inputs/InputStubs.cpp \
        io/inputs/InputTemplate.cpp \
		io/inputs/ModButtonInput.cpp \
        io/inputs/LogicButton.cpp \
        io/inserts/WsIOInsert.cpp \
        io/inserts/DebounceInsert.cpp \
        io/inserts/ToggleInsert.cpp \
        io/inserts/DeadzoneInsert.cpp \
        io/inserts/SafetyInsert.cpp \
        io/inserts/InvertInsert.cpp \
        subsystems/SubsystemManager.cpp \
        subsystems/Subsystem.cpp \
        auto/AutoProgram.cpp \
        auto/AutoManager.cpp \
        auto/steps/control/AutoStepStopAutonomous.cpp \
        auto/steps/control/AutoStepDelay.cpp \
        auto/steps/control/AutoStepChangeProgram.cpp \
        auto/steps/AutoStep.cpp \
        auto/steps/AutoStepGroup.cpp \
        auto/steps/groups/AutoParallelShortStepGroup.cpp \
        auto/steps/groups/AutoParallelStepGroup.cpp \
        auto/steps/groups/AutoSerialStepGroup.cpp \
        auto/program/SleeperProgram.cpp \
        auto/script/AutoStepFactory.cpp \
        auto/script/AutoStepFactoryArgs.cpp \
        auto/script/StepFactory.cpp \
        core/RobotCore.cpp \
        core/WildstangModule.cpp \
        core/HardwareHandle.cpp \
        pid/PidOutput.cpp \
        pid/PidController.cpp \
        pid/PidInput.cpp \
        logger/WsSmartDashboard.cpp \
        logger/Probe.cpp \
        config/ConfigValue.cpp \
        config/ConfigConsole.cpp \
        config/ConfigValueRef.cpp \
        config/ConfigValueLocking.cpp \
        vision/WsMJPEGStreamer.cpp \
        vision/SocketWorker.cpp \
        vision/WsVision.cpp \
        utils/ProfilingTimer.cpp \

CXXFLAGS = -O2 -Wall --std=c++11 -DNAMESPACED_WPILIB -g -rdynamic
CROSS = dummy-
CXX = g++
CC = gcc
AR = ar
ECHO = /bin/echo
RM = rm
TR = tr
MKDIR = mkdir
SED = sed

INCLUDES = -I../libwsutil/src \
           -Isrc

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
	@$(ECHO) -e "\x1b[33m[CC]\x1b[0m \x1b[35m[libwscore]\x1b[0m $@"
	@$(MKDIR) -p $(@D)
	@$(CROSS)$(CXX) $(INCLUDES) $(CXXFLAGS) -c $^ -o $@

clean:
	@$(RM) -rf obj
	@$(RM) -rf lib
	@$(RM) -f ./.depend
