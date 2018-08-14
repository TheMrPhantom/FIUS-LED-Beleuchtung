CORE_DEBUG_LEVEL := 2
C_FLAGS := -std=gnu11
CPP_FLAGS := -std=gnu++17 -Os
USER_FLAGS := -Wall
NPROCS := 4
BUILD := /tmp/arduino-esp32

include makeConfig.mk

SKETCH := src/Main.ino
USER_DIRS := src arduino-esp32/libraries/WiFi/src libraries/FastLED

################################################################################

LIBS := arduino-esp32/tools/sdk/lib
CORE_DIRS = arduino-esp32/variants/esp32 arduino-esp32/cores/esp32 $(wildcard arduino-esp32/tools/sdk/include/*/)
INCLUDES :=$(USER_DIRS) $(CORE_DIRS)
USER_INPUTS := $(wildcard $(addsuffix /*.c, $(USER_DIRS)) $(addsuffix /*.cpp, $(USER_DIRS))) $(SKETCH)
CORE_INPUTS := $(wildcard $(addsuffix /*.c, $(CORE_DIRS)) $(addsuffix /*.cpp, $(CORE_DIRS)))
USER_OUTPUTS := $(patsubst %, $(BUILD)/%.o, $(USER_INPUTS))
CORE_OUTPUTS := $(patsubst %, $(BUILD)/%.o, $(CORE_INPUTS))
C_COM := arduino-esp32/tools/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc
CPP_COM := arduino-esp32/tools/xtensa-esp32-elf/bin/xtensa-esp32-elf-g++
AR_COM := arduino-esp32/tools/xtensa-esp32-elf/bin/xtensa-esp32-elf-ar
FLAGS := \
	$(patsubst %, -I%, $(INCLUDES)) \
	-MMD \
	-DESP_PLATFORM \
	-DMBEDTLS_CONFIG_FILE=\"mbedtls/esp_config.h\" \
	-DHAVE_CONFIG_H \
	-DF_CPU=240000000L \
	-DARDUINO=10605 \
	-DARDUINO_ESP32_DEV \
	-DARDUINO_ARCH_ESP32 \
	-DARDUINO_BOARD=\"ESP32_DEV\" \
	-DARDUINO_VARIANT=\"esp32\" \
	-DESP32 \
	-mlongcalls \
	-DCORE_DEBUG_LEVEL=$(CORE_DEBUG_LEVEL)
FLASH_FLAGS = \
	--chip esp32 \
	--port $(UPLOAD_PORT) \
	--baud 921600 \
	--before default_reset \
	--after hard_reset write_flash \
	-z \
	--flash_mode dio \
	--flash_freq 40m \
	--flash_size detect

all: $(BUILD)/Main.bin
	
clean:
	rm -rf $(BUILD)

bootloader:
	[ -r $(UPLOAD_PORT) ] && [ -w $(UPLOAD_PORT) ] || exit 1
	@chmod +x arduino-esp32/tools/esptool/esptool.py
	@arduino-esp32/tools/esptool/esptool.py $(FLASH_FLAGS) \
		0xe000 arduino-esp32/tools/partitions/boot_app0.bin \
		0x1000 arduino-esp32/tools/sdk/bin/bootloader_dio_40m.bin \
		0x8000 arduino-esp32/tools/partitions/default.bin

flash:
	[ -r $(UPLOAD_PORT) ] && [ -w $(UPLOAD_PORT) ] || exit 1
	@chmod +x arduino-esp32/tools/esptool/esptool.py
	@arduino-esp32/tools/esptool/esptool.py $(FLASH_FLAGS) 0x10000 $(BUILD)/Main.bin

listen:
	[ -r $(UPLOAD_PORT) ] || exit 1
	stty -F /dev/ttyS3 115200
	cat /dev/ttyS3

$(BUILD)/%.c.o: %.c
	@echo $@
	@mkdir -p $(dir $@)
	@$(C_COM) $(FLAGS) $(C_FLAGS) $(USER_FLAGS) -c $^ -o $@

$(BUILD)/%.cpp.o: %.cpp
	@echo $@
	@mkdir -p $(dir $@)
	@$(CPP_COM) $(FLAGS) $(CPP_FLAGS) $(USER_FLAGS) -c $^ -o $@

$(BUILD)/%.ino.o: %.ino
	@echo $@
	@mkdir -p $(dir $@)
	@$(CPP_COM) $(FLAGS) $(CPP_FLAGS) $(USER_FLAGS) -c -x c++ $^ -o $@

$(BUILD)/arduino-esp32/%.c.o: arduino-esp32/%.c
	@echo $@
	@mkdir -p $(dir $@)
	@$(C_COM) $(FLAGS) $(C_FLAGS) -c $^ -o $@

$(BUILD)/arduino-esp32/%.cpp.o: arduino-esp32/%.cpp
	@echo $@
	@mkdir -p $(dir $@)
	@$(CPP_COM) $(FLAGS) $(CPP_FLAGS) -c $^ -o $@

$(BUILD)/arduino.ar: $(CORE_OUTPUTS)
	@echo $@
	@$(AR_COM) cru $@ $^

$(BUILD)/Main.elf: $(USER_OUTPUTS) $(BUILD)/arduino.ar
	@echo $@
	@$(CPP_COM) \
		-nostdlib \
		$(patsubst %, -L%, $(LIBS)) -Larduino-esp32/tools/sdk/ld \
		-T esp32_out.ld -T esp32.common.ld -T esp32.rom.ld -T esp32.peripherals.ld -T esp32.rom.spiram_incompatible_fns.ld \
		-u ld_include_panic_highint_hdl -u call_user_start_cpu0 -u __cxa_guard_dummy -u __cxx_fatal_exception \
		-Wl,--gc-sections -Wl,-static -Wl,--undefined=uxTopUsedPriority -Wl,--start-group \
		$^ -o $@ \
		$(patsubst lib%.a,-l%, $(notdir $(wildcard $(addsuffix /*.a, $(LIBS))))) -lstdc++ -lgcc \
		-Wl,--end-group -Wl,-EL

$(BUILD)/Main.bin: $(BUILD)/Main.elf
	@echo $@
	@chmod +x arduino-esp32/tools/esptool/esptool.py
	@python arduino-esp32/tools/esptool/esptool.py --chip esp32 elf2image --flash_mode dio --flash_freq 40m --flash_size 4MB -o $@ $<
