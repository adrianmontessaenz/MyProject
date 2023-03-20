/*// -----------------------------------------------------------------
*  File:		pch.h
*  Brief:		Precompiled header for engine
*  Creation:	16/10/2022
*  Last Update:	19/03/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once
//Data structures
#include <vector>
#include <map>
#include <string>
#include <array>
#include <queue>
#include <stack>

//other
#include <ctime>
#include <iostream>
#include <sstream>
#include <fstream>
#include <chrono>
#include <typeinfo>
#include <assert.h>

//ThirdParty
#include <SDL.h>
#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <gtc/type_ptr.hpp>
#include <stb_image.h>

#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include <json.hpp>

//Engine
//Core
#include "Core/Base/RunTime.hpp"
#include "Core/Base/Singleton.hpp"
#include "Core/Base/Identity.hpp"
#include "Core/Base/RTTI.hpp"
#include "Core/Events/SDLEvents.hpp"
#include "Core/Serialization/Serialized.hpp"
#include "Core/Entity-Component/Components.hpp"
#include "Math/Transform.hpp"
#include "Core/Entity-Component/Object.hpp"
#include "Core/Scene/Space.hpp"
#include "Core/Scene/ObjectManager.hpp"
#include "Core/Platform/Window.hpp"

//Debug
#include "Debug/DebugSystem.hpp"

//Graphics
#include "Graphics/Utils/Shader.hpp"
#include "Graphics/Utils/Texture.hpp"
