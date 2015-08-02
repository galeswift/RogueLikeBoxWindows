//
//  Box2DSystem.h
//  RogueLikeBox
//
//  Created by Keith Miron on 2/27/15.
//  Copyright (c) 2015 Acoustic Games. All rights reserved.
//

#ifndef __RogueLikeBox__Box2DSystem__
#define __RogueLikeBox__Box2DSystem__

#include <stdio.h>
#include "System.h"

class b2World;
class Entity;

class Box2DSystem : public System
{
public:
    virtual void Init(Game* owner);
    virtual void Draw(sf::RenderWindow* window);
    virtual void Update(float f32);
    
private:
    void __AddCube(const sf::Vector2f& pos);
    void __AddCircle(const sf::Vector2f& pos);
	void __InitTestBodies();
    b2World* m_world;
    float m_spawnInterval;
    Entity* m_playerEntity;
};

#endif /* defined(__RogueLikeBox__Box2DSystem__) */
