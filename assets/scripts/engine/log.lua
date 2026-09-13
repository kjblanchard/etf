local l = {}

local function logInt(lv, m) Log.Log(lv, "Lua: " .. m) end

function l.Debug(m) logInt(1, m) end

function l.Info(m) logInt(2, m) end

function l.Warn(m) logInt(3, m) end

function l.Error(m) logInt(4, m) end

function l.Critical(m) logInt(5, m) end

function l.LogLevel(lv) Log.SetLogLevel(lv) end

return l
