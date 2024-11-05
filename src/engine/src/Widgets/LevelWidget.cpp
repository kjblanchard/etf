#include <SDL3/SDL.h>
#include <SupergoonEngine/imgui/imgui.h>

#include <Supergoon/ECS/Components/GameStateComponent.hpp>
#include <Supergoon/Events.hpp>
#include <Supergoon/Widgets/LevelWidget.hpp>
#include <Supergoon/Widgets/Widgets.hpp>
#include <Supergoon/World/Level.hpp>
#include <regex>
#include <string>
#include <vector>
using namespace Supergoon;
std::vector<std::string> levelNames;
std::vector<const char*> levelNamesC;
static std::string levelNameStrip;
static bool inited = false;
static void GetLevels() {
	auto path = std::string(SDL_GetBasePath()) + "assets/tiled/";
	const char* pattern = "*.tmj";
	int count = 0;
	levelNames.clear();
	char** levelFiles = SDL_GlobDirectory(path.c_str(), pattern, 0, &count);
	for (size_t i = 0; i < (size_t)count; i++) {
		levelNames.push_back(std::string(levelFiles[i]));
	}
	for (const std::string& name : levelNames) {
		levelNamesC.push_back(name.c_str());
	}
}
void LevelWidget::ShowLevelWidget() {
	if (!inited) {
		GetLevels();
		inited = true;
	}
	auto window_flags = Widgets::GetDefaultWindowFlags();
	bool p_open;
	if (!ImGui::Begin("Level", &p_open, window_flags)) {
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}
	if (Level::_currentLevel) {
		auto gamestate = GameObject::FindComponent<GameState>();
		if (gamestate) {
			ImGui::Checkbox("Camera Following Target", &gamestate->CameraFollowTarget);
		}
		if (ImGui::DragInt("CameraX", &Level::_currentLevel->cameraX)) {
		}
		if (ImGui::DragInt("CameraY", &Level::_currentLevel->cameraY)) {
		}
	}

	static int item_current = 0;
	ImGui::ListBox("LevelLoader", &item_current, levelNamesC.data(), levelNamesC.size(), 4);
	if (ImGui::Button("LoadLevel")) {
		auto levelFull = std::string(levelNamesC[item_current]);
		std::regex dotRegex("\\.tmj");
		std::vector<std::string> result(std::sregex_token_iterator(levelFull.begin(), levelFull.end(), dotRegex, -1), std::sregex_token_iterator());
		levelNameStrip = result[0];
		// TODO is this a leak since I strdup?
		Events::PushEvent(Events::BuiltinEvents.LevelChangeEvent, 0, (void*)strdup(levelNameStrip.c_str()));
	}
	ImGui::End();
}
