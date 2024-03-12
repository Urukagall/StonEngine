#include "Shoot.h"

Shoot::Shoot(Entity* pEntity) : Script(pEntity) {

}

void Shoot::OnLoad()
{
}

void Shoot::Update(float dt) {
	//OutputDebugStringA("aaaaaaaaa\n");
	Input input;
	if (input.getKey(rollLeft)) {
		OutputDebugStringA("cccccccccccccc\n");
	}

}

Shoot::~Shoot()
{
}