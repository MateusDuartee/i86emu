// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#include "Disassembler.hpp"

#include <format>
#include <array>
#include <algorithm>

namespace disassembler
{

	constexpr std::array<const char*, 120> Keywords =
	{
		 "ADD",    "ADC",   "SUB",   "SBB",   "MUL",  "IMUL",   "DIV",
		"IDIV",     "OR",   "AND",   "XOR",   "ROL",   "ROR",   "RCL",
		 "RCR",    "SHL",   "SAL",   "SHR",   "SAR",   "NOT",   "NEG",
		"TEST",   "PUSH",   "POP",   "INC",   "DEC",   "CMP",   "DAA",
		 "DAS",    "AAA",   "AAS",    "JO",   "JNO",    "JB",  "JNAE",
		  "JC",    "JNB",   "JAE",   "JNC",    "JE",    "JZ",   "JNE",
		 "JNZ",    "JBE",   "JNA",  "JNBE",    "JA",    "JS",   "JNS",
		  "JP",    "JPE",   "JNP",   "JPO",    "JL",  "JNGE",   "JNL",
		 "JGE",    "JLE",   "JNG",  "JNLE",    "JG",  "XCHG",   "MOV", 
		 "LEA",    "CBW",   "CWD",  "CALL",  "WAIT", "PUSHF",  "POPF",
		"SAHF",   "LAHF", "MOVSB", "MOVSW", "CMPSB", "CMPSW", "STOSB",
	   "STOSW",  "LODSB", "LODSW", "SCASB", "SCASW",   "RET",  "RETF",
	     "LES",    "LDS",   "INT",   "AAM",   "AAD",  "XLAT",   "ESC",
	  "LOOPNE", "LOOPNZ", "LOOPE", "LOOPZ",  "LOOP",  "JCXZ",    "IN",
		 "OUT",    "JMP",  "LOCK", "REPNE", "REPNZ",   "REP",  "REPE",
		"REPZ",    "HLT",   "CMC",   "CLC",   "STC",   "CLI",   "STI",
		 "CLD",    "STD",   "NOP",  "INTO",  "IRET",  "BYTE",  "WORD",
		 "PTR"
	};

	constexpr std::array<const char*, 20> Registers =
	{
		"AX", "AH", "AL",
		"BX", "BH", "BL",
		"CX", "CH", "CL",
		"DX", "DH", "DL",
		"SP", "BP", "SI", "DI",
		"CS", "DS", "SS", "ES"
	};

	constexpr std::array<const Instr, 256> InstrTable = 
	{{
			{  "ADD {}, {}",     rm8,   r8,  true}, {  "ADD {}, {}",    rm16,  r16,  true}, {  "ADD {}, {}",      r8,     rm8,  true}, {  "ADD {}, {}",     r16,    rm16,  true}, {"ADD AL, {}{}", implied,   i8, false}, {"ADD AX, {}{}", implied,  i16, false}, {     "PUSH ES", implied,    none, false}, {      "POP ES", implied,    none, false}, {    "OR {}, {}",     rm8,   r8,  true}, {    "OR {}, {}",    rm16,  r16,  true}, {   "OR {}, {}",      r8,  rm8,  true}, {   "OR {}, {}",     r16, rm16,  true}, { "OR AL, {}{}", implied,      i8, false}, { "OR AX, {}{}", implied,     i16, false}, {     "PUSH CS", implied,    none, false}, {      "POP CS", implied,    none, false},
			{  "ADC {}, {}",     rm8,   r8,  true}, {  "ADC {}, {}",    rm16,  r16,  true}, {  "ADC {}, {}",      r8,     rm8,  true}, {  "ADC {}, {}",     r16,    rm16,  true}, {"ADC AL, {}{}", implied,   i8, false}, {"ADC AX, {}{}", implied,  i16, false}, {     "PUSH SS", implied,    none, false}, {      "POP SS", implied,    none, false}, {   "SBB {}, {}",     rm8,   r8,  true}, {   "SBB {}, {}",    rm16,  r16,  true}, {  "SBB {}, {}",      r8,  rm8,  true}, {  "SBB {}, {}",     r16, rm16,  true}, {"SBB AL, {}{}", implied,      i8, false}, {"SBB AX, {}{}", implied,     i16, false}, {     "PUSH DS", implied,    none, false}, {      "POP DS", implied,    none, false},
			{  "AND {}, {}",     rm8,   r8,  true}, {  "AND {}, {}",    rm16,  r16,  true}, {  "AND {}, {}",      r8,     rm8,  true}, {  "AND {}, {}",     r16,    rm16,  true}, {"AND AL, {}{}", implied,   i8, false}, {"AND AX, {}{}", implied,  i16, false}, {         "ES:",    none,    none, false}, {         "DAA",    none,    none, false}, {   "SUB {}, {}",     rm8,   r8,  true}, {   "SUB {}, {}",    rm16,  r16,  true}, {  "SUB {}, {}",      r8,  rm8,  true}, {  "SUB {}, {}",     r16, rm16,  true}, {"SUB AL, {}{}", implied,      i8, false}, {"SUB AX, {}{}", implied,     i16, false}, {         "CS:",    none,    none, false}, {         "DAS",    none,    none, false},
			{  "XOR {}, {}",     rm8,   r8,  true}, {  "XOR {}, {}",    rm16,  r16,  true}, {  "XOR {}, {}",      r8,     rm8,  true}, {  "XOR {}, {}",     r16,    rm16,  true}, {"XOR AL, {}{}", implied,   i8, false}, {"XOR AX, {}{}", implied,  i16, false}, {         "SS:",    none,    none, false}, {         "AAA",    none,    none, false}, {   "CMP {}, {}",     rm8,   r8,  true}, {   "CMP {}, {}",    rm16,  r16,  true}, {  "CMP {}, {}",      r8,  rm8,  true}, {  "CMP {}, {}",     r16, rm16,  true}, {"CMP AL, {}{}", implied,      i8, false}, {"CMP AX, {}{}", implied,     i16, false}, {         "DS:",    none,    none, false}, {         "AAS",    none,    none, false},
			{      "INC AX", implied, none, false}, {      "INC CX", implied, none, false}, {      "INC DX", implied,    none, false}, {      "INC BX", implied,    none, false}, {      "INC SP", implied, none, false}, {      "INC BP", implied, none, false}, {      "INC SI", implied,    none, false}, {      "INC DI", implied,    none, false}, {       "DEC AX", implied, none, false}, {       "DEC CX", implied, none, false}, {      "DEC DX", implied, none, false}, {      "DEC BX", implied, none, false}, {      "DEC SP", implied,    none, false}, {      "DEC BP", implied,    none, false}, {      "DEC SI", implied,    none, false}, {      "DEC DI", implied,    none, false},
			{     "PUSH AX", implied, none, false}, {     "PUSH CX", implied, none, false}, {     "PUSH DX", implied,    none, false}, {     "PUSH BX", implied,    none, false}, {     "PUSH SP", implied, none, false}, {     "PUSH BP", implied, none, false}, {     "PUSH SI", implied,    none, false}, {     "PUSH DI", implied,    none, false}, {       "POP AX", implied, none, false}, {       "POP CX", implied, none, false}, {      "POP DX", implied, none, false}, {      "POP BX", implied, none, false}, {      "POP SP", implied,    none, false}, {      "POP BP", implied,    none, false}, {      "POP SI", implied,    none, false}, {      "POP DI", implied,    none, false},
			{         "NOP",    none, none, false}, {         "NOP",    none, none, false}, {         "NOP",    none,    none, false}, {         "NOP",    none,    none, false}, {         "NOP",    none, none, false}, {         "NOP",    none, none, false}, {         "NOP",    none,    none, false}, {         "NOP",    none,    none, false}, {          "NOP",    none, none, false}, {          "NOP",    none, none, false}, {         "NOP",    none, none, false}, {         "NOP",    none, none, false}, {         "NOP",    none,    none, false}, {         "NOP",    none,    none, false}, {         "NOP",    none,    none, false}, {         "NOP",    none,    none, false},
			{     "JO {}{}",    rel8, none, false}, {    "JNO {}{}",    rel8, none, false}, {     "JC {}{}",    rel8,    none, false}, {    "JNC {}{}",    rel8,    none, false}, {     "JZ {}{}",    rel8, none, false}, {    "JNZ {}{}",    rel8, none, false}, {    "JNA {}{}",    rel8,    none, false}, {     "JA {}{}",    rel8,    none, false}, {      "JS {}{}",    rel8, none, false}, {     "JNS {}{}",    rel8, none, false}, {     "JP {}{}",    rel8, none, false}, {    "JNP {}{}",    rel8, none, false}, {     "JL {}{}",    rel8,    none, false}, {    "JNL {}{}",    rel8,    none, false}, {    "JLE {}{}",    rel8,    none, false}, {     "JG {}{}",    rel8,    none, false},
			{         "GRP",    none, none,  true}, {         "GRP",    none, none,  true}, {         "GRP",    none,    none,  true}, {         "GRP",    none,    none,  true}, { "TEST {}, {}",     rm8,   r8,  true}, { "TEST {}, {}",    rm16,  r16,  true}, { "XCHG {}, {}",      r8,     rm8,  true}, { "XCHG {}, {}",     r16,    rm16,  true}, {   "MOV {}, {}",     rm8,   r8,  true}, {   "MOV {}, {}",    rm16,  r16,  true}, {   "MOV {}, {}",     r8,  rm8,  true}, {   "MOV {}, {}",    r16, rm16,  true}, {         "GRP",    none,    none,  true}, {  "LEA {}, {}",     r16,    rm16,  true}, {         "GRP",    none,    none,  true}, {         "GRP",    none,    none,  true},
			{         "NOP",    none, none, false}, {     "XCHG CX", implied, none, false}, {     "XCHG DX", implied,    none, false}, {     "XCHG BX", implied,    none, false}, {     "XCHG SP", implied, none, false}, {     "XCHG BP", implied, none, false}, {     "XCHG SI", implied,    none, false}, {     "XCHG DI", implied,    none, false}, {          "CBW",    none, none, false}, {          "CWD",    none, none, false}, {   "CALL {}{}", segAddr, none, false}, {        "WAIT",    none, none, false}, {       "PUSHF",    none,    none, false}, {        "POPF",    none,    none, false}, {        "SAHF",    none,    none, false}, {        "LAHF",    none,    none, false},
			{"MOV AL, {}{}", implied, addr, false}, {"MOV AX, {}{}", implied, addr, false}, {"MOV {}{}, AL",    addr, implied, false}, {"MOV {}{}, AX",    addr, implied, false}, {       "MOVSB",    none, none, false}, {       "MOVSW",    none, none, false}, {       "CMPSB",    none,    none, false}, {       "CMPSW",    none,    none, false}, {"TEST AL, {}{}", implied,   i8, false}, {"TEST AX, {}{}", implied,  i16, false}, {       "STOSB",    none, none, false}, {       "STOSW",    none, none, false}, {       "LODSB",    none,    none, false}, {       "LODSW",    none,    none, false}, {       "SCASB",    none,    none, false}, {       "SCASW",    none,    none, false},
			{"MOV AL, {}{}", implied,   i8, false}, {"MOV CL, {}{}", implied,   i8, false}, {"MOV DL, {}{}", implied,      i8, false}, {"MOV BL, {}{}", implied,      i8, false}, {"MOV AH, {}{}", implied,   i8, false}, {"MOV CH, {}{}", implied,   i8, false}, {"MOV DH, {}{}", implied,      i8, false}, {"MOV BH, {}{}", implied,      i8, false}, { "MOV AX, {}{}", implied,  i16, false}, { "MOV CX, {}{}", implied,  i16, false}, {"MOV DX, {}{}", implied,  i16, false}, {"MOV BX, {}{}", implied,  i16, false}, {"MOV SP, {}{}", implied,     i16, false}, {"MOV BP, {}{}", implied,     i16, false}, {"MOV SI, {}{}", implied,     i16, false}, {"MOV DI, {}{}", implied,     i16, false},
			{         "NOP",    none, none, false}, {         "NOP",    none, none, false}, {    "RET {}{}",     i16,    none, false}, {         "RET",    none,    none, false}, {  "LES {}, {}",     r16, rm16,  true}, {  "LDS {}, {}",     r16, rm16,  true}, {         "GRP",    none,    none,  true}, {         "GRP",    none,    none,  true}, {          "NOP",    none, none, false}, {          "NOP",    none, none, false}, {   "RETF {}{}",     i16, none, false}, {        "RETF",    none, none, false}, {       "INT 3", implied,    none, false}, {    "INT {}{}",      i8,    none, false}, {        "INTO",    none,    none, false}, {        "IRET",    none,    none, false},
			{         "GRP",    none, none,  true}, {         "GRP",    none, none,  true}, {         "GRP",    none,    none,  true}, {         "GRP",    none,    none,  true}, {         "AAM",    none, none, false}, {         "AAD",    none, none, false}, {         "NOP",    none,    none, false}, {        "XLAT",    none,    none, false}, {          "ESC",    none, none, false}, {          "ESC",    none, none, false}, {         "ESC",    none, none, false}, {         "ESC",    none, none, false}, {         "ESC",    none,    none, false}, {         "ESC",    none,    none, false}, {         "ESC",    none,    none, false}, {         "ESC",    none,    none, false},
			{ "LOOPNZ {}{}",    rel8, none, false}, {  "LOOPZ {}{}",    rel8, none, false}, {   "LOOP {}{}",    rel8,    none, false}, {   "JCXZ {}{}",    rel8, implied, false}, { "IN AL, {}{}", implied,   i8, false}, { "IN AX, {}{}", implied,   i8, false}, {  "OUT {}, AL",      i8, implied, false}, {  "OUT {}, AX",      i8, implied, false}, {      "CALL {}",    addr, none, false}, {       "JMP {}",    addr, none, false}, {    "JMP {}{}", segAddr, none, false}, {    "JMP {}{}",    rel8, none, false}, {   "IN AL, DX", implied, implied, false}, {   "IN AX, DX", implied, implied, false}, {  "OUT DX, AL", implied, implied, false}, {  "OUT DX, AX", implied, implied, false},
			{        "LOCK",    none, none, false}, {         "NOP",    none, none, false}, {       "REPNZ",    none,    none, false}, {         "REP",    none,    none, false}, {         "HLT",    none, none, false}, {         "CMC",    none, none, false}, {         "GRP",    none,    none,  true}, {         "GRP",    none,    none,  true}, {          "CLC",    none, none, false}, {          "STC",    none, none, false}, {         "CLI",    none, none, false}, {         "STI",    none, none, false}, {         "CLD",    none,    none, false}, {         "STD",    none,    none, false}, {         "GRP",    none,    none,  true}, {         "GRP",    none,    none,  true}
	}};

