/*// -----------------------------------------------------------------
*  File:		main.cpp
*  Brief:		Main of engine (temporal)
*  Creation:	13/10/2022
*  Last Update:	13/10/2022
* 
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include <Core/Platform/Window.hpp>
#include <Core/Platform/InputManager.hpp>
#include <Core/Time/TimeSystem.hpp>
#include <iostream>
#include <Core/Entity-Component/Object.hpp>
#include <Graphics/GraphicsManager.hpp>
#include <Graphics/Renderable.hpp>

int main(void)
{
	gWindow->Initialize();
	gTimeSys->Initialize();
	gGfxMgr->Initialize();

	//Test for components
	Engine::Object* obj = new Engine::Object();
	obj->Initialize();
	Engine::Renderable* comp = obj->AddEngineComp<Engine::Renderable>();
	comp->Initialize();
	comp->SetColor({ 0.f,0.0f,1.f });
	obj->GetEngineComp<Engine::Transform>()->SetWorldPos({ -0.5f,2.5f,-10.f });
	
	Engine::Object* obj2 = new Engine::Object();
	obj->AddChildren(obj2);
	obj2->Initialize();
	Engine::Renderable* comp2 = obj2->AddEngineComp<Engine::Renderable>();
	comp2->Initialize();
	comp2->SetColor({ 0.f,0.0f,0.f });
	obj2->GetEngineComp<Engine::Transform>()->SetLocalPos({ -1.f,0.f,0.5f });

	while (gWindow->IsEnabled())
	{
		//Update
		gSDLSys->Update();
		gWindow->Update();
		gInputMgr->Update();

		//Logic Update
		if (gInputMgr->IsKeyTriggered(SDL_SCANCODE_F11))
		{
			if (gWindow->GetWindowMode() == Engine::WindowMode::WINDOW_FULLSCREEN)
				gWindow->SetWindowMode(Engine::WindowMode::WINDOW_NORMAL);
			else
				gWindow->SetWindowMode(Engine::WindowMode::WINDOW_FULLSCREEN);
		}
		else if (gInputMgr->IsKeyTriggered(SDL_SCANCODE_ESCAPE))
			gWindow->SetEnabled(false);

		Engine::Transform* trans = obj->GetEngineComp<Engine::Transform>();
		Engine::Transform* trans2 = obj2->GetEngineComp<Engine::Transform>();
		if (gInputMgr->IsKeyTriggered(SDL_SCANCODE_A) || gInputMgr->IsKeyPressed(SDL_SCANCODE_A))
			trans->SetWorldPos(trans->GetWorldPos() + glm::vec3(-1.f, 0.f, 0.f));
		if (gInputMgr->IsKeyTriggered(SDL_SCANCODE_W) || gInputMgr->IsKeyPressed(SDL_SCANCODE_W))
			trans->SetWorldPos(trans->GetWorldPos() + glm::vec3(0.f, 1.f, 0.f));
		if (gInputMgr->IsKeyTriggered(SDL_SCANCODE_S) || gInputMgr->IsKeyPressed(SDL_SCANCODE_S))
			trans->SetWorldPos(trans->GetWorldPos() + glm::vec3(0.f, -1.f, 0.f));
		if (gInputMgr->IsKeyTriggered(SDL_SCANCODE_D) || gInputMgr->IsKeyPressed(SDL_SCANCODE_D))
			trans->SetWorldPos(trans->GetWorldPos() + glm::vec3(1.f, 0.f, 0.f));
		if (gInputMgr->IsKeyTriggered(SDL_SCANCODE_E) || gInputMgr->IsKeyPressed(SDL_SCANCODE_E))
			trans->SetWorldPos(trans->GetWorldPos() + glm::vec3(0.f, 0.f, 1.f));
		if (gInputMgr->IsKeyTriggered(SDL_SCANCODE_Q) || gInputMgr->IsKeyPressed(SDL_SCANCODE_Q))
			trans->SetWorldPos(trans->GetWorldPos() + glm::vec3(0.f, 0.f, -1.f));

		if (gInputMgr->IsKeyTriggered(SDL_SCANCODE_R) || gInputMgr->IsKeyPressed(SDL_SCANCODE_R))
			trans->SetWorldRot(trans->GetWorldRot() + glm::vec3(0.f, 0.f, 1.f));
		if (gInputMgr->IsKeyTriggered(SDL_SCANCODE_T) || gInputMgr->IsKeyPressed(SDL_SCANCODE_T))
			trans->SetWorldRot(trans->GetWorldRot() + glm::vec3(0.f, 1.f, 0.f));
		if (gInputMgr->IsKeyTriggered(SDL_SCANCODE_Y) || gInputMgr->IsKeyPressed(SDL_SCANCODE_Y))
			trans->SetWorldRot(trans->GetWorldRot() + glm::vec3(1.f, 0.f, 0.f));

		if (gInputMgr->IsKeyTriggered(SDL_SCANCODE_F) || gInputMgr->IsKeyPressed(SDL_SCANCODE_F))
			trans->SetWorldScale(trans->GetWorldScale() + glm::vec3(1.f, 1.f, 1.f));
		if (gInputMgr->IsKeyTriggered(SDL_SCANCODE_G) || gInputMgr->IsKeyPressed(SDL_SCANCODE_G))
			trans2->SetWorldScale(trans2->GetWorldScale() + glm::vec3(1.f, 1.f, 1.f));
		if (gInputMgr->IsKeyTriggered(SDL_SCANCODE_H) || gInputMgr->IsKeyPressed(SDL_SCANCODE_H))
			trans2->SetLocalScale(trans2->GetLocalScale() + glm::vec3(1.f, 1.f, 1.f));
		if (gInputMgr->IsKeyTriggered(SDL_SCANCODE_J) || gInputMgr->IsKeyPressed(SDL_SCANCODE_J))
			trans2->SetLocalScale(trans2->GetLocalScale() - glm::vec3(1.f, 1.f, 1.f));
			

		//Render
		/*std::cout << "Delta Time (Seconds): " << gTimeSys->GetDeltaTime() << std::endl;*/
		//std::cout << "FrameRate: " << gTimeSys->GetFrameRate() << "/ms" << std::endl;
		gGfxMgr->Render();
		gWindow->Render();
		gTimeSys->Update();
	}

	gWindow->Shutdown();
	return 0;
}