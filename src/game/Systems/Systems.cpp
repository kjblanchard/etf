#include <Systems/AsepriteSystem.hpp>
#include <Systems/Battle/BattleAbilitySystem.hpp>
#include <Systems/Battle/BattleLocationSystem.hpp>
#include <Systems/Battle/BattleSystem.hpp>
#include <Systems/Battle/BattleTurnMarkerSystem.hpp>
#include <Systems/Battle/BattleUISystem.hpp>
#include <Systems/Battle/BattleZoneSystem.hpp>
#include <Systems/Battle/EnterBattleSystem.hpp>
#include <Systems/CameraSystem.hpp>
#include <Systems/DebugDrawSystem.hpp>
#include <Systems/GameStateSystem.hpp>
#include <Systems/ImageSystem.hpp>
#include <Systems/PlayerSystem.hpp>
#include <Systems/Systems.hpp>
#include <Systems/TextInteractionSystem.hpp>
#ifdef imgui
#include <Debug/BattleWidget.hpp>
#include <Debug/PlayerCollider.hpp>
#include <Supergoon/Widgets/Global.hpp>
#endif

namespace Supergoon {

void InitializeSystems() {
  InitializeTextInteractionUI();
  InitializeBattleSystem();
  InitializeEnterBattleSystem();
}

void StartSystems() {
  LoadPlayers();
  LoadBattlers();
  LoadBattleAbilities();
  LoadBattleTurnMarkerSystem();
  LoadAnimationComponents();
  LoadTextInteractions();
  StartPlayers();
  StartBattlers();
}

void UpdateSystems() {
  UpdateGameState();
  PlayerInput();
  UpdateBattleZones();
  UpdateAnimationComponents();
  UpdateTextInteractions();
  UpdateEnterBattleSystem();
  UpdateBattle();
  UpdateCamera();
}

void DrawSystems() {
  DrawAnimationComponents();
  DrawImages();
  DrawTextInteractionDisplay();
}

void DrawSystemsDebugWidgets() {
#ifdef imgui
  if (GlobalWidget::ShowDebugBoxSolid) {
    DrawDebugBoxesSolid();
  }
  PlayerWidget::ShowPlayerColliderWindow();
  BattleWidget::ShowBattleWidget();
  if (PlayerWidget::ShowPlayerColliderDebugBox) {
    DrawDebugBoxesPlayer();
  }
  if (PlayerWidget::ShowPlayerExitDebugBox) {
    DrawDebugBoxesPlayerExit();
  }
  if (PlayerWidget::ShowPlayerInteractionDebugBox) {
    DrawDebugBoxesPlayerInteractionBox();
  }
  if (PlayerWidget::ShowInteractionDebugBox) {
    DrawDebugBoxesTextInteractionBox();
  }
#endif
}

} // namespace Supergoon
