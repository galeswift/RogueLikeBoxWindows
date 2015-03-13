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

class Cell
{
public:
    Cell();
    void Draw(sf::RenderWindow* window);
    void Update(float dt);
    void SetDimensions(const sf::Vector2i& dimensions);
    void SetPos(const sf::Vector2i& pos);
    const sf::RectangleShape* GetShape() { return &m_renderShape; }
    const sf::FloatRect* GetGlobalBounds() { return &m_globalBounds; }
    void AddForce(const sf::Vector2i& force);
    bool IsRoom();
private:
    void __UpdateRenderShape();
    sf::RectangleShape m_renderShape;
    sf::Vector2i m_dimensions;
    sf::Vector2i m_pos; // gridPos
    sf::Vector2i m_velocity;
    sf::FloatRect m_globalBounds;
};

class DungeonComponent : public Component
{
public:
    DECLARE_COMPONENT(DungeonComponent);
    
    
    class DungeonGenerationState
    {
    public:
        virtual void Init(DungeonComponent* comp) {m_owner = comp;} 
        virtual void Update(float dt) {};
        virtual void Draw(sf::RenderWindow* window) {};
    protected:
        DungeonComponent* m_owner;
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
    private:
        Delaunay m_delaunay;
        vertexSet m_vertices;
        triangleSet m_triangles;
        edgeSet m_edges;
    };
    
    DungeonComponent();
    virtual void Init(Entity* entity);
    virtual void Draw(sf::RenderWindow* window);
    virtual void Update(float dt);
    virtual void SetState(DungeonGenerationState* state);
    DungeonGenerationState* GetState() { return m_state; }

 private:
    std::vector<Cell> m_cells;
    std::vector<Cell> m_rooms;
    DungeonGenerationState* m_state;
};


#endif /* defined(__RogueLikeBox__DungeonComponent__) */