	constexpr std::array<const Instr, 136> GroupinstrTable =
	{{
		{  "ADD {}, {}",     rm8,      i8}, {   "OR {}, {}",     rm8,      i8}, {  "ADC {}, {}",     rm8,      i8}, {  "SBB {}, {}",     rm8,      i8}, {"AND {}, {}",  rm8,      i8}, {"SUB {}, {}",     rm8,      i8}, {"XOR {}, {}",  rm8,   i8}, {"CMP {}, {}",  rm8,      i8},
		{  "ADD {}, {}",    rm16,     i16}, {   "OR {}, {}",    rm16,     i16}, {  "ADC {}, {}",    rm16,     i16}, {  "SBB {}, {}",    rm16,     i16}, {"AND {}, {}", rm16,     i16}, {"SUB {}, {}",    rm16,     i16}, {"XOR {}, {}", rm16,  i16}, {"CMP {}, {}", rm16,     i16},
		{  "ADD {}, {}",     rm8,      i8}, {   "OR {}, {}",     rm8,      i8}, {  "ADC {}, {}",     rm8,      i8}, {  "SBB {}, {}",     rm8,      i8}, {"AND {}, {}",  rm8,      i8}, {"SUB {}, {}",     rm8,      i8}, {"XOR {}, {}",  rm8,   i8}, {"CMP {}, {}",  rm8,      i8},
		{  "ADD {}, {}",    rm16,     se8}, {   "OR {}, {}",    rm16,     se8}, {  "ADC {}, {}",    rm16,     se8}, {  "SBB {}, {}",    rm16,     se8}, {"AND {}, {}", rm16,     se8}, {"SUB {}, {}",    rm16,     se8}, {"XOR {}, {}", rm16,  se8}, {"CMP {}, {}", rm16,     se8},
		{  "MOV {}, ES",    rm16, implied}, {  "MOV {}, CS",    rm16, implied}, {  "MOV {}, SS",    rm16, implied}, {  "MOV {}, DS",    rm16, implied}, {       "NOP", none,    none}, {       "NOP",    none,    none}, {       "NOP", none, none}, {       "NOP", none,    none},
		{"MOV ES, {}{}", implied,    rm16}, {"MOV CS, {}{}", implied,    rm16}, {"MOV SS, {}{}", implied,    rm16}, {"MOV DS, {}{}", implied,    rm16}, {       "NOP", none,    none}, {       "NOP",    none,    none}, {       "NOP", none, none}, {       "NOP", none,    none},
		{      "POP {}",    rm16,    none}, {         "NOP",    none,    none}, {         "NOP",    none,    none}, {         "NOP",    none,    none}, {       "NOP", none,    none}, {       "NOP",    none,    none}, {       "NOP", none, none}, {       "NOP", none,    none},
		{  "MOV {}, {}",     rm8,      i8}, {         "NOP",    none,    none}, {         "NOP",    none,    none}, {         "NOP",    none,    none}, {       "NOP", none,    none}, {       "NOP",    none,    none}, {       "NOP", none, none}, {       "NOP", none,    none},
		{  "MOV {}, {}",    rm16,     i16}, {         "NOP",    none,    none}, {         "NOP",    none,    none}, {         "NOP",    none,    none}, {       "NOP", none,    none}, {       "NOP",    none,    none}, {       "NOP", none, none}, {       "NOP", none,    none},
		{   "ROL {}, 1",     rm8, implied}, {   "ROR {}, 1",     rm8, implied}, {   "RCL {}, 1",     rm8, implied}, {   "RCR {}, 1",     rm8, implied}, { "SHL {}, 1",  rm8, implied}, { "SHR {}, 1",     rm8, implied}, {       "NOP", none, none}, { "SAR {}, 1",  rm8, implied},
		{   "ROL {}, 1",    rm16, implied}, {   "ROR {}, 1",    rm16, implied}, {   "RCL {}, 1",    rm16, implied}, {   "RCR {}, 1",    rm16, implied}, { "SHL {}, 1", rm16, implied}, { "SHR {}, 1",    rm16, implied}, {       "NOP", none, none}, { "SAR {}, 1", rm16, implied},
		{  "ROL {}, CL",     rm8, implied}, {  "ROR {}, CL",     rm8, implied}, {  "RCL {}, CL",     rm8, implied}, {  "RCR {}, CL",     rm8, implied}, {"SHL {}, CL",  rm8, implied}, {"SHR {}, CL",     rm8, implied}, {       "NOP", none, none}, {"SAR {}, CL",  rm8, implied},
		{  "ROL {}, CL",    rm16, implied}, {  "ROR {}, CL",    rm16, implied}, {  "RCL {}, CL",    rm16, implied}, {  "RCR {}, CL",    rm16, implied}, {"SHL {}, CL", rm16, implied}, {"SHR {}, CL",    rm16, implied}, {       "NOP", none, none}, {"SAR {}, CL", rm16, implied},
		{ "TEST {}, {}",     rm8,      i8}, {         "NOP",    none,    none}, {      "NOT {}",     rm8,    none}, {      "NEG {}",     rm8,    none}, {    "MUL {}",  rm8,    none}, {   "IMUL {}",     rm8,    none}, {    "DIV {}",  rm8, none}, {   "IDIV {}",  rm8,    none},
		{ "TEST {}, {}",    rm16,     i16}, {         "NOP",    none,    none}, {      "NOT {}",    rm16,    none}, {      "NEG {}",    rm16,    none}, {    "MUL {}", rm16,    none}, {   "IMUL {}",    rm16,    none}, {    "DIV {}", rm16, none}, {   "IDIV {}", rm16,    none},
		{      "INC {}",     rm8,    none}, {      "DEC {}",     rm8,    none}, {         "NOP",    none,    none}, {         "NOP",    none,    none}, {       "NOP", none,    none}, {       "NOP",    none,    none}, {       "NOP", none, none}, {       "NOP", none,    none},
		{      "INC {}",    rm16,    none}, {      "DEC {}",    rm16,    none}, {     "CALL {}",    rm16,    none}, {     "CALL {}", segAddr,    none}, {    "JMP {}", rm16,    none}, {    "JMP {}", segAddr,    none}, {   "PUSH {}", addr, none}, {       "NOP", none,    none}
	}};

