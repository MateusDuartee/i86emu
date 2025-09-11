// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#include "I8086.hpp"
#include "Instructions.hpp"

#include <fstream>
#include <algorithm>

namespace i8086
{

	I8086::I8086(MemoryBus* const bus) : mBus(bus)
	{
		SP = 0xFFFE;

		using I86 = I8086;

		mOpcodeTable = {
			// 0x00 - 0x0F
			&I86::ADD_RM_R,
			&I86::ADD_RM_R,
			&I86::ADD_R_RM,
			&I86::ADD_R_RM,
			&I86::ADD_AL_I8,
			&I86::ADD_AX_i16,
			&I86::PUSH_ES,
			&I86::POP_ES,
			&I86::OR_RM_R,
			&I86::OR_RM_R,
			&I86::OR_R_RM,
			&I86::OR_R_RM,
			&I86::OR_AL_I8,
			&I86::OR_AX_i16,
			&I86::PUSH_CS,
			&I86::POP_CS,

			// 0x10 - 0x1F
			&I86::ADC_RM_R,
			&I86::ADC_RM_R,
			&I86::ADC_R_RM,
			&I86::ADC_R_RM,
			&I86::ADC_AL_I8,
			&I86::ADC_AX_I16,
			&I86::PUSH_SS,
			&I86::POP_SS,
			&I86::SBB_RM_R,
			&I86::SBB_RM_R,
			&I86::SBB_R_RM,
			&I86::SBB_R_RM,
			&I86::SBB_AL_I8,
			&I86::SBB_AX_I16,
			&I86::PUSH_DS,
			&I86::POP_DS,

			// 0x20 - 0x2F
			&I86::AND_RM_R,
			&I86::AND_RM_R,
			&I86::AND_R_RM,
			&I86::AND_R_RM,
			&I86::AND_AL_I8,
			&I86::AND_AX_I16,
			&I86::ES_OVERRIDE,
			&I86::DAA,
			&I86::SUB_RM_R,
			&I86::SUB_RM_R,
			&I86::SUB_R_RM,
			&I86::SUB_R_RM,
			&I86::SUB_AL_I8,
			&I86::SUB_AX_I16,
			&I86::CS_OVERRIDE,
			&I86::DAS,

			// 0x30 - 0x3F
			&I86::XOR_RM_R,
			&I86::XOR_RM_R,
			&I86::XOR_R_RM,
			&I86::XOR_R_RM,
			&I86::XOR_AL_I8,
			&I86::XOR_AX_I16,
			&I86::SS_OVERRIDE,
			&I86::AAA,
			&I86::CMP_RM_R,
			&I86::CMP_RM_R,
			&I86::CMP_R_RM,
			&I86::CMP_R_RM,
			&I86::CMP_AL_I8,
			&I86::CMP_AX_I16,
			&I86::DS_OVERRIDE,
			&I86::AAS,

			// 0x40 - 0x4F
			&I86::INC_AX,
			&I86::INC_CX,
			&I86::INC_DX,
			&I86::INC_BX,
			&I86::INC_SP,
			&I86::INC_BP,
			&I86::INC_SI,
			&I86::INC_DI,
			&I86::DEC_AX,
			&I86::DEC_CX,
			&I86::DEC_DX,
			&I86::DEC_BX,
			&I86::DEC_SP,
			&I86::DEC_BP,
			&I86::DEC_SI,
			&I86::DEC_DI,

			// 0x50 - 0x5F
			&I86::PUSH_AX,
			&I86::PUSH_CX,
			&I86::PUSH_DX,
			&I86::PUSH_BX,
			&I86::PUSH_SP,
			&I86::PUSH_BP,
			&I86::PUSH_SI,
			&I86::PUSH_DI,
			&I86::POP_AX,
			&I86::POP_CX,
			&I86::POP_DX,
			&I86::POP_BX,
			&I86::POP_SP,
			&I86::POP_BP,
			&I86::POP_SI,
			&I86::POP_DI,

			// 0x60 - 0x6F
			&I86::NOP,
			&I86::NOP,
			&I86::NOP,
			&I86::NOP,
			&I86::NOP,
			&I86::NOP,
			&I86::NOP,
			&I86::NOP,
			&I86::NOP,
			&I86::NOP,
			&I86::NOP,
			&I86::NOP,
			&I86::NOP,
			&I86::NOP,
			&I86::NOP,
			&I86::NOP,

			// 0x70 - 0x7F
			&I86::JO_REL8,
			&I86::JNO_REL8,
			&I86::JNAE_JB_JC_REL8,
			&I86::JAE_JNB_JNC_REL8,
			&I86::JE_JZ_REL8,
			&I86::JNE_JNZ_REL8,
			&I86::JBE_JNA_REL8,
			&I86::JNBE_JA_REL8,
			&I86::JS_REL8,
			&I86::JNS_REL8,
			&I86::JP_JPE_REL8,
			&I86::JNP_JPO_REL8,
			&I86::JL_JNGE_REL8,
			&I86::JGE_JNL_REL8,
			&I86::JLE_JNG_REL8,
			&I86::JG_JNLE_REL8,

			// 0x80 - 0x8F
			&I86::GROUP0,
			&I86::GROUP1,
			&I86::GROUP2,
			&I86::GROUP3,
			&I86::TEST_RM_R,
			&I86::TEST_RM_R,
			&I86::XCHG_R_RM,
			&I86::XCHG_R_RM,
			&I86::MOV_RM_R,
			&I86::MOV_RM_R,
			&I86::MOV_R_RM,
			&I86::MOV_R_RM,
			&I86::GROUP4,
			&I86::LEA_R16_RM16,
			&I86::GROUP5,
			&I86::GROUP6,

			// 0x90 - 0x9F
			&I86::XCHG_AX,
			&I86::XCHG_CX,
			&I86::XCHG_DX,
			&I86::XCHG_BX,
			&I86::XCHG_SP,
			&I86::XCHG_BP,
			&I86::XCHG_SI,
			&I86::XCHG_DI,
			&I86::CBW,
			&I86::CWD,
			&I86::CALL_FAR,
			&I86::WAIT,
			&I86::PUSHF,
			&I86::POPF,
			&I86::SAHF,
			&I86::LAHF,

			// 0xA0 - 0xAF
			&I86::MOV_AL_MOFFS16,
			&I86::MOV_AX_MOFFS16,
			&I86::MOV_MOFFS16_AL,
			&I86::MOV_MOFFS16_AX,
			&I86::MOVSB,
			&I86::MOVSW,
			&I86::CMPSB,
			&I86::CMPSW,
			&I86::TEST_AL_I8,
			&I86::TEST_AX_I16,
			&I86::STOSB,
			&I86::STOSW,
			&I86::LODSB,
			&I86::LODSW,
			&I86::SCASB,
			&I86::SCASW,

			// 0xB0 - 0xBF
			&I86::MOV_AL_I8,
			&I86::MOV_CL_I8,
			&I86::MOV_DL_I8,
			&I86::MOV_BL_I8,
			&I86::MOV_AH_I8,
			&I86::MOV_CH_I8,
			&I86::MOV_DH_I8,
			&I86::MOV_BH_I8,
			&I86::MOV_AX_I16,
			&I86::MOV_CX_I16,
			&I86::MOV_DX_I16,
			&I86::MOV_BX_I16,
			&I86::MOV_SP_I16,
			&I86::MOV_BP_I16,
			&I86::MOV_SI_I16,
			&I86::MOV_DI_I16,

			// 0xC0 - 0xCF
			&I86::NOP,
			&I86::NOP,
			&I86::RET_I16,
			&I86::RET,
			&I86::LES_R16_M16,
			&I86::LDS_R16_M16,
			&I86::GROUP7,
			&I86::GROUP8,
			&I86::NOP,
			&I86::NOP,
			&I86::RETF_I16,
			&I86::RETF,
			&I86::INT3,
			&I86::INT_I8,
			&I86::INTO,
			&I86::IRET,

			// 0xD0 - 0xDF
			&I86::GROUP9,
			&I86::GROUP10,
			&I86::GROUP11,
			&I86::GROUP12,
			&I86::AAM,
			&I86::AAD,
			&I86::NOP,
			&I86::XLAT,
			&I86::ESC,
			&I86::ESC,
			&I86::ESC,
			&I86::ESC,
			&I86::ESC,
			&I86::ESC,
			&I86::ESC,
			&I86::ESC,

			// 0xE0 - 0xEF
			&I86::LOOPNE_LOOPNZ_REL8,
			&I86::LOOPE_LOOPZ_REL8,
			&I86::LOOP_REL8,
			&I86::JCXZ_REL8,
			&I86::IN_AL_I8,
			&I86::IN_AX_I8,
			&I86::OUT_I8_AL,
			&I86::OUT_I8_AX,
			&I86::CALL_REL16,
			&I86::JMP_REL16,
			&I86::JMP_FAR,
			&I86::JMP_REL8,
			&I86::IN_AL_DX,
			&I86::IN_AX_DX,
			&I86::OUT_DX_AL,
			&I86::OUT_DX_AX,

			// 0xF0 - 0xFF
			&I86::LOCK,
			&I86::NOP,
			&I86::REPNE_REPNZ,
			&I86::REP_REPE_REPZ,
			&I86::HLT,
			&I86::CMC,
			&I86::GROUP13,
			&I86::GROUP14,
			&I86::CLC,
			&I86::STC,
			&I86::CLI,
			&I86::STI,
			&I86::CLD,
			&I86::STD,
			&I86::GROUP15,
			&I86::GROUP16
		};
	}

