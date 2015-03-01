//
//  Entity.h
//  RogueLikeBox
//
//  Created by Keith Miron on 2/27/15.
//  Copyright (c) 2015 Acoustic Games. All rights reserved.
//

#ifndef __RogueLikeBox__Entity__
#define __RogueLikeBox__Entity__

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "Components.h"

class Entity
{
public:
    Entity() {};
    ~Entity()
    {
        for (std::vector<Component*>::iterator it = m_components.begin() ; it != m_components.end() ; it++)
        {
            (*it)->Remove();
        }
    }
    
    template <class T>
    T* GetComponent(T*)
    {
        for (std::vector<Component*>::iterator it = m_components.begin() ; it != m_components.end() ; it++)
        {
            if( dynamic_cast<T*>(*it) != NULL)
            {
                return dynamic_cast<T*>(*it);
            }
        }
        
        return NULL;
    }

    void AddComponent(Component* comp)
    {
        m_components.push_back(comp);
    }
    
private:
    std::vector<Component*> m_components;
};

#endif /* defined(__RogueLikeBox__Entity__) */
