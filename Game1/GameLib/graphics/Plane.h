#pragma once
#include <common/GameObject.h>
#include <dynamics/Math.h>

using Hirai::Vector2;
using Hirai::Vector3;

class Renderer;

class Plane : public GameObject
{
public:
	Plane() {}
	Plane(const char* textureName, int tex_w, int tex_h);
	virtual ~Plane();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void Finalize() override;

	/* フェードイン・フェードアウト処理のための内部クラス */
	class State
	{
	public:
		/**
		*	@return 処理が完了したらtrueを返す
		*/
		virtual bool Update(Plane* plane) = 0;
	};

	State* state;

	bool UpdateFade() {
		return state->Update(this);
	}

	class FadeIn : public State
	{
	private:
		bool isUpdated;
		float targetAlpha, currentAlpha;

	public:
		FadeIn() : isUpdated(false), targetAlpha(1.f), currentAlpha(0.f) {}
		bool Update(Plane* plane) override;
	};

	/**
	*	Displayクラス
	*	@brief 画像を表示し続ける。そのため、Updateの戻り値は常にfalseとなるため、
	*			表示終了のハンドリングは呼び出し元で行う必要がある。
	*/
	class Display : public State
	{
	public:
		bool Update(Plane* plane) { return false; }
	};

	class FadeOut : public State
	{
	public:
		bool Update(Plane* plane) override;
	};

private:
	float life;	// 消えるまでの猶予
	bool useAlpha;
};