	u16 I8086::Fetch(u8 size)
	{
		const u16 fetchedData = mBus->Read(IP.X, CS, size);

		IP.X += size / 8;

		return fetchedData;
	}

	void I8086::Cycles(u8 count)
	{
		if (!mHalted)
		{
			ExecuteInstructions(count);
		}

		if (mPendingInterruptFlag)
		{
			SF.I = true;
			mPendingInterruptFlag = false;
		}
	}

	void I8086::FetchModrm()
	{
		const u8 modrmByte = Fetch();

		Mod = (modrmByte & 0xC0) >> 6;
		Reg = (modrmByte & 0x38) >> 3;
		Rm  =  modrmByte & 0x07;
	}

	void I8086::HandleREP()
	{
		const u8 opcode = Fetch();

		OperandSize = (opcode & 1) * 8 + 8;

		const u8 maskedOpcode = opcode & 0xFE;

		// the loop can be exited by a condition if the instruction is SCAS or CMPS
		const bool useZStopCondition = (maskedOpcode == 0xA6 || maskedOpcode == 0xAE);
		const bool zStopCondition = opcode & 1;

		while (C.X)
		{
			(this->*mOpcodeTable[opcode])();
			C.X--;

			if (useZStopCondition && (SF.Z == zStopCondition))
			{
				break;
			}
		}

		mREP = false;
	}

	void I8086::ExecuteInstructions(u8 count)
	{

		u8 opcode{};

		for (u8 i = 0; i < count; ++i)
		{
			opcode = Fetch();

			OperandSize = (opcode & 1) * 8 + 8;

			(this->*mOpcodeTable[opcode])();

			if (mREP)
			{
				HandleREP();
			}

		}

	}

	void I8086::GetInternalState(CPUState& state) const
	{
		state = CPUState(*this);
	}

	void I8086::SetBreakpoint(u32 address, bool state)
	{
		const auto it = std::find(mBreakpoints.begin(), mBreakpoints.end(), address);
		
		const bool isInBreakpoints = (it != mBreakpoints.end());
		
		if (!isInBreakpoints && state)
		{
			mBreakpoints.push_back(address);
		}

		else if (isInBreakpoints && !state)
		{
			mBreakpoints.erase(it);
		}
	}

	void I8086::CalculateEffectiveAddress()
	{

		if (Mod == 0 && Rm == 6)
		{
			EA = Fetch(WORD);
			mSeg = DS;

			ApplyRegisterOverrideIfNeeded();

			return;
		}

		else if (Mod == 3)
		{

			mSeg = DS;

			ApplyRegisterOverrideIfNeeded();

			return;

		}

		switch (Rm)
		{

		case 0:

			EA = B + SI;
			mSeg = DS;

			ApplyRegisterOverrideIfNeeded();

			break;

		case 1:

			EA = B + DI;
			mSeg = DS;

			ApplyRegisterOverrideIfNeeded();

			break;

		case 2:

			EA = BP + SI;
			mSeg = SS;

			break;

		case 3:

			EA = BP + DI;
			mSeg = SS;

			break;

		case 4:

			EA = SI.X;
			mSeg = DS;

			ApplyRegisterOverrideIfNeeded();

			break;

		case 5:
			
			EA = DI.X;
			mSeg = DS;

			ApplyRegisterOverrideIfNeeded();
			
			break;

		case 6:

			EA = BP.X;
			mSeg = SS;

			break;

		case 7:

			EA = B.X;
			mSeg = DS;

			ApplyRegisterOverrideIfNeeded();

			break;

		default:
			break;

		}

		if (Mod == 1)
		{
			const s16 SignExtendedDisp = static_cast<s8>(Fetch());

			EA += SignExtendedDisp;

		}

		else if (Mod == 2)
		{
			const u16 displacement = Fetch(WORD);

			EA += displacement;
		}
	}

	void I8086::ApplyRegisterOverrideIfNeeded()
	{
		if (mRegisterOverride.pending)
		{
			mSeg = mRegisterOverride.segment;
			mRegisterOverride.pending = false;
		}
	}

	void I8086::SetReg(u8 reg, u16 value, u8 size)
	{
		if (size == 8)
		{
			*mRegs8[reg] = value & 0xFF;
			return;
		}

		mRegs16[reg]->X = value;
	}

	u16 I8086::GetReg(u8 reg, u8 size) const
	{
		if (size == 8)
		{
			return *mRegs8[reg];
		}

		return mRegs16[reg]->X;
	}

	u16 I8086::ReadRMOperand(u8 operandSize) const
	{
		if (Mod == 3)
		{
			return GetReg(Rm, operandSize);
		}

		return mBus->Read(EA, mSeg, operandSize);
	}

	void I8086::WriteRMOperand(u16 data, u8 operandSize)
	{
		if (Mod == 3)
		{
			SetReg(Rm, data, operandSize);
			return;
		}

		mBus->Write(EA, data, mSeg, operandSize);
	}

	// NOP
	void I8086::NOP()
	{

	}

	// ADD r/m, r
	void I8086::ADD_RM_R()
	{
		FetchModrm();

		CalculateEffectiveAddress();

		const u16 result = Instr::ADD(ReadRMOperand(OperandSize), GetReg(Reg, OperandSize), this);

		WriteRMOperand(result, OperandSize);
	}

	// ADD r, r/m
	void I8086::ADD_R_RM()
	{
		FetchModrm();

		CalculateEffectiveAddress();

		const u16 result = Instr::ADD(GetReg(Reg, OperandSize), ReadRMOperand(OperandSize), this);

		SetReg(Reg, result, OperandSize);
	}

	// ADD AL, i8
	void I8086::ADD_AL_I8()
	{
		A.L = Instr::ADD(A.L, Fetch(), this);
	}

	// ADD AX, i16
	void I8086::ADD_AX_i16()
	{
		A.X = Instr::ADD(A.X, Fetch(WORD), this);
	}

	// PUSH ES
	void I8086::PUSH_ES()
	{
		Instr::PUSH(ES, this, mBus);
	}

	// POP ES
	void I8086::POP_ES()
	{
		Instr::POP(ES, this, mBus);
	}

	// OR r/m, r
	void I8086::OR_RM_R()
	{
		FetchModrm();
		
		CalculateEffectiveAddress();

		const u16 result = Instr::OR(ReadRMOperand(OperandSize), GetReg(Reg, OperandSize), this);

		WriteRMOperand(result, OperandSize);
	}

	// OR r, r/m
	void I8086::OR_R_RM()
	{
		FetchModrm();

		CalculateEffectiveAddress();

		const u16 result = Instr::OR(GetReg(Reg, OperandSize), ReadRMOperand(OperandSize), this);

		SetReg(Reg, result, OperandSize);
	}

	// OR AL, i8
	void I8086::OR_AL_I8()
	{
		A.L = Instr::OR(A.L, Fetch(), this);
	}

	// OR AX, i16
	void I8086::OR_AX_i16()
	{
		A.X = Instr::OR(A.X, Fetch(WORD), this);
	}

	// PUSH CS - Illegal
	void I8086::PUSH_CS()
	{

	}

	// POP CS - Illegal
	void I8086::POP_CS()
	{

	}


	// ADC r/m, r
	void I8086::ADC_RM_R()
	{
		FetchModrm();

		CalculateEffectiveAddress();

		const u16 result = Instr::ADC(ReadRMOperand(OperandSize), GetReg(Reg, OperandSize), this);

		WriteRMOperand(result, OperandSize);
	}

	// ADC r, r/m
	void I8086::ADC_R_RM()
	{
		FetchModrm();

		CalculateEffectiveAddress();

		const u16 result = Instr::ADC(GetReg(Reg, OperandSize), ReadRMOperand(OperandSize), this);

		SetReg(Reg, result, OperandSize);
	}

	// ADC AL, i8
	void I8086::ADC_AL_I8()
	{
		A.L = Instr::ADC(A.L, Fetch(), this);
	}

	// ADC AX, i16
	void I8086::ADC_AX_I16()
	{
		A.X = Instr::ADC(A.X, Fetch(WORD), this);
	}

	// PUSH SS
	void I8086::PUSH_SS()
	{
		Instr::PUSH(SS, this, mBus);
	}

	// POP SS
	void I8086::POP_SS()
	{
		Instr::POP(SS, this, mBus);
	}

	// SBB r/m8, r8
	void I8086::SBB_RM_R()
	{
		FetchModrm();

		CalculateEffectiveAddress();

		const u16 result = Instr::SBB(ReadRMOperand(OperandSize), GetReg(Reg, OperandSize), this);

		WriteRMOperand(result, OperandSize);

	}

	// SBB r8, r/m8
	void I8086::SBB_R_RM()
	{
		FetchModrm();

		CalculateEffectiveAddress();

		const u16 result = Instr::SBB(GetReg(Reg, OperandSize), ReadRMOperand(OperandSize), this);

		SetReg(Reg, result, OperandSize);

	}

	// SBB AL, i8
	void I8086::SBB_AL_I8()
	{
		A.L = Instr::SBB(A.L, Fetch(), this);
	}

	// SBB AX, i16
	void I8086::SBB_AX_I16()
	{
		A.X = Instr::SBB(A.X, Fetch(WORD), this);
	}

	// PUSH DS
	void I8086::PUSH_DS()
	{
		Instr::PUSH(DS, this, mBus);
	}

	// POP DS
	void I8086::POP_DS()
	{
		Instr::POP(DS, this, mBus);
	}

	// AND r/m8, r8
	void I8086::AND_RM_R()
	{
		FetchModrm();

		CalculateEffectiveAddress();

		const u16 result = Instr::AND(ReadRMOperand(OperandSize), GetReg(Reg, OperandSize), this);

		WriteRMOperand(result, OperandSize);

	}

