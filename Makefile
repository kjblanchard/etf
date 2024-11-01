.PHONY: all configure build clean debug release
BUILD_DIR = build
EXECUTABLE_NAME = EscapeTheFate
DEFAULT_GENERATOR ?= "Ninja"
BACKUP_GENERATOR ?= "Unix Makefiles"
WINDOWS_GENERATOR ?= "Visual Studio 17 2022"
APPLE_GENERATOR ?= Xcode
CONFIGURE_COMMAND ?= "cmake"
EMSCRIPTEN_CONFIGURE_COMMAND = "emcmake cmake"
BUILD_TYPE ?= Debug
DEFAULT_IMGUI ?= ON
SYSTEM_PACKAGES ?= ON
# default, should be used after a rebuild of some sort.
all: build run
clean:
	@rm -rf $(BUILD_DIR)
configure:
	@$(CONFIGURE_COMMAND) -G "$(CMAKE_GENERATOR)" . -B $(BUILD_DIR) -Dimgui=$(DEFAULT_IMGUI) -DSYSTEM_PACKAGES=$(SYSTEM_PACKAGES) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
build:
	@cmake --build $(BUILD_DIR) --config $(BUILD_TYPE)
install:
	@cmake --install $(BUILD_DIR) --config $(BUILD_TYPE)
valgrind:
	@valgrind --track-origins=yes --leak-check=yes --leak-resolution=low --show-leak-kinds=definite ./build/bin/$(EXECUTABLE_NAME) 2>&1 | tee memcheck.txt
run:
	@open ./build/bin/$(EXECUTABLE_NAME).app || ./build/bin/$(EXECUTABLE_NAME)


# Generate dsym for bloaty

# Custom build commands that set variables accordingly based on platform.. rebuild is macos, brebuild is backup, wrebuild is windows, erebuild is emscripten
rebuild:
	@$(MAKE) CMAKE_GENERATOR=$(DEFAULT_GENERATOR) clean configure build install
xrebuild:
	@$(MAKE) CMAKE_GENERATOR=$(APPLE_GENERATOR) SYSTEM_PACKAGES=OFF clean configure build install
brebuild:
	@$(MAKE) CMAKE_GENERATOR=$(BACKUP_GENERATOR) clean configure build install
wrebuild:
	@$(MAKE) CMAKE_GENERATOR=$(WINDOWS_GENERATOR) SYSTEM_PACKAGES=OFF configure build install
erebuild:
	@$(MAKE) CMAKE_GENERATOR=$(BACKUP_GENERATOR) CONFIGURE_COMMAND=$(EMSCRIPTEN_CONFIGURE_COMMAND) SYSTEM_PACKAGES=OFF clean configure build
# Custom run commands
erun:
	emrun ./build/bin/$(EXECUTABLE_NAME).html
#Helpers
bloaty:
	dsymutil ./SupergoonClient -o SupergoonClient.dSYM
	bloaty -d compileunits SupergoonClient --debug-file SupergoonClient.dSYM/Contents/Resources/DWARF/SupergoonClient

