#ifndef SCENE_H_
#define SCENE_H_

#include <vector>
#include <memory>

using std::vector;

class Camera;

class Scene
{
	static Scene* kCurrentScene;

public:
	std::shared_ptr<Camera> camera;

	Scene() {}
	virtual ~Scene();

	void SetCamera(Camera* cam);

	static void InitializeScene();
	static void UpdateScene();
	static void DrawScene();
	static void SetScene(Scene* sc);
	static Scene* GetScene() { return Scene::kCurrentScene; }
	static void FinalizeScene();

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Finalize() = 0;
};

#endif // !SCENE_H_
