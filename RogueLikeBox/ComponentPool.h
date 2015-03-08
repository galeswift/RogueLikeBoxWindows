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
    class Iterator
    {
    public:
        Iterator(ComponentPool* pool) : m_index(0), m_pool(pool)
        {
            
        }
        
        T* First()
        {
            if( m_index < m_pool->m_components.size())
            {
                return m_pool->m_components.at(0);
            }
            return NULL;
        }
        
        T* Current()
        {
            if( m_index < m_pool->m_components.size())
            {
                return m_pool->m_components.at(m_index);
            }
            return NULL;
        }
        
        void Next()
        {
            m_index++;
        }
        
    private:
        size_t m_index;
        ComponentPool* m_pool;
    };
 
private:
    std::vector<T*> m_components;
};
#endif /* defined(__RogueLikeBox__ComponentPool__) */
