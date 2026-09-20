local e = require("engine")
local c = require("gameconfig")
local cn = require("constants")
local p = require("gameobjects.localplayer")
local gs = require("gamestate")

e.GameObject.GameObjectLoadFuncs[cn.ObjectTypePlayer] = p.Create -- add to the gameobject create functions

e.Graphics.SetLogicalWorld(c.WorldWidth, c.WorldHeight)
e.Graphics.PreloadTextures()
e.Game.LoadScene(c.DefaultScene)
e.GameObject.LoadGameObjectsFromTiledMap()
local root = e.UI.CreateObject("Root", { 0, 0, 24, 24 }, nil, 0)
e.UI.SetRoot(root)
gs.RootUI = root
local _ = e.UI.CreateObject("Thing1", { 20, 20, 12, 12 }, root, 0)
local thing2 = e.UI.CreateObject("Thing2", { 21, 20, 12, 12 }, root, 0)

local tx = e.Graphics.LoadTextureFromPng("player1")
local sprite = e.Sprite.Create(0, 0, tx, { 0, 0, 26, 36 }, { 0, 0, 26, 36 })
local anim = e.Animator.Create("player1", sprite)
e.UI.CreateAnimator(thing2, sprite, anim)
e.Animator.PlayAnimation(anim, "walkD", -1)
