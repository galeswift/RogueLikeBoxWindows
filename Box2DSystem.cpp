//
//  Box2DSystem.cpp
//  RogueLikeBox
//
//  Created by Keith Miron on 2/27/15.
//  Copyright (c) 2015 Acoustic Games. All rights reserved.
//

#include "Box2DSystem.h"
#include "Box2DComponent.h"
#include "Entity.h"
#include "Game.h"

#include <Box2D/Box2D.h>

void Box2DSystem::Init(Game* owner)
{
    System::Init(owner);
    
    // Define the gravity vector.
    b2Vec2 gravity(0.0f, 20.0f);
    
    // Construct a world object, which will hold and simulate the rigid bodies.
    m_world = new b2World(gravity);
    
	//__InitTestBodies();
}

void Box2DSystem::__InitTestBodies()
{
	// Define the ground body.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, 5.0f);

	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	b2Body* groundBody = m_world->CreateBody(&groundBodyDef);

	// Define the ground box shape.
	b2PolygonShape groundBox;

	// The extents are the half-widths of the box.
	groundBox.SetAsBox(40, 5);

	// Define the ground box shape.
	b2PolygonShape groundBox2;
	b2PolygonShape groundBox3;
	int count = 4;
	b2Vec2 vertices[4];
	vertices[0].Set(20, -30);
	vertices[1].Set(30, -30);
	vertices[2].Set(30, 0);
	vertices[3].Set(20, 0);

	// The extents are the half-widths of the box.
	groundBox2.Set(vertices, count);

	vertices[0].Set(-20, -30);
	vertices[1].Set(-10, -30);
	vertices[2].Set(-10, 0);
	vertices[3].Set(-20, 0);
	groundBox3.Set(vertices, count);

	// Add the ground fixture to the ground body.
	groundBody->CreateFixture(&groundBox, 0.0f);
	groundBody->CreateFixture(&groundBox2, 0.0f);
	groundBody->CreateFixture(&groundBox3, 0.0f);

	Entity* groundEntity = GetOwner()->CreateEntity();
	Box2DComponent* groundComponent = Box2DComponent::Pool.CreateComponent();
	groundEntity->AddComponent(groundComponent);
	groundComponent->m_body = groundBody;

	// Player entity
	b2BodyDef playerBodyDef;
	playerBodyDef.type = b2_dynamicBody;
	playerBodyDef.position.Set(10, 0);
	b2Body* playerBody = m_world->CreateBody(&playerBodyDef);
	playerBody->SetBullet(true);

	b2PolygonShape playerBodyShape;
	playerBodyShape.SetAsBox(2, 4);
	playerBody->CreateFixture(&playerBodyShape, 0.0f);

	m_playerEntity = GetOwner()->CreateEntity();
	Box2DComponent* playerCmp = Box2DComponent::Pool.CreateComponent();
	m_playerEntity->AddComponent(playerCmp);
	playerCmp->m_body = playerBody;
}

void Box2DSystem::Draw(sf::RenderWindow* window)
{
}

void Box2DSystem::Update(float dt)
{
    
    // Prepare for simulation. Typically we use a time step of 1/60 of a
    // second (60Hz) and 10 iterations. This provides a high quality simulation
    // in most game scenarios.
    int32 velocityIterations = 10;
    int32 positionIterations = 10;
    
    // Instruct the world to perform a single step of simulation.
    // It is generally best to keep the time step and iterations fixed.
    m_world->Step(dt, velocityIterations, positionIterations);
    
	if (m_playerEntity != NULL)
	{
		Box2DComponent* playerComp = m_playerEntity->GetComponent<Box2DComponent>(NULL);
		b2Vec2 vel = playerComp->m_body->GetLinearVelocity();
		b2Vec2 pos = playerComp->m_body->GetPosition();

		m_spawnInterval += dt;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			playerComp->m_body->ApplyLinearImpulse(b2Vec2(-1, 0), pos, true);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			playerComp->m_body->ApplyLinearImpulse(b2Vec2(0, -1), pos, true);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			playerComp->m_body->ApplyLinearImpulse(b2Vec2(0, 1), pos, true);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			playerComp->m_body->ApplyLinearImpulse(b2Vec2(1, 0), pos, true);
		}
	}
   
    if (m_spawnInterval > .01f)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            m_spawnInterval = 0.0f;
            __AddCube(sf::Vector2f(0,-3.0));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
        {
            m_spawnInterval = 0.0f;
            __AddCircle(sf::Vector2f(0,-3.0));
        }

    }
    ComponentPool<Box2DComponent>::Iterator it(&Box2DComponent::Pool);
    for (it.First() ; it.Current() ; it.Next())
    {
        it.Current()->Update(dt);
    }
}


void Box2DSystem::__AddCircle(const sf::Vector2f& pos)
{
    // Define the dynamic body. We set its position and call the body factory.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(pos.x, pos.y);
    b2Body* body = m_world->CreateBody(&bodyDef);
    
    // Define another box shape for our dynamic body.
    b2CircleShape dynamicCircle;
    dynamicCircle.m_p.Set(pos.x,pos.y);
    dynamicCircle.m_radius = .5;
    
    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicCircle;
    
    // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.density = 1.0f;
    
    // Override the default friction.
    fixtureDef.friction = 0.3f;
    
    // Add the shape to the body.
    body->CreateFixture(&fixtureDef);
    
    Entity* boxEntity = GetOwner()->CreateEntity();
    Box2DComponent* boxComponent = Box2DComponent::Pool.CreateComponent();
    boxEntity->AddComponent(boxComponent);
    boxComponent->m_body = body;
    
}

void Box2DSystem::__AddCube(const sf::Vector2f& pos)
{
    // Define the dynamic body. We set its position and call the body factory.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(pos.x, pos.y);
    b2Body* body = m_world->CreateBody(&bodyDef);
    
    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    float width = Random::NextNormal(4.0f,1.0f);
    float height = Random::NextNormal(10.0f, 3.0f);
    dynamicBox.SetAsBox(width,height);
    
    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    
    // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.density = 1.0f;
    
    // Override the default friction.
    fixtureDef.friction = 0.3f;
    
    // Add the shape to the body.
    body->CreateFixture(&fixtureDef);
    
    
    Entity* boxEntity = GetOwner()->CreateEntity();
    Box2DComponent* boxComponent = Box2DComponent::Pool.CreateComponent();
    boxEntity->AddComponent(boxComponent);
    boxComponent->m_body = body;
}