local config = {}
config.Logos = {}
config.BGMVolume = 1.0
config.SFXVolume = 1.0
config.WorldWidth = 480
config.WorldHeight = 270
config.WindowWidth = 1920
config.WindowHeight = 1080
config.WindowTitle = "EscapeTheFate"
config.DefaultScene = "debugTown"
config.Scenes = {
  debugTown = {
    UI = "debugTownU",
    BGM = "town2",
    BGMVolume = 1.0,
    Display = "Debug Town"
  },
  debugSouth = {
    UI = "",
    BGM = "forest1",
    BGMVolume = 1.0,
    Display = "Shotka Trail"
  },
  debugTownHome = {
    UI = "",
    BGM = "town2",
    BGMVolume = 1.0,
    Display = ""
  },
  cloud = {
    UI = "cloudU",
    BGM = "town1",
    BGMVolume = 1.0,
    Display = ""
  },
}
return config
