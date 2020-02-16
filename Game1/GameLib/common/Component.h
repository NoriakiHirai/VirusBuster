#ifndef GAME_LIB_GAME_LIB_COMPONENT_H_
#define GAME_LIB_GAME_LIB_COMPONENT_H_

/////////////////////////////////////////////////////////////////
// �C���N���[�h�錾
/////////////////////////////////////////////////////////////////
#include <string>
#include <d3dx9.h>
#include "../dynamics/Math.h"

using std::string;
using Hirai::Vector2;

/////////////////////////////////////////////////////////////////
// �O���錾
/////////////////////////////////////////////////////////////////
class GameObject;

class Component
{
	friend class GameObject;

private:
	GameObject* game_object_;
	string		tag_;

protected:
	virtual ~Component() {}

public:
	Component() :game_object_(nullptr), tag_("") {}
	GameObject& gameObject() { return *game_object_; }
	string Tag() const { return tag_; }
	void SetTag(string tag) { tag_ = tag; }
	string Name() const;
	D3DXVECTOR3 GetObjectPosition() const;
};

#endif // !GAME_LIB_GAME_LIB_COMPONENT_H_
