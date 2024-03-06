#include "pch.h"
#include "Atom.h"



Atom::Atom( float startLife)
    :  life(startLife) {}

void Atom::Update(float deltaTime) {


    // Decrease particle life
    life -= deltaTime;
}