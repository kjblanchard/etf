BUILD_DIR = build
EXECUTABLE_NAME = SupergoonBlackJack
.PHONY: all configure build clean debug release
DEFAULT_GENERATOR ?= "Ninja"
BACKUP_GENERATOR ?= "Unix Makefiles"
WINDOWS_GENERATOR ?= "Visual Studio 17 2022"
DEFAULT_IMGUI ?= OFF
SYSTEM_PACKAGES ?= ON
# default, should be used after a rebuild of some sort.
all: build run

rebuild:
	$(MAKE) CMAKE_GENERATOR=$(DEFAULT_GENERATOR) clean configure build install
brebuild:
	$(MAKE) CMAKE_GENERATOR=$(BACKUP_GENERATOR) clean configure build install
wrebuild:
	powershell -Command "Remove-Item -Recurse -Force 'build' -ErrorAction SilentlyContinue
	$(MAKE) CMAKE_GENERATOR=$(WINDOWS_GENERATOR) configure build install
rrebuild:
	$(MAKE) CMAKE_GENERATOR=$(DEFAULT_GENERATOR) DEFAULT_IMGUI=OFF clean configure build install
configure:
	cmake -G "$(CMAKE_GENERATOR)" . -B $(BUILD_DIR) -Dimgui=$(DEFAULT_IMGUI) -DSYSTEM_PACKAGES=$(SYSTEM_PACKAGES)
valgrind:
	valgrind --track-origins=yes --leak-check=yes --leak-resolution=low --show-leak-kinds=definite ./build/bin/$(EXECUTABLE_NAME) 2>&1 | tee memcheck.txt
build:
	cmake --build $(BUILD_DIR)
install:
	cmake --install $(BUILD_DIR)
clean:
	rm -rf $(BUILD_DIR)
run:
	./build/bin/$(EXECUTABLE_NAME)

# Generate dsym for bloaty
# dsymutil ./SupergoonClient -o SupergoonClient.dSYM
# bloaty -d compileunits SupergoonClient --debug-file SupergoonClient.dSYM/Contents/Resources/DWARF/SupergoonClient
