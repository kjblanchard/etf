local c = require("gameconfig")
local s = require("engine.sound")
local l = require("engine.log")
local game = {}

local cachedMaps = {}

---Loads a scene from the gameconfig scene table
---@param name string name of the key in the table
function game.LoadScene(name)
  local scene = c.Scenes[name]
  if not scene then return end
  if cachedMaps[name] then
    Scene.LoadSceneFromMap(cachedMaps[name])
  else
    cachedMaps[name] = Scene.LoadScene(name)
    if not cachedMaps[name] then l.Critical("Could not load map, we shoudl quit bois") end
  end
  if scene.BGM then
    s.PlayBgm(scene.BGM, scene.BGMVolume, -1)
  end
end

function game.GetGameObjectsFromTiledMap()
  return Scene.GetGameObjectData()
end

function game.DebugSetBreak()
  local d = require 'debugger'
  d()
end

return game
