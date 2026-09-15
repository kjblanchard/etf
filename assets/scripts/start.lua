local e = require("engine")
local c = require("gameconfig")

e.Graphics.SetLogicalWorld(c.WorldWidth, c.WorldHeight)
e.Game.LoadScene(c.DefaultScene)
e.Sound.PlayBgm("battle1", 1.0)
local o = e.Game.GetGameObjectsFromTiledMap()
e.Game.DebugSetBreak()
print("Hello")
