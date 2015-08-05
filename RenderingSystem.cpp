//
//  RenderingSystem.cpp
//  RogueLikeBox
//
//  Created by Keith Miron on 2/28/15.
//  Copyright (c) 2015 Acoustic Games. All rights reserved.
//

#include "RenderingSystem.h"
#include "Components.h"
#include "Box2DComponent.h"
#include "DungeonComponent.h"
#include "Game.h"

RenderingSystem::RenderingSystem() :
    m_viewInitialized(false) ,
    m_currentZoom(1.0f)
{
}

void RenderingSystem::Init(Game* owner)
{
	System::Init(owner);
}

void RenderingSystem::Draw(sf::RenderWindow* window)
{
    if (!m_viewInitialized)
    {
        m_defaultView = window->getDefaultView();
		m_defaultView.zoom(50.0f);
        m_viewInitialized = true;
    }
    else
    {
        window->setView(m_defaultView);
    }
    // Clear screen
    window->clear();
    
    {
        ComponentPool<Box2DComponent>::Iterator it(&Box2DComponent::Pool);
        for (it.First() ; it.Current() ; it.Next())
        {
            it.Current()->Draw(window);
        }
    }
    
    {
        ComponentPool<DungeonComponent>::Iterator it(&DungeonComponent::Pool);
        for (it.First() ; it.Current() ; it.Next())
        {
            it.Current()->Draw(window);
        }
    }
    
    
    // Update the window
    window->display();
}

void RenderingSystem::Update(float dt)
{
    float moveDelta = 150 * dt * m_currentZoom;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        if( m_viewInitialized)
        {
            m_defaultView.move(0,-moveDelta);
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        if( m_viewInitialized)
        {
            m_defaultView.move(0,moveDelta);
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        if( m_viewInitialized)
        {
            m_defaultView.move(-moveDelta,0);
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        if( m_viewInitialized)
        {
            m_defaultView.move(moveDelta,0);
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
    {
        if( m_viewInitialized)
        {
            m_defaultView.zoom(1.0f/1.1f);
            m_currentZoom *= 1/1.1f;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
    {
        if( m_viewInitialized)
        {
            m_defaultView.zoom(1.1f);
            m_currentZoom *= 1.1f;
        }
    }
    

}