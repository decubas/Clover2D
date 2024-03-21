#pragma once

#include "Clover.h"

#include <imgui.h>
#include "imgui_internal.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/detail/setup.hpp"

namespace ImUI
{

	static int s_UIContextID = 0;
	static uint32_t s_Counter = 0;
	static char s_IDBuffer[16];

	static void BeginProperty(const char* label)
	{
		ImGui::Spacing();
		ImGui::PushID(label);

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 75.f);
		ImGui::Text(label);
		ImGui::NextColumn();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 10, 0 });
	}

	static bool EndProperty(bool Dirty)
	{
		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
		ImGui::Spacing();

		return Dirty;
	}

	template<typename T>
	static bool EndPropertyReset(bool Dirty, T& Value, T DefaultValue)
	{
		ImGui::SameLine();
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 20.0f, lineHeight };
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.4f, 0.4f, 0.4f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f });
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Reset", buttonSize))
		{
			Value = DefaultValue;
			Dirty = true;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		return EndProperty(Dirty);
	}

	static void PushID()
	{
		ImGui::PushID(s_UIContextID++);
		s_Counter = 0;
	}

	static void PopID()
	{
		ImGui::PopID();
		s_UIContextID--;
	}

	static void BeginWindow(const char* name)
	{
		ImGui::Begin(name);
	}

	static void EndWindow()
	{
		ImGui::End();
	}

	static void BeginPropertyGrid()
	{
		PushID();
		ImGui::Columns(2);
	}

	template<typename T, typename ... Args>
	static bool Property(const char* label, T& value, Args&& ... args)
	{

		BeginProperty(label);

		bool Dirty = false;

		if constexpr (std::is_same_v<T, int>)
		{
			if (ImGui::DragInt("##X", &value, std::forward<Args>(args)...))
				Dirty = true;
		}
		if constexpr (std::is_same_v<T, int[2]>)
		{
			if (ImGui::DragInt2("##X", value, std::forward<Args>(args)...))
				Dirty = true;
		}
		if constexpr (std::is_same_v<T, int[3]>)
		{
			if (ImGui::DragInt3("##X", value, std::forward<Args>(args)...))
				Dirty = true;
		}
		if constexpr (std::is_same_v<T, float>)
		{
			if (ImGui::DragFloat("##X", &value, std::forward<Args>(args)...))
				Dirty = true;
		}
		if constexpr (std::is_same_v<T, float[2]>)
		{
			if (ImGui::DragFloat2("##X", value, std::forward<Args>(args)...))
				Dirty = true;
		}
		if constexpr (std::is_same_v<T, float[3]>)
		{
			if (ImGui::DragFloat3("##X", value, std::forward<Args>(args)...))
				Dirty = true;
		}
		if constexpr (std::is_same_v<T, bool>)
		{
			if (ImGui::Checkbox("##X", &value))
				Dirty = true;
		}
		if constexpr (std::is_same_v<T, std::string>)
		{
			char buffer[256];
			strcpy(buffer, value.c_str());
			
			if (ImGui::InputText("##X", buffer, 256))
				Dirty = true;
		}
		if constexpr (std::is_same_v<T, const char*>)
		{
			ImGui::Text(value);
		}

		return EndProperty(Dirty);
	}

	static bool PropertySlider(const char* label, int& value, int min, int max)
	{
		BeginProperty(label);

		bool Dirty = false;

		if (ImGui::SliderInt("##X", &value, min, max))
			Dirty = true;

		return EndProperty(Dirty);
	}

	static bool PropertyColor(const char* label, float value[], bool bAlpha = true)
	{
		BeginProperty(label);

		bool Dirty = false;
		
		if (ImGui::ColorEdit4("##X", value, bAlpha ? ImGuiColorEditFlags_NoAlpha : ImGuiColorEditFlags_None ))
			Dirty = true;

		return EndProperty(Dirty);
	}

	static void PropertyText(const char* label, const char* value, glm::vec4 color = {1, 1, 1, 1})
	{
		BeginProperty(label);

		ImGui::TextColored(ImVec4(color.x, color.y, color.z, color.w), value);

		EndProperty(false);
	}

	static bool PropertySelection(const char* label, int& current_item, const std::vector<std::string>& items, int max_items)
	{
		BeginProperty(label);

		bool Dirty = false;

		if (ImGui::BeginCombo("##X", items[current_item].c_str()))
		{
			for (int n = 0; n < items.size(); n++)
			{
				const bool is_selected = (current_item == n);
				if (ImGui::Selectable(items[n].c_str(), is_selected))
				{
					current_item = n;
					Dirty = true;
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		return EndProperty(Dirty);

	}

	static void EndPropertyGrid()
	{
		ImGui::Columns(1);
		PopID();
	}

	static bool BeginTreeNode(const char* name, bool defaultOpen = true)
	{
		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
		if (defaultOpen)
			treeNodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;

		return ImGui::TreeNodeEx(name, treeNodeFlags);
	}

	static void EndTreeNode()
	{
		ImGui::TreePop();
	}

	static void BeginDocking()
	{
		bool dockingEnable = true;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (dockingEnable)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockingEnable, window_flags);
		ImGui::PopStyleVar();

		ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowMinSize.x = 300.0f;
		ImGuiID dockspace_id = ImGui::GetID("CoffeeDockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		style.WindowMinSize.x = 32.0f;
	}

	static bool PropertyVec3(const std::string& label, float values[3], float resetValue = 0.0f, float columnWidth = 75.0f)
	{
		ImGui::Spacing();
		bool Dirty = false;
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
		{
			values[0] = resetValue;
			Dirty = true;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##X", &values[0], 0.1f, 0.0f, 0.0f, "%.2f"))
			Dirty = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
		{
			values[1] = resetValue;
			Dirty = true;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if(ImGui::DragFloat("##Y", &values[1], 0.1f, 0.0f, 0.0f, "%.2f"))
			Dirty = true;

		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
		{
			values[2] = resetValue;
			Dirty = true;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if(ImGui::DragFloat("##Z", &values[2], 0.1f, 0.0f, 0.0f, "%.2f"))
			Dirty = true;
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
		ImGui::Spacing();

		return Dirty;
	}
	
	static bool PropertyVec2(const std::string& label, float values[2], float resetValue = 0.0f, float columnWidth = 75.0f)
	{
		ImGui::Spacing();
		bool Dirty = false;
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
		{
			values[0] = resetValue;
			Dirty = true;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##X", &values[0], 0.1f, 0.0f, 0.0f, "%.2f"))
			Dirty = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
		{
			values[1] = resetValue;
			Dirty = true;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##Y", &values[1], 0.1f, 0.0f, 0.0f, "%.2f"))
			Dirty = true;

		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
		ImGui::Spacing();

		return Dirty;
	}

	static bool ComboAutoSelect(const char* label, char* buffer, int bufferlen, int* current_item, void* data, int items_count) 
	{
		struct Funcs { static bool ItemGetter(void* data, int n, const char** out_str) { *out_str = ((const char**)data)[n]; return true; } };
		bool(*items_getter)(void*, int, const char**) = &Funcs::ItemGetter;
		ImGuiComboFlags flags = 0;
		// Always consume the SetNextWindowSizeConstraint() call in our early return paths
		ImGuiContext& g = *GImGui;

		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		// Call the getter to obtain the preview string which is a parameter to BeginCombo()

		const ImGuiID popupId = window->GetID(label);
		bool popupIsAlreadyOpened = ImGui::IsPopupOpen(popupId, 0); //ImGuiPopupFlags_AnyPopupLevel);
		const char* sActiveidxValue1 = NULL;
		items_getter(data, *current_item, &sActiveidxValue1);
		bool popupNeedsToBeOpened = (buffer[0] != 0) && strcmp(buffer, sActiveidxValue1);
		bool popupJustOpened = false;

		IM_ASSERT((flags & (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)) != (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)); // Can't use both flags together

		const ImGuiStyle& style = g.Style;

		const float arrow_size = (flags & ImGuiComboFlags_NoArrowButton) ? 0.0f : ImGui::GetFrameHeight();
		const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
		const float expected_w = ImGui::CalcItemWidth();
		const float w = (flags & ImGuiComboFlags_NoPreview) ? arrow_size : expected_w;
		const ImRect frame_bb(window->DC.CursorPos, ImVec2(window->DC.CursorPos.x + w, window->DC.CursorPos.y + label_size.y + style.FramePadding.y * 2.0f));
		const ImRect total_bb(frame_bb.Min, ImVec2((label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f) + frame_bb.Max.x, frame_bb.Max.y));
		const float value_x2 = ImMax(frame_bb.Min.x, frame_bb.Max.x - arrow_size);
		ImGui::ItemSize(total_bb, style.FramePadding.y);
		if (!ImGui::ItemAdd(total_bb, popupId, &frame_bb))
			return false;

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(frame_bb, popupId, &hovered, &held);

		if (!popupIsAlreadyOpened) {
			const ImU32 frame_col = ImGui::GetColorU32(hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
			ImGui::RenderNavHighlight(frame_bb, popupId);
			if (!(flags & ImGuiComboFlags_NoPreview))
				window->DrawList->AddRectFilled(frame_bb.Min, ImVec2(value_x2, frame_bb.Max.y), frame_col, style.FrameRounding, (flags & ImGuiComboFlags_NoArrowButton) ? ImDrawFlags_RoundCornersAll : ImDrawFlags_RoundCornersLeft);
		}
		if (!(flags & ImGuiComboFlags_NoArrowButton)) {
			ImU32 bg_col = ImGui::GetColorU32((popupIsAlreadyOpened || hovered) ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
			ImU32 text_col = ImGui::GetColorU32(ImGuiCol_Text);
			window->DrawList->AddRectFilled(ImVec2(value_x2, frame_bb.Min.y), frame_bb.Max, bg_col, style.FrameRounding, (w <= arrow_size) ? ImDrawFlags_RoundCornersAll : ImDrawFlags_RoundCornersRight);
			if (value_x2 + arrow_size - style.FramePadding.x <= frame_bb.Max.x)
				ImGui::RenderArrow(window->DrawList, ImVec2(value_x2 + style.FramePadding.y, frame_bb.Min.y + style.FramePadding.y), text_col, ImGuiDir_Down, 1.0f);
		}

		if (!popupIsAlreadyOpened) {
			ImGui::RenderFrameBorder(frame_bb.Min, frame_bb.Max, style.FrameRounding);
			if (buffer != NULL && !(flags & ImGuiComboFlags_NoPreview))

				ImGui::RenderTextClipped(ImVec2(frame_bb.Min.x + style.FramePadding.x, frame_bb.Min.y + style.FramePadding.y), ImVec2(value_x2, frame_bb.Max.y), buffer, NULL, NULL, ImVec2(0.0f, 0.0f));

			if ((pressed || g.NavActivateId == popupId || popupNeedsToBeOpened) && !popupIsAlreadyOpened) {
				if (window->DC.NavLayerCurrent == 0)
					window->NavLastIds[0] = popupId;
				ImGui::OpenPopupEx(popupId);
				popupIsAlreadyOpened = true;
				popupJustOpened = true;
			}
		}

		if (label_size.x > 0) {
			ImGui::RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);
		}

		if (!popupIsAlreadyOpened) {
			return false;
		}

		const float totalWMinusArrow = w - arrow_size;
		struct ImGuiSizeCallbackWrapper {
			static void sizeCallback(ImGuiSizeCallbackData* data) {
				float* totalWMinusArrow = (float*)(data->UserData);
				data->DesiredSize = ImVec2(*totalWMinusArrow, 200.f);
			}
		};
		ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(totalWMinusArrow, 150.f), ImGuiSizeCallbackWrapper::sizeCallback, (void*)&totalWMinusArrow);

		char name[16];
		ImFormatString(name, IM_ARRAYSIZE(name), "##Combo_%02d", g.BeginPopupStack.Size); // Recycle windows based on depth

		// Peek into expected window size so we can position it
		if (ImGuiWindow* popup_window = ImGui::FindWindowByName(name)) {
			if (popup_window->WasActive) {
				ImVec2 size_expected = ImGui::CalcWindowNextAutoFitSize(popup_window);
				if (flags & ImGuiComboFlags_PopupAlignLeft)
					popup_window->AutoPosLastDirection = ImGuiDir_Left;
				ImRect r_outer = ImGui::GetPopupAllowedExtentRect(popup_window);
				ImVec2 pos = ImGui::FindBestWindowPosForPopupEx(frame_bb.GetBL(), size_expected, &popup_window->AutoPosLastDirection, r_outer, frame_bb, ImGuiPopupPositionPolicy_ComboBox);

				pos.y -= label_size.y + style.FramePadding.y * 2.0f;

				ImGui::SetNextWindowPos(pos);
			}
		}

		// Horizontally align ourselves with the framed text
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;
		//    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(style.FramePadding.x, style.WindowPadding.y));
		bool ret = ImGui::Begin(name, NULL, window_flags);

		ImGui::PushItemWidth(ImGui::GetWindowWidth());
		ImGui::SetCursorPos(ImVec2(0.f, window->DC.CurrLineTextBaseOffset));
		if (popupJustOpened) {
			ImGui::SetKeyboardFocusHere(0);
		}

		bool done = ImGui::InputTextEx("##inputText", NULL, buffer, bufferlen, ImVec2(0, 0), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue, NULL, NULL);
		ImGui::PopItemWidth();

		if (*current_item < 0) {
			IM_ASSERT(false); //Undefined behaviour
			return false;
		}

		if (!ret) {
			ImGui::EndChild();
			ImGui::PopItemWidth();
			ImGui::EndPopup();
			IM_ASSERT(0);   // This should never happen as we tested for IsPopupOpen() above
			return false;
		}

		ImGuiWindowFlags window_flags2 = ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus; //0; //ImGuiWindowFlags_HorizontalScrollbar
		ImGui::BeginChild("ChildL", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), false, window_flags2);

		struct fuzzy {
			static int score(const char* str1, const char* str2) {
				int score = 0, consecutive = 0, maxerrors = 0;
				while (*str1 && *str2) {
					int is_leading = (*str1 & 64) && !(str1[1] & 64);
					if ((*str1 & ~32) == (*str2 & ~32)) {
						int had_separator = (str1[-1] <= 32);
						int x = had_separator || is_leading ? 10 : consecutive * 5;
						consecutive = 1;
						score += x;
						++str2;
					}
					else {
						int x = -1, y = is_leading * -3;
						consecutive = 0;
						score += x;
						maxerrors += y;
					}
					++str1;
				}
				return score + (maxerrors < -9 ? -9 : maxerrors);
			}
			//static int search(const char *str, int num, const char *words[]) {
			static int search(const char* str, bool(*items_getter)(void*, int, const char**), void* data, int items_count) {
				int scoremax = 0;
				int best = -1;
				for (int i = 0; i < items_count; ++i) {
					const char* word_i = NULL;
					items_getter(data, i, &word_i);
					int score = fuzzy::score(word_i, str);
					int record = (score >= scoremax);
					int draw = (score == scoremax);
					if (record) {
						scoremax = score;
						if (!draw) best = i;
						else {
							const char* word_best = NULL;
							items_getter(data, best, &word_best);
							best = best >= 0 && strlen(word_best) < strlen(word_i) ? best : i;
						}
					}
				}
				return best;
			}
		};

		bool selectionChanged = false;
		if (buffer[0] != '\0') {
			int new_idx = fuzzy::search(buffer, items_getter, data, items_count);
			int idx = new_idx >= 0 ? new_idx : *current_item;
			selectionChanged = *current_item != idx;
			*current_item = idx;
		}

		bool arrowScroll = false;
		int arrowScrollIdx = *current_item;

		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_UpArrow))) {
			if (*current_item > 0)
			{
				*current_item -= 1;
				arrowScroll = true;
				ImGui::SetWindowFocus();
			}
		}
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_DownArrow))) {
			if (*current_item < items_count - 1)
			{
				*current_item += 1;
				arrowScroll = true;
				ImGui::SetWindowFocus();
			}
		}

		if (done && !arrowScroll) {
			ImGui::CloseCurrentPopup();
		}

		bool done2 = false;

		for (int n = 0; n < items_count; n++)
		{
			bool is_selected = n == *current_item;
			if (is_selected && (ImGui::IsWindowAppearing() || selectionChanged)) 
			{
				ImGui::SetScrollHereY();
			}

			if (is_selected && arrowScroll) {
				ImGui::SetScrollHereY();
			}

			const char* select_value = NULL;
			items_getter(data, n, &select_value);
			if (ImGui::Selectable(select_value, is_selected)) 
			{
				selectionChanged = *current_item != n;
				*current_item = n;
				strcpy(buffer, select_value);
				ImGui::CloseCurrentPopup();

				done2 = true;
			}
		}

		if (arrowScroll) 
		{
			const char* sActiveidxValue2 = NULL;
			items_getter(data, *current_item, &sActiveidxValue2);
			strcpy(buffer, sActiveidxValue2);
			ImGuiWindow* window = ImGui::FindWindowByName(name);
			const ImGuiID id = window->GetID("##inputText");
			ImGuiInputTextState* state = ImGui::GetInputTextState(id);

			const char* buf_end = NULL;
			state->CurLenW = ImTextStrFromUtf8(state->TextW.Data, state->TextW.Size, buffer, NULL, &buf_end);
			state->CurLenA = (int)(buf_end - buffer);
			state->CursorClamp();
		}

		ImGui::EndChild();
		ImGui::EndPopup();

		const char* sActiveidxValue3 = NULL;
		items_getter(data, *current_item, &sActiveidxValue3);
		bool ret1 = (selectionChanged && !strcmp(sActiveidxValue3, buffer));

		bool widgetRet = done || done2 || ret1;

		return widgetRet;
	}

	static bool BeginCustomComboBox(const char* label, const char* default_item)
	{
		BeginProperty(label);
		return ImGui::BeginCombo("##X", default_item);
	}

	static void EndCustomComboBox(bool closeCombo)
	{
		if(closeCombo) ImGui::EndCombo();
		EndProperty(false);
	}

	// WIP
	static bool SimpleComboSearch(const char* label, int& current_item, const char* items[], int items_count)
	{
		ImGui::Spacing();
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);

		if (ImGui::BeginCombo(s_IDBuffer, items[current_item]))
		{
			for (int n = 0; n < items_count; n++)
			{
				const bool is_selected = (current_item == n);
				if (ImGui::Selectable(items[n], is_selected))
				{
					current_item = n;
					modified = true;
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}



		ImGui::PopItemWidth();
		ImGui::NextColumn();
		ImGui::Spacing();
		return modified;
	}

	static bool PropertySelectionSearch(const char* label, int& current_item, const char* items[], int max_items)
	{
		ImGui::Spacing();
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);
		static char buf[128] = { 0x00 };
		static char sel[128] = { 0x00 };
		if (ComboAutoSelect(s_IDBuffer, buf, IM_ARRAYSIZE(buf), &current_item, items, max_items))
		{
			modified = true;
			sprintf(sel, "%s", buf);
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();
		ImGui::Spacing();

		return modified;
	}

	static bool PropertySelection(const char* label, int& current_item, const char* items[], int max_items)
	{
		ImGui::Spacing();
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::Combo(s_IDBuffer, &current_item, items, max_items))
		{
			modified = true;
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();
		ImGui::Spacing();

		return modified;
	}

}
