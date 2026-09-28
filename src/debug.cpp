#include <Supergoon/lua.h>
#include <imgui.h>
#include <sgtools/log.h>

#include <DebugWindow.hpp>
#include <debug.hpp>

namespace {
int gameConfigFunc = 0;
int loadSceneFunc = 0;
const int maxScenes = 24;
void debugSceneTab() {
	if (ImGui::CollapsingHeader("Scene")) {
		// We need to get a list of all the scenes from LUA, and then put them here to switch scenes
		// Get gameconfig table on stack
		LuaPushRefValueInLuaRegistry(luaGlobalState, gameConfigFunc);
		RunLuaFunctionOnStack(luaGlobalState, 0, 1);
		// Get scene table on top of stack
		LuaPushTableFromStackTip(luaGlobalState, "Scenes");
		auto numItems = 0;
		const char* sceneNames[maxScenes];
		LuaStartTableKeyValueIteration(luaGlobalState);
		while (LuaNextTableKeyValueIterate(luaGlobalState)) {
			sceneNames[numItems++] = LuaGetStringi(luaGlobalState, -2);
			LuaPopStack(luaGlobalState, 1);
		}
		LuaEndTableKeyValueIteration(luaGlobalState);
		static int currentSelection = 0;
		ImGui::ListBox("Scenes", &currentSelection, sceneNames, numItems);
		if (ImGui::Button("Load Scene")) {
			LuaPushRefValueInLuaRegistry(luaGlobalState, loadSceneFunc);
			LuaPushString(luaGlobalState, sceneNames[currentSelection]);
			RunLuaFunctionOnStack(luaGlobalState, 1, 0);
		}
	}
}

}  // namespace

void InitializeDebugUI() {
	gameConfigFunc = LuaRefFileFromBuffer(luaGlobalState, "gameconfig.lua");
	loadSceneFunc = LuaRefFuncFromFile(luaGlobalState, "game.lua", "LoadScene");
	// We also need to ref load scene.
	DebugWindowAddTabFuncToMainDebugWindow(debugSceneTab);
}
