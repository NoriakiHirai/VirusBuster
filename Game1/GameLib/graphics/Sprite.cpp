#include "Sprite.h"
#include "DXUtil.h"

Sprite::Sprite()
	: poly_w(256), poly_h(256),
	pivot_x(0.f), pivot_y(0.f),
	uv_left(0.f), uv_top(0.f),
	uv_w(1.f), uv_h(1.f),
	alpha(1.f), posZ(0.f),
	texture(nullptr), is_active(true) {}

Sprite::Sprite(int polyW, int polyH)
	: poly_w(polyW), poly_h(polyH),
	pivot_x(0.f), pivot_y(0.f),
	uv_left(0.f), uv_top(0.f),
	uv_w(1.f), uv_h(1.f),
	alpha(1.f), posZ(0.f),
	texture(nullptr), is_active(true) {}

Sprite::Sprite(const Sprite& sprite)
{
	this->copy(sprite);
}

Sprite::~Sprite()
{
	SAFE_RELEASE(texture);
}

void Sprite::copy(const Sprite& sprite)
{
	SAFE_RELEASE(texture);
	if (sprite.texture) {
		texture = sprite.texture;
	}

	poly_w = sprite.poly_w;
	poly_h = sprite.poly_h;
	is_active = sprite.is_active;
	pivot_x = sprite.pivot_x;
	pivot_y = sprite.pivot_y;
	uv_left = sprite.uv_left;
	uv_top = sprite.uv_top;
	uv_w = sprite.uv_w;
	uv_h = sprite.uv_h;
	alpha = sprite.alpha;
	posZ = sprite.posZ;	
}

Sprite& Sprite::operator=(const Sprite& sprite)
{
	this->copy(sprite);
	return *this;
}

void Sprite::SetTexture(IDirect3DTexture9* tex, bool is_resize)
{
	if (tex)
		tex->AddRef();
	if (this->texture)
		this->texture->Release();
	this->texture = tex;

	if (is_resize)
	{
		D3DSURFACE_DESC desc;
		tex->GetLevelDesc(0, &desc);
		SetSize(desc.Width, desc.Height);
	}
}