BUILD_DIR = build
EXECUTABLE_NAME = SupergoonClient
.PHONY: all configure build clean debug release
all: build run
configure:
	cmake . -B $(BUILD_DIR)
build:
	cmake --build $(BUILD_DIR)
clean:
	rm -rf $(BUILD_DIR)
run:
	./build/src/client/$(EXECUTABLE_NAME)

	# Generate dsym for bloaty
	# dsymutil ./SupergoonClient -o SupergoonClient.dSYM
	# bloaty -d compileunits SupergoonClient --debug-file SupergoonClient.dSYM/Contents/Resources/DWARF/SupergoonClient