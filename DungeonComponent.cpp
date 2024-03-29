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
#define CELL_THICKNESS 35
#define CELL_COUNT_MIN 250
#define CELL_COUNT_MAX 500
#define CELL_MEAN 15
#define CELL_RANGE 4
#define CELL_ROOM_MIN (CELL_MEAN * .99)
#define CELL_RADIUS (CELL_COUNT_MAX/2)
#define EXTRA_EDGE_PERCENT (.1)

Cell::Cell() :
    m_velocity(0.0f,0.0f),
    m_color(sf::Color::Yellow) ,
    m_weight(999999) ,
	m_destroyed(false)
{
    
}

void Cell::AddForce(const sf::Vector2f &force)
{
	printf("Adding force[%f,%f]\n", force.x, force.y);
    m_velocity.x += force.x;
    m_velocity.y += force.y;
}

void Cell::AddConnectedCell(Cell* other)
{
    for( int i=0 ; i<m_edges.size() ; i++)
    {
        if (m_edges[i].m_to == other->GetRoomIdx())
        {
			// Already exists
            return;
        }
    }
    
	Edge newEdge;
	newEdge.m_from = GetRoomIdx();
	newEdge.m_to = other->GetRoomIdx();
	newEdge.m_owner = m_owner;
	sf::Vector2f fromCell = VectorMath::Sub(GetPos(), other->GetPos());
	newEdge.m_weight = VectorMath::Mag(fromCell);
	m_edges.push_back(newEdge);
}

bool Cell::IsRoom() const
{
    return m_dimensions.x > CELL_ROOM_MIN && m_dimensions.y > CELL_ROOM_MIN;
}

void Cell::SetOwner(DungeonComponent* owner)
{
	m_owner = owner;
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
    m_pos = sf::Vector2f(pos.x, pos.y);
    __UpdateRenderShape();
}

void Cell::SetColor(const sf::Color& color)
{
    m_color = color;
    __UpdateRenderShape();
}

void Cell::__UpdateRenderShape()
{
    m_renderShape.setSize(sf::Vector2f(m_dimensions.x * CELL_THICKNESS, m_dimensions.y * CELL_THICKNESS));
    m_renderShape.setOrigin(sf::Vector2f(m_pos.x * CELL_THICKNESS,m_pos.y*CELL_THICKNESS));
    
    if (IsRoom())
    {
        m_renderShape.setOutlineColor(sf::Color::Green);
    }
  
    m_renderShape.setFillColor(m_color);
    m_renderShape.setOutlineThickness(5);
    m_globalBounds = m_renderShape.getGlobalBounds();
}

void DungeonComponent::DungeonGenerationState_Init::Init(DungeonComponent* comp)
{
    DungeonGenerationState::Init(comp);
	for ( int i=0 ; i< 1000 ; i++)
	{
		printf("[%f] ", Random::Next<double>(0.0, CELL_RADIUS));
	}

	printf ("\n");
    int numCells = Random::Next(CELL_COUNT_MIN,CELL_COUNT_MAX);
    for( int i=0 ; i<numCells ; i++)
    {
        comp->m_cells.emplace_back();
        
        sf::Vector2i dimensions(sf::Vector2i(Random::NextNormal<float>(CELL_MEAN,CELL_RANGE), Random::NextNormal<float>(CELL_MEAN,CELL_RANGE)));
		double randAngle = Random::Next<double>(0.0, 2 * R_PI);
		double randRadius = Random::Next<double>(0.0, CELL_RADIUS);
		sf::Vector2f randPos(sin(randAngle) * randRadius, cos(randAngle) * randRadius);
        sf::Vector2i pos(randPos);
        comp->m_cells[i].SetDimensions(dimensions);
        comp->m_cells[i].SetPos(pos);
		comp->m_cells[i].SetOwner(comp);
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
		Cell& cellA = m_owner->m_cells[i];
		if (cellA.IsDestroyed())
		{
			continue;
		}

        for (int j=0 ; j<sizeB ; j++)
        {
			Cell& cellB = m_owner->m_cells[j];
			if (cellB.IsDestroyed())
			{
				continue;
			}
            if (i==j)
            {
                continue;
            }
            else
            {
                const sf::RectangleShape* a = cellA.GetShape();
                const sf::RectangleShape* b = cellB.GetShape();
                
                const sf::FloatRect* myBounds = cellA.GetGlobalBounds();
                const sf::FloatRect* otherBounds = cellB.GetGlobalBounds();
				if (abs((int)(a->getOrigin().x - b->getOrigin().x)) > (2 * CELL_THICKNESS * (CELL_MEAN)) ||
					abs((int)(a->getOrigin().y - b->getOrigin().y)) > (2 * CELL_THICKNESS * (CELL_MEAN)) )
				{
					continue;
				}

                if (myBounds->intersects(*otherBounds))
                {
                    finished = false;
                    sf::Vector2f fromCell = VectorMath::Sub(b->getOrigin(), a->getOrigin());
					printf("b[%d] origin was [%f,%f]\t",j, b->getOrigin().x, b->getOrigin().y);
					printf("a[%d] origin was [%f,%f]\n",i, a->getOrigin().x, a->getOrigin().y);
					fromCell = VectorMath::Normalize(fromCell);
               
					// Two rectangles sharing the same position
                    if (fromCell.x == 0 && fromCell.y ==0)
                    {
						cellB.Destroy();
                    }
					else
					{
						printf("fromCell is [%f,%f]\n", fromCell.x, fromCell.y);
						cellA.AddForce(VectorMath::Mul(fromCell, -1.0/VectorMath::Mag(fromCell)));
					}					
                }
            }
        }
    }
    
    if (finished)
    {
        for (int i=0 ; i<m_owner->m_cells.size(); i++)
        {
			if (m_owner->m_cells[i].IsDestroyed())
			{
				m_owner->m_cells.erase(m_owner->m_cells.begin() + i);
				i--;
				continue;
			}

            if (m_owner->m_cells[i].IsRoom())
            {
                Cell newRoom = m_owner->m_cells[i];                
				newRoom.SetRoomIdx(m_owner->m_rooms.size());
                m_owner->m_rooms.push_back(newRoom);
				
            }			
        }
        
        m_owner->SetState(new DungeonGenerationState_Triangulate());
    }
}

void DungeonComponent::DungeonGenerationState_Triangulate::Init(DungeonComponent* comp)
{
    DungeonComponent::DungeonGenerationState::Init(comp);
    m_delay=0.5f;
    
    size_t roomCount = m_owner->m_rooms.size();
    for (int i=0 ; i<roomCount; i++)
    {
        const sf::RectangleShape* renderShape = m_owner->m_rooms[i].GetShape();
        vertex newVert(renderShape->getOrigin().x - renderShape->getSize().x/2.0f, renderShape->getOrigin().y - renderShape->getSize().y/2.0f);
        newVert.SetUserData(i);
        m_vertices.insert(newVert);
    }
    m_delaunay.Triangulate(m_vertices, m_triangles);
    m_delaunay.TrianglesToEdges(m_triangles, m_edges);
    
    // Create the graph
    for( edgeIterator it = m_edges.begin() ; it != m_edges.end() ; it++)
    {
        Cell* room1 = &m_owner->m_rooms[(*it).m_pV0->GetUserData()];
        Cell* room2 = &m_owner->m_rooms[(*it).m_pV1->GetUserData()];
        
        room1->SetColor(sf::Color::Green);
        room2->SetColor(sf::Color::Green);
        
        room1->AddConnectedCell(room2);
        room2->AddConnectedCell(room1);
    }

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

void DungeonComponent::DungeonGenerationState_Triangulate::Update(float dt)
{
	DungeonComponent::DungeonGenerationState::Update(dt);
    if (m_delay < 0 )
    {
         m_owner->SetState(new DungeonGenerationState_FindMST());
    }
}

void DungeonComponent::DungeonGenerationState_FindMST::Init(DungeonComponent* comp)
{
	m_delay = 0.5f;
    DungeonComponent::DungeonGenerationState::Init(comp);
	m_owner->m_mstEdges.clear();

	if (m_owner->m_rooms.size() > 0)
	{
		std::vector<Edge> unvisitedEdges;
		Cell* currentCell = &m_owner->m_rooms[0];

		std::map<int, Cell> visitedCells;		
		while (m_owner->m_mstEdges.size() < m_owner->m_rooms.size()-1)
		{		
			visitedCells[currentCell->GetRoomIdx()] = *currentCell;
			for (int i = 0; i < currentCell->GetEdges().size(); i++)
			{
				const Edge& currentEdge = currentCell->GetEdges()[i];

				bool found = false;
				// Already exists in the list?
				for (int j = 0; j < unvisitedEdges.size(); j++)
				{
					if (unvisitedEdges[j] == currentEdge)
					{
						found = true;
						break;
					}
				}

				if (found)
				{
					continue;
				}

				// Already exists in the list?
				for (int j = 0; j < m_owner->m_mstEdges.size(); j++)
				{
					if (m_owner->m_mstEdges[j] == currentEdge)
					{
						found = true;
						break;
					}
				}

				if (found)
				{
					continue;
				}

				// Safely add this one
				unvisitedEdges.push_back(currentEdge);
			}

			// Clean up any edges that already have both sides visited
			for (std::vector<Edge>::iterator it = unvisitedEdges.begin() ; it != unvisitedEdges.end() ; ) 
			{
				const Edge& currentEdge = *it;
				if (visitedCells.find(currentEdge.m_to) != visitedCells.end() &&
					visitedCells.find(currentEdge.m_from) != visitedCells.end()
					)
				{
					it = unvisitedEdges.erase(it);
				}
				else
				{
					it++;
				}
			}
		
			// Now pick amongst the unvisited cells and add the one with lowest weight
			Edge bestEdge;
			bestEdge.m_weight = -1;
			for (int i = 0; i<unvisitedEdges.size(); i++)
			{
				const Edge& currentEdge = unvisitedEdges[i];
				if (currentEdge.m_weight < bestEdge.m_weight || bestEdge.m_weight == -1)
				{
					bestEdge = currentEdge;
				}
			}

			if (bestEdge.m_weight == -1)
			{
				// Done!
				return;
			}

			m_owner->m_mstEdges.push_back(bestEdge);
			for (int i = 0; i<unvisitedEdges.size(); i++)
			{
				if (bestEdge == unvisitedEdges[i])
				{
					unvisitedEdges.erase(unvisitedEdges.begin() + i);
					break;
				}
			}

			if (unvisitedEdges.size() > 0)
			{
				currentCell = &m_owner->m_rooms[bestEdge.m_to];
			}
		}
	}	
}

void DungeonComponent::DungeonGenerationState_FindMST::Update(float dt)
{
	DungeonComponent::DungeonGenerationState::Update(dt);
	if (m_delay < 0)
	{
		m_owner->SetState(new DungeonGenerationState_AddExtraLinks());
	}
}

void DungeonComponent::DungeonGenerationState_AddExtraLinks::Init(DungeonComponent* comp)
{
	DungeonComponent::DungeonGenerationState::Init(comp);

	for (int i=0 ; i<m_owner->m_rooms.size() ; i++)	
	{
		const Cell& currentCell = m_owner->m_rooms[i];
		for (int j = 0; j < currentCell.GetEdges().size(); j++)
		{
			if (Random::Next<double>(0, 1.0) > EXTRA_EDGE_PERCENT)
			{
				continue;
			}

			const Edge& currentEdge = currentCell.GetEdges()[j];
			bool exists=false;
			for (int k=0 ; k<m_owner->m_mstEdges.size() ; k++)
			{
				if (m_owner->m_mstEdges[k] == currentEdge)
				{
					exists = true;
				}
			}

			if (!exists)
			{
				m_owner->m_mstEdges.push_back(currentEdge);
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
	Reset();
}

void DungeonComponent::Reset()
{
	m_cells.clear();
	m_rooms.clear();
	m_mstEdges.clear();
	SetState(new DungeonGenerationState_Init());
}

void DungeonComponent::Draw(sf::RenderWindow* window)
{
    if (m_rooms.size() > 0)
    {
        for ( int i=0 ; i<m_rooms.size() ; i++)
        {
            m_rooms[i].Draw(window);
        }
    }
    else
    {
        for ( int i=0 ; i<m_cells.size() ; i++)
        {
            m_cells[i].Draw(window);
        }
    }

	for (int i = 0; i < m_mstEdges.size(); i++)
	{
		m_mstEdges[i].Draw(window);
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

void Edge::Draw(sf::RenderWindow * window)
{
	const Cell& fromRoom = m_owner->m_rooms[m_from];
	const Cell& toRoom = m_owner->m_rooms[m_to];
	sf::Vertex line[] =
	{
	sf::Vertex(sf::Vector2f(-fromRoom.GetShape()->getOrigin().x + fromRoom.GetShape()->getSize().x / 2.0f, -fromRoom.GetShape()->getOrigin().y + fromRoom.GetShape()->getSize().y / 2.0f)),
	sf::Vertex(sf::Vector2f(-toRoom.GetShape()->getOrigin().x + toRoom.GetShape()->getSize().x / 2.0f, -toRoom.GetShape()->getOrigin().y + toRoom.GetShape()->getSize().y / 2.0f))
	};

	window->draw(line, 2, sf::LinesStrip);

	static sf::Font* font = NULL;

	if (font == NULL)
	{
		font = new sf::Font();
		font->loadFromFile("RogueLikeBox\\sansation.ttf");
	}

	sf::Text weightTxt;
	weightTxt.setFont(*font);
	weightTxt.setCharacterSize(15);
	weightTxt.setColor(sf::Color::White);
	weightTxt.setString(std::to_string(m_weight));
	weightTxt.setPosition(VectorMath::Mul(VectorMath::Add(line[0].position, line[1].position), 0.5f));
	window->draw(weightTxt);

	{
		sf::Text roomTxt;
		roomTxt.setFont(*font);
		roomTxt.setCharacterSize(20);
		roomTxt.setColor(sf::Color::Red);
		roomTxt.setString(std::to_string(m_to));
		roomTxt.setPosition(-toRoom.GetShape()->getOrigin().x, -toRoom.GetShape()->getOrigin().y);
		window->draw(roomTxt);
	}

	{
		sf::Text roomTxt;
		roomTxt.setFont(*font);
		roomTxt.setCharacterSize(20);
		roomTxt.setColor(sf::Color::Red);
		roomTxt.setString(std::to_string(m_from));
		roomTxt.setPosition(-fromRoom.GetShape()->getOrigin().x, -fromRoom.GetShape()->getOrigin().y);
		window->draw(roomTxt);
	}
}
