#include "pch.h"
#include "Atom.h"

float generateRandomNumber(float minValue, float maxValue) {
    // Initialiser le générateur de nombres aléatoires avec la graine actuelle du temps
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Générer un nombre aléatoire entre les valeurs spécifiées
    float randomNumber = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (maxValue - minValue) + minValue;

    return randomNumber;
}

Atom::Atom( float startLife, ComPtr<ID3D12Device> md3dDevice, ComPtr<ID3D12GraphicsCommandList> mCommandList, ComPtr<ID3D12DescriptorHeap> mCbvHeap) {
    life = startLife;
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Generate a random number between 0 and RAND_MAX, then normalize it to be between 0 and 1
    float randomNumber = static_cast<float>(std::rand()) / RAND_MAX;
    position = XMFLOAT3(generateRandomNumber(0.01,0.05), 0.0,0.0);
    scale = XMFLOAT3(1.0, 1.0, 1.0);
    m_oEntity = new Entity(md3dDevice, mCommandList, mCbvHeap);
    m_oEntity->CreateCube(DirectX::XMFLOAT4(Colors::Black));
    m_oEntity->m_mTransform.Scale(scale.x,scale.y,scale.z);
}

void Atom::Update(float deltaTime) {


    // Decrease particle life
    life -= deltaTime;
    m_oEntity->m_mTransform.Translation(position.x,position.y,position.z);
}

