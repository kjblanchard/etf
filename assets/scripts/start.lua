local e = require("engine")
local c = require("gameconfig")
local cn = require("constants")
local p = require("gameobjects.localplayer")

e.GameObject.GameObjectLoadFuncs[cn.ObjectTypePlayer] = p.Create -- add to the gameobject create functions

e.Graphics.SetLogicalWorld(c.WorldWidth, c.WorldHeight)
e.Graphics.PreloadTextures()
e.Game.LoadScene(c.DefaultScene)
e.GameObject.LoadGameObjectsFromTiledMap()
-- e.Sound.PlayBgm("battle1", 1.0)
