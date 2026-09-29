local c = require("gameconfig")
local s = require("engine.sound")
local l = require("engine.log")
local ui = require("engine.ui")
local go = require("engine.gameobject")
local game = {}

local cachedMaps = {}
local cachedUIFiles = {}

function game.PreloadAllScenes()
  --Load all the maps
  for sceneName, sceneObject in pairs(c.Scenes) do
    if not cachedMaps[sceneName] then
      cachedMaps[sceneName] = Scene.LoadScene(sceneName)
    end
    if sceneObject.UI ~= "" and not cachedUIFiles[sceneObject.UI] then
      l.Debug("caching ui" .. sceneObject.UI)
      ui.CreateUIFromLuaFile("ui." .. sceneObject.UI)
      cachedUIFiles[sceneObject.UI] = true
    end
    ui.SetTopLevelNotVisible()
  end
end

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
  go.LoadGameObjectsFromTiledMap()
end


function game.DebugSetBreak()
  local d = require 'debugger'
  d()
end

return game
