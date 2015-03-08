//
//  Game.cpp
//  RogueLikeBox
//
//  Created by Keith Miron on 2/18/15.
//  Copyright (c) 2015 Acoustic Games. All rights reserved.
//

#include "Game.h"
#include "Components.h"
#include "Entity.h"
#include "System.h"
#include "Box2DSystem.h"
#include "DungeonSystem.h"
#include "RenderingSystem.h"
#include <stdio.h>

std::random_device Random::s_rd;
std::linear_congruential_engine<std::uint_fast32_t, 48271, 0, 2147483647> Random::s_re(Random::s_rd());
Game::Game()
{};

Game::~Game()
{
    for (std::vector<Entity*>::iterator it = m_objects.begin() ; it != m_objects.end() ; it++)
    {
        delete *it;
    }
}

void Game::Init()
{
    RegisterSystem(new Box2DSystem());
    RegisterSystem(new RenderingSystem());
    RegisterSystem(new DungeonSystem());
}

void Game::Draw(sf::RenderWindow* window)
{
    for (std::vector<System*>::iterator it = m_systems.begin() ; it != m_systems.end() ; it++)
    {
        (*it)->Draw(window);
    }
}

void Game::Update(float dt)
{
    for (std::vector<System*>::iterator it = m_systems.begin() ; it != m_systems.end() ; it++)
    {
        (*it)->Update(dt);
    }
}

Entity* Game::CreateEntity()
{
    Entity* newEntity = new Entity();
    m_objects.push_back(newEntity);
    return newEntity;
}

void Game::RegisterSystem(System* system)
{
    m_systems.push_back(system);
    system->Init(this);
}

