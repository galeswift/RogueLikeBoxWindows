//
//  Components.h
//  RogueLikeBox
//
//  Created by Keith Miron on 2/27/15.
//  Copyright (c) 2015 Acoustic Games. All rights reserved.
//

#ifndef __RogueLikeBox__Components__
#define __RogueLikeBox__Components__

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "ComponentPool.h"

#define DECLARE_COMPONENT(C) \
public: \
virtual void Remove() { Pool.RemoveComponent(this); } \
static ComponentPool<C> Pool;


#define IMPLEMENT_COMPONENT(C) \
ComponentPool<C> C::Pool

class Entity;

class Component
{
public:
    virtual void Init(Entity* entity) {};
    virtual void Draw(sf::RenderWindow* window) {};
    virtual void Update(float f32) {};
    virtual void Remove() {};
};

#endif /* defined(__RogueLikeBox__Components__) */
