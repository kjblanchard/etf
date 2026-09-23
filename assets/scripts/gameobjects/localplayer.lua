local p = {}
local go = require("engine.gameobject")
local gfx = require("engine.graphics")
local s = require("engine.sprite")
local c = require("constants")
local gs = require("gamestate")


---Creates player and adds to gameobject list
---@param args any
function p.Create(args)
  if args.Properties.loadLocation ~= gs.LoadLocation then return end
  local player = {}
  player.Direction = args.Properties.direction
  player.X = args.X
  player.Y = args.Y
  player.W = args.W
  player.H = args.H
  player.PlayerNum = 1
  --Get Sprite for player based on player num / gamestate selection
  local tx = gfx.LoadTextureFromPng("player1")
  player.Sprite = s.Create({ player.X, player.Y, 26, 36 }, tx, { 0, 0, 26, 36 })
  -- If we are exiting from battle, we need to update our coordinates and direction from where we saved them from, and if p2, offset by 3px on x
  --Start animation so that we are facing the proper direction
  -- Set follow target, or update camera if we are the follow target
  -- Update next load location to this location if needed
  go.AddGameObject(player, c.ObjectTypePlayer)
  -- If there is multiple players joined, add them too
end

return p