	constexpr std::array<const std::array<const char*, 8>, 5> ModrmLookUpTable = {{
		{           "[BX + SI]",            "[BX + DI]",            "[BP + SI]",            "[BP + DI]",            "[SI]",            "[DI]",      "[0x{:04X}]", "[BX]"},
		{       "[BX + SI{:+}]",        "[BX + DI{:+}]",        "[BP + SI{:+}]",        "[BP + DI{:+}]",        "[SI{:+}]",        "[DI{:+}]",        "[BP{:+}]", "[BX{:+}]"},
		{"[BX + SI + 0x{:04X}]", "[BX + DI + 0x{:04X}]", "[BP + SI + 0x{:04X}]", "[BP + DI + 0x{:04X}]", "[SI + 0x{:04X}]", "[DI + 0x{:04X}]", "[BP + 0x{:04X}]", "[BX + 0x{:04X}]"},
		{"AL", "CL", "DL", "BL", "AH", "CH", "DH", "BH"},
		{"AX", "CX", "DX", "BX", "SP", "BP", "SI", "DI"}
	}};

	Disassembler::Disassembler(const i8086::MemoryBus* bus) : mBus(bus)
	{
		mOperandGetters.insert({
			{    none, &Disassembler::GetNoneOperand },
			{ implied, &Disassembler::GetImpliedOperand },
			{    rel8, &Disassembler::GetRelativeOperand },
			{     se8, &Disassembler::GetSignExtendedOperand },
			{      i8, &Disassembler::GetI8Operand },
			{     i16, &Disassembler::GetI16Operand },
			{     rm8, &Disassembler::GetRm8Operand },
			{    rm16, &Disassembler::GetRm16Operand },
			{      r8, &Disassembler::GetR8Operand },
			{     r16, &Disassembler::GetR16Operand },
			{    addr, &Disassembler::GetAddressOperand },
			{ segAddr, &Disassembler::GetRegisterOverrideOperand }
		});
	}

