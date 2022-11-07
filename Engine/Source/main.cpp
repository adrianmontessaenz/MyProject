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
#include <Core/Scene/Space.hpp>

int main(void)
{
	gWindow->Initialize();
	gTimeSys->Initialize();
	gGfxMgr->Initialize();
	Engine::Space* test = new Engine::Space;
	test->Initialize();

	//Test for components
	Engine::Object* obj = test->AddObject();
	obj->Initialize();
	Engine::Renderable* comp = obj->AddEngineComp<Engine::Renderable>();
	comp->Initialize();
	comp->SetColor(glm::vec4(1.f, 0.f, 1.f, 1.f));
	comp->SetTexture("ID.jpeg");
	obj->GetEngineComp<Engine::Transform>()->SetWorldPos({ 0.f,0.f,-2.f });
	obj->GetEngineComp<Engine::Transform>()->SetWorldRot({ 90.f,0.f,0.f });
	
	Engine::Object* obj2 = test->AddObject();
	obj->AddChild(obj2);
	obj2->Initialize();
	Engine::Renderable* comp2 = obj2->AddEngineComp<Engine::Renderable>();
	comp2->Initialize();
	comp2->SetColor(glm::vec4(1.f));
	comp2->SetTexture("pepe.png");
	obj2->GetEngineComp<Engine::Transform>()->SetLocalPos({ -1.f,0.f,0.5f });

	Engine::Object* obj3 = test->AddObject();
	obj2->AddChild(obj3);
	obj3->Initialize();
	Engine::Renderable* comp3 = obj3->AddEngineComp<Engine::Renderable>();
	comp3->Initialize();
	comp3->SetColor(glm::vec4(1.f));
	comp3->SetTexture("pepe.png");
	obj3->GetEngineComp<Engine::Transform>()->SetLocalPos({ 1.f,0.f,0.5f });
	obj3->GetEngineComp<Engine::Transform>()->SetWorldRot({ 180.f,0.f,0.f });

	while (gWindow->IsEnabled())
	{
		//Update
		gSDLSys->Update();
		gWindow->Update();
		gInputMgr->Update();
		test->Update();

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

		//TEST SPACES
		{
			if (gInputMgr->IsKeyTriggered(SDL_SCANCODE_A))
			{
				obj2->Shutdown();
			}
		}
		
		//TEST TRANSFORM
		{
			//Engine::Transform* trans = obj->GetEngineComp<Engine::Transform>();
			////Engine::Transform* trans2 = obj2->GetEngineComp<Engine::Transform>();
			//if (gInputMgr->IsKeyTriggered(SDL_SCANCODE_A) || gInputMgr->IsKeyPressed(SDL_SCANCODE_A))
			//	trans->SetWorldPos(trans->GetWorldPos() + glm::vec3(-1.f, 0.f, 0.f));
			//if (gInputMgr->IsKeyTriggered(SDL_SCANCODE_W) || gInputMgr->IsKeyPressed(SDL_SCANCODE_W))
			//	trans->SetWorldPos(trans->GetWorldPos() + glm::vec3(0.f, 1.f, 0.f));
			//if (gInputMgr->IsKeyTriggered(SDL_SCANCODE_S) || gInputMgr->IsKeyPressed(SDL_SCANCODE_S))
			//	trans->SetWorldPos(trans->GetWorldPos() + glm::vec3(0.f, -1.f, 0.f));
			//if (gInputMgr->IsKeyTriggered(SDL_SCANCODE_D) || gInputMgr->IsKeyPressed(SDL_SCANCODE_D))
			//	trans->SetWorldPos(trans->GetWorldPos() + glm::vec3(1.f, 0.f, 0.f));
			//if (gInputMgr->IsKeyTriggered(SDL_SCANCODE_E) || gInputMgr->IsKeyPressed(SDL_SCANCODE_E))
			//	trans->SetWorldPos(trans->GetWorldPos() + glm::vec3(0.f, 0.f, 1.f));
			//if (gInputMgr->IsKeyTriggered(SDL_SCANCODE_Q) || gInputMgr->IsKeyPressed(SDL_SCANCODE_Q))
			//	trans->SetWorldPos(trans->GetWorldPos() + glm::vec3(0.f, 0.f, -1.f));

			//if (gInputMgr->IsKeyTriggered(SDL_SCANCODE_R) || gInputMgr->IsKeyPressed(SDL_SCANCODE_R))
			//	trans->SetWorldRot(trans->GetWorldRot() + glm::vec3(0.f, 0.f, 1.f));
			//if (gInputMgr->IsKeyTriggered(SDL_SCANCODE_T) || gInputMgr->IsKeyPressed(SDL_SCANCODE_T))
			//	trans->SetWorldRot(trans->GetWorldRot() + glm::vec3(0.f, 1.f, 0.f));
			//if (gInputMgr->IsKeyTriggered(SDL_SCANCODE_Y) || gInputMgr->IsKeyPressed(SDL_SCANCODE_Y))
			//	trans->SetWorldRot(trans->GetWorldRot() + glm::vec3(1.f, 0.f, 0.f));

			//if (gInputMgr->IsKeyTriggered(SDL_SCANCODE_F) || gInputMgr->IsKeyPressed(SDL_SCANCODE_F))
			//	trans->SetWorldScale(trans->GetWorldScale() + glm::vec3(1.f, 1.f, 1.f));
			//if (gInputMgr->IsKeyTriggered(SDL_SCANCODE_G) || gInputMgr->IsKeyPressed(SDL_SCANCODE_G))
			//	trans2->SetWorldScale(trans2->GetWorldScale() + glm::vec3(1.f, 1.f, 1.f));
			//if (gInputMgr->IsKeyTriggered(SDL_SCANCODE_H) || gInputMgr->IsKeyPressed(SDL_SCANCODE_H))
			//	trans2->SetLocalScale(trans2->GetLocalScale() + glm::vec3(1.f, 1.f, 1.f));
			//if (gInputMgr->IsKeyTriggered(SDL_SCANCODE_J) || gInputMgr->IsKeyPressed(SDL_SCANCODE_J))
			//	trans2->SetLocalScale(trans2->GetLocalScale() - glm::vec3(1.f, 1.f, 1.f));
		}

		//TEST TIME
		{
			/*std::cout << "Delta Time (Seconds): " << gTimeSys->GetDeltaTime() << std::endl;*/
			//std::cout << "FrameRate: " << gTimeSys->GetFrameRate() << "/ms" << std::endl;
		}

		//Render
		gGfxMgr->Render();
		gWindow->Render();
		gTimeSys->Update();
	}

	gWindow->Shutdown();
	return 0;
}