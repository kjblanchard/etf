local g = {}
local l = require("engine.log")
local gameobjects = {}

function g.GetGameObjects() return gameobjects end

function g.GetGameObjectsOfType(t) return gameobjects[t] end

function g.AddGameObject(go, t)
  if not t then
    l.Warn("Could not add gameobject without proper type added"); return
  end
  gameobjects[t] = gameobjects[t] or {}
  gameobjects[t][#gameobjects[t] + 1] = go
end

return g