	void Disassembler::Disassembly(u32 StartAddress, u32 EndAddress)
	{
		IP = StartAddress;
		mMaxInstrBytesCount = 0;
		
		disassembledInstructions = std::vector<DisassembledInstruction>();

		while (IP < EndAddress)
		{
			mTempInstruction.Address = IP;

			Decode(Fetch());

			disassembledInstructions.push_back(mTempInstruction);
			
			if (mTempInstruction.Bytes.size() > mMaxInstrBytesCount)
			{
				mMaxInstrBytesCount = static_cast<u8>(mTempInstruction.Bytes.size());
			}

			mTempInstruction.Clear();
		}

	}

	static inline bool IsKeyword(const std::string& str)
	{
		for (const auto& keyword : Keywords)
		{
			if (str == keyword)
			{
				return true;
			}
		}

		return false;
	}

	static inline bool IsRegister(const std::string& str)
	{
		for (const auto& reg : Registers)
		{
			if (str == reg)
			{
				return true;
			}
		}

		return false;
	}

	u8 Disassembler::Fetch()
	{

		mTempInstruction.Bytes.push_back(mBus->Read(IP, 0x0000, 8));

		return mBus->Read(IP++, 0x0000, 8);
	
	}

	u16 Disassembler::Fetch16()
	{
		const u8 lowByte = Fetch();
		const u8 highByte = Fetch();

		return (highByte << 8) | lowByte;
	}