	// AND r8, r/m8
	void I8086::AND_R_RM()
	{
		FetchModrm();

		CalculateEffectiveAddress();

		const u16 result = Instr::AND(GetReg(Reg, OperandSize), ReadRMOperand(OperandSize), this);

		SetReg(Reg, result, OperandSize);

	}

	// AND AL, i8
	void I8086::AND_AL_I8()
	{
		A.L = Instr::AND(A.L, Fetch(), this);
	}

	// AND AX, i16
	void I8086::AND_AX_I16()
	{
		A.X = Instr::AND(A.X, Fetch(WORD), this);
	}

	void I8086::ES_OVERRIDE()
	{
		Instr::RegisterOverride(ES, this);
	}

	// DAA
	void I8086::DAA()
	{
		Instr::DAA(this);
	}

	// SUB r/m8, r8
	void I8086::SUB_RM_R()
	{
		FetchModrm();

		CalculateEffectiveAddress();

		const u16 result = Instr::SUB(ReadRMOperand(OperandSize), GetReg(Reg, OperandSize), this);

		WriteRMOperand(result, OperandSize);

	}

	// SUB r8, r/m8
	void I8086::SUB_R_RM()
	{
		FetchModrm();

		CalculateEffectiveAddress();

		const u16 result = Instr::SUB(GetReg(Reg, OperandSize), ReadRMOperand(OperandSize), this);

		SetReg(Reg, result, OperandSize);

	}

	// SUB AL, i8
	void I8086::SUB_AL_I8()
	{
		A.L = Instr::SUB(A.L, Fetch(), this);
	}

	// SUB AX, i16
	void I8086::SUB_AX_I16()
	{
		A.X = Instr::SUB(A.X, Fetch(WORD), this);
	}

	void I8086::CS_OVERRIDE()
	{
		Instr::RegisterOverride(CS, this);
	}

	// DAS
	void I8086::DAS()
	{
		Instr::DAS(this);
	}

	// XOR r/m, r
	void I8086::XOR_RM_R()
	{
		FetchModrm();

		CalculateEffectiveAddress();

		const u16 result = Instr::XOR(ReadRMOperand(OperandSize), GetReg(Reg, OperandSize), this);

		WriteRMOperand(result, OperandSize);

	}

	// XOR r8, r/m8
	void I8086::XOR_R_RM()
	{
		FetchModrm();

		CalculateEffectiveAddress();

		const u16 result = Instr::XOR(GetReg(Reg, OperandSize), ReadRMOperand(OperandSize), this);

		SetReg(Reg, result, OperandSize);

	}

	// XOR AL, i8
	void I8086::XOR_AL_I8()
	{
		A.L = Instr::XOR(A.L, Fetch(), this);
	}

	// XOR AX, i16
	void I8086::XOR_AX_I16()
	{
		A.X = Instr::XOR(A.X, Fetch(WORD), this);
	}

	void I8086::SS_OVERRIDE()
	{
		Instr::RegisterOverride(SS, this);
	}

	// AAA
	void I8086::AAA()
	{
		Instr::AAA(this);
	}

	// CMP r/m8, r8
	void I8086::CMP_RM_R()
	{
		FetchModrm();

		CalculateEffectiveAddress();

		Instr::SUB(ReadRMOperand(OperandSize), GetReg(Reg, OperandSize), this);
	}

	// CMP r8, r/m8
	void I8086::CMP_R_RM()
	{
		FetchModrm();

		CalculateEffectiveAddress();

		Instr::SUB(GetReg(Reg, OperandSize), ReadRMOperand(OperandSize), this);
	}

	// CMP AL, i8
	void I8086::CMP_AL_I8()
	{
		Instr::SUB(A.L, Fetch(), this);
	}

	// CMP AX, i16
	void I8086::CMP_AX_I16()
	{
		Instr::SUB(A.X, Fetch(WORD), this);
	}

	void I8086::DS_OVERRIDE()
	{
		Instr::RegisterOverride(DS, this);
	}

	// AAS
	void I8086::AAS()
	{
		Instr::AAS(this);
	}

	// INC AX
	void I8086::INC_AX()
	{
		Instr::INC(A, SF);
	}

	// INC CX
	void I8086::INC_CX()
	{
		Instr::INC(C, SF);
	}

	// INC DX
	void I8086::INC_DX()
	{
		Instr::INC(D, SF);
	}

	// INC BX
	void I8086::INC_BX()
	{
		Instr::INC(B, SF);
	}

	// INC SP
	void I8086::INC_SP()
	{
		Instr::INC(SP, SF);
	}

	// INC BP
	void I8086::INC_BP()
	{
		Instr::INC(BP, SF);
	}

	// INC SI
	void I8086::INC_SI()
	{
		Instr::INC(SI, SF);
	}

	// INC DI
	void I8086::INC_DI()
	{
		Instr::INC(DI, SF);
	}

	// DEC AX
	void I8086::DEC_AX()
	{
		Instr::DEC(A, SF);
	}

	// DEC CX
	void I8086::DEC_CX()
	{
		Instr::DEC(C, SF);
	}

	// DEC DX
	void I8086::DEC_DX()
	{
		Instr::DEC(D, SF);
	}

	// DEC BX
	void I8086::DEC_BX()
	{
		Instr::DEC(B, SF);
	}

	// DEC SP
	void I8086::DEC_SP()
	{
		Instr::DEC(SP, SF);
	}

	// DEC BP
	void I8086::DEC_BP()
	{
		Instr::DEC(BP, SF);
	}

	// DEC SI
	void I8086::DEC_SI()
	{
		Instr::DEC(SI, SF);
	}

	// DEC DI
	void I8086::DEC_DI()
	{
		Instr::DEC(DI, SF);
	}

	// PUSH AX
	void I8086::PUSH_AX()
	{
		Instr::PUSH(A, this, mBus);
	}

	// PUSH CX
	void I8086::PUSH_CX()
	{
		Instr::PUSH(C, this, mBus);
	}

	// PUSH DX
	void I8086::PUSH_DX()
	{
		Instr::PUSH(D, this, mBus);
	}

	// PUSH BX
	void I8086::PUSH_BX()
	{
		Instr::PUSH(B, this, mBus);
	}

	// PUSH SP
	void I8086::PUSH_SP()
	{
		Instr::PUSH(SP, this, mBus);
	}

	// PUSH BP
	void I8086::PUSH_BP()
	{
		Instr::PUSH(BP, this, mBus);
	}

	// PUSH SI
	void I8086::PUSH_SI()
	{
		Instr::PUSH(SI, this, mBus);
	}

	// PUSH DI
	void I8086::PUSH_DI()
	{
		Instr::PUSH(DI, this, mBus);
	}

	// POP AX
	void I8086::POP_AX()
	{
		Instr::POP(A, this, mBus);
	}

	// POP CX
	void I8086::POP_CX()
	{
		Instr::POP(C, this, mBus);
	}

	// POP DX
	void I8086::POP_DX()
	{
		Instr::POP(D, this, mBus);
	}

	// POP BX
	void I8086::POP_BX()
	{
		Instr::POP(B, this, mBus);
	}

	// POP SP
	void I8086::POP_SP()
	{
		Instr::POP(SP, this, mBus);
	}

	// POP BP
	void I8086::POP_BP()
	{
		Instr::POP(BP, this, mBus);
	}

	// POP SI
	void I8086::POP_SI()
	{
		Instr::POP(SI, this, mBus);
	}

	// POP DI
	void I8086::POP_DI()
	{
		Instr::POP(DI, this, mBus);
	}

	// JO rel8
	void I8086::JO_REL8()
	{

		const s8 offset = Fetch();

		if (SF.O)
		{
			IP.X += offset;
		}

	}

	// JNO rel8
	void I8086::JNO_REL8()
	{
		const s8 offset = Fetch();

		if (!SF.O)
		{
			IP.X += offset;
		}

	}

	// JC rel8
	void I8086::JNAE_JB_JC_REL8()
	{
		const s8 offset = Fetch();

		if (SF.C)
		{
			IP.X += offset;
		}

	}

	// JNC rel8
	void I8086::JAE_JNB_JNC_REL8()
	{
		const s8 offset = Fetch();

		if (!SF.C)
		{
			IP.X += offset;
		}

	}

	// JZ rel8
	void I8086::JE_JZ_REL8()
	{
		const s8 offset = Fetch();

		if (SF.Z)
		{
			IP.X += offset;
		}

	}

	// JNZ rel8
	void I8086::JNE_JNZ_REL8()
	{
		const s8 offset = Fetch();

		if (!SF.Z)
		{
			IP.X += offset;
		}

	}

	// JNA rel8
	void I8086::JBE_JNA_REL8()
	{
		const s8 offset = Fetch();

		if (SF.C || SF.Z)
		{
			IP.X += offset;
		}

	}

	// JA rel8
	void I8086::JNBE_JA_REL8()
	{
		const s8 offset = Fetch();

		if (!SF.C && !SF.Z)
		{
			IP.X += offset;
		}

	}

	// JS rel8
	void I8086::JS_REL8()
	{
		const s8 offset = Fetch();

		if (SF.S)
		{
			IP.X += offset;
		}

	}

	// JNS rel8
	void I8086::JNS_REL8()
	{
		const s8 offset = Fetch();

		if (!SF.S)
		{
			IP.X += offset;
		}

	}

	// JP rel8
	void I8086::JP_JPE_REL8()
	{
		const s8 offset = Fetch();

		if (SF.P)
		{
			IP.X += offset;
		}

	}

	// JNP rel8
	void I8086::JNP_JPO_REL8()
	{
		const s8 offset = Fetch();

		if (!SF.P)
		{
			IP.X += offset;
		}

	}

	// JL rel8
	void I8086::JL_JNGE_REL8()
	{
		const s8 offset = Fetch();

		if (SF.S != SF.O)
		{
			IP.X += offset;
		}

	}

	// JNL rel8
	void I8086::JGE_JNL_REL8()
	{
		const s8 offset = Fetch();

		if (SF.S == SF.O)
		{
			IP.X += offset;
		}

	}

	// JNG rel8
	void I8086::JLE_JNG_REL8()
	{
		const s8 offset = Fetch();

		if (SF.Z || (SF.S != SF.O))
		{
			IP.X += offset;
		}

	}

	// JG rel8
	void I8086::JG_JNLE_REL8()
	{
		const s8 offset = Fetch();

		if (!SF.Z && (SF.S == SF.O))
		{
			IP.X += offset;
		}

	}

	// Group 0
	void I8086::GROUP0()
	{
		FetchModrm();

		CalculateEffectiveAddress();

		const u8 op1 = ReadRMOperand(BYTE);
		const u8 op2 = Fetch();

		u8 result{};

		switch (Reg)
		{

		case 0:
			// ADD r/m8, i8
			result = Instr::ADD(op1, op2, this);
			break;

		case 1:
			// OR r/m8, i8
			result = Instr::OR(op1, op2, this);
			break;

		case 2:
			// ADC r/m8, i8
			result = Instr::ADC(op1, op2, this);
			break;

		case 3:
			// SBB r/m8, i8
			result = Instr::SBB(op1, op2, this);
			break;

		case 4:
			// AND r/m8, i8
			result = Instr::AND(op1, op2, this);
			break;

		case 5:
			// SUB r/m8, i8
			result = Instr::SUB(op1, op2, this);
			break;

		case 6:
			// XOR r/m8, i8
			result = Instr::XOR(op1, op2, this);
			break;

		case 7:
			// CMP r/m8, i8
			Instr::SUB(op1, op2, this);
			result = op1;
			return;
		}

		WriteRMOperand(result, BYTE);

	}

	// Group 1
	void I8086::GROUP1()
	{

		FetchModrm();

		CalculateEffectiveAddress();

		const u16 op1 = ReadRMOperand(WORD);
		const u16 op2 = Fetch(WORD);

		u16 result{};

		switch (Reg)
		{
		case 0:
			// ADD r/m16, i16
			result = Instr::ADD(op1, op2, this);
			break;

		case 1:
			// OR r/m16, i16
			result = Instr::OR(op1, op2, this);
			break;

		case 2:
			// ADC r/m16, i16
			result = Instr::ADC(op1, op2, this);
			break;

		case 3:
			// SBB r/m16, i16
			result = Instr::SBB(op1, op2, this);
			break;

		case 4:
			// AND r/m16, i16
			result = Instr::AND(op1, op2, this);
			break;

		case 5:
			// SUB r/m16, i16
			result = Instr::SUB(op1, op2, this);
			break;

		case 6:
			// XOR r/m16, i16
			result = Instr::XOR(op1, op2, this);
			break;

		case 7:
			// CMP r/m16, i16
			Instr::SUB(op1, op2, this);
			result = op1;
			return;

		}

		WriteRMOperand(result, WORD);

	}

