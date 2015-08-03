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
#include <random>
#include <SFML/Graphics.hpp>
#include "Components.h"
#include <stdint.h>

class System;
class Entity;

class Random
{
public:
    template<class T>
    static T Next(T min, T max)
    {
        std::uniform_int_distribution<T> uniform_dist(min, max);
        return uniform_dist(s_re);
    }
    
	template<>
	static double Next(double min, double max)
	{
		std::uniform_real_distribution<double> uniform_dist(min, max);
		return uniform_dist(s_re);
	}

    template<class T>
    static T NextNormal(T mean, T range)
    {
        std::normal_distribution<T> distribution(mean,range);
        return distribution(s_re);
    }
    
    static std::random_device s_rd;
    static std::linear_congruential_engine<uint_fast32_t, 48271, 0, 2147483647> s_re;
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