	void Disassembler::Decode(u8 opcode)
	{

		if (InstrTable[opcode].hasModRM)
		{

			const u8 modrm = Fetch();

			mMod = (modrm & 0xc0) >> 6;
			mReg = (modrm & 0x38) >> 3;
			mRm  =  modrm & 0x07;

		}

		u8 index{};

		switch (opcode)
		{
		case 0x80:
			index = 8 * 0;
			break;

		case 0x81:
			index = 8 * 1;
			break;

		case 0x82:
			index = 8 * 2;
			break;

		case 0x83:
			index = 8 * 3;
			break;

		case 0x8C:
			index = 8 * 4;
			break;

		case 0x8E:
			index = 8 * 5;
			break;

		case 0x8F:
			index = 8 * 6;
			break;

		case 0xC6:
			index = 8 * 7;
			break;

		case 0xC7:
			index = 8 * 8;
			break;

		case 0xD0:
			index = 8 * 9;
			break;

		case 0xD1:
			index = 8 * 10;
			break;

		case 0xD2:
			index = 8 * 11;
			break;

		case 0xD3:
			index = 8 * 12;
			break;

		case 0xF6:
			index = 8 * 13;
			break;

		case 0xF7:
			index = 8 * 14;
			break;

		case 0xFE:
			index = 8 * 15;
			break;

		case 0xFF:
			index = 8 * 16;
			break;

		default:
			Tokenizer(GetInstrLine(InstrTable[opcode]), mTempInstruction.Tokens);
			return;
		}

		index += mReg;

		Tokenizer(GetInstrLine(GroupinstrTable[index]), mTempInstruction.Tokens);
	}

