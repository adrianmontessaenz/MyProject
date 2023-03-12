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

/// -----------------------------------------------------------------
/// Custom combo of strings
/// -----------------------------------------------------------------
std::string Editor::MyStringComboFromPath(const char* txt, const std::string& myItem, const std::string& myPath, const bool cutend, const std::string& cutpoint)
{
	//Push id and set text
	ImGui::PushID(&myPath);
	ImGui::Text(txt);
	ImGui::SameLine();

	//Get name and cut and begin combo
	std::string model_name;
	if (cutend)
		model_name = myItem.substr(0, myItem.find_last_of(cutpoint));
	std::string result = myItem;
	if (ImGui::BeginCombo("##", model_name.c_str()))
	{
		//Open folder and search
		for (const auto& entry : std::filesystem::directory_iterator(myPath))
		{
			//Get file and check if its selected
			std::string file = entry.path().filename().string();
			std::string cut;
			size_t offset = 0;
			if (cutend)
			{
				offset = file.find_last_of(cutpoint);
				cut = file.substr(offset, file.size() - offset);
				file = file.substr(0, offset);
			}

			bool selected = file == model_name;
			//If selected, set result to name of file
			if (ImGui::Selectable(file.c_str(), selected))
				result = file + cut;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	//Return result
	ImGui::PopID();
	return result;
}
