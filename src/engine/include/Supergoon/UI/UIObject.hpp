#pragma once
#include <Supergoon/Primitives/Rectangle.hpp>
#include <Supergoon/Primitives/Vector2.hpp>
#include <Supergoon/UI/UIObjectAnimator.hpp>
namespace Supergoon {
enum class BuiltinWidgetTypes {
	Image = 1,
	Text,
};
class UIObject {
   public:
	inline UIObject() = default;
	inline UIObject(UIObject* parent, bool enabled = true, bool visible = true) : Enabled(enabled), Visible(visible), Parent(parent) {}
	virtual ~UIObject() = default;
	bool Enabled = true;
	bool Visible = true;
	bool Dirty = true;
	UIObject* Parent;
	RectangleF Bounds;
	Vector2 Offset;
	int WidgetType;
	std::vector<std::shared_ptr<UIObjectAnimatorBase>> Animators;
	inline virtual void OnDirty() {
		Bounds.X = Offset.X + Parent->Bounds.X;
		Bounds.Y = Offset.Y + Parent->Bounds.Y;
	}
	inline void UpdateInternal() {
		if (!Enabled) {
			return;
		}
		if (Dirty) {
			OnDirty();
			Dirty = false;
		}
		Update();
		for (auto&& animator : Animators) {
			animator->Update();
		}
	}
	inline void DrawInternal() {
		if (!Visible) {
			return;
		}
		Draw();
	}

   protected:
	virtual void Update() {}
	virtual void Draw() {}
};

}  // namespace Supergoon
