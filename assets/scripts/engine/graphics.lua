local graphics = {}
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

return graphics
