local c = require("constants")
local d = require("engine.game")
local graphics = {}
local cachedTextures = {}
---Sets window settings
---@param w integer width of window
---@param h integer height of window
---@param n string name of window
function graphics.SetWindowOptions(w, h, n)
  Graphics.SetWindow(w, h, n)
end

---Sets the scaling of the game, needs to be called at start and not init
---@param w integer actual width of the game in pixels
---@param h integer actual height of the game in pixels
function graphics.SetLogicalWorld(w, h)
  Graphics.SetLogicalWorld(w, h)
end

---Load all textures into cache, so we don't need to load any during gameplay
function graphics.PreloadTextures()
  for _, value in ipairs(c.PreloadTextures) do
    graphics.LoadTextureFromPng(value)
  end
end

---Loads a texture into cache if it isn't already loaded
---@param n string Name without png in it
function graphics.LoadTextureFromPng(n)
  n = n .. ".png"
  if cachedTextures[n] then return cachedTextures[n] end
  local t = Graphics.LoadTexture(n)
  cachedTextures[n] = t
  return t
end

return graphics
