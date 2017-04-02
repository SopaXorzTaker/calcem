#! /usr/bin/env lua5.3

local handle_in = io.open("nxu8-disasm-test.data.in", "rb")
if not handle_in then
	print("failed to open 'nxu8-disasm-test.data.in' for reading")
	return 1
end
local test_data = handle_in:read("*a")
handle_in:close()

math.randomseed(os.time())
local out_buffer = {}

local function emit_code(value)
	table.insert(out_buffer, string.char(value & 0xFF))
	table.insert(out_buffer, string.char(value >> 8))
end

local mnemonic, op1, op2
for _mnemonic, _op1, _op2, opcode, data in test_data:gmatch("([^\t]*)\t([^\t]*)\t([^\t]*)\t([^\t]*)\t([^\t]*)\n") do
	if _mnemonic ~= "" then mnemonic = _mnemonic end
	if _op1 ~= "" then op1 = _op1 end
	if _op2 ~= "" then op2 = _op2 end
	if _mnemonic == "~" then mnemonic = "" end
	if _op1 == "~" then op1 = "" end
	if _op2 == "~" then op2 = "" end
	
	local opcode_sane = opcode:gsub("_", "")
	
	local constant_mask = 0
	for pos, cval in opcode_sane:gmatch("()([01])") do
		constant_mask = constant_mask | (1 << (16 - pos)) * (cval:byte() - 48)
	end
	
	local random_mask = 0
	for pos in opcode_sane:gmatch("()[^01]") do
		random_mask = random_mask | (1 << (16 - pos))
	end
	
	local final_opcode = constant_mask | (math.random(0x0000, 0xFFFF) & random_mask)
	local final_data
	
	if data == "" then
		print(("%20s %20s %20s %20s %20s %04X"):format(mnemonic, op1, op2, opcode, data, final_opcode))
		emit_code(final_opcode)
	else
		final_data = math.random(0x0000, 0xFFFF)
		print(("%20s %20s %20s %20s %20s %04X %04X"):format(mnemonic, op1, op2, opcode, data, final_opcode, final_data))
		emit_code(final_opcode)
		emit_code(final_data)
	end
end

emit_code(0xE356) emit_code(0x9C32) -- * L ERn, 0x56:[EA]
emit_code(0xFE9F) emit_code(0x9C32) -- * L ERn, DSR:[EA]
emit_code(0x907F) emit_code(0x9C32) -- * L ERn, R7:[EA]

local handle_out = io.open("nxu8-disasm-test.data.out", "wb")
if not handle_out then
	print("failed to open 'nxu8-disasm-test.data.out' for writing")
	return 2
end
handle_out:write(table.concat(out_buffer))
handle_out:close()
