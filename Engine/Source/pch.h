/*// -----------------------------------------------------------------
*  File:		pch.h
*  Brief:		Precompiled header for engine
*  Creation:	16/10/2022
*  Last Update:	16/10/2022
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
#include <gtc/type_ptr.hpp>

#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

//Engine
//Core
#include <Core/Base/RunTime.hpp>
#include <Core/Base/Singleton.hpp>
#include <Core/Base/Identity.hpp>
#include <Core/Base/RTTI.hpp>
#include <Core/Events/SDLEvents.hpp>
#include <Core/Entity-Component/Components.hpp>
#include <Core/Platform/Window.hpp>

//Debug
#include <Debug/DebugSystem.hpp>

//Math
#include <Math/Transform.hpp>

//Graphics
#include <Graphics/Shader.hpp>
