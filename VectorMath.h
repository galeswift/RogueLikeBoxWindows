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

#define R_PI (3.14159265358979323846)

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
        double mag = Mag(a);
		if (mag > 0.0f)
		{
			return T(a.x/mag, a.y/mag);
		}
		else
		{
			return T(0.0f,0.0f);
		}
    }
};

#endif /* defined(__RogueLikeBox__Math__) */
