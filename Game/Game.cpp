// Game.cpp : Définit le point d'entrée de l'application.
//

#include "framework.h"
#include "Game.h"
#include "../Project1/StonEngine.h"


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
        theApp.CreateParticlesExplosion(3.0, 3.0, 3.0);
        theApp.CreateEntityCube(2.0, 2.0, 2.0, "blue");
        theApp.CreateEntityMissiles(3.0, 3.0, 3.0);
        theApp.CreateEntityEnemy(5.0, 5.0, 5.0);
        Input input;
        if (input.getKeyDown(ARROW_LEFT)) {
            OutputDebugStringA("ffff");
        };
        return theApp.Run();
    }
    catch (DxException& e)
    {
        MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
        return 0;
    }

}