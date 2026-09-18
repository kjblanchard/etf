local g = {}
local l = require("engine.log")
local game = require("engine.game")
local gameobjects = {}

---Should add a key of tile object type, and value of a function that takes an object args as param.
g.GameObjectLoadFuncs = {}

---Gets all gameobjects loaded
---@return table keys are object type, value is lua table of object
function g.GetGameObjects() return gameobjects end

---Gets all gameobjects loaded of a specific type
---@param t integer the tiled type of the gameobject
---@return table array of gameobjects of specified type
function g.GetGameObjectsOfType(t) return gameobjects[t] end

---Gets all object entities from the tield map, and loads them all if we have a gameobject load func for them.
function g.LoadGameObjectsFromTiledMap()
  local o = game.GetGameObjectsFromTiledMap()
  for _, value in ipairs(o) do
    if g.GameObjectLoadFuncs[value.ObjectType] then g.GameObjectLoadFuncs[value.ObjectType](value) end
  end
end

---Adds a gameobject of a specific type
---@param go table gameobject
---@param t integer the type of gameobject this is
function g.AddGameObject(go, t)
  if not go or not t then
    l.Warn("Could not add gameobject without proper type and go added"); return
  end
  gameobjects[t] = gameobjects[t] or {}
  gameobjects[t][#gameobjects[t] + 1] = go
end

return g
