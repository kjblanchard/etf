local sound = {}
local playingBgm = ""

---Play a bgm unless it's already playing
---@param name string BGM to play
---@param volume number  0.0 - 1.0
---@param loops integer|nil how many loops, nil means loop forever
function sound.PlayBgm(name, volume, loops)
  if playingBgm == name then return end
  loops = loops or -1
  Audio.PlayBgm(name, volume, loops)
  playingBgm = name
end

return sound
