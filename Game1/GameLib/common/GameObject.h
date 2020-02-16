#ifndef GAME_LIB_GAME_LIB_GAMEOBJECT_H_
#define GAME_LIB_GAME_LIB_GAMEOBJECT_H_

/////////////////////////////////////////////////////////////////
// インクルード宣言
/////////////////////////////////////////////////////////////////
#include <vector>
#include <map>
#include <string>
#include <d3dx9.h>
#include <stdexcept>

using std::vector;
using std::map;
using std::string;

/////////////////////////////////////////////////////////////////
// 列挙型
/////////////////////////////////////////////////////////////////
// レイヤー名
enum Layer {
	kNone = 0,
	kPlayerUI,
	kCloud,
	kStage,
	kPlayer,
	kMsg,
	kOther,
};

/////////////////////////////////////////////////////////////////
// 定数
/////////////////////////////////////////////////////////////////
static const int kObjectMax = 1024;
static const float kGravity = 0.8f;	// 重力加速度

/////////////////////////////////////////////////////////////////
// 前方宣言
/////////////////////////////////////////////////////////////////
class Component;

class GameObject
{	
	friend class Component;

private:
	//static map<string*, GameObject*> game_objects_;
	static vector<GameObject*> game_object_list_;
	map<string, Component*> component_list_;
	bool update_flag_;
	bool rendering_flag_;
	bool collision_flag_;
	string name_;
	string tag_;

protected:
	bool active_;

public:
	D3DXVECTOR3 local_position_;
	D3DXQUATERNION local_rotation_;
	//D3DXVECTOR3 local_rotation_;
	D3DXVECTOR3 local_scale_;

	// レイヤー番号。デフォルトは5
	Layer layer_;

	/* フレンド関数用インターフェイス */
	// いずれTransformクラスを作って、まとめて渡したい
	D3DXVECTOR3 GetPosition() const { return local_position_; }

	GameObject() : GameObject("") {}
	GameObject(string name);
	virtual ~GameObject() {}

	virtual void Initialize() {};
	virtual void Update() {};
	virtual void Draw() {};
	virtual void Finalize() {};

	//virtual void CreateObject(int type, const D3DXVECTOR3& init_pos) = 0;
	static void AddGameObject(GameObject* obj) {
		if (obj) game_object_list_.push_back(obj);
	}
	static void InitObjectAll();
	static void UpdateObjectAll();
	static void DrawObjectAll();
	static void DeleteObjectAll();
	//static GameObject& GetGameObject(string* name)
	//{
	//	return *game_objects_[name];
	//}

	static GameObject* Find(const string& name)
	{
		for (GameObject* obj : game_object_list_)
		{
			if (obj->name_ == name)
			{
				return obj;
			}
		}
		return nullptr;
	}
	
	void Destroy(GameObject* obj);
	void AddComponent(string name, Component* compo);

	Component* GetComponent(const string& name) const
	{
		Component* compo = nullptr;
		try
		{
			compo = component_list_.at(name);
		}
		catch (const std::out_of_range&)
		{
			// 特に何もしない
		}
		return compo;
	}

	void SetTag(string tag) { tag_ = tag; }
	string Tag() const { return tag_; }
	void SetName(string name) { name_ = name; }
	string Name() const { return name_; }
	void SetActive(bool flag) { active_ = flag; }
	bool IsActive() const { return active_; }
	bool CanUpdate() const { return update_flag_; }
	bool CanRendering() const { return rendering_flag_; }
	void SetRenderingFlag(bool flag) { rendering_flag_ = flag; }
	void SetLayer(Layer layer) { layer_ = layer; }

};

#endif // !GAME_LIB_GAME_LIB_GAMEOBJECT_H_
