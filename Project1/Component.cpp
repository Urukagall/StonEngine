#include "pch.h"
#include "Component.h"

Component::Component(Entity* pEntity) {
	m_oEntity = pEntity;
}

Component::~Component() {
	//delete mBoxGeo;
}