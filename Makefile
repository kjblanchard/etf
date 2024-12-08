.PHONY: all configure build clean debug release
BUILD_DIR = build
BINARY_FOLDER = bin
EXECUTABLE_NAME = EscapeTheFate
BINARY_FOLDER_REL_PATH = $(BUILD_DIR)/$(BINARY_FOLDER)
DEFAULT_GENERATOR ?= "Ninja"
BACKUP_GENERATOR ?= "Unix Makefiles"
WINDOWS_GENERATOR ?= "Visual Studio 17 2022"
APPLE_GENERATOR ?= Xcode
CONFIGURE_COMMAND ?= "cmake"
EMSCRIPTEN_CONFIGURE_COMMAND = "emcmake cmake"
BUILD_TYPE ?= Debug
DEFAULT_IMGUI ?= ON
SYSTEM_PACKAGES ?= ON
BUILD_COMMAND ?= cmake --build $(BUILD_DIR) --config $(BUILD_TYPE)
UNIX_PACKAGE_COMMAND ?= tar --exclude='*.aseprite' -czvf $(BUILD_DIR)/$(EXECUTABLE_NAME).tgz -C $(BINARY_FOLDER_REL_PATH) .
WINDOWS_PACKAGE_COMMAND ?= "7z a -r $(BUILD_DIR)/$(EXECUTABLE_NAME).zip $(BINARY_FOLDER_REL_PATH)"
PACKAGE_COMMAND ?= $(UNIX_PACKAGE_COMMAND)
ADDITIONAL_OPTIONS ?=
# default, should be used after a rebuild of some sort.
all: build run
clean:
	@rm -rf $(BUILD_DIR)
configure:
	$(CONFIGURE_COMMAND) -G "$(CMAKE_GENERATOR)" . -B $(BUILD_DIR) -Dimgui=$(DEFAULT_IMGUI) -DSYSTEM_PACKAGES=$(SYSTEM_PACKAGES) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) $(ADDITIONAL_OPTIONS)
build:
	@$(BUILD_COMMAND)
install:
	@cmake --install $(BUILD_DIR) --config $(BUILD_TYPE)
run:
	@open ./build/bin/$(EXECUTABLE_NAME).app || ./build/bin/$(EXECUTABLE_NAME)
package:
	$(PACKAGE_COMMAND)

# Custom build commands that set variables accordingly based on platform.. rebuild is macos, brebuild is backup, wrebuild is windows, erebuild is emscripten, irebuild is ios simulator
rebuild:
	@$(MAKE) CMAKE_GENERATOR=$(DEFAULT_GENERATOR) clean configure build install
xrebuild:
	@$(MAKE) CMAKE_GENERATOR=$(APPLE_GENERATOR) SYSTEM_PACKAGES=OFF clean configure build install package
brebuild:
	@$(MAKE) CMAKE_GENERATOR=$(BACKUP_GENERATOR) SYSTEM_PACKAGES=OFF clean configure build install package
wrebuild:
	$(MAKE) CMAKE_GENERATOR=$(WINDOWS_GENERATOR) PACKAGE_COMMAND=$(WINDOWS_PACKAGE_COMMAND) SYSTEM_PACKAGES=OFF configure build install package
erebuild:
	@$(MAKE) CMAKE_GENERATOR=$(BACKUP_GENERATOR) CONFIGURE_COMMAND=$(EMSCRIPTEN_CONFIGURE_COMMAND) BUILD_COMMAND='sudo $(BUILD_COMMAND)' SYSTEM_PACKAGES=OFF clean configure build
irebuild:
	$(MAKE) CMAKE_GENERATOR=$(APPLE_GENERATOR) DEFAULT_IMGUI=OFF ADDITIONAL_OPTIONS="-DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_SYSROOT=iphonesimulator -DCMAKE_OSX_ARCHITECTURES=x86_64 -DCMAKE_OSX_DEPLOYMENT_TARGET=12.0" clean configure build install package
# Custom run commands
erun:
	emrun ./build/bin/$(EXECUTABLE_NAME).html
irun:
	xcrun simctl install 083ACF27-C316-43AA-AC2D-2DF7F9C50DC1 build/bin/Debug/EscapeTheFate.app
idevices:
	xcrun simctl list devices
#Helpers
bloaty:
	dsymutil ./$(EXECUTABLE_NAME) -o SupergoonClient.dSYM
	bloaty -d compileunits SupergoonClient --debug-file SupergoonClient.dSYM/Contents/Resources/DWARF/SupergoonClient
valgrind:
	valgrind --track-origins=yes --leak-check=yes --leak-resolution=low --show-leak-kinds=definite ./build/bin/$(EXECUTABLE_NAME) 2>&1 | tee memcheck.txt

