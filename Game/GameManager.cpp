#include "GameManager.h"

GameManager::GameManager(Entity* pEntity) : Script(pEntity) {
	this->pEntity = pEntity;
}

//On Load
void GameManager::OnLoad() {
    pEntity->m_pRender->CreateParticle(0.0, 0.0, 0.0, "blue", 6000, 10000, 10, 80, 10, 200, 100);
    pEntity->m_pRender->CreateEntityCube(2.0, 2.0, 2.0, "blue");
    pEntity->m_pRender->CreateEntityMissiles(3.0, 3.0, 3.0);
    pEntity->m_pRender->CreateEntityEnemy(5.0, 5.0, 5.0);
}

//Update
void GameManager::Update(float dt) {
	
}

//On Destroy
GameManager::~GameManager()
{

}

//Methods