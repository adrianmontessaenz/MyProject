/*// -----------------------------------------------------------------
*  File:		Components.cpp
*  Brief:		Implementation for components base
*  Creation:	21/10/2022
*  Last Update:	21/10/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/

#include <pch.h>
#include "Components.hpp"
#include <Core/Entity-Component/Object.hpp>

/// -----------------------------------------------------------------
/// Sets owner of component
/// -----------------------------------------------------------------
void Engine::Component::SetOwner(Engine::Object* owner)
{
	mOwner = owner;
}

/// -----------------------------------------------------------------
/// Returns owner of components
/// -----------------------------------------------------------------
const Engine::Object* Engine::Component::GetOwner() const
{
	return mOwner;
}
