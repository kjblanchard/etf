#include <Supergoon/Widgets/Content.hpp>
#include <Supergoon/Widgets/Game.hpp>
#include <Supergoon/Widgets/Sound.hpp>
#include <Supergoon/Widgets/Widgets.hpp>
namespace Supergoon {
void Widgets::ShowWidgets() {
	ContentWidget::ShowContentDebugWindow();
	SoundWidgets::ShowSoundDebugWindow();
	GameWidget::ShowGameDebugWindow();
}

}  // namespace Supergoon
