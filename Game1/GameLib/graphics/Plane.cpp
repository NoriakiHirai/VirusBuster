#include "Plane.h"
#include <graphics/TextureManager.h>
#include <graphics/Sprite.h>
#include <graphics/UIRenderer.h>
#include <common/utility.h>

Plane::Plane(const char* textureName, int tex_w, int tex_h)
{
	TextureManager::ReadTexture(textureName);
	Sprite* sprite = new Sprite(tex_w, tex_h);
	sprite->SetTexture(TextureManager::GetTexture(textureName), false);

	UIRenderer* renderer = new UIRenderer;
	renderer->SetSprite(sprite);
	AddComponent("UIRenderer", renderer);
}

Plane::~Plane()
{
	SAFE_DELETE(state);
}

void Plane::Initialize()
{
}

void Plane::Update()
{
}

void Plane::Draw()
{
	if (!IsActive()) return;
	UIRenderer* renderer = (UIRenderer*)GetComponent("UIRenderer");
	renderer->Draw(*this);
}

void Plane::Finalize()
{
}

static const float ALPHA_DELTA = 0.03f;
bool Plane::FadeIn::Update(Plane* plane)
{
	UIRenderer* renderer = (UIRenderer*)plane->GetComponent("UIRenderer");
	Sprite* sprite = renderer->GetSprite();
	if (!isUpdated)
	{
		sprite->SetAlpha(0.f);
		isUpdated = true;
	}

	currentAlpha += ALPHA_DELTA;
	bool is_fade_in_comp = currentAlpha >= targetAlpha ? true : false;
	if (is_fade_in_comp) currentAlpha = targetAlpha;

	sprite->SetAlpha(currentAlpha);
	if (is_fade_in_comp)
	{
		delete this;
		return true;
	}
	return false;
}

bool Plane::FadeOut::Update(Plane* plane)
{
	Sprite* sprite = ((UIRenderer*)plane->GetComponent("UIRenderer"))
		->GetSprite();
	float a = sprite->GetAlpha();
	a -= ALPHA_DELTA;

	if (a <= 0.f)
	{
		delete this;
		return true;
	}

	sprite->SetAlpha(a);
	return false;
}
