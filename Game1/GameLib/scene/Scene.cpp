#include "Scene.h"
#include <camera/Camera.h>
#include <common/utility.h>

Scene* Scene::kCurrentScene = nullptr;

Scene::~Scene()
{
	//for (auto cam : camera_list_) {
	//	SAFE_DELETE(cam);
	//}
}

void Scene::SetCamera(Camera* cam)
{
	//for (auto camera : camera_list_)
	//{
	//	if (camera->Name() == cam->Name()) {
	//		return;
	//	}
	//}
	//camera_list_.push_back(cam);
}

void Scene::InitializeScene()
{
}

void Scene::UpdateScene()
{
	kCurrentScene->Update();
}

void Scene::DrawScene()
{
	kCurrentScene->Draw();
}

void Scene::SetScene(Scene* sc)
{
	if (kCurrentScene)
	{
		kCurrentScene->Finalize();
		delete kCurrentScene;
		kCurrentScene = nullptr;
	}
	if (sc)
	{
		sc->Initialize();
		kCurrentScene = sc;
	}
}

void Scene::FinalizeScene()
{
	if (kCurrentScene)
	{
		kCurrentScene->Finalize();
		SAFE_DELETE(kCurrentScene);
	}
}
