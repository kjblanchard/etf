local config = {}
config.Logos = {}
config.BGMVolume = 0.0
config.SFXVolume = 1.0
config.WorldWidth = 480
config.WorldHeight = 270
config.WindowWidth = 1920
config.WindowHeight = 1080
config.WindowTitle = "EscapeTheFate"
config.DefaultScene = "debugTown"
config.Scenes = {
  debugTownHome = {
    UI = "",
    BGM = "town2",
    BGMVolume = 1.0,
    Display = ""
  },
  cloud = {
    UI = "",
    BGM = "town1",
    BGMVolume = 1.0,
    Display = ""
  },
  debugTown = {
    UI = "debugTown",
    BGM = "town2",
    BGMVolume = 1.0,
    Display = "Debug Town"
  },
  debugSouth = {
    UI = "debugTown",
    BGM = "forest1",
    BGMVolume = 1.0,
    Display = "Shotka Trail"
  },
}
return config
