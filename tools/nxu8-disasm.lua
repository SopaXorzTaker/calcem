#! /usr/bin/env lua5.3

local args = {...}

if not args[1] then
	print("no code dump passed")
	return 1
end

local content
do
	local handle = io.open(args[1], "rb")
	if not handle then
		print("failed to open '" .. argv[1] .. "' for reading");
		return 2
	end
	content = handle:read("*a")
	handle:close()
end

local content_pages
do
	content = content:sub(1, 0x100000)
	local padding_size = #content & 0xFFFF
	if padding_size ~= 0 then
		padding_size = 0x10000 - padding_size
	end
	content = content .. ("\0"):rep(padding_size)
	content_pages = #content / 0x10000
end

local alu_basic_mnemonic_lookup = {
	[0] = "MOV", "ADD", "AND", "OR", "XOR", "CMPC", "ADDC", "CMP", "SUB", "SUBC", "SLL", "SLLC", "SRL", "SRLC", "SRA"
}
local reg_size_prefix_lookup = {
	[0] = "", "E", "X", "Q"
}
local conditional_branch_lookup = {
	[0] = "BGE", "BLT", "BGT", "BLE", "BGES", "BLTS", "BGTS", "BLES", "BNZ", "BZ", "BNV", "BOV", "BPS", "BNS", "BAL"
}
local alu_shift_lookup = {
	[0x900A] =  "SLL",
	[0x900B] = "SLLC",
	[0x900C] =  "SRL",
	[0x900D] = "SRLC",
	[0x900E] =  "SRA"
}
local alu_bitop_lookup = {
	[0] = "SB", "TB", "RB"
}
local code_a003_4_7_b_c_f_op_lookup = {
	[0xA003] =  "PSW",
	[0xA004] = "EPSW",
	[0xA007] = "ECSR",
	[0xA00B] =  "PSW",
	[0xA00C] = "EPSW",
	[0xA00F] = "ECSR"
}

local function sign_extend(value, sign_bit)
	if value >= sign_bit then
		value = value - 2 * sign_bit
	end
	return tostring(value)
end

local current_csr, current_pc
local function fetch_code()
	local instr_low  = content:byte(current_csr + (current_pc & 0xFFFE) + 1)
	local instr_high = content:byte(current_csr + (current_pc & 0xFFFE) + 2)
	return instr_low + (instr_high << 8)
end

