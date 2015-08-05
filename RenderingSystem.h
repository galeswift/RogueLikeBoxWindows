//
//  RenderingSystem.h
//  RogueLikeBox
//
//  Created by Keith Miron on 2/28/15.
//  Copyright (c) 2015 Acoustic Games. All rights reserved.
//

#ifndef __RogueLikeBox__RenderingSystem__
#define __RogueLikeBox__RenderingSystem__

#include <stdio.h>
#include "System.h"

class RenderingSystem : public System
{
public:
    RenderingSystem();
	virtual void Init(Game* owner);
    virtual void Draw(sf::RenderWindow* window);
    virtual void Update(float dt);
    
private:
    bool m_viewInitialized;
    sf::View m_defaultView;
    float m_currentZoom;
};

#endif /* defined(__RogueLikeBox__RenderingSystem__) */