	std::string Disassembler::GetInstrLine(const Instr& instr)
	{
		return std::vformat(instr.mnemonic, std::make_format_args(ParseOperand(instr.operand1), ParseOperand(instr.operand2)));
	}

	const std::string Disassembler::ParseOperand(const OperandType& operandType)
	{
		return (this->*mOperandGetters[operandType])();
	}

	std::string Disassembler::GetRmAddressOperand()
	{

		std::format_args args = std::make_format_args();
		
		if (mMod == 0)
		{
			if (mRm != 6)
			{
				return ModrmLookUpTable[mMod][mRm];
			}

			const u16 address = Fetch16();
			args = std::make_format_args(address);
		}

		else if (mMod == 1)
		{
			const s16 signExtendedByte = static_cast<s8>(Fetch());
			args = std::make_format_args(signExtendedByte);
		}

		else if (mMod == 2)
		{
			const u16 address = Fetch16();
			args = std::make_format_args(address);
		}

		return std::vformat(ModrmLookUpTable[mMod][mRm], args);
	
	}

	std::string Disassembler::GetNoneOperand()
	{
		return "";
	}

	std::string Disassembler::GetImpliedOperand()
	{
		return "";
	}

	std::string Disassembler::GetRelativeOperand()
	{
		return std::to_string(static_cast<s8>(Fetch()));
	}

