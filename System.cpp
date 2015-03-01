//
//  System.cpp
//  RogueLikeBox
//
//  Created by Keith Miron on 2/27/15.
//  Copyright (c) 2015 Acoustic Games. All rights reserved.
//

#include "System.h"
#include "Game.h"

void System::Init(Game* owner)
{
    m_owner = owner;
}

Game* System::GetOwner()
{
    return m_owner;
}