################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include arduino/core/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
AR += \
/Users/Toxa/Documents/Arduino/CIVIC_BOARD/Release/arduino.ar \

ELF += \
CIVIC_BOARD.elf \

EEP += \
CIVIC_BOARD.eep \

FLASH_IMAGE += \
CIVIC_BOARD.hex \


# All Target
all: CIVIC_BOARD

# Tool invocations
CIVIC_BOARD: $(FLASH_IMAGE)
	@echo 'Building target: $@'
	@echo 'Printing size:'
	"/Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin/avr-size" -A "/Users/Toxa/Documents/Arduino/CIVIC_BOARD/Release/CIVIC_BOARD.elf"
	@echo 'Finished building target: $@'
	@echo ' '

/Users/Toxa/Documents/Arduino/CIVIC_BOARD/Release/arduino.ar: $(AR_OBJ)
	@echo 'Starting archiver'
	"/Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin/avr-ar" rcs  "/Users/Toxa/Documents/Arduino/CIVIC_BOARD/Release/arduino.ar"   $(AR_OBJ)
	@echo 'Finished building: $@'
	@echo ' '

CIVIC_BOARD.elf: $(LINK_OBJ) $(AR)
	@echo 'Starting combiner'
	"/Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin/avr-gcc" -Os -Wl,--gc-sections -mmcu=atmega328p  -o "/Users/Toxa/Documents/Arduino/CIVIC_BOARD/Release/CIVIC_BOARD.elf"   $(LINK_OBJ) $(AR)  "/Users/Toxa/Documents/Arduino/CIVIC_BOARD/Release/arduino.ar" "-L/Users/Toxa/Documents/Arduino/CIVIC_BOARD/Release" -lm
	@echo 'Finished building: $@'
	@echo ' '

CIVIC_BOARD.eep: $(ELF)
	@echo 'Create eeprom image'
	"/Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin/avr-objcopy" -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0  "/Users/Toxa/Documents/Arduino/CIVIC_BOARD/Release/CIVIC_BOARD.elf" "/Users/Toxa/Documents/Arduino/CIVIC_BOARD/Release/CIVIC_BOARD.eep"
	@echo 'Finished building: $@'
	@echo ' '

CIVIC_BOARD.hex: $(EEP)
	@echo 'Create Flash image (ihex format)'
	"/Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin/avr-objcopy" -O ihex -R .eeprom  "/Users/Toxa/Documents/Arduino/CIVIC_BOARD/Release/CIVIC_BOARD.elf" "/Users/Toxa/Documents/Arduino/CIVIC_BOARD/Release/CIVIC_BOARD.hex"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C_UPPER_DEPS)$(C_DEPS)$(AR)$(EEP)$(FLASH_IMAGE)$(CC_DEPS)$(AR_OBJ)$(C++_DEPS)$(LINK_OBJ)$(CXX_DEPS)$(ASM_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(ELF)$(CPP_DEPS) CIVIC_BOARD
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
