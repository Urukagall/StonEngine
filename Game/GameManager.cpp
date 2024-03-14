#include "GameManager.h"
#include "Shoot.h"
#include "Ship.h"
GameManager::GameManager(Entity* pEntity) : Script(pEntity) {
}

//On Load
void GameManager::OnLoad() {
    //m_oEntity->m_pRender->CreateParticle(0.0, 0.0, 0.0, "blue", 6000, 10000, 10, 80, 10, 200, 100);
    //m_oEntity->m_pRender->CreateEntityCube(2.0, 2.0, 2.0, "blue");
    ////pEntity->m_pRender->CreateEntityMissiles(3.0, 3.0, 3.0);
    //m_oEntity->m_pRender->CreateEntityEnemy(5.0, 5.0, 5.0);
    //m_oEntity->m_pRender->CreateParticlesExplosion(1.0,1.0,1.0);

    

    Entity* eShip = m_oEntity->m_pRender->CreateEntity(0.f, 0.f, 0.f);
    Ship* pShip = new Ship(eShip);
    pShip->m_vGun = &pShoot->m_vGun;
    eShip->CreateScript(pShip);

    Shoot* pShoot = new Shoot(m_oEntity->m_pRender->CreateEntity(0.f, 0.f, 0.f), pShip->GetShips());
    m_oEntity->CreateScript(pShoot);
}

//Update
void GameManager::Update(float dt) {
	
}

//On Destroy
GameManager::~GameManager()
{

}

//Methods