#ifndef GAME_LIB_GAME_LIB_RENDERER_H_
#define GAME_LIB_GAME_LIB_RENDERER_H_

#include <common/Component.h>

class Camera;
class GameObject;

class Renderer :
	public Component
{
public:
	//Camera* camera_;

	//Renderer() : camera_(nullptr) {}
	Renderer() {}
	virtual ~Renderer();

	virtual void Draw(const GameObject&) = 0;
};

#endif // !GAME_LIB_GAME_LIB_RENDERER_H_
