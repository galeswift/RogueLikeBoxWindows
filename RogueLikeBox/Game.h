//
//  Game.h
//  RogueLikeBox
//
//  Created by Keith Miron on 2/18/15.
//  Copyright (c) 2015 Acoustic Games. All rights reserved.
//

#ifndef __RogueLikeBox__Game__
#define __RogueLikeBox__Game__

#include <stdio.h>
#include <vector>
#include <SFML/Graphics.hpp>

#include "Components.h"

#define RAD_TO_DEG (57.2957795)
#define DEG_TO_RAD (1/RAD_TO_DEG)

class System;
class Entity;

class Random
{
public:
    template<class T>
    static T Next(T min, T max)
    {
        float normalized = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        return (normalized * (max + 1 -min)) + min;
    }
};

class Game
{
public:
    Game();
    ~Game();
    void Init();
    void Draw(sf::RenderWindow* window);
    void Update(float dt);
    
    Entity* CreateEntity();
    void RegisterSystem(System* system);
    
private:
    std::vector<System*> m_systems;
    std::vector<Entity*> m_objects;
};
#endif /* defined(__RogueLikeBox__Game__) */
