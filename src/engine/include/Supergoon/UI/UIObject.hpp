#pragma once
#include <Supergoon/Primitives/Rectangle.hpp>
#include <Supergoon/Primitives/Vector2.hpp>
#include <Supergoon/UI/UIObjectAnimator.hpp>
namespace Supergoon {
class UIWidget;
enum class BuiltinWidgetTypes {
	Image = 1,
	Text,
	Panel,
};
class UIObject {
   public:
	inline UIObject() = default;
	inline UIObject(UIObject* parent, bool enabled = true, bool visible = true) : Enabled(enabled), Parent(parent), _visible(visible) {}
	virtual ~UIObject() = default;
	bool Enabled = true;
	bool Dirty = true;
	inline void SetVisible(bool visible) {
		_visible = visible;
		Dirty = true;
	}
	inline void SetAlpha(int alpha) {
		alpha < 0 ? _alpha = 0 : alpha > 255 ? _alpha = 255
											 : _alpha = alpha;
		Dirty = true;
	}
	inline unsigned int Layer() { return _layer; }
	inline void SetLayer(unsigned int layer) {
		_layer = layer;
		if (Parent) {
			Parent->Dirty = true;
		}
		Dirty = true;
	}
	// TODO Used for tweening alpha.. probably isn't great
	inline int* AlphaHandle() { return &_alpha; }
	UIObject* Parent = nullptr;
	RectangleF Bounds = {0, 0, 0, 0};
	Vector2 Offset = {0, 0};
	// Get the effective alpha for this object, taking the parents into consideration.
	inline int EffectiveAlpha() { return Parent ? (int)((Parent->EffectiveAlpha() / 255.0) * (_alpha / 255.0) * 255) : _alpha; }
	inline bool Visible() { return Parent ? Parent->Visible() ? _visible : false : _visible; }
	int WidgetType = 0;
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
		if (!Visible()) {
			return;
		}
		Draw();
	}

   protected:
	int _alpha = 255;
	bool _visible = true;
	int _layer = 0;
	virtual void Update() {}
	virtual void Draw() {}
	friend UIWidget;
};

}  // namespace Supergoon
