// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#include "StateWindow.hpp"

#include <Utils/Panel.hpp>

#include <bitset>

namespace UI
{

	constexpr ImGuiTableFlags TABLE_FLAGS = ImGuiTableFlags_Borders | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_RowBg;
	constexpr ImU32 TITLE_BG_COL = IM_COL32(15, 15, 15, 240);

	static inline void DrawRegisterRow(const char* name, u16 value)
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::TextUnformatted(name);
		ImGui::TableNextColumn();
		ImGui::Text(" 0x%04X ", value);
	}

	void StateWindow::ShowIfOpen()
	{
		if (!mIsOpen)
		{
			return;
		}

		if (ImGui::Begin("State", &mIsOpen))
		{

			mCPUController->GetState(mState);
			
			ImGui::Separator();

			Panel("##RegistersPanel", "Registers", 105.0f, TITLE_BG_COL, [&]() { RenderGeneralRegisters(); });
			Panel("##SegmentRegistersPanel", "Segment Registers", 105.0f, TITLE_BG_COL, [&]() { RenderSegmentRegisters(); });
			Panel("##FlagsPanel", "Flags", 150.0f, TITLE_BG_COL, [&]() { RenderFlags(); });
			Panel("##ModRMPanel", "ModR/M", 90.0f, TITLE_BG_COL, [&]() { RenderModRM(); });
			Panel("##StackPanel", "Stack", 90.0f, TITLE_BG_COL, [&]() { RenderStack(); });

		}

		ImGui::End();
	}

	inline void StateWindow::RenderGeneralRegisters() const
	{
		
		if (ImGui::BeginTable("##RegistersTable", 2, TABLE_FLAGS)) {

			DrawRegisterRow(" IP ", mState.IP.X);
			DrawRegisterRow(" AX ", mState.A.X);
			DrawRegisterRow(" BX ", mState.B.X);
			DrawRegisterRow(" CX ", mState.C.X);
			DrawRegisterRow(" DX ", mState.D.X);
			DrawRegisterRow(" SI ", mState.SI.X);
			DrawRegisterRow(" DI ", mState.DI.X);
			DrawRegisterRow(" SP ", mState.SP.X);
			DrawRegisterRow(" BP ", mState.BP.X);
			DrawRegisterRow(" SF ", mState.SF.Get());

			ImGui::EndTable();
		}
		
	}

	inline void StateWindow::RenderSegmentRegisters() const
	{
		if (ImGui::BeginTable("##SegmentRegistersTable", 2, TABLE_FLAGS)) {

			DrawRegisterRow(" CS ", mState.CS.X);
			DrawRegisterRow(" DS ", mState.DS.X);
			DrawRegisterRow(" SS ", mState.SS.X);
			DrawRegisterRow(" ES ", mState.ES.X);

			ImGui::EndTable();
		}
		
	}

	inline void StateWindow::RenderFlags() const
	{
		static constexpr char flagsNames[] = {'O','D','I','T','S','Z','A','P','C'};

		if (ImGui::BeginTable("##FlagsTable", 9, TABLE_FLAGS)) {

			auto drawFlag = [](bool state) {
				ImGui::TableNextColumn();
				ImGui::Text("%s", state ? "1" : "0");
			};

			ImGui::TableNextRow();

			for (int i = 0; i < 9; ++i)
			{
				ImGui::TableNextColumn();
				ImGui::Text("%c", flagsNames[i]);
			}

			ImGui::TableNextRow();

			drawFlag(mState.SF.O);
			drawFlag(mState.SF.D);
			drawFlag(mState.SF.I);
			drawFlag(mState.SF.T);
			drawFlag(mState.SF.S);
			drawFlag(mState.SF.Z);
			drawFlag(mState.SF.A);
			drawFlag(mState.SF.P);
			drawFlag(mState.SF.C);

			ImGui::EndTable();
		}

	}

	inline void StateWindow::RenderModRM() const
	{

		if (ImGui::BeginTable("##ModRmTable", 3, TABLE_FLAGS)) {

			ImGui::TableSetupColumn("Mod");
			ImGui::TableSetupColumn("Reg");
			ImGui::TableSetupColumn("Rm");
			ImGui::TableHeadersRow();

			ImGui::TableNextRow();

			ImGui::TableNextColumn();
			ImGui::Text("%s", std::bitset<2>(mState.Mod).to_string().c_str());

			ImGui::TableNextColumn();
			ImGui::Text("%s", std::bitset<3>(mState.Reg).to_string().c_str());

			ImGui::TableNextColumn();
			ImGui::Text("%s", std::bitset<3>(mState.Rm).to_string().c_str());

			ImGui::EndTable();
		}

	}

	inline void StateWindow::RenderStack() const
	{

		if (ImGui::BeginTable("##StackTable", 2, TABLE_FLAGS)) {

			const u16 stackSize = mCPUInitialState.SP.X - mState.SP.X;

			for (u16 i = 0; i <= stackSize; ++i)
			{
				ImGui::TableNextRow();

				ImGui::TableNextColumn();
				ImGui::Text(" %2d ", i);

				ImGui::TableNextColumn();
				ImGui::Text(" 0x%02X ", mBus->Read(mCPUInitialState.SP.X - i, mState.SS, 8));
			}

			ImGui::EndTable();

		}
	
	}

} // namespace UI