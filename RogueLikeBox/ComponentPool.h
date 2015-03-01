//
//  ComponentPool.h
//  RogueLikeBox
//
//  Created by Keith Miron on 2/28/15.
//  Copyright (c) 2015 Acoustic Games. All rights reserved.
//

#ifndef __RogueLikeBox__ComponentPool__
#define __RogueLikeBox__ComponentPool__

#include <stdio.h>
#include <vector>

template<class T>
class ComponentPool
{
public:
    ComponentPool() {};
    
    T* CreateComponent()
    {
        T* newComp = new T();
        m_components.push_back(newComp);
        return newComp;
    }
    
    void RemoveComponent(T* comp)
    {
        typedef typename std::vector<T*>::iterator componentIterator;
        for (componentIterator it = m_components.begin() ; it != m_components.end() ; it++)
        {
            if( (*it) == comp)
            {
                delete (*it);
            }
        }
    }
    
    const std::vector<T*>* GetComponents() { return &m_components; }
    
private:
    std::vector<T*> m_components;
};
#endif /* defined(__RogueLikeBox__ComponentPool__) */
