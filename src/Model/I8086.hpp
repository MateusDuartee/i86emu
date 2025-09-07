// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include "Register.hpp"
#include "CPUState.hpp"
#include "MemoryBus.hpp"

#include <vector>
#include <array>

namespace i8086
{

	class I8086 : private CPUState
	{

	public:

		I8086(MemoryBus* const bus);

		void Cycles(u8 count);

		void GetInternalState(CPUState& state) const;

		void SetBreakpoint(u32 address, bool state);

	protected:

		u16 Fetch(u8 size = 8);

		void FetchModrm();
		void HandleREP();
		void ExecuteInstructions(u8 count);
		void CalculateEffectiveAddress();
		void ApplyRegisterOverrideIfNeeded();

		void SetReg(u8 reg, u16 value, u8 size);
		u16 GetReg(u8 reg, u8 size) const;

	protected:

		MemoryBus* const mBus;
		std::vector<u32> mBreakpoints;
		std::array<void (I8086::*)(), 256> mOpcodeTable;

		bool mStepMode{ false };
		bool mREP{ false };
		bool mHalted{ false };
		bool mPendingInterruptFlag{ false };

		/* Registers Maps */

		std::array<Register*, 8> mRegs16 = {
			&A, &C, &D, &B, &SP, &BP, &SI, &DI
		};

		std::array<RegByte*, 8> mRegs8 = {
			&A.L, &C.L, &D.L, &B.L, &A.H, &C.H, &D.H, &B.H
		};

	protected:

		u16 ReadRMOperand(u8 operandSize) const;
		void WriteRMOperand(u16 data, u8 operandSize);
		
		/* Instructions */
		
		void PUSH(Register& reg);
		void PUSH(u16 value);
		void POP(Register& reg);
		u16 POP();
		void INT(u8 interruptNumber);

		void ADD_RM_R();
		void ADD_R_RM();
		void ADD_AL_I8();
		void ADD_AX_i16();
		void PUSH_ES();
		void POP_ES();
		void OR_RM_R();
		void OR_R_RM();
		void OR_AL_I8();
		void OR_AX_i16();
		void PUSH_CS();
		void POP_CS();
		void ADC_RM_R();
		void ADC_R_RM();
		void ADC_AL_I8();
		void ADC_AX_I16();
		void PUSH_SS();
		void POP_SS();
		void SBB_RM_R();
		void SBB_R_RM();
		void SBB_AL_I8();
		void SBB_AX_I16();
		void PUSH_DS();
		void POP_DS();
		void AND_RM_R();
		void AND_R_RM();
		void AND_AL_I8();
		void AND_AX_I16();
		void ES_OVERRIDE();
		void DAA();
		void SUB_RM_R();
		void SUB_R_RM();
		void SUB_AL_I8();
		void SUB_AX_I16();
		void CS_OVERRIDE();
		void DAS();
		void XOR_RM_R();
		void XOR_R_RM();
		void XOR_AL_I8();
		void XOR_AX_I16();
		void SS_OVERRIDE();
		void AAA();
		void CMP_RM_R();
		void CMP_R_RM();
		void CMP_AL_I8();
		void CMP_AX_I16();
		void DS_OVERRIDE();
		void AAS();
		void INC_AX();
		void INC_CX();
		void INC_DX();
		void INC_BX();
		void INC_SP();
		void INC_BP();
		void INC_SI();
		void INC_DI();
		void DEC_AX();
		void DEC_CX();
		void DEC_DX();
		void DEC_BX();
		void DEC_SP();
		void DEC_BP();
		void DEC_SI();
		void DEC_DI();
		void PUSH_AX();
		void PUSH_CX();
		void PUSH_DX();
		void PUSH_BX();
		void PUSH_SP();
		void PUSH_BP();
		void PUSH_SI();
		void PUSH_DI();
		void POP_AX();
		void POP_CX();
		void POP_DX();
		void POP_BX();
		void POP_SP();
		void POP_BP();
		void POP_SI();
		void POP_DI();
		void NOP();
		void JO_REL8();
		void JNO_REL8();
		void JNAE_JB_JC_REL8();
		void JAE_JNB_JNC_REL8();
		void JE_JZ_REL8();
		void JNE_JNZ_REL8();
		void JBE_JNA_REL8();
		void JNBE_JA_REL8();
		void JS_REL8();
		void JNS_REL8();
		void JP_JPE_REL8();
		void JNP_JPO_REL8();
		void JL_JNGE_REL8();
		void JGE_JNL_REL8();
		void JLE_JNG_REL8();
		void JG_JNLE_REL8();
		void GROUP0();
		void GROUP1();
		void GROUP2();
		void GROUP3();
		void TEST_RM_R();
		void XCHG_R_RM();
		void MOV_RM_R();
		void MOV_R_RM();
		void GROUP4();
		void LEA_R16_RM16();
		void GROUP5();
		void GROUP6();
		void XCHG_AX();
		void XCHG_CX();
		void XCHG_DX();
		void XCHG_BX();
		void XCHG_SP();
		void XCHG_BP();
		void XCHG_SI();
		void XCHG_DI();
		void CBW();
		void CWD();
		void CALL_FAR();
		void WAIT();
		void PUSHF();
		void POPF();
		void SAHF();
		void LAHF();
		void MOV_AL_MOFFS16();
		void MOV_AX_MOFFS16();
		void MOV_MOFFS16_AL();
		void MOV_MOFFS16_AX();
		void MOVSB();
		void MOVSW();
		void CMPSB();
		void CMPSW();
		void TEST_AL_I8();
		void TEST_AX_I16();
		void STOSB();
		void STOSW();
		void LODSB();
		void LODSW();
		void SCASB();
		void SCASW();
		void MOV_AL_I8();
		void MOV_CL_I8();
		void MOV_DL_I8();
		void MOV_BL_I8();
		void MOV_AH_I8();
		void MOV_CH_I8();
		void MOV_DH_I8();
		void MOV_BH_I8();
		void MOV_AX_I16();
		void MOV_CX_I16();
		void MOV_DX_I16();
		void MOV_BX_I16();
		void MOV_SP_I16();
		void MOV_BP_I16();
		void MOV_SI_I16();
		void MOV_DI_I16();
		void RET_I16();
		void RET();
		void LES_R16_M16();
		void LDS_R16_M16();
		void GROUP7();
		void GROUP8();
		void RETF_I16();
		void RETF();
		void INT3();
		void INT_I8();
		void INTO();
		void IRET();
		void GROUP9();
		void GROUP10();
		void GROUP11();
		void GROUP12();
		void AAM();
		void AAD();
		void XLAT();
		void ESC();
		void LOOPNE_LOOPNZ_REL8();
		void LOOPE_LOOPZ_REL8();
		void LOOP_REL8();
		void JCXZ_REL8();
		void IN_AL_I8();
		void IN_AX_I8();
		void OUT_I8_AL();
		void OUT_I8_AX();
		void CALL_REL16();
		void JMP_REL16();
		void JMP_FAR();
		void JMP_REL8();
		void IN_AL_DX();
		void IN_AX_DX();
		void OUT_DX_AL();
		void OUT_DX_AX();
		void LOCK();
		void REPNE_REPNZ();
		void REP_REPE_REPZ();
		void HLT();
		void CMC();
		void GROUP13();
		void GROUP14();
		void CLC();
		void STC();
		void CLI();
		void STI();
		void CLD();
		void STD();
		void GROUP15();
		void GROUP16();
	};

} // namespace i8086