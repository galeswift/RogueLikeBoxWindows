//
//  DungeonSystem.h
//  RogueLikeBox
//
//  Created by Keith Miron on 3/1/15.
//  Copyright (c) 2015 Acoustic Games. All rights reserved.
//

#ifndef __RogueLikeBox__DungeonSystem__
#define __RogueLikeBox__DungeonSystem__

#include <stdio.h>
#include "System.h"

class DungeonSystem : public System
{
public:
    DungeonSystem() {};
    virtual void Init(Game* owner);
    virtual void Update(float dt);

};

#endif /* defined(__RogueLikeBox__DungeonSystem__) */
