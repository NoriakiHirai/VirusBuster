#include "Component.h"
#include "GameObject.h"

string Component::Name() const {
	return game_object_ == nullptr ? "" : game_object_->Name();
}

D3DXVECTOR3 Component::GetObjectPosition() const {
	return game_object_->GetPosition();
}