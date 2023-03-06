/*// -----------------------------------------------------------------
*  File:		EditorHelpers.hpp
*  Brief:		Header for helpers of ImGui in editor
*  Creation:	06/03/2023
*  Last Update:	06/03/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once
#include <pch.h>

namespace Editor
{
	//Drag Int helpers
	void MyDragInt(const char* txt, int* value, const float size = 100.f, const int min = 0.f, const int max = std::numeric_limits<int>().max(), const float speed = 1.0f);

	//Drag Float helpers
	void MyDragFloat(const char* txt, float* value, const float size = 100.f, const float min = 0.f, const float max = std::numeric_limits<float>().max(), const float speed = 1.0f);
}