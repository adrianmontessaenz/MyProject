/*// -----------------------------------------------------------------
*  File:		Object.cpp
*  Brief:		Implementation of Object class
*  Creation:	21/10/2022
*  Last Update:	21/10/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "Object.hpp"

void Engine::Object::Initialize()
{
	//Add to RTTI
	RTTI::AddParentedType<Object, RunTime>();
	RTTI::AddParentedType<Object, Identity>();

	for (auto comp : mEngineComps)
		comp->Initialize();
	for (auto comp : mLogicComps)
		comp->Initialize();
}

void Engine::Object::Update()
{
	for (auto comp : mEngineComps)
		comp->Update();
}

void Engine::Object::LogicUpdate()
{
	for (auto comp : mLogicComps)
		comp->LogicUpdate();
}

void Engine::Object::Shutdown()
{
	for (auto comp : mEngineComps)
		comp->Shutdown();
	for (auto comp : mLogicComps)
		comp->Shutdown();
}
