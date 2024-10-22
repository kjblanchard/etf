#pragma once
#include <Supergoon/Primitives/Rectangle.hpp>
#include <Supergoon/Primitives/Vector2.hpp>
namespace Supergoon {
enum class BuiltinWidgetTypes {
	Image = 1,
};
class UIObject {
   public:
	inline UIObject() = default;
	inline UIObject(UIObject* parent, bool enabled = true, bool visible = true) : Enabled(enabled), Visible(visible), Parent(parent) {}
	bool Enabled;
	bool Visible;
	bool Dirty = true;
	UIObject* Parent;
	RectangleF Bounds;
	Vector2 Offset;
	int WidgetType;
	inline virtual void OnDirty() {
		Bounds.X = Offset.X + Parent->Bounds.X;
		Bounds.Y = Offset.Y + Parent->Bounds.Y;
	}
	inline void UpdateInternal() {
		if (Dirty) {
			OnDirty();
			Dirty = false;
		}
		Update();
	}
	virtual void Draw() {}

   protected:
	virtual void Update() {}
};

}  // namespace Supergoon
