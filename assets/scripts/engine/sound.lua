local sound = {}
local playingBgm = ""

local bgmVolume = 0.0

function sound.SetGameVolume(v)
  if v < 0.0 or v > 1.0 then return end
  bgmVolume = v
end

---Play a bgm unless it's already playing
---@param name string BGM to play
---@param volume number  0.0 - 1.0
---@param loops integer|nil how many loops, nil means loop forever
function sound.PlayBgm(name, volume, loops)
  if playingBgm == name then return end
  loops = loops or -1
  Audio.PlayBgm(name, volume * bgmVolume, loops)
  playingBgm = name
end

return sound