	// Group 2 - this is a duplicate of Group 0
	void I8086::GROUP2()
	{
		GROUP0();
	}

	// Group 3 - Same as Group 1 but with 8-bit sign extension
	void I8086::GROUP3()
	{
		FetchModrm();

		CalculateEffectiveAddress();

		const u16 op1 = ReadRMOperand(WORD);
		const s16 op2 = static_cast<s8>(Fetch());

		u16 result{};

		switch (Reg)
		{

		case 0:
			// ADD r/m16, s8
			result = Instr::ADD(op1, op2, this);
			break;

		case 1:
			// OR r/m16, s8
			result = Instr::OR(op1, op2, this);
			break;

		case 2:
			// ADC r/m16, s8
			result = Instr::ADC(op1, op2, this);
			break;

		case 3:
			// SBB r/m16, s8
			result = Instr::SBB(op1, op2, this);
			break;

		case 4:
			// AND r/m16, s8
			result = Instr::AND(op1, op2, this);
			break;

		case 5:
			// SUB r/m16, s8
			result = Instr::SUB(op1, op2, this);
			break;

		case 6:
			// XOR r/m16, s8
			result = Instr::XOR(op1, op2, this);
			break;

		case 7:
			// CMP r/m16, s8
			Instr::SUB(op1, op2, this);
			result = op1;
			return;

		}

		WriteRMOperand(result, WORD);

	}

	// TEST r/m8, r8
	void I8086::TEST_RM_R()
	{
		FetchModrm();

		CalculateEffectiveAddress();

		const u8 op1 = ReadRMOperand(BYTE);
		const u8 op2 = GetReg(Reg, BYTE);

		Instr::AND(op1, op2, this);

	}

	// XCHG R, R/M
	void I8086::XCHG_R_RM()
	{
		FetchModrm();

		CalculateEffectiveAddress();

		const u16 temp = ReadRMOperand(OperandSize);
		const u16 regValue = GetReg(Reg, OperandSize);

		if (Mod != 3)
		{
			mBus->Write(EA, regValue, mSeg, OperandSize);
			SetReg(Reg, temp, OperandSize);
			return;
		}

		SetReg(Rm, regValue, OperandSize);
		SetReg(Reg, temp, OperandSize);

	}

	// MOV r/m8, r8
	void I8086::MOV_RM_R()
	{

		FetchModrm();

		CalculateEffectiveAddress();

		WriteRMOperand(GetReg(Reg, BYTE), BYTE);

	}

	// MOV r8, r/m8
	void I8086::MOV_R_RM()
	{

		FetchModrm();

		CalculateEffectiveAddress();

		const u8 op2 = ReadRMOperand(BYTE);

		SetReg(Reg, op2, BYTE);

	}

	// Group 4
	void I8086::GROUP4()
	{
		FetchModrm();

		CalculateEffectiveAddress();

		switch (Reg)
		{

		case 0:
			// MOV r/m16, ES

			WriteRMOperand(ES.X, WORD);

			break;

		case 1:
			// MOV r/m16, CS

			WriteRMOperand(CS.X, WORD);

			break;

		case 2:
			// MOV r/m16, SS

			WriteRMOperand(SS.X, WORD);
			
			break;

		case 3:
			// MOV r/m16, DS

			WriteRMOperand(DS.X, WORD);
			
			break;

		default:
			// No operation for other cases
			return;

		}

	}

	// LEA r16, r/m16
	void I8086::LEA_R16_RM16()
	{
		
		FetchModrm();
		
		if (Mod != 3)
		{
			CalculateEffectiveAddress();
			SetReg(Reg, EA, WORD);
		}

		// Mod == 3 for this instruction is undefined behavior
		// In this case, no operation is performed
	}

	// Group 5
	void I8086::GROUP5()
	{

		FetchModrm();

		CalculateEffectiveAddress();

		const u16 op2 = ReadRMOperand(WORD);

		switch (Reg)
		{

		case 0:
			// MOV ES, r/m16

			ES = op2;

			break;

		case 1:
			// MOV CS, r/m16

			CS = op2;

			break;

		case 2:
			// MOV SS, r/m16

			SS = op2;

			break;

		case 3:
			// MOV DS, r/m16

			DS = op2;

			break;

		default:
			// No operation for other cases
			return;

		}
	}

	// Group 6
	void I8086::GROUP6()
	{
		FetchModrm();

		if (Reg == 0)
		{
			// POP r/m16

			CalculateEffectiveAddress();

			WriteRMOperand(Instr::POP(this, mBus), WORD);
			
		}
	}

	// XCHG AX - Same as NOP
	void I8086::XCHG_AX()
	{

	}

	// XCHG CX
	void I8086::XCHG_CX()
	{
		const u16 temp = A.X;
		A = C;
		C = temp;
	}

	// XCHG DX
	void I8086::XCHG_DX()
	{
		const u16 temp = A.X;
		A = D;
		D = temp;
	}

	// XCHG BX
	void I8086::XCHG_BX()
	{
		const u16 temp = A.X;
		A = B;
		B = temp;

	}

	// XCHG SP
	void I8086::XCHG_SP()
	{
		const u16 temp = A.X;
		A  = SP;
		SP = temp;

	}

	// XCHG BP
	void I8086::XCHG_BP()
	{
		const u16 temp = A.X;
		A  = BP;
		BP = temp;
	}

	// XCHG SI
	void I8086::XCHG_SI()
	{
		const u16 temp = A.X;
		A  = SI;
		SI = temp;

	}

	// XCHG DI
	void I8086::XCHG_DI()
	{
		const u16 temp = A.X;
		A  = DI;
		DI = temp;
	}

	// CBW
	void I8086::CBW()
	{

		if (A.L & 0x80)
		{
			A.H = 0xFF;
		}

		else
		{
			A.H = 0x00;
		}

	}

	// CWD
	void I8086::CWD()
	{

		if (A.H & 0x80)
		{
			D.X = 0xFFFF;
		}

		else
		{
			D.X = 0x0000;
		}

	}

	// CALL FAR [addr]
	void I8086::CALL_FAR()
	{
		const u16 addr = Fetch(WORD);
		const u16 segment = Fetch(WORD);

		Instr::PUSH(CS, this, mBus);
		Instr::PUSH(IP, this, mBus);

		IP = addr;
		CS = segment;

	}

	// WAIT - FPU instruction(not implemented)
	void I8086::WAIT()
	{

	}

	// PUSHF
	void I8086::PUSHF()
	{
		const u16 flags = SF.Get();
		Instr::PUSH(flags, this, mBus);
	}

