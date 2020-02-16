#include "GameObject.h"
#include <iostream>
#include <algorithm>
#include <graphics/DirectX.h>
//#include "Scene.h"
#include <camera/Camera.h>
#include <graphics/Renderer.h>
#include "utility.h"

// グローバル変数
//map<string*, GameObject*> GameObject::game_objects_;
vector<GameObject*> GameObject::game_object_list_;

GameObject::GameObject(string name = { "" })
	: local_position_(D3DXVECTOR3{ 0.f, 0.f, 0.f }),
	local_rotation_(D3DXQUATERNION{ 0.f, 0.f, 0.f, 1.f }),	// 初期化として単位回転を設定している
	local_scale_(D3DXVECTOR3{ 1.f, 1.f, 1.f }),
	active_(true), update_flag_(true), rendering_flag_(true),
	name_(name), layer_(Layer::kOther)
{
	component_list_ = {};
	game_object_list_.push_back(this);
}

void GameObject::InitObjectAll()
{
	for (GameObject* obj : game_object_list_)
	{
		obj->Initialize();
	}
}

void GameObject::UpdateObjectAll()
{
	for (GameObject* obj : game_object_list_)
	{
		if (obj->CanUpdate())
			obj->Update();
	}
}

void GameObject::Destroy(GameObject* obj)
{
	if (obj == nullptr) return;
	component_list_.clear();

	std::cout << game_object_list_.size() << std::endl;
	for (vector<GameObject*>::iterator itr = game_object_list_.begin(); itr != game_object_list_.end(); itr++)
	{
		if (*itr == obj)
		{
			game_object_list_.erase(itr);
			delete this;
			return;
		}
	}
}

void GameObject::AddComponent(string name, Component* compo)
{
	if (compo->game_object_ == nullptr) {
		component_list_[name] = compo;
		compo->game_object_ = this;
	}
}

static bool CompareLayer(GameObject* objA, GameObject* objB)
{
	return objA->layer_ < objB->layer_;
}

void GameObject::DrawObjectAll()
{
	using Hirai::DirectX;
	LPDIRECT3DDEVICE9 device = DirectX::GetD3DDevice();
	device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(255, 255, 255), 1.f, 0);
	if (SUCCEEDED(device->BeginScene())) {
		for (GameObject* obj : game_object_list_)
		{
			if (obj->CanRendering()) {
				obj->Draw();
			}
		}
	}
	device->EndScene();
	device->Present(nullptr, nullptr, nullptr, nullptr);


	////Camera* cam = Scene::GetScene()->camera_list_[0];
	////D3DCOLOR col = D3DCOLOR_XRGB(
	////	cam->back_ground_color_.r,
	////	cam->back_ground_color_.g,
	////	cam->back_ground_color_.b
	////);
	//D3DCOLOR col = D3DCOLOR_XRGB(255, 255, 255);

	//// カリングはオフ
	//d3d_dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//// ライトをオフ
	//d3d_dev->SetRenderState(D3DRS_LIGHTING, false);
	//d3d_dev->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, col, 1.f, 0);
	//d3d_dev->BeginScene();
	//// レイヤー順に並べ替え
	//std::sort(game_object_list_.begin(), game_object_list_.end(), CompareLayer);
	//for (GameObject* obj : game_object_list_)
	//{
	//	if (obj->CanRendering()) {
	//		obj->Draw();
	//	}
	//}
	//d3d_dev->EndScene();
	//d3d_dev->Present(nullptr, nullptr, nullptr, nullptr);
}

void GameObject::DeleteObjectAll()
{
	game_object_list_.erase(game_object_list_.begin(), game_object_list_.end());
}
