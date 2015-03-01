//
//  Box2DComponent.cpp
//  RogueLikeBox
//
//  Created by Keith Miron on 2/28/15.
//  Copyright (c) 2015 Acoustic Games. All rights reserved.
//

#include "Box2DComponent.h"

#include <Box2D/Box2D.h>

float Box2DComponent::s_box2DScale = 10.0f;

IMPLEMENT_COMPONENT(Box2DComponent);

Box2DComponent::~Box2DComponent()
{
    if(m_body)
    {
        m_body->GetWorld()->DestroyBody(m_body);
    }
}

void Box2DComponent::Draw(sf::RenderWindow* window)
{
    const b2Vec2& pos = m_body->GetPosition();
    const b2Transform& xf = m_body->GetTransform();
    
    for (b2Fixture* f = m_body->GetFixtureList(); f; f = f->GetNext())
    {
        switch (f->GetType())
        {
            case b2Shape::e_circle:
            {
                b2CircleShape* circle = (b2CircleShape*) f->GetShape();
                
                b2Vec2 center = b2Mul(xf, circle->m_p);
                float32 radius = circle->m_radius;
                
                sf::CircleShape shape;
                shape.setRadius(radius * s_box2DScale);
                shape.setFillColor(sf::Color::Red);
                shape.setOrigin(radius * s_box2DScale, radius * s_box2DScale);
                shape.setPosition(center.x*s_box2DScale,center.y*s_box2DScale);
                window->draw(shape);
                
            }
                break;
                
            case b2Shape::e_polygon:
            {
                b2PolygonShape* poly = (b2PolygonShape*) f->GetShape();
                int32 vertexCount = poly->GetVertexCount();
                b2Assert(vertexCount <= b2_maxPolygonVertices);
                
                sf::ConvexShape shape;
                shape.setPointCount(vertexCount);
                for (int32 i = 0; i < vertexCount; ++i)
                {
                    shape.setPoint(i, sf::Vector2f(s_box2DScale * poly->m_vertices[i].x,s_box2DScale * poly->m_vertices[i].y));
                }
                shape.setFillColor(sf::Color::White);
                shape.setPosition(pos.x*s_box2DScale,pos.y*s_box2DScale);
                shape.setRotation(RAD_TO_DEG * m_body->GetAngle());
                window->draw(shape);
            }
                break;
        }
    }
}

void Box2DComponent::Update(float f32)
{
}
