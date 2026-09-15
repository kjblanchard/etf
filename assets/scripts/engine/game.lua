local c = require("gameconfig")
local s = require("engine.sound")
local game = {}

-- local cachedMaps = {}

---Loads a scene from the gameconfig scene table
---@param n string name of the key in the table
function game.LoadScene(n)
  local scene = c.Scenes[n]
  if not scene then return end
  Scene.LoadScene(n)
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