	std::string Disassembler::GetSignExtendedOperand()
	{
		const s16 signExtendedByte = static_cast<s8>(Fetch());
		return std::to_string(signExtendedByte);
	}

	std::string Disassembler::GetI8Operand()
	{
		return std::to_string(Fetch());
	}

	std::string Disassembler::GetI16Operand()
	{
		return std::to_string(Fetch16());
	}

	std::string Disassembler::GetRm8Operand()
	{
		
		if (mMod != 3)
		{
			return "BYTE PTR " + GetRmAddressOperand();
		}

		return ModrmLookUpTable[3][mRm];
	}

	std::string Disassembler::GetRm16Operand()
	{
		if (mMod != 3)
		{
			return "WORD PTR " + GetRmAddressOperand();
		}

		return ModrmLookUpTable[4][mRm];
	}

	std::string Disassembler::GetR8Operand()
	{
		return ModrmLookUpTable[3][mReg];
	}

	std::string Disassembler::GetR16Operand()
	{
		return ModrmLookUpTable[4][mReg];
	}

	std::string Disassembler::GetRegisterOverrideOperand()
	{
		const u16 address = Fetch16();
		const u16 segment = Fetch16();

		return std::format("0x{:04X}", segment) + ":" + std::format("0x{:04X}", address);
	}

	std::string Disassembler::GetAddressOperand()
	{
		const u16 address = Fetch16();
		return std::format("0x{:04X}", address);
	}

	void Disassembler::Tokenizer(const std::string& line, std::vector<Token>& tokens) const
	{
		size_t i = 0;
		size_t lineSize = line.size();

		tokens.clear();

		auto peek = [&](size_t offset = 0) -> char {
			return (i + offset < lineSize) ? line[i + offset] : '\0';
		};

		while (i < lineSize)
		{

			if (std::isspace(peek()))
			{
				++i;
			}

			if (std::isalpha(peek()))
			{
				const size_t firstCharPos = i;

				while (std::isalpha(peek())) i++;

				const std::string word = line.substr(firstCharPos, i - firstCharPos);

				if (IsKeyword(word))
				{
					tokens.push_back({ word, TokenType::tKeyword, true });
				}

				else if (IsRegister(word))
				{
					tokens.push_back({ word, TokenType::tRegister });
				}

				else
				{
					tokens.push_back({ word, TokenType::tIdentifier, true });
				}
			}

			if (isdigit(peek()))
			{

				const size_t firstCharPos = i;

				if (peek() == '0' && (peek(1) == 'x' || peek(1) == 'X'))
				{
					i += 2;

					while (std::isxdigit(peek())) i++;
				}

				else
				{
					while (std::isdigit(peek())) i++;
				}

				const std::string number = line.substr(firstCharPos, i - firstCharPos);

				tokens.push_back({ number, TokenType::tNumber });

			}

			switch (peek())
			{

			case ',':
				tokens.push_back({ ",", TokenType::tComma, true });
				break;

			case ':':
				tokens.push_back({ ":", TokenType::tColon });
				break;

			case '[':
				tokens.push_back({ "[", TokenType::tLBracket });
				break;

			case ']':
				tokens.push_back({ "]", TokenType::tRBracket });
				break;

			case '+':
				tokens.push_back({ "+", TokenType::tPlus });
				break;

			case '-':
				tokens.push_back({ "-", TokenType::tMinus });
				break;

			default:
				break;
			}

			++i;
		}

	}

} // namespace disassembler