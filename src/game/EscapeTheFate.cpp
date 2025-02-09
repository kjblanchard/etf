#include <Components/BattleComponent.hpp>
#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/ECS/Components/GameStateComponent.hpp>
#include <Supergoon/ECS/Gameobject.hpp>
#include <Supergoon/Events.hpp>
#include <Supergoon/Game.hpp>
#include <Supergoon/Sound.hpp>
#include <Supergoon/UI/UI.hpp>
#include <Supergoon/World/Level.hpp>
#include <SupergoonEngine/nlohmann/json.hpp>
#include <Systems/Systems.hpp>
#include <UI/Logos/Logos.hpp>
#include <Utilities/Events.hpp>
#include <Utilities/Utilities.hpp>
#ifdef imgui
#include <Supergoon/Widgets/Widgets.hpp>
#endif

using json = nlohmann::json;
using namespace std;
extern json configData;
using namespace Supergoon;
static bool inGame = false;
static bool initialized = false;

static void initializeEscapeTheFateSystems() {
  InitializeEvents();
  InitializeSystems();
}

static void loadLevel() {
  if (!initialized) {
    initializeEscapeTheFateSystems();
    initialized = true;
  }
  CreateUITextbox("screen", {145, 15}, {200, 48}, true);
  // TODO this shouldn't be here, waste of a call Maybe this is needed after we create the uitextbox?
  ContentRegistry::LoadAllContent();
  auto gamestateGameObject = GameObject::GetGameObjectWithComponents<GameState>();
  assert(gamestateGameObject);
  StartSystems();
  ContentRegistry::LoadAllContent();
  inGame = true;
}

class EscapeTheFateGame : public Game {
public:
  ~EscapeTheFateGame() = default;
  void Start() override;
  void Update() override;
  void Draw() override;
  void Reset() override;
};

void EscapeTheFateGame::Start() {
  Level::LoadFunc = loadLevel;
  auto muted = configData["mute"];
  if (muted) {
    Sound::Instance()->SetGlobalBgmVolume(0);
    Sound::Instance()->SetGlobalSfxVolume(0);
  }
  bool skipLogos = configData["skipLogos"];
  string startingScreen = configData["startingScreen"];
  if (skipLogos) {
    Events::PushEvent(Events::BuiltinEvents.LevelChangeEvent, 0, (void *)strdup(startingScreen.c_str()));
  } else {
    CreateLogoUI(startingScreen);
  }
}

void EscapeTheFateGame::Update() {
  if (inGame) {
    UpdateSystems();
  }
  UI::Update();
}

void EscapeTheFateGame::Draw() {
  if (inGame) {
    Level::Draw();
    DrawSystems();
  }
  UI::Draw();
#ifdef imgui
  Widgets::ShowWidgets(this);
  DrawSystemsDebugWidgets();
#endif
}

void EscapeTheFateGame::Reset() {
  inGame = false;
  initialized = false;
  Level::Reset();
}

REGISTER_GAME(EscapeTheFateGame);
