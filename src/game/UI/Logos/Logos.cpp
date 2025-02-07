#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/Events.hpp>
#include <Supergoon/UI/UI.hpp>
#include <Supergoon/UI/UIImage.hpp>
#include <UI/Logos/Logos.hpp>
#include <Utilities/Utilities.hpp>
using namespace std;

namespace Supergoon {
void CreateLogoUI(string levelToSwitchTo) {
  UI::LoadUIFromFile("logos");
  auto ui = UI::UIInstance.get();
  auto thing = (UIImage *)ui->GetChildByName("logoImage");
  auto thing2 = (UIImage *)ui->GetChildByName("logoImage2");
  assert(thing && thing2 && "Thing is null somehow");
  auto animator = new UIObjectAnimatorBase("logo");
  auto animator2 = new UIObjectAnimatorBase("logo2");
  auto fadeInTween = new Tween(0, 255, 3.0, thing->AlphaHandle(), Supergoon::Easings::Linear);
  auto fadeOutTween = new Tween(255, 0, 3.0, thing->AlphaHandle(), Supergoon::Easings::Linear);
  auto fadeInTween2 = new Tween(0, 255, 3.0, thing2->AlphaHandle(), Supergoon::Easings::Linear);
  auto fadeOutTween2 = new Tween(255, 0, 3.0, thing2->AlphaHandle(), Supergoon::Easings::Linear);
  fadeOutTween->EndFunc = [animator2]() {
    animator2->Play();
  };
  fadeOutTween2->EndFunc = [levelToSwitchTo]() {
    CreateUITextbox("screen", {145, 15}, {200, 48}, true);
    // InitializeTextInteractionUI();
    Events::PushEvent(Events::BuiltinEvents.LevelChangeEvent, 0, (void *)strdup(levelToSwitchTo.c_str()));
    Events::PushEvent(Events::BuiltinEvents.UiDestroyObject, 0, (void *)"logoImage");
    Events::PushEvent(Events::BuiltinEvents.UiDestroyObject, 0, (void *)"logoImage2");
  };
  animator->AddUIObjectTween(fadeInTween, thing);
  animator->AddUIObjectTween(fadeOutTween, thing);
  animator2->AddUIObjectTween(fadeInTween2, thing2);
  animator2->AddUIObjectTween(fadeOutTween2, thing2);
  thing->Animators.push_back(std::shared_ptr<UIObjectAnimatorBase>(animator));
  thing2->Animators.push_back(std::shared_ptr<UIObjectAnimatorBase>(animator2));
  animator->Play();
  ContentRegistry::LoadAllContent();
}
} // namespace Supergoon
