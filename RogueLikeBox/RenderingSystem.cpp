//
//  RenderingSystem.cpp
//  RogueLikeBox
//
//  Created by Keith Miron on 2/28/15.
//  Copyright (c) 2015 Acoustic Games. All rights reserved.
//

#include "RenderingSystem.h"
#include "Components.h"
#include "Game.h"

RenderingSystem::RenderingSystem() :
    m_viewInitialized(false)
{
}
void RenderingSystem::Draw(sf::RenderWindow* window)
{
    if (!m_viewInitialized)
    {
        m_defaultView = window->getDefaultView();
        m_viewInitialized = true;
    }
    else
    {
        window->setView(m_defaultView);
    }
    // Clear screen
    window->clear();
    
    for (std::vector<Box2DComponent*>::const_iterator it = Box2DComponent::Pool.GetComponents()->begin() ; it != Box2DComponent::Pool.GetComponents()->end() ; it++)
    {
        (*it)->Draw(window);
    }
    
    // Update the window
    window->display();
}

void RenderingSystem::Update(float dt)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        if( m_viewInitialized)
        {
            m_defaultView.move(0,-150*dt);
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        if( m_viewInitialized)
        {
            m_defaultView.move(0,150*dt);
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        if( m_viewInitialized)
        {
            m_defaultView.move(-150*dt,0);
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        if( m_viewInitialized)
        {
            m_defaultView.move(150*dt,0);
        }
    }
}