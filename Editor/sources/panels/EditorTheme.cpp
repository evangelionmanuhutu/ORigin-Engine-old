#include "EditorTheme.h"

namespace Origin
{
	void EditorTheme::SetColors(int backGroundColor, int textColor, int mainColor, int mainAccentColor, int highlightColor)
	{
		BackGroundColor = backGroundColor;
		TextColor = textColor;
		MainColor = mainColor;
		MainAccentColor = mainAccentColor;
		HighlightColor = highlightColor;
	}

	void EditorTheme::ApplyDarkPurple()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("assets/fonts/JetBrainsMono-SemiBold.ttf", 16);

		// Origin Gui Theme Settings
		ImGuiStyle& style = ImGui::GetStyle();

		// frame
		style.FrameRounding = 6;
		style.GrabRounding = 4;
		style.FramePadding.x = 1;
		style.FramePadding.y = 1;

		// window
		style.WindowMenuButtonPosition = 0;
		style.WindowTitleAlign.x = 0.5;
		style.WindowTitleAlign.y = 0.5;
		style.WindowRounding = 0;
		style.WindowPadding = ImVec2(3.0f, 3.0f);

		ImVec4* colors = ImGui::GetStyle().Colors;

		colors[ImGuiCol_Text] = GetColor(TextColor);
		colors[ImGuiCol_TextDisabled] = Disabled(colors[ImGuiCol_Text]);
		colors[ImGuiCol_WindowBg] = GetColor(BackGroundColor);
		colors[ImGuiCol_ChildBg] = GetColor(Black, Alpha20);
		colors[ImGuiCol_PopupBg] = GetColor(BackGroundColor, Alpha90);
		colors[ImGuiCol_Border] = Lighten(GetColor(BackGroundColor), 0.4f);
		colors[ImGuiCol_BorderShadow] = GetColor(Black);
		colors[ImGuiCol_FrameBg] = GetColor(MainAccentColor, Alpha40);
		colors[ImGuiCol_FrameBgHovered] = Hovered(colors[ImGuiCol_FrameBg]);
		colors[ImGuiCol_FrameBgActive] = Active(colors[ImGuiCol_FrameBg]);
		colors[ImGuiCol_TitleBg] = GetColor(BackGroundColor, Alpha80);
		colors[ImGuiCol_TitleBgActive] = Active(colors[ImGuiCol_TitleBg]);
		colors[ImGuiCol_TitleBgCollapsed] = Collapsed(colors[ImGuiCol_TitleBg]);
		colors[ImGuiCol_MenuBarBg] = Darken(GetColor(BackGroundColor), 0.2f);
		colors[ImGuiCol_ScrollbarBg] = Lighten(GetColor(BackGroundColor, Alpha50), 0.4f);
		colors[ImGuiCol_ScrollbarGrab] = Lighten(GetColor(BackGroundColor), 0.3f);
		colors[ImGuiCol_ScrollbarGrabHovered] = Hovered(colors[ImGuiCol_ScrollbarGrab]);
		colors[ImGuiCol_ScrollbarGrabActive] = Active(colors[ImGuiCol_ScrollbarGrab]);
		colors[ImGuiCol_CheckMark] = GetColor(HighlightColor);
		colors[ImGuiCol_SliderGrab] = GetColor(HighlightColor);
		colors[ImGuiCol_SliderGrabActive] = Active(colors[ImGuiCol_SliderGrab]);
		colors[ImGuiCol_Button] = GetColor(MainColor, Alpha80);
		colors[ImGuiCol_ButtonHovered] = Hovered(colors[ImGuiCol_Button]);
		colors[ImGuiCol_ButtonActive] = Active(colors[ImGuiCol_Button]);
		colors[ImGuiCol_Header] = GetColor(MainAccentColor, Alpha80);
		colors[ImGuiCol_HeaderHovered] = Hovered(colors[ImGuiCol_Header]);
		colors[ImGuiCol_HeaderActive] = Active(colors[ImGuiCol_Header]);
		colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
		colors[ImGuiCol_SeparatorHovered] = Hovered(colors[ImGuiCol_Separator]);
		colors[ImGuiCol_SeparatorActive] = Active(colors[ImGuiCol_Separator]);
		colors[ImGuiCol_ResizeGrip] = GetColor(MainColor, Alpha20);
		colors[ImGuiCol_ResizeGripHovered] = Hovered(colors[ImGuiCol_ResizeGrip]);
		colors[ImGuiCol_ResizeGripActive] = Active(colors[ImGuiCol_ResizeGrip]);
		colors[ImGuiCol_Tab] = GetColor(MainColor, Alpha60);
		colors[ImGuiCol_TabHovered] = Hovered(colors[ImGuiCol_Tab]);
		colors[ImGuiCol_TabActive] = Active(colors[ImGuiCol_Tab]);
		colors[ImGuiCol_TabUnfocused] = colors[ImGuiCol_Tab];
		colors[ImGuiCol_TabUnfocusedActive] = colors[ImGuiCol_TabActive];
		colors[ImGuiCol_DockingPreview] = Darken(colors[ImGuiCol_HeaderActive], 0.2f);
		colors[ImGuiCol_DockingEmptyBg] = Darken(colors[ImGuiCol_HeaderActive], 0.6f);
		colors[ImGuiCol_PlotLines] = GetColor(HighlightColor);
		colors[ImGuiCol_PlotLinesHovered] = Hovered(colors[ImGuiCol_PlotLines]);
		colors[ImGuiCol_PlotHistogram] = GetColor(HighlightColor);
		colors[ImGuiCol_PlotHistogramHovered] = Hovered(colors[ImGuiCol_PlotHistogram]);
		colors[ImGuiCol_TextSelectedBg] = GetColor(HighlightColor, Alpha40);
		colors[ImGuiCol_DragDropTarget] = GetColor(HighlightColor, Alpha80);;
		colors[ImGuiCol_NavHighlight] = GetColor(White);
		colors[ImGuiCol_NavWindowingHighlight] = GetColor(White, Alpha80);
		colors[ImGuiCol_NavWindowingDimBg] = GetColor(White, Alpha20);
		colors[ImGuiCol_ModalWindowDimBg] = GetColor(Black, Alpha60);

		ImGui::GetStyle().WindowMenuButtonPosition = ImGuiDir_Right;
	}

	void EditorTheme::ApplyRayTek()
	{

		ImGuiIO& io = ImGui::GetIO();

		io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto-Medium.ttf", 14);
		// Origin Gui Theme Settings
		ImGuiStyle* style = &ImGui::GetStyle();

		// frame
		style->FramePadding.x = 5;
		style->FramePadding.y = 1;
		style->FrameRounding = 2.0f;

		// window
		style->WindowMenuButtonPosition = 0;
		style->WindowTitleAlign.x = 0.5;
		style->WindowTitleAlign.y = 0.5;
		style->WindowPadding = ImVec2(6.0f, 6.0f);
		style->WindowBorderSize = 0;
		style->ScrollbarSize = 10.0f;
		style->ScrollbarRounding = 2.0f;
		style->GrabRounding = 2.0f;

		// Item
		style->ItemSpacing = ImVec2(5, 5);
		style->ItemInnerSpacing = ImVec2(8, 6);
		style->IndentSpacing = 10.0f;


		style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
		style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
		style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
		style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	}
}