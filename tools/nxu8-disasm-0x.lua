#! /usr/bin/env lua5.3

local chunkname = debug.getinfo(1).source:gsub("nxu8%-disasm%-0x%.lua", "nxu8-disasm.lua"):gsub("^@", "")
local handle = io.open(chunkname, "rb")
local content = handle:read("*a")
handle:close()

_ENV.emit_0x_notation = true
return load(content:gsub("#![^\n]*\n", ""), chunkname, "bt", _ENV)(...)
