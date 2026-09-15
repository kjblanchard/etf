local p = {}
local go = require("engine.gameobject")
-- local movespeed <const> = 100;
-- local collisionOffsetSizeRect <const> = { 8, 8, 16, 22 };
-- local interactionEastWestWidthHeight <const> = { 26, 8 };
-- local interactionNorthSouthWidthHeight <const> = { 8, 26 };

---Creates player and adds to gameobject list
---@param args any
function p.Create(args)
  local player = {}
  go.AddGameObject(player, "player")
  -- int loadLocation = -1;
  -- Direction direction = Direction::South;
  -- for (auto i = 0; i < objData->NumProperties; ++i) {
  -- auto prop = objData->Properties[i];
  -- if (prop.Name == string_view("loadLocation")) {
  -- loadLocation = prop.Data.IntData;
  -- } else if (prop.Name == string_view("direction")) {
  -- direction = static_cast<Direction>(prop.Data.IntData);
  -- }
  -- }
  -- if (loadLocation != GameState::NextLoadScreen) return;
  -- sgLogDebug("Making player start at pos %d!!", loadLocation);

  -- auto player1Controller = PlayerControllerSystem::GetPlayerByNum(0);
  -- auto p1 = new LocalPlayer(objData, player1Controller, 0,
  -- GameState::SelectedOverworldSprite, GameState::SelectedOverworldFrameW, GameState::SelectedOverworldFrameH);

  -- if (GameState::Battle::ExitingFromBattle) {
  -- p1->SetX(GameState::NextLoadLocation.X);
  -- p1->SetY(GameState::NextLoadLocation.Y);
  -- p1->Direction_ = static_cast<Direction>(GameState::NextLoadDirection);
  -- } else {
  -- p1->Direction_ = direction;
  -- }
  -- p1->Animator_->StartAnimation(p1->getAnimNameFromDirection());
  -- SetCameraFollowTarget(p1->GetXHandle(), p1->GetYHandle());
  -- GameState::NextLoadLocation.X = p1->X();
  -- GameState::NextLoadLocation.Y = p1->Y();
  -- GameState::NextLoadDirection = (int)p1->Direction_;
  -- AddGameObjectToGameObjectSystem(p1);

  -- if (GameState::IsMultiplayer) {
  -- auto player2Controller = PlayerControllerSystem::GetPlayerByNum(1);
  -- auto p2 = new LocalPlayer(objData, player2Controller, 1,
  -- GameState::SelectedOverworldSprite2, GameState::SelectedOverworldFrameW2, GameState::SelectedOverworldFrameH2);
  -- if (GameState::Battle::ExitingFromBattle) {
  -- p2->SetX(GameState::NextLoadLocation2.X);
  -- p2->SetY(GameState::NextLoadLocation2.Y);
  -- } else {
  -- p2->SetX(p1->X() + 16);
  -- p2->SetY(p1->Y());
  -- }
  -- p2->Direction_ = p1->Direction_;
  -- p2->Animator_->StartAnimation(p2->getAnimNameFromDirection());
  -- GameState::NextLoadLocation2.X = p2->X();
  -- GameState::NextLoadLocation2.Y = p2->Y();
  -- AddGameObjectToGameObjectSystem(p2);
end

return p
