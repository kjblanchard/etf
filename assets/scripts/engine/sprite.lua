local sprite = {}
function sprite.Create(x, y, tex, offsetszrect, txsrcrect)
  local s = Sprite.CreateSprite(x, y, tex, offsetszrect, txsrcrect)
  return s
end

return sprite
