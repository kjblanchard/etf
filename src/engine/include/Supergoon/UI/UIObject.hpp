#pragma once
#include <Supergoon/Primitives/Rectangle.hpp>
namespace Supergoon {
class UIObject {
   public:
	bool Enabled;
	bool Visible;
	bool Dirty;
	UIObject* Parent;
	RectangleF Bounds;
	virtual void OnDirty() {}
	virtual void Update() {}
	virtual void Draw() {}
};

}  // namespace Supergoon
