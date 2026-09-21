local e = require("engine")
local c = require("gameconfig")
local cn = require("constants")
local p = require("gameobjects.localplayer")
local gs = require("gamestate")

e.GameObject.GameObjectLoadFuncs[cn.ObjectTypePlayer] = p.Create -- add to the gameobject create functions

e.Graphics.SetLogicalWorld(c.WorldWidth, c.WorldHeight)
e.Graphics.PreloadTextures()
e.Animator.PreloadAnimatorData()
e.Game.LoadScene(c.DefaultScene)
e.GameObject.LoadGameObjectsFromTiledMap()
--Testing UI
local root = e.UI.CreateObject("Root", { 0, 0, 0, 0 }, nil, 0)
e.UI.SetRoot(root)
e.UI.CreateUIFromLuaFile("ui.debugTown")
local obj = e.UI.GetUIObject("thing2")
print("Did it")

-- local thing1Obj = e.UI.CreateObject("Thing1", { 20, 20, 12, 12 }, root, 0)
-- local tx = e.Graphics.LoadTextureFromPng("player1")
-- local thing2Sprite = e.Sprite.Create(0, 0, tx, { 0, 0, 26, 36 }, { 0, 0, 26, 36 })
-- local thing2SpriteObject = e.UI.CreateImage(thing1Obj, thing2Sprite)


-- local thing2 = e.UI.CreateObject("Thing2", { 21, 20, 12, 12 }, root, 0)
-- local sprite = e.Sprite.Create(0, 0, tx, { 0, 0, 26, 36 }, { 0, 0, 26, 36 })
-- local anim = e.Animator.Create("player1", sprite)
-- local _ = e.UI.CreateAnimator(thing2, sprite, anim)
e.Animator.PlayAnimation(obj.animator, "walkD", -1)