current_csr = 0
while current_csr < content_pages do
	current_pc = 0
	local instr_dsr_prefix = false
	local dsr_prefix_code
	
	while current_pc < 0x10000 do
		local immediate_code
		
		local instruction = fetch_code()
		local new_dsr_prefix
		local instr_mnemonic, instr_op1, instr_op2
		local instr_note
		
		if     instruction & 0x8000 == 0x0000 then
			instr_mnemonic = alu_basic_mnemonic_lookup[instruction >> 12]
			instr_op1 = "R" .. ((instruction >> 8) & 0xF)
			instr_op2 = ("0x%02X"):format(instruction & 0xFF)
			
		elseif instruction & 0xF01F == 0x800F then
			instr_mnemonic = "EXTBW"
			instr_op1 = "R" .. ((instruction >> 4) & 0xF)
			if ((instruction >> 4) & 0xF) + 1 ~= ((instruction >> 8) & 0xF) then
				instr_note = "probably invalid, n is not m + 1"
			end
			
		elseif instruction & 0xF000 == 0x8000 then
			instr_mnemonic = alu_basic_mnemonic_lookup[instruction & 0xF]
			instr_op1 = "R" .. ((instruction >> 8) & 0xF)
			instr_op2 = "R" .. ((instruction >> 4) & 0xF)
			
		elseif instruction & 0xF0FF == 0x801F then
			instr_mnemonic = "DAA"
			instr_op1 = "R" .. ((instruction >> 8) & 0xF)
			
		elseif instruction & 0xF0FF == 0x803F then
			instr_mnemonic = "DAS"
			instr_op1 = "R" .. ((instruction >> 8) & 0xF)
			
		elseif instruction & 0xF0FF == 0x805F then
			instr_mnemonic = "NEG"
			instr_op1 = "R" .. ((instruction >> 8) & 0xF)
			
		elseif instruction & 0xF01C == 0x9000 then
			instr_mnemonic = (instruction & 0x0001 == 0) and "L" or "ST"
			instr_op1 = ((instruction & 0x0002 == 0) and "" or "E") .. "R" .. ((instruction >> 8) & 0xF)
			instr_op2 = "[ER" .. ((instruction >> 4) & 0xF) .. "]"
			
		elseif instruction & 0xF00E == 0x9008 or
		       instruction & 0xF00E == 0xA008 then
			current_pc = current_pc + 2
			instr_mnemonic = (instruction & 0x0001 == 0) and "L" or "ST"
			instr_op1 = ((instruction & 0xF000 == 0x9000) and "" or "E") .. "R" .. ((instruction >> 8) & 0xF)
			immediate_code = fetch_code()
			instr_op2 = "[ER" .. ((instruction >> 4) & 0xF) .. "+" .. ("0x%04X"):format(immediate_code) .. "]"
			
		elseif instruction & 0xF0F8 == 0x9030 then
			instr_mnemonic = (instruction & 0x0001 == 0) and "L" or "ST"
			instr_op1 = reg_size_prefix_lookup[(instruction & 0x0006) >> 1] .. "R" .. ((instruction >> 8) & 0xF)
			instr_op2 = "[EA]"
			
		elseif instruction & 0xF0F8 == 0x9050 then
			instr_mnemonic = (instruction & 0x0001 == 0) and "L" or "ST"
			instr_op1 = reg_size_prefix_lookup[(instruction & 0x0006) >> 1] .. "R" .. ((instruction >> 8) & 0xF)
			instr_op2 = "[EA+]"
			
		elseif instruction & 0xF00F == 0x900A or
		       instruction & 0xF00F == 0x900B or
		       instruction & 0xF00F == 0x900C or
		       instruction & 0xF00F == 0x900D or
		       instruction & 0xF00F == 0x900E then
			instr_mnemonic = alu_shift_lookup[instruction & 0xF00F]
			instr_op1 = "R" .. ((instruction >> 8) & 0xF)
			instr_op2 = ((instruction >> 4) & 0x7)
			if instruction & 0x0080 ~= 0 then
				instr_note = "probably invalid, &0x0080 is not 0"
			end
			
		elseif instruction & 0xF0FC == 0x9010 then
			current_pc = current_pc + 2
			instr_mnemonic = (instruction & 0x0001 == 0) and "L" or "ST"
			instr_op1 = ((instruction & 0x0002 == 0) and "" or "E") .. "R" .. ((instruction >> 8) & 0xF)
			immediate_code = fetch_code()
			instr_op2 = ("[0x%04X]"):format(immediate_code)
		
		elseif instruction & 0xF00F == 0x900F then
			new_dsr_prefix = "R" .. ((instruction >> 4) & 0xF)
			if instruction & 0x0F00 ~= 0 then
				instr_note = "probably invalid, &0x0F00 is not 0"
			end
			
		elseif instruction & 0xF00F == 0xA000 or
		       instruction & 0xF00F == 0xA001 or
		       instruction & 0xF00F == 0xA002 then
			instr_mnemonic = alu_bitop_lookup[instruction & 0xF]
			if instruction & 0x0080 == 0 then
				instr_op1 = "R" .. ((instruction >> 8) & 0xF) .. "." .. ((instruction >> 4) & 0x7)
			else
				current_pc = current_pc + 2
				immediate_code = fetch_code()
				instr_op1 = "[" .. ("0x%04X"):format(immediate_code) .. "]." .. ((instruction >> 4) & 0x7)
				if instruction & 0x0F00 ~= 0 then
					instr_note = "probably invalid, &0x0F00 is not 0"
				end
			end
			
		elseif instruction & 0xF00F == 0xA005 then
		    instr_mnemonic = "MOV"
		    instr_op1 = "ER" .. ((instruction >> 8) & 0xF)
		    instr_op2 = "ELR"
			if instruction & 0x00F0 ~= 0 then
				instr_note = "probably invalid, &0x00F0 is not 0"
			end
			
		elseif instruction & 0xF00F == 0xA006 then
		    instr_mnemonic = "MOV"
		    instr_op1 = "R" .. ((instruction >> 8) & 0xF)
		    instr_op2 = "CR" .. ((instruction >> 4) & 0xF)
		
		elseif instruction & 0xF00F == 0xA003 or
		       instruction & 0xF00F == 0xA004 or
		       instruction & 0xF00F == 0xA007 then
		    instr_mnemonic = "MOV"
		    instr_op1 = "R" .. ((instruction >> 8) & 0xF)
		    instr_op2 = code_a003_4_7_b_c_f_op_lookup[instruction & 0xF00F]
			if instruction & 0x00F0 ~= 0 then
				instr_note = "probably invalid, &0x00F0 is not 0"
			end
		    
		elseif instruction & 0xFF0F == 0xA10A then
		    instr_mnemonic = "MOV"
		    instr_op1 = "SP"
		    instr_op2 = "ER" .. ((instruction >> 4) & 0xF)
		    
		elseif instruction & 0xF0FF == 0xA01A then
		    instr_mnemonic = "MOV"
		    instr_op1 = "ER" .. ((instruction >> 8) & 0xF)
		    instr_op2 = "SP"
		    
		elseif instruction & 0xF00F == 0xA00D then
		    instr_mnemonic = "MOV"
		    instr_op1 = "ELR"
		    instr_op2 = "ER" .. ((instruction >> 8) & 0xF)
			if instruction & 0x00F0 ~= 0 then
				instr_note = "probably invalid, &0x00F0 is not 0"
			end
			
		elseif instruction & 0xF00F == 0xA00E then
		    instr_mnemonic = "MOV"
		    instr_op1 = "CR" .. ((instruction >> 8) & 0xF)
		    instr_op2 = "R" .. ((instruction >> 4) & 0xF)
		    
		elseif instruction & 0xF00F == 0xA00B or
		       instruction & 0xF00F == 0xA00C or
		       instruction & 0xF00F == 0xA00F then
		    instr_mnemonic = "MOV"
		    instr_op1 = code_a003_4_7_b_c_f_op_lookup[instruction & 0xF00F]
		    instr_op2 = "R" .. ((instruction >> 4) & 0xF)
			if instruction & 0x0F00 ~= 0 then
				instr_note = "probably invalid, &0x0F00 is not 0"
			end
		    
		elseif instruction & 0xF000 == 0xC000 then
			instr_mnemonic = conditional_branch_lookup[(instruction & 0x0F00) >> 8]
			instr_op1 = ("0x%01X:0x%04X"):format(current_csr, (current_pc + 2 + sign_extend(instruction & 0xFF, 0x80) * 2) & 0xFFFE)
			
		elseif instruction & 0xF000 == 0xB000 or
		       instruction & 0xF000 == 0xD000 then
			instr_mnemonic = (instruction & 0x0080 == 0) and "L" or "ST"
			instr_op1 = ((instruction & 0xF000 == 0xD000) and "" or "E") .. "R" .. ((instruction >> 8) & 0xF)
			instr_op2 = "[" .. ((instruction & 0x0040 == 0) and "BP" or "FP") .. "+" .. sign_extend(instruction & 0x3F, 0x20) .. "]"
			
		elseif instruction & 0xF180 == 0xE000 then
			instr_mnemonic = "MOV"
			instr_op1 = "ER" .. ((instruction >> 8) & 0xF)
			instr_op2 = sign_extend(instruction & 0x7F, 0x40)
			
		elseif instruction & 0xF180 == 0xE080 then
			instr_mnemonic = "ADD"
			instr_op1 = "ER" .. ((instruction >> 8) & 0xF)
			instr_op2 = sign_extend(instruction & 0x7F, 0x40)
		
		elseif instruction & 0xFF00 == 0xE100 then
			instr_mnemonic = "ADD"
			instr_op1 = "SP"
			instr_op2 = sign_extend(instruction & 0xFF, 0x80)
		
		elseif instruction & 0xFF00 == 0xE300 then
			new_dsr_prefix = ("0x%02X"):format(instruction & 0xFF)
		
		elseif instruction & 0xFF00 == 0xE500 then
			instr_mnemonic = "SWI"
			instr_op1 = tostring(instruction & 0x3F)
			if (instruction & 0xFF) > 0x3F then
				instr_note = "vector > 63"
			end
		
		elseif instruction & 0xFF00 == 0xE900 then
			instr_mnemonic = "MOV"
			instr_op1 = "PSW"
			instr_op2 = ("0x%02X"):format(instruction & 0xFF)
		
		elseif instruction & 0xFF00 == 0xEB00 then
			if     instruction & 0x0000 == 0x00F7 then
				instr_mnemonic = "DI"
			elseif instruction & 0x0000 == 0x007F then
				instr_mnemonic = "RC"
			else
				instr_mnemonic = "PSW &= " .. ("0x%02X"):format(instruction & 0xFF)
				instr_note = "probably invalid"
			end
		
		elseif instruction & 0xFF00 == 0xED00 then
			if     instruction & 0x0000 == 0x0008 then
				instr_mnemonic = "EI"
			elseif instruction & 0x0000 == 0x0080 then
				instr_mnemonic = "SC"
			else
				instr_mnemonic = "PSW |= " .. ("0x%02X"):format(instruction & 0xFF)
				instr_note = "probably invalid"
			end
		
		elseif instruction & 0xF00F == 0xF000 then
			current_pc = current_pc + 2
			instr_mnemonic = "B"
			immediate_code = fetch_code()
			instr_op1 = ("0x%01X:0x%04X"):format((instruction & 0x0F00) >> 8, immediate_code & 0xFFFE)
			if instruction & 0x00F0 ~= 0 then
				instr_note = "probably invalid, &0x00F0 is not 0"
			end
		
		elseif instruction & 0xF00F == 0xF001 then
			current_pc = current_pc + 2
			instr_mnemonic = "BL"
			immediate_code = fetch_code()
			instr_op1 = ("0x%01X:0x%04X"):format((instruction & 0x0F00) >> 8, immediate_code & 0xFFFE)
			if instruction & 0x00F0 ~= 0 then
				instr_note = "probably invalid, &0x00F0 is not 0"
			end
		
		elseif instruction & 0xF00F == 0xF002 then
			instr_mnemonic = "B"
			instr_op1 = ("0x%01X:ER%i"):format(current_csr, (instruction & 0x00F0) >> 4)
			if instruction & 0x0F00 ~= 0 then
				instr_note = "probably invalid, &0x0F00 is not 0"
			end
		
		elseif instruction & 0xF00F == 0xF003 then
			instr_mnemonic = "BL"
			instr_op1 = ("0x%01X:ER%i"):format(current_csr, (instruction & 0x00F0) >> 4)
			if instruction & 0x0F00 ~= 0 then
				instr_note = "probably invalid, &0x0F00 is not 0"
			end
		
		elseif instruction & 0xF00F == 0xF004 then
			instr_mnemonic = "MUL"
			instr_op1 = "ER" .. ((instruction >> 8) & 0xF)
			instr_op2 = "R" .. ((instruction >> 4) & 0xF)
		
		elseif instruction & 0xF00F == 0xF005 then
			instr_mnemonic = "MOV"
			instr_op1 = "ER" .. ((instruction >> 8) & 0xF)
			instr_op2 = "ER" .. ((instruction >> 4) & 0xF)
		
		elseif instruction & 0xF00F == 0xF006 then
			instr_mnemonic = "ADD"
			instr_op1 = "ER" .. ((instruction >> 8) & 0xF)
			instr_op2 = "ER" .. ((instruction >> 4) & 0xF)
		
		elseif instruction & 0xF00F == 0xF007 then
			instr_mnemonic = "CMP"
			instr_op1 = "ER" .. ((instruction >> 8) & 0xF)
			instr_op2 = "ER" .. ((instruction >> 4) & 0xF)
		
		elseif instruction & 0xF00F == 0xF009 then
			instr_mnemonic = "DIV"
			instr_op1 = "ER" .. ((instruction >> 8) & 0xF)
			instr_op2 = "R" .. ((instruction >> 4) & 0xF)
		
		elseif instruction & 0xF00F == 0xF00A then
			instr_mnemonic = "LEA"
			instr_op1 = "[ER" .. ((instruction >> 4) & 0xF) .. "]"
			if instruction & 0x0F00 ~= 0 then
				instr_note = "probably invalid, &0x0F00 is not 0"
			end
		
		elseif instruction & 0xF00F == 0xF00B then
			current_pc = current_pc + 2
			instr_mnemonic = "LEA"
			immediate_code = fetch_code()
			instr_op1 = "[ER" .. ((instruction >> 4) & 0xF) .. "+" .. ("0x%04X"):format(immediate_code) .. "]"
			if instruction & 0x0F00 ~= 0 then
				instr_note = "probably invalid, &0x0F00 is not 0"
			end
		
		elseif instruction & 0xF00F == 0xF00C then
			current_pc = current_pc + 2
			instr_mnemonic = "LEA"
			immediate_code = fetch_code()
			instr_op1 = "[" .. ("0x%04X"):format(immediate_code) .. "]"
			if instruction & 0x0F00 ~= 0 then
				instr_note = "probably invalid, &0x0F00 is not 0"
			end
		
		elseif instruction & 0xF00F == 0xF00D then
			instr_mnemonic = "MOV"
			instr_op1 = "C" .. reg_size_prefix_lookup[(instruction & 0x0060) >> 5] .. "R" .. ((instruction >> 8) & 0xF)
			instr_op2 = (instruction & 0x0010 == 0) and "[EA]" or "[EA+]"
			if instruction & 0x0080 ~= 0 then
				instr_op1, instr_op2 = instr_op2, instr_op1
			end
		
		elseif instruction & 0xF0CF == 0xF00E then
			instr_mnemonic = "POP"
			instr_op1 = reg_size_prefix_lookup[(instruction & 0x0030) >> 4] .. "R" .. ((instruction >> 8) & 0xF)
		
		elseif instruction & 0xF0CF == 0xF04E then
			instr_mnemonic = "PUSH"
			instr_op1 = reg_size_prefix_lookup[(instruction & 0x0030) >> 4] .. "R" .. ((instruction >> 8) & 0xF)
		
		elseif instruction & 0xF0FF == 0xF08E then
			instr_mnemonic = "POP"
			local reg_list = {}
			if instruction & 0x0100 ~= 0 then table.insert(reg_list,   "EA") end
			if instruction & 0x0200 ~= 0 then table.insert(reg_list,   "PC") end
			if instruction & 0x0400 ~= 0 then table.insert(reg_list,  "PSW") end
			if instruction & 0x0800 ~= 0 then table.insert(reg_list,   "LR") end
			instr_op1 = table.concat(reg_list, ", ")
		
		elseif instruction & 0xF0FF == 0xF0CE then
			instr_mnemonic = "PUSH"
			local reg_list = {}
			if instruction & 0x0100 ~= 0 then table.insert(reg_list,   "EA") end
			if instruction & 0x0200 ~= 0 then table.insert(reg_list,  "ELR") end
			if instruction & 0x0400 ~= 0 then table.insert(reg_list, "EPSW") end
			if instruction & 0x0800 ~= 0 then table.insert(reg_list,   "LR") end
			instr_op1 = table.concat(reg_list, ", ")
		
		elseif instruction          == 0xFE0F then
			instr_mnemonic = "RTI"
		
		elseif instruction          == 0xFE1F then
			instr_mnemonic = "RT"
		
		elseif instruction          == 0xFE2F then
			instr_mnemonic = "INC"
			instr_op1 = "[EA]"
		
		elseif instruction          == 0xFE3F then
			instr_mnemonic = "DEC"
			instr_op1 = "[EA]"
		
		elseif instruction          == 0xFE8F then
			instr_mnemonic = "NOP"
		
		elseif instruction          == 0xFE9F then
			new_dsr_prefix = "DSR"
		
		elseif instruction          == 0xFECF then
			instr_mnemonic = "CPLC"
		
		elseif instruction          == 0xFFFF then
			instr_mnemonic = "BRK"
		
		end
		
		if new_dsr_prefix then
			if instr_dsr_prefix then
				print(("%06X   %-17s  %-8s"):format(
					current_pc - 2,
					("%02X %02X"):format(dsr_prefix_code & 0xFF, dsr_prefix_code >> 8),
					"redundant DSR prefix"
				))
			end
			
			dsr_prefix_code = instruction
			
			instr_dsr_prefix = new_dsr_prefix
		else
			local this_pc = current_pc
			
			local code_repr_tbl = {("%02X %02X"):format(instruction & 0xFF, instruction >> 8)}
			if immediate_code then
				this_pc = this_pc - 2
				table.insert(code_repr_tbl, ("%02X %02X"):format(immediate_code & 0xFF, immediate_code >> 8))
			end
			
			if instr_dsr_prefix then
				this_pc = this_pc - 2
			
				if     instr_op1 and instr_op1:find("^%[") then
					instr_op1 = instr_dsr_prefix .. ":" .. instr_op1
				elseif instr_op2 and instr_op2:find("^%[") then
					instr_op2 = instr_dsr_prefix .. ":" .. instr_op2
				end
				
				table.insert(code_repr_tbl, 1, ("%02X %02X"):format(dsr_prefix_code & 0xFF, dsr_prefix_code >> 8))
			end
			
			if instr_mnemonic then
				print(("%06X   %-17s  %-8s%s%s%s%s"):format(
					this_pc,
					table.concat(code_repr_tbl, " "),
					instr_mnemonic,
					instr_op1 and instr_op1 or "",
					instr_op2 and ", " or "",
					instr_op2 and instr_op2 or "",
					instr_note and (" (" .. instr_note .. ")") or ""
				))
			else
				print(("%06X   %-17s  Unrecognized command"):format(
					this_pc,
					table.concat(code_repr_tbl, " ")
				))
			end
		
			instr_dsr_prefix = false
			dsr_prefix_code = nil
		end
		
		current_pc = current_pc + 2
	end
	
	current_csr = current_csr + 0x10000
end

