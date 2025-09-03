// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include <imgui.h>

namespace UI
{

	class ChildPanel
	{

	public:

		ChildPanel(const char* childId, const char* title, float contentWidth, ImU32 titleBgColor)
			: mTitle(title), mBgColor(titleBgColor)
		{
			mCursorPos = ImGui::GetCursorScreenPos();
			
			ImGui::BeginChild(childId, { 0,0 }, ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Border);

			ImGui::Spacing();

			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvail().x - contentWidth) * 0.5f);
		}

		~ChildPanel()
		{
			ImGui::EndChild();

			const ImVec2 textSize = ImGui::CalcTextSize(mTitle);
			const ImVec2 titlePos(mCursorPos.x + 15, mCursorPos.y - textSize.y * 0.5f);		

			ImGui::GetWindowDrawList()->AddRectFilled(
				ImVec2(titlePos.x - 8, titlePos.y),
				ImVec2(titlePos.x + textSize.x + 8, titlePos.y + textSize.y),
				mBgColor, 0.0f
			);

			ImGui::GetWindowDrawList()->AddText(titlePos, IM_COL32(100, 100, 255, 255), mTitle);

			ImGui::Spacing();
		}

	private:
	
		const char* mTitle;
		ImU32 mBgColor;
		ImVec2 mCursorPos;

	};

} // namespace UI