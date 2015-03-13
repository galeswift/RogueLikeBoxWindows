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
#define CELL_THICKNESS 3
#define CELL_COUNT_MIN 200
#define CELL_COUNT_MAX 250
#define CELL_MEAN 15
#define CELL_RANGE 5
#define CELL_ROOM_MIN (CELL_MEAN * 0.95)
#define CELL_RADIUS 50 * CELL_THICKNESS

Cell::Cell() :
    m_velocity(0.0f,0.0f)
{
    
}

void Cell::AddForce(const sf::Vector2i &force)
{
    m_velocity.x += force.x;
    m_velocity.y += force.y;
}

bool Cell::IsRoom()
{
    return m_dimensions.x > CELL_ROOM_MIN && m_dimensions.y > CELL_ROOM_MIN;
}
void Cell::Update(float dt)
{
    m_pos.x += m_velocity.x;
    m_pos.y += m_velocity.y;
    
    if (m_velocity.x != 0 || m_velocity.y != 0)
    {
        __UpdateRenderShape();
    }
    
    m_velocity.x = 0;
    m_velocity.y = 0;
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
    m_globalBounds = m_renderShape.getGlobalBounds();
}

void DungeonComponent::DungeonGenerationState_Init::Init(DungeonComponent* comp)
{
    DungeonGenerationState::Init(comp);
    
    int numCells = Random::Next(CELL_COUNT_MIN,CELL_COUNT_MAX);
    for( int i=0 ; i<numCells ; i++)
    {
        comp->m_cells.emplace_back();
        
        sf::Vector2i dimensions(sf::Vector2i(Random::NextNormal<float>(CELL_MEAN,CELL_RANGE), Random::NextNormal<float>(CELL_MEAN,CELL_RANGE)));
        sf::Vector2i pos(sf::Vector2i(Random::Next(-CELL_RADIUS, CELL_RADIUS), Random::Next(-CELL_RADIUS,CELL_RADIUS)));
        comp->m_cells[i].SetDimensions(dimensions);
        comp->m_cells[i].SetPos(pos);
    }
    
    comp->SetState(new DungeonGenerationState_Separate());
}

void DungeonComponent::DungeonGenerationState_Separate::Update(float dt)
{
    bool finished = true;
    size_t sizeA = m_owner->m_cells.size();
    size_t sizeB = m_owner->m_cells.size();
    for (int i=0 ; i<sizeA; i++)
    {
        for (int j=0 ; j<sizeB ; j++)
        {
            if (i==j)
            {
                continue;
            }
            else
            {
                const sf::RectangleShape* a = m_owner->m_cells[i].GetShape();
                const sf::RectangleShape* b = m_owner->m_cells[j].GetShape();
                
                const sf::FloatRect* myBounds = m_owner->m_cells[i].GetGlobalBounds();
                const sf::FloatRect* otherBounds = m_owner->m_cells[j].GetGlobalBounds();
                if (myBounds->intersects(*otherBounds))
                {
                    finished = false;
                    sf::Vector2f fromCell = VectorMath::Sub(b->getOrigin(), a->getOrigin());
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
                        force.x = Random::Next(-1, 1);
                        force.y = Random::Next(-1, 1);
                    }
                    m_owner->m_cells[i].AddForce(force);
                }
            }
        }
    }
    
    if (finished)
    {
        size_t cellCount = m_owner->m_cells.size();
        for (int i=0 ; i<cellCount; i++)
        {
            if (m_owner->m_cells[i].IsRoom())
            {
                Cell newRoom = m_owner->m_cells[i];
                m_owner->m_rooms.push_back(newRoom);
            }
        }
        
        m_owner->SetState(new DungeonGenerationState_Triangulate());
    }
}

void DungeonComponent::DungeonGenerationState_Triangulate::Init(DungeonComponent* comp)
{
    DungeonComponent::DungeonGenerationState::Init(comp);
    size_t roomCount = m_owner->m_rooms.size();
    for (int i=0 ; i<roomCount; i++)
    {
        const sf::RectangleShape* renderShape = m_owner->m_rooms[i].GetShape();
        m_vertices.insert(vertex(renderShape->getOrigin().x - renderShape->getSize().x/2.0f, renderShape->getOrigin().y - renderShape->getSize().y/2.0f));
    }
    m_delaunay.Triangulate(m_vertices, m_triangles);
    m_delaunay.TrianglesToEdges(m_triangles, m_edges);
}

void DungeonComponent::DungeonGenerationState_Triangulate::Draw(sf::RenderWindow* window)
{
    sf::Vertex* linesToDraw = new sf::Vertex[m_edges.size() * 2];
    int i=0 ;
    for( edgeIterator it = m_edges.begin() ; it != m_edges.end() ; it++)
    {
        linesToDraw[i * 2 + 0] = sf::Vector2f(-(*it).m_pV0->GetX(),-(*it).m_pV0->GetY());
        linesToDraw[i * 2 + 1] = sf::Vector2f(-(*it).m_pV1->GetX(),-(*it).m_pV1->GetY());
        linesToDraw[i*2 + 0].color = sf::Color::Red;
        linesToDraw[i*2 + 1].color = sf::Color::Red;
        i++;
    }
    
   window->draw(linesToDraw, m_edges.size() * 2, sf::Lines);
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
    
    if (m_state)
    {
        m_state->Draw(window);
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