//
//  DungeonComponent.h
//  RogueLikeBox
//
//  Created by Keith Miron on 3/1/15.
//  Copyright (c) 2015 Acoustic Games. All rights reserved.
//

#ifndef __RogueLikeBox__DungeonComponent__
#define __RogueLikeBox__DungeonComponent__

#include <stdio.h>
#include "Components.h"
#include "Delaunay.h"

class DungeonComponent;
struct Edge
{
	const bool operator==(const Edge& other) const 
	{ 
		bool sameLink = (m_from == other.m_from && m_to == other.m_to) || (m_from == other.m_to && m_to == other.m_from) ; 
		return sameLink;
	}

	const bool operator !=(const Edge& other) const
	{
		return !(*this != other);
	}

	void Draw(sf::RenderWindow* window);
	int m_from;
	int m_to;
	int m_weight;
	DungeonComponent *m_owner;
};

class Cell
{
public:
    Cell();
    void Draw(sf::RenderWindow* window);
    void Update(float dt);
    void SetDimensions(const sf::Vector2i& dimensions);
    void SetPos(const sf::Vector2i& pos);
    void SetColor(const sf::Color& color);
    const sf::RectangleShape* GetShape() const { return &m_renderShape; }
    const sf::FloatRect* GetGlobalBounds() const { return &m_globalBounds; }
    void AddForce(const sf::Vector2f& force);
    void AddConnectedCell(Cell* other);
    bool IsRoom() const;
    void SetRoomIdx(int val) { m_roomIdx = val; }
	const sf::Vector2f& GetPos() const { return m_pos;  }
    int GetRoomIdx() const { return m_roomIdx; }
    int GetWeight() const { return m_weight; }
    void SetWeight(int val) { m_weight = val; }
	void Destroy() { m_destroyed = true; }
	bool IsDestroyed() const { return m_destroyed; }
	void SetOwner(DungeonComponent* owner);
	DungeonComponent* GetOwner() { return m_owner; }
	const std::vector<Edge>& GetEdges() const { return m_edges; }

private:
    void __UpdateRenderShape();
    sf::RectangleShape m_renderShape;
    sf::Vector2i m_dimensions;
    sf::Vector2f m_pos; // gridPos
    sf::Vector2f m_velocity;
    sf::FloatRect m_globalBounds;
    sf::Color m_color;
	std::vector<Edge> m_edges;
	DungeonComponent* m_owner;
    int m_roomIdx;
    int m_weight;
	bool m_destroyed;
};

class DungeonComponent : public Component
{
public:
    DECLARE_COMPONENT(DungeonComponent);
    
    
    class DungeonGenerationState
    {
    public:
        virtual void Init(DungeonComponent* comp) {m_owner = comp;} 
		virtual void Update(float dt) {m_delay -= dt;}
        virtual void Draw(sf::RenderWindow* window) {};
    protected:
        DungeonComponent* m_owner;
		float m_delay;
    };
    
    class DungeonGenerationState_Init : public DungeonGenerationState
    {
    public:
        virtual void Init(DungeonComponent* comp);
    };
    
    class DungeonGenerationState_Separate : public DungeonGenerationState
    {
    public:
        virtual void Update(float dt);
    };
    
    class DungeonGenerationState_Triangulate : public DungeonGenerationState
    {
    public:
        virtual void Init(DungeonComponent* comp);
        virtual void Draw(sf::RenderWindow* window);
        virtual void Update(float dt);
    private:
        Delaunay m_delaunay;
        vertexSet m_vertices;
        triangleSet m_triangles;
        edgeSet m_edges;        
    };
    
    class DungeonGenerationState_FindMST : public DungeonGenerationState
    {
    public:
        virtual void Init(DungeonComponent* comp);
		virtual void Update(float dt);
    };
    
	class DungeonGenerationState_AddExtraLinks : public DungeonGenerationState
	{
	public:
		virtual void Init(DungeonComponent* comp);
	};

    DungeonComponent();
    virtual void Init(Entity* entity);
	virtual void Reset();
    virtual void Draw(sf::RenderWindow* window);
    virtual void Update(float dt);
    virtual void SetState(DungeonGenerationState* state);
    DungeonGenerationState* GetState() { return m_state; }

    std::vector<Cell> m_cells;	
	std::vector<Edge> m_mstEdges;
    std::vector<Cell> m_rooms;
    DungeonGenerationState* m_state;
};


#endif /* defined(__RogueLikeBox__DungeonComponent__) */
