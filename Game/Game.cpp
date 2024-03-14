// Game.cpp : Définit le point d'entrée de l'application.
//

#include "framework.h"
#include "Game.h"
#include "../Project1/StonEngine.h"
#include "Shoot.h"
#include "GameManager.h"


#pragma comment(lib,"dxguid.lib")
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
    PSTR cmdLine, int showCmd)
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    try
    {
        Render theApp(hInstance);
        if (!theApp.Initialize())
            return 0;
        /*Entity* fire = theApp.m_Entities[0];
        Shoot* shoot = new Shoot(fire);
        theApp.m_Entities[0]->CreateScript(shoot);*/
        
        Entity* pEntity = theApp.CreateEntity(0.f, 0.f, 0.f);
        GameManager* pGameManager = new GameManager(pEntity);
        pEntity->CreateScript(pGameManager);
        return theApp.Run();
    }
    catch (DxException& e)
    {
        MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
        return 0;
    }

}