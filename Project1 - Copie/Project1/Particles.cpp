#include "pch.h"
#include "Particles.h"

Particles::Particles(Entity* pEntity) : MeshRenderer(pEntity) {
	
	Box(XMFLOAT4(Colors::Black));
}

