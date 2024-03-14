#include "GameManager.h"
#include "Shoot.h"

GameManager::GameManager(Entity* pEntity) : Script(pEntity) {
}

void GameManager::InitResources() {
}

//On Load
void GameManager::OnLoad() {
    m_oEntity->m_pRender->CreateParticle(0.0, 0.0, 0.0, "blue", 6000, 10000, 10, 80, 10, 200, 100);
    m_oEntity->m_pRender->CreateEntityCube(2.0, 2.0, 2.0, "blue");
    //pEntity->m_pRender->CreateEntityMissiles(3.0, 3.0, 3.0);
    m_oEntity->m_pRender->CreateEntityEnemy(5.0, 5.0, 5.0);
    m_oEntity->newTexture("image", "../img/rocks.dds");
    m_oEntity->m_oMeshRenderers["cube"]->setTexture(m_oEntity->getTexture("rocks"));

    Shoot* pShoot = new Shoot(m_oEntity);
    m_oEntity->CreateScript(pShoot);
}

//Update
void GameManager::Update(float dt) {
    InitResources();
}

//On Destroy
GameManager::~GameManager()
{

}

//Methods