---@diagnostic disable: unused-local
local engine = require("engine")
--Init our files so we have them all in the packages table for everyone else, gamestate is required for update
local gs = require("gamestate")
local c = require("gameconfig")
engine.Graphics.SetWindowOptions(c.WindowWidth, c.WindowHeight,c.WindowTitle)
