local gs = require("gamestate")
local gfx = require("engine.graphics")
local sprite = require("engine.sprite")
local anim = require("engine.animator")
local ui = {}

local gameUI = {}

---Gets a UI object by name, recursive search so try and cache result
---@param name string name of the object key
---@param currentObjTable table|nil used by recursive lookup, to search from root send nil
---@return table|nil the object or nil if not found
function ui.GetUIObject(name, currentObjTable)
  local start = currentObjTable or gameUI
  --Check current level
  for panelName, panelValue in pairs(start) do
    if panelName == name then return panelValue end
  end
  --Check children
  for _, panelValue in pairs(start) do
    if not panelValue.children then goto continue end
    local find = ui.GetUIObject(name, panelValue.children)
    if find then
      return find
    end
    ::continue::
  end
  return nil
end

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

---Creates a UIImage from the object passed in
---@param o userdata ptr to object already created
---@param sp userdata ptr to a sprite that is already created
---@return userdata the uiimage, likely the same ptr passed in with o
function ui.CreateImage(o, sp)
  return UI.CreateUIImage(o, sp)
end

function ui.CreateAnimator(o, sp, animator)
  return UI.CreateUIAnimator(o, sp, animator)
end

function ui.DrawObject(o)
  UI.DrawUIObject(o)
end

function ui.SetRoot(o)
  UI.SetRootUI(o)
  gameUI["root"] = { userdata = o }
  gs.RootUI = o
end

function ui.DrawRootUI()
  if gs.RootUI then
    UI.DrawUIObject(gs.RootUI)
  end
end

local function handleUIImageCreation(data, objectTable)
  objectTable.t = "image"
  local tx = gfx.LoadTextureFromPng(data.file)
  local s = sprite.Create(data.rect, tx, data.srcRect)
  objectTable.sprite = s
  ui.CreateImage(objectTable.userdata, s)
end

local function handleUIAnimatorCreation(data, objectTable)
  objectTable.t = "anim"
  local tx = gfx.LoadTextureFromPng(data.file)
  local s = sprite.Create(data.rect, tx, data.srcRect)
  objectTable.sprite = s
  local a = anim.Create(data.file, s)
  objectTable.animator = a
  ui.CreateAnimator(objectTable.userdata, s, a)
end

local function handlUITypeCreation(data, objectTable)
  local t = data.t or "panel"
  if t == "image" then
    handleUIImageCreation(data, objectTable)
  elseif t == "anim" then
    handleUIAnimatorCreation(data, objectTable)
  end
end

local function createUIObjectsRecursive(data, parentTable)
  for panelKey, panelObj in pairs(data) do
    parentTable.children = parentTable.children or {}
    parentTable.children[panelKey] = {}
    local currentObjectTable = parentTable.children[panelKey]
    local o = ui.CreateObject(panelKey, panelObj.rect or { 0, 0, 0, 0 }, parentTable.userdata, panelObj.priority or 0)
    currentObjectTable.userdata = o
    handlUITypeCreation(panelObj, currentObjectTable)
    if panelObj.children then
      createUIObjectsRecursive(panelObj.children, currentObjectTable)
    end
  end
end

function ui.CreateUIFromLuaFile(file)
  local data = require(file)
  if not data then return end
  createUIObjectsRecursive(data, gameUI["root"])
end

return ui