	// POPF
	void I8086::POPF()
	{
		SF.Set(mBus->Read(SP.X, SS, WORD));
		SP.X += 2;
	}

	// SAHF
	void I8086::SAHF()
	{
		const u16 flags = SF.Get();

		SF.Set((flags & 0xFF00) | A.H);
	}

	// LAHF
	void I8086::LAHF()
	{
		A.H = SF.Get() & 0x00FF;
	}

	// MOV AL, [addr]
	void I8086::MOV_AL_MOFFS16()
	{
		A.L = mBus->Read(Fetch(WORD), DS, BYTE);
	}

	// MOV AX, [addr]
	void I8086::MOV_AX_MOFFS16()
	{
		A.X = mBus->Read(Fetch(WORD), DS, WORD);
	}

	// MOV [addr], AL
	void I8086::MOV_MOFFS16_AL()
	{
		mBus->Write(Fetch(WORD), A.L, DS, BYTE);
	}

	// MOV [addr], AX
	void I8086::MOV_MOFFS16_AX()
	{
		mBus->Write(Fetch(WORD), A.X, DS, WORD);
	}

	// MOVSB
	void I8086::MOVSB()
	{

		mBus->Write(DI.X, mBus->Read(SI.X, DS, BYTE), ES, BYTE);

		if (SF.D)
		{
			SI--;
			DI--;
		}

		else
		{
			SI++;
			DI++;
		}

	}

	// MOVSW
	void I8086::MOVSW()
	{

		mBus->Write(DI.X, mBus->Read(SI.X, DS, WORD), ES, WORD);

		if (SF.D)
		{
			SI -= 2;
			DI -= 2;
		}

		else
		{
			SI += 2;
			DI += 2;
		}

	}

	// CMPSB
	void I8086::CMPSB()
	{

		Instr::SUB(mBus->Read(SI.X, DS, BYTE), mBus->Read(DI.X, ES, BYTE), this);

		if (SF.D)
		{
			SI--;
			DI--;
		}

		else
		{
			SI++;
			DI++;
		}

	}

	// CMPSW
	void I8086::CMPSW()
	{

		Instr::SUB(mBus->Read(SI.X, DS, WORD), mBus->Read(DI.X, ES, WORD), this);

		if (SF.D)
		{
			SI -= 2;
			DI -= 2;
		}

		else
		{
			SI += 2;
			DI += 2;
		}

	}

	// TEST AL, i8
	void I8086::TEST_AL_I8()
	{
		Instr::AND(A.L, Fetch(), this);
	}

	// TEST AX, i16
	void I8086::TEST_AX_I16()
	{
		Instr::AND(A.X, Fetch(WORD), this);
	}

	// STOSB
	void I8086::STOSB()
	{

		mBus->Write(DI.X, A.L, ES, BYTE);

		if (SF.D)
		{
			DI--;
		}

		else
		{
			DI++;
		}

	}

	// STOSW
	void I8086::STOSW()
	{

		mBus->Write(DI.X, A.X, ES, WORD);

		if (SF.D)
		{
			DI -= 2;
		}

		else
		{
			DI += 2;
		}

	}

	// LODSB
	void I8086::LODSB()
	{

		A.L = mBus->Read(SI.X, DS, BYTE);

		if (SF.D)
		{
			SI--;
		}

		else
		{
			SI++;
		}

	}

	// LODSW
	void I8086::LODSW()
	{

		A.X = mBus->Read(SI.X, DS, WORD);

		if (SF.D)
		{
			SI -= 2;
		}

		else
		{
			SI += 2;
		}

	}

	// SCASB
	void I8086::SCASB()
	{

		Instr::SUB(mBus->Read(DI.X, ES, BYTE), A.L, this);

		if (SF.D)
		{
			DI--;
		}

		else
		{
			DI++;
		}

	}

	// SCASW
	void I8086::SCASW()
	{

		Instr::SUB(mBus->Read(DI.X, ES, WORD), A.X, this);

		if (SF.D)
		{
			DI -= 2;
		}

		else
		{
			DI += 2;
		}

	}

	// MOV AL, i8
	void I8086::MOV_AL_I8()
	{
		A.L = Fetch();
	}

	// MOV CL, i8
	void I8086::MOV_CL_I8()
	{
		C.L = Fetch();
	}

	// MOV DL, i8
	void I8086::MOV_DL_I8()
	{
		D.L = Fetch();
	}

	// MOV BL, i8
	void I8086::MOV_BL_I8()
	{
		B.L = Fetch();
	}

	// MOV AH, i8
	void I8086::MOV_AH_I8()
	{
		A.H = Fetch();
	}

	// MOV CH, i8
	void I8086::MOV_CH_I8()
	{
		C.H = Fetch();
	}

	// MOV DH, i8
	void I8086::MOV_DH_I8()
	{
		D.H = Fetch();
	}

	// MOV BH, i8
	void I8086::MOV_BH_I8()
	{
		B.H = Fetch();
	}

	// MOV AX, i16
	void I8086::MOV_AX_I16()
	{
		A = Fetch(WORD);
	}

	// MOV CX, i16
	void I8086::MOV_CX_I16()
	{
		C = Fetch(WORD);
	}

	// MOV DX, i16
	void I8086::MOV_DX_I16()
	{
		D = Fetch(WORD);
	}

	// MOV BX, i16
	void I8086::MOV_BX_I16()
	{
		B = Fetch(WORD);
	}

	// MOV SP, i16
	void I8086::MOV_SP_I16()
	{
		SP = Fetch(WORD);
	}

	// MOV BP, i16
	void I8086::MOV_BP_I16()
	{
		BP = Fetch(WORD);
	}

	// MOV SI, i16
	void I8086::MOV_SI_I16()
	{
		SI = Fetch(WORD);
	}

	// MOV DI, i16
	void I8086::MOV_DI_I16()
	{
		DI = Fetch(WORD);
	}

	// RET i16
	void I8086::RET_I16()
	{
		const u16 offset = Fetch(WORD);

		Instr::POP(IP, this, mBus);

		SP += offset;
	}

	// RET
	void I8086::RET()
	{
		Instr::POP(IP, this, mBus);
	}

	// LES r16, [addr]
	void I8086::LES_R16_M16()
	{
		FetchModrm();

		CalculateEffectiveAddress();

		SetReg(Reg, mBus->Read(EA, mSeg, WORD), WORD);

		ES = mBus->Read(EA + 2, mSeg, WORD);
	}

