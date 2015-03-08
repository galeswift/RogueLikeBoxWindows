//
//  DungeonSystem.cpp
//  RogueLikeBox
//
//  Created by Keith Miron on 3/1/15.
//  Copyright (c) 2015 Acoustic Games. All rights reserved.
//

#include "DungeonSystem.h"
#include "DungeonComponent.h"
#include "Entity.h"
#include "Game.h"

void DungeonSystem::Init(Game* owner)
{
    System::Init(owner);
    
    Entity* dungeonEntity = GetOwner()->CreateEntity();
    DungeonComponent* dungeonComponent = DungeonComponent::Pool.CreateComponent();
    dungeonEntity->AddComponent(dungeonComponent);
}

void DungeonSystem::Update(float dt)
{
    {
        ComponentPool<DungeonComponent>::Iterator it(&DungeonComponent::Pool);
        for (it.First() ; it.Current() ; it.Next())
        {
            it.Current()->Update(dt);
        }
    }
}