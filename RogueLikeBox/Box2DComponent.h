//
//  Box2DComponent.h
//  RogueLikeBox
//
//  Created by Keith Miron on 2/28/15.
//  Copyright (c) 2015 Acoustic Games. All rights reserved.
//

#ifndef __RogueLikeBox__Box2DComponent__
#define __RogueLikeBox__Box2DComponent__

#include <stdio.h>
#include "Components.h"

class b2World;
class b2Body;

class Box2DComponent : public Component
{
public:
    DECLARE_COMPONENT(Box2DComponent);
    
    ~Box2DComponent();
    virtual void Draw(sf::RenderWindow* window);
    virtual void Update(float f32);
    
    static float s_box2DScale;
    
    b2Body* m_body;
};

#endif /* defined(__RogueLikeBox__Box2DComponent__) */
