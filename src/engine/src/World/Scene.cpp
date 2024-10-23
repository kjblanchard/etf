#include <Supergoon/UI/UI.hpp>
#include <Supergoon/Widgets/Widgets.hpp>
#include <Supergoon/World/Scene.hpp>
using namespace Supergoon;
Scene* Scene::Instance = nullptr;
Scene::Scene(Level* level, Panel* panel) : CurrentLevel(std::unique_ptr<Level>(level)), SceneUI(std::unique_ptr<Panel>(panel)) {
	Instance = this;
}
void Scene::Start() {
	if (CurrentLevel) {
		// CurrentLevel->CreateBackgroundImage();
		// LoadPlayers();
		// LoadAnimationComponents();
	}
}
void Scene::Update() {
	if (CurrentLevel) {
		// PlayerInput();
		// UpdateAnimationComponents();
		// UpdateCamera();
	}
	if (SceneUI) {
		SceneUI->Update();
	}
	for (auto&& animator : Animators) {
		animator->Update();
	}
}
void Scene::Draw() {
	if (CurrentLevel) {
		// level->Draw();
		// DrawAnimationComponents();
		// DrawImages();
#ifdef imgui
		// DrawDebugBoxes();
		// ShowPlayerColliderWindow();
#endif
	}
	if (SceneUI) {
		SceneUI->Draw();
	}
#ifdef imgui
	Widgets::ShowWidgets();
#endif
}
