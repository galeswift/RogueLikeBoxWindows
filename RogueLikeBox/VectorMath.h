//
//  Math.h
//  RogueLikeBox
//
//  Created by Keith Miron on 3/3/15.
//  Copyright (c) 2015 Acoustic Games. All rights reserved.
//

#ifndef __RogueLikeBox__Math__
#define __RogueLikeBox__Math__

#include <math.h>
#include <stdio.h>
#include <SFML/Graphics.hpp>

class VectorMath
{
public:
    template<class T>
    static double Mag(const T& a)
    {
        return sqrt(a.x * a.x + a.y * a.y);
    }
    
    template <class T>
    static T Add(const T& a,const T& b)
    {
        T v;
        v.x = a.x + b.x;
        v.y = a.y + b.y;
        return v;
    }
    
    template <class T>
    static T Sub(const T& a,const T& b)
    {
        T v;
        v.x = a.x - b.x;
        v.y = a.y - b.y;
        return v;
    }
    
    template <class T>
    static T Divide(const T& a, float val)
    {
        T v;
        v.x = a.x/val;
        v.y = a.y/val;
        return v;
    }
    
    template <class T>
    static T Mul(const T& a, float val)
    {
        T v;
        v.x = a.x*val;
        v.y = a.y*val;
        return v;
    }
    
    template <class T>
    static T Normalize(const T& a)
    {
        float mag = Mag(a);
        return T(a.x/mag, a.y/mag);
    }
};

#endif /* defined(__RogueLikeBox__Math__) */
