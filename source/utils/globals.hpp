#pragma once
#pragma once
#include <memory>

//usage is like this: vars->UI->ToggleMenu
//i prefere this way because it is easier to read and understand
// and fuck namespaces giving 500 errors and needing to external call it

class Camera {
public:


};

class Player {
public:


};

class Weapon {
public:


};

class Movement {
public:


};

class Visuals {
public:


};

class World {
public:


};

class Wardrobe {
public:


};

class Vehicles {
public:


};

class AI {
public:


};

class UI {
public:
    bool ToggleMenu = true;
    bool RadialMenu = false;
    bool Watermark = true;
    bool CustomCursor = true;
    int tab = 1;
};

class Engine {
public:


};

class Audio {
public:


};

class Exploits {
public:


};

class Inventory {
public:


};

class Physics {
public:


};

class Network {
public:


};

class LOD { // for imgui init etc bla bla bla 
public:

    bool init = false;
    bool init_hook = false;
};

class vars {
public:
    Camera* Camera = new class Camera();
    Player* Player = new class Player();
    Weapon* Weapon = new class Weapon();
    Movement* Movement = new class Movement();
    Visuals* Visuals = new class Visuals();
    World* World = new class World();
    Wardrobe* Wardrobe = new class Wardrobe();
    Vehicles* Vehicles = new class Vehicles();
    AI* AI = new class AI();
    UI* UI = new class UI();
    Engine* Engine = new class Engine();
    Audio* Audio = new class Audio();
    Exploits* Exploits = new class Exploits();
    Inventory* Inventory = new class Inventory();
    Physics* Physics = new class Physics();
    Network* Network = new class Network();
    LOD* LOD = new class LOD();

    static vars* Get() {
        static vars instance;
        return &instance;
    }
};

#define vars vars::Get()