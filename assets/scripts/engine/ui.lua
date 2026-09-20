local gs = require("gamestate")
local ui = {}

---Creates a UI object, and adds it to the parent listed
---@param name string name of the ui object
---@param rect table rect
---@param parent userdata|nil the thing to attach it to
---@param priority integer|nil higher priority is better
---@return userdata the object created
function ui.CreateObject(name, rect, parent, priority)
  priority = priority or 0
  return UI.CreateUIObject(name, rect, parent, priority)
end

function ui.CreateImage(o, sprite)
  return UI.CreateUIImage(o, sprite)
end

function ui.CreateAnimator(o, sprite, animator)
  return UI.CreateUIAnimator(o, sprite, animator)
end

function ui.DrawObject(o)
  UI.DrawUIObject(o)
end

function ui.SetRoot(o)
  UI.SetRootUI(o)
end

function ui.DrawRootUI()
  if gs.RootUI then
    UI.DrawUIObject(gs.RootUI)
  end
end

return ui
