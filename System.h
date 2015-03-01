//
//  System.h
//  RogueLikeBox
//
//  Created by Keith Miron on 2/27/15.
//  Copyright (c) 2015 Acoustic Games. All rights reserved.
//

#ifndef __RogueLikeBox__System__
#define __RogueLikeBox__System__

#include <stdio.h>
#include <SFML/Graphics.hpp>
class Game;

class System
{
public:
    virtual void Init(Game* owner);
    virtual void Draw(sf::RenderWindow* window) {};
    virtual void Update(float f32){};
    Game* GetOwner();
    
private:
    Game* m_owner;
};

#endif /* defined(__RogueLikeBox__System__) */
