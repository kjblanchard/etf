local sprite = {}
function sprite.Create(location, tex, txsrcrect)
  local s = Sprite.CreateSprite(location, tex, txsrcrect)
  return s
end

return sprite
