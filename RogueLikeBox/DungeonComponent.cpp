//
//  DungeonComponent.cpp
//  RogueLikeBox
//
//  Created by Keith Miron on 3/1/15.
//  Copyright (c) 2015 Acoustic Games. All rights reserved.
//

#include "DungeonComponent.h"
#include "Entity.h"
#include "VectorMath.h"
#include "Game.h"

IMPLEMENT_COMPONENT(DungeonComponent);
#define CELL_THICKNESS 10
#define CELL_COUNT_MIN 100
#define CELL_COUNT_MAX 150
#define CELL_MEAN 15
#define CELL_RANGE 4
#define CELL_ROOM_MIN (CELL_MEAN * 0.9)

Cell::Cell() :
    m_velocity(0.0f,0.0f)
{
    
}

void Cell::AddForce(const sf::Vector2i &force)
{
    m_velocity.x += force.x;
    m_velocity.y += force.y;
}

void Cell::Update(float dt)
{
    m_pos.x += m_velocity.x;
    m_pos.y += m_velocity.y;
    
    m_velocity.x = 0;
    m_velocity.y = 0;
    __UpdateRenderShape();
}

void Cell::Draw(sf::RenderWindow* window)
{
    window->draw(m_renderShape);
}

void Cell::SetDimensions(const sf::Vector2i& dimensions)
{
    m_dimensions = dimensions;
    __UpdateRenderShape();
}
void Cell::SetPos(const sf::Vector2i& pos)
{
    m_pos = pos;
    __UpdateRenderShape();
}

void Cell::__UpdateRenderShape()
{
    m_renderShape.setSize(sf::Vector2f(m_dimensions.x * CELL_THICKNESS, m_dimensions.y * CELL_THICKNESS));
    m_renderShape.setOrigin(sf::Vector2f(m_pos.x * CELL_THICKNESS,m_pos.y*CELL_THICKNESS));
    
    if (m_dimensions.x > CELL_ROOM_MIN && m_dimensions.y > CELL_ROOM_MIN)
    {
        m_renderShape.setFillColor(sf::Color::Blue);
        m_renderShape.setOutlineColor(sf::Color::Green);
        
    }
    else{
        m_renderShape.setFillColor(sf::Color(255,100,100,50));
        m_renderShape.setOutlineColor(sf::Color(100,255,100,50));
        
    }
   
    m_renderShape.setOutlineThickness(5);
}

void DungeonComponent::DungeonGenerationState_Init::Init(DungeonComponent* comp)
{
    DungeonGenerationState::Init(comp);
    
    int numCells = Random::Next(CELL_COUNT_MIN,CELL_COUNT_MAX);
    for( int i=0 ; i<numCells ; i++)
    {
        comp->m_cells.emplace_back();
        
        sf::Vector2i dimensions(sf::Vector2i(Random::NextNormal<float>(CELL_MEAN,CELL_RANGE), Random::NextNormal<float>(CELL_MEAN,CELL_RANGE)));
        sf::Vector2i pos(sf::Vector2i(Random::Next(0, 0), Random::Next(0,0)));
        comp->m_cells[i].SetDimensions(dimensions);
        comp->m_cells[i].SetPos(pos);
    }
    
    comp->SetState(new DungeonGenerationState_Separate());
}

void DungeonComponent::DungeonGenerationState_Separate::Update(float dt)
{
    for (int i=0 ; i<m_owner->m_cells.size() ; i++)
    {
        for (int j=0 ; j<m_owner->m_cells.size() ; j++)
        {
            if (i==j)
            {
                continue;
            }
            else
            {
                const sf::RectangleShape* a = m_owner->m_cells[i].GetShape();
                const sf::RectangleShape* b = m_owner->m_cells[j].GetShape();
                
                sf::FloatRect otherBounds = b->getGlobalBounds();
                //otherBounds.height *= 1.5f;
                //otherBounds.width *= 1.5f;
                if (a->getGlobalBounds().intersects(otherBounds))
                {
                    sf::Vector2f fromCell = VectorMath::Sub(b->getOrigin(), a->getOrigin());
                    sf::Vector2f fromCellN = VectorMath::Normalize(fromCell);
                    float fromCellMag = VectorMath::Mag(fromCell);
                    sf::Vector2i force(0,0);
                    if (fromCell.x < 0)
                    {
                        force.x = 1;
                    }
                    else if (fromCell.x > 0)
                    {
                        force.x = -1;
                    }
                    if (fromCell.y < 0)
                    {
                        force.y = 1;
                    }
                    else if (fromCell.y > 0)
                    {
                        force.y = -1;
                    }
                    
                    if (fromCell.x == 0 && fromCell.y ==0)
                    {
                        force.x = Random::Next(-1,1);
                        force.y = Random::Next(-1,1);
                    }
                    m_owner->m_cells[i].AddForce(force);
                }
            }
        }
    }
}

DungeonComponent::DungeonComponent() :
    m_state(NULL)
{
    
}
void DungeonComponent::Init(Entity *entity)
{
    Component::Init(entity);
    SetState(new DungeonGenerationState_Init());
}

void DungeonComponent::Draw(sf::RenderWindow* window)
{
    for ( int i=0 ; i<m_cells.size() ; i++)
    {
        m_cells[i].Draw(window);
    }
}

void DungeonComponent::Update(float dt)
{
    if (m_state)
    {
        m_state->Update(dt);
    }
    
    for ( int i=0 ; i<m_cells.size() ; i++)
    {
        m_cells[i].Update(dt);
    }
}

void DungeonComponent::SetState(DungeonGenerationState *state)
{
    if (m_state)
    {
        delete m_state;
    }
    
    m_state = state;
    
    if (m_state)
    {
        m_state->Init(this);
    }
}