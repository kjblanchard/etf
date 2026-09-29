local e = require("engine")
local c = require("gameconfig")
local cn = require("constants")
local p = require("gameobjects.localplayer")

e.GameObject.GameObjectLoadFuncs[cn.ObjectTypePlayer] = p.Create -- add to the gameobject create functions

e.Graphics.SetLogicalWorld(c.WorldWidth, c.WorldHeight)
e.Graphics.PreloadTextures()
e.Animator.PreloadAnimatorData()
e.Sound.SetGameVolume(c.BGMVolume)
e.UI.SetRoot(e.UI.CreateObject("Root", { 0, 0, 0, 0 }, nil, 0))
e.Game.PreloadAllScenes()
e.Game.LoadScene(c.DefaultScene)
--Testing playing an animation
local obj = e.UI.GetUIObject("thing2")
e.Animator.PlayAnimation(obj.animator, "walkD")
