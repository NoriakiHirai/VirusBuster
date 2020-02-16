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

	/* �t�F�[�h�C���E�t�F�[�h�A�E�g�����̂��߂̓����N���X */
	class State
	{
	public:
		/**
		*	@return ����������������true��Ԃ�
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
	*	Display�N���X
	*	@brief �摜��\����������B���̂��߁AUpdate�̖߂�l�͏��false�ƂȂ邽�߁A
	*			�\���I���̃n���h�����O�͌Ăяo�����ōs���K�v������B
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
	float life;	// ������܂ł̗P�\
	bool useAlpha;
};

