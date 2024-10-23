#pragma once
#include <Supergoon/UI/Panel.hpp>
#include <Supergoon/UI/UIObjectAnimator.hpp>
#include <Supergoon/World/Level.hpp>
#include <memory>
#include <vector>
namespace Supergoon {
class Scene {
   public:
	Scene(Level* level, Panel* panel);
	void Start();
	void Update();
	void Draw();
	std::unique_ptr<Level> CurrentLevel = nullptr;
	std::unique_ptr<Panel> SceneUI = nullptr;
	std::vector<std::shared_ptr<UIObjectAnimatorBase>> Animators;
	static Scene* Instance;
};
}  // namespace Supergoon