	// LDS r16, [addr]
	void I8086::LDS_R16_M16()
	{
		FetchModrm();

		CalculateEffectiveAddress();
		
		SetReg(Reg, mBus->Read(EA, mSeg, WORD), WORD);

		DS = mBus->Read(EA + 2, mSeg, WORD);

	}

	// Group 7
	void I8086::GROUP7()
	{
		FetchModrm();

		if (Reg == 0)
		{
			// MOV r/m8, i8

			CalculateEffectiveAddress();

			WriteRMOperand(Fetch(), BYTE);
		}
	}

	// Group 8
	void I8086::GROUP8()
	{
		FetchModrm();

		if (Reg == 0)
		{
			// MOV r/m16, i16

			CalculateEffectiveAddress();

			WriteRMOperand(Fetch(WORD), WORD);
		}
	}

	// RETF i16
	void I8086::RETF_I16()
	{
		const u16 offset = Fetch(WORD);

		Instr::POP(IP, this, mBus);
		Instr::POP(CS, this, mBus);

		SP += offset;

	}

	// RETF
	void I8086::RETF()
	{
		Instr::POP(IP, this, mBus);
		Instr::POP(CS, this, mBus);
	}

	// INT 3 - Breakpoint interrupt
	void I8086::INT3()
	{
		Instr::INT(3, this, mBus);
	}

	// INT i8 - Software interrupt
	void I8086::INT_I8()
	{
		const u8 interrupt = Fetch() * 4;

		Instr::INT(interrupt, this, mBus);
	}

	// INTO - Interrupt on overflow
	void I8086::INTO()
	{
		if (SF.O)
		{
			Instr::INT(4, this, mBus);
		}
	}

	// IRET - Interrupt return
	void I8086::IRET()
	{
		Instr::POP(IP, this, mBus);
		Instr::POP(CS, this, mBus);
		POPF(); // POPF instruction
	}

	// Group 9
	void I8086::GROUP9()
	{

	}

	// Group 10
	void I8086::GROUP10()
	{

	}

	// Group 11
	void I8086::GROUP11()
	{

	}

	// Group 12
	void I8086::GROUP12()
	{

	}

	// AAM
	void I8086::AAM()
	{
		const u8 base = Fetch();

		A.H = A.L / base;
		A.L = A.L % base;

		SF.CheckParity(A.L);
		SF.CheckZero(A.L, 8);
		SF.CheckSign(A.L, 8);

	}

	// AAD
	void I8086::AAD()
	{
		u8 base = Fetch();

		A.L = GET_L((A.H * 10) + A.L);
		A.H = 0;

		SF.CheckParity(A.L);
		SF.CheckZero(A.L, 8);
		SF.CheckSign(A.L, 8);
	}

	// XLAT
	void I8086::XLAT()
	{
		const u16 offset = B.X + A.L;
		A.L = mBus->Read(offset, DS, BYTE);
	}

	// ESC - FPU instruction(not implemented)
	void I8086::ESC()
	{

	}
	
	// LOOPNE/Z rel8
	void I8086::LOOPNE_LOOPNZ_REL8()
	{
		const s8 offset = Fetch();

		C--;

		if (C.X != 0 && !SF.Z)
		{
			IP += offset;
		}

	}

	// LOOPE/Z rel8
	void I8086::LOOPE_LOOPZ_REL8()
	{
		const s8 offset = Fetch();

		C--;

		if (C.X != 0 && SF.Z)
		{
			IP += offset;
		}

	}

	// LOOP rel8
	void I8086::LOOP_REL8()
	{
		const s8 offset = Fetch();
		
		C--;
		
		if (C.X != 0)
		{
			IP += offset;
		}

	}

	// JCXZ rel8
	void I8086::JCXZ_REL8()
	{
		const s8 offset = Fetch();

		if (C.X == 0)
		{
			IP += offset;
		}

	}

	// IN AL, i8
	void I8086::IN_AL_I8()
	{
		// TODO
	}

	// IN AX, i8
	void I8086::IN_AX_I8()
	{
		// TODO
	}

	// OUT i8, AL
	void I8086::OUT_I8_AL()
	{
		// TODO
	}

	// OUT i8, AX
	void I8086::OUT_I8_AX()
	{
		// TODO
	}

	// CALL rel16
	void I8086::CALL_REL16()
	{
		const u16 offset = Fetch(WORD);

		Instr::PUSH(IP.X, this, mBus);

		IP += offset;

	}

	// JMP rel16
	void I8086::JMP_REL16()
	{

		const u16 offset = Fetch(WORD);

		IP += offset;

	}

	// JMP seg:addr
	void I8086::JMP_FAR()
	{

		IP = Fetch(WORD);
		CS = Fetch(WORD);

	}

	// JMP rel8
	void I8086::JMP_REL8()
	{

		const s8 offset = Fetch();

		IP += offset;

	}

	// IN AL, DX
	void I8086::IN_AL_DX()
	{
		// TODO
	}

	// IN AX, DX
	void I8086::IN_AX_DX()
	{
		// TODO
	}

	// OUT DX, AL
	void I8086::OUT_DX_AL()
	{
		// TODO
	}

	// OUT DX, AX
	void I8086::OUT_DX_AX()
	{
		// TODO
	}

	// LOCK prefix - Used to ensure exclusive use of shared memory in multiprocessor systems(Not implemented)
	void I8086::LOCK()
	{

	}

	// REPNE/NZ prefix
	void I8086::REPNE_REPNZ()
	{
		mREP = true;
	}

	// REP/REPE/REPZ prefix
	void I8086::REP_REPE_REPZ()
	{
		mREP = true;
	}

	// HLT
	void I8086::HLT()
	{
		mHalted = true;
	}

	// CMC
	void I8086::CMC()
	{
		SF.C = !SF.C;
	}

	// Group 13
	void I8086::GROUP13()
	{

	}

	// Group 14
	void I8086::GROUP14()
	{

	}

	// CLC
	void I8086::CLC()
	{
		SF.C = 0;
	}

	// STC
	void I8086::STC()
	{
		SF.C = 1;
	}

	// CLI
	void I8086::CLI()
	{
		SF.I = 0;
	}

	// STI
	void I8086::STI()
	{
		mPendingInterruptFlag = true;
	}

	// CLD
	void I8086::CLD()
	{
		SF.D = 0;
	}

	// STD
	void I8086::STD()
	{
		SF.D = 1;
	}

	void I8086::GROUP15()
	{

	}

	void I8086::GROUP16()
	{

	}


} // namespace i8086