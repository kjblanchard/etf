#include <Supergoon/Graphics/graphics.h>
#include <Supergoon/Graphics/shader.h>
#include <Supergoon/engine.h>
#include <Supergoon/filesystem.h>
#include <Supergoon/lua.h>
#include <Supergoon/map.h>
#include <Supergoon/state.h>
#include <Supergoon/string.h>
#include <Supergoon/window.h>
#include <sgforge/directory.h>
#include <sgforge/unpack.h>
#include <sgtools/log.h>

static int updatefunc;
static int drawfunc;

static void initialize(void) {
	sgSetLogLevel(sgLogLevelWarn);
	const char* filePath = GetBasePath();
	String fullFile = StringConcat(filePath, "data/etf.sg");
	AssetDirectory = LoadDirectoryFromFile(fullFile.Data);
	LuaRunFileFromBuffer("init.lua");
	updatefunc = LuaRefFileFromBuffer(luaGlobalState, "update.lua");
	drawfunc = LuaRefFileFromBuffer(luaGlobalState, "draw.lua");
}

static void startEngine(void) {
	LuaRunFileFromBuffer("start.lua");
}

static void drawEngine(void) {
	LuaRunRefFunction(luaGlobalState, drawfunc);
}

static void updateGame(void) {
	// set lua deltatimes
	LuaPushTableFromGlobal(luaGlobalState, "package");
	LuaGetTable(luaGlobalState, "loaded");
	LuaGetTable(luaGlobalState, "gamestate");
	LuaPushFloatToTable(luaGlobalState, "DeltaTimeSeconds", DeltaTimeSeconds);
	LuaPushFloatToTable(luaGlobalState, "DeltaTimeMilliseconds", DeltaTimeMilliseconds);
	LuaClearStack(luaGlobalState);
	LuaRunRefFunction(luaGlobalState, updatefunc);
}

// wire our functions to the engine, as engine calls this func
void InitializeEngineFunctions(void) {
	SetInitializeFunction(initialize);
	SetUpdateFunction(updateGame);
	SetStartFunction(startEngine);
	SetDrawFunction(drawEngine);
}
