/*// -----------------------------------------------------------------
*  File:		EditorHelpers.cpp
*  Brief:		Implementation for helpers of ImGui in editor
*  Creation:	06/03/2023
*  Last Update:	06/03/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include "EditorHelpers.hpp"

/// -----------------------------------------------------------------
/// Custom DragInt
/// -----------------------------------------------------------------
void Editor::MyDragInt(const char* txt, int* value, const float size, const int min, const int max, const float speed)
{
	//Push id and width
	ImGui::PushID(value);
	ImGui::PushItemWidth(size);

	//Set text and drag
	if (txt)
	{
		ImGui::Text(txt);
		ImGui::SameLine();
	}
	ImGui::DragInt("##", value, speed, min, max);

	//Pop width and id
	ImGui::PopItemWidth();
	ImGui::PopID();
}

/// -----------------------------------------------------------------
/// Custom DragFloat
/// -----------------------------------------------------------------
void Editor::MyDragFloat(const char* txt, float* value, const float size, const float min, const float max, const float speed)
{
	//Push id and width
	ImGui::PushID(value);
	ImGui::PushItemWidth(size);

	//Set text and drag
	if (txt)
	{
		ImGui::Text(txt);
		ImGui::SameLine();
	}
	ImGui::DragFloat("##", value, speed, min, max);

	//Pop width and id
	ImGui::PopItemWidth();
	ImGui::PopID();
}
