local c = {}
local l = require("engine.log")
local gs = require("gamestate")

local currentCoroutines = {}
---Starts a coroutine, and adds the coroutine to the update table if yield returns a value in seconds, and will be updated every frame.
---@param co thread
---@return boolean false if finished, true if still updating each frame
function c.Start(co)
  -- add to coroutine table after running
  local success, time = coroutine.resume(co)
  local s = coroutine.status(co)
  if s == 'dead' then return false end
  if not success then
    l.Error("Error running coroutine!"); return false
  end
  if not time then
    l.Error("No time returned from yield, not adding coroutine!"); return false
  end
  currentCoroutines[co] = time
  return true
end

---Updates all game coroutines started with c.Start() that are still alive, should be ran in the engine update
function c.Update()
  local deadFuncs = {}
  for co, time in pairs(currentCoroutines) do
    local r = time - gs.DeltaTimeSeconds
    if r < 0 then
      if not c.Start(co) then deadFuncs[#deadFuncs + 1] = co end
    end
    currentCoroutines[co] = r
  end
  for i = 1, #deadFuncs do
    l.Debug("Killing func " .. tostring(deadFuncs[i]))
    currentCoroutines[deadFuncs[i]] = nil
  end
end

return c
