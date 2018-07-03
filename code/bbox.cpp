//
//  bbox.cpp
//  RayTracer
//
//  Created by Pedro Figueirêdo on 14/05/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#include "bbox.h"

Bbox::Bbox(){}

// Same as in capagot's swpathtracer
// https://github.com/capagot/swpathtracer/
bool Bbox::intersect(const Ray& ray) const{

    double tmin;
    double tmax;
    double tymin;
    double tymax;
    double tzmin;
    double tzmax;
    double divx;
    double divy;
    double divz;

    divx = 1.0 / ray.direction_.x;
    if ( divx >= 0.0 )
    {
        tmin = ( negativeCorner.x - ray.origin_.x ) * divx;
        tmax = ( positiveCorner.x - ray.origin_.x ) * divx;
    }
    else
    {
        tmin = ( positiveCorner.x - ray.origin_.x ) * divx;
        tmax = ( negativeCorner.x - ray.origin_.x ) * divx;
    }

    divy = 1.0 / ray.direction_.y;
    if ( divy >= 0.0 )
    {
        tymin = ( negativeCorner.y - ray.origin_.y ) * divy;
        tymax = ( positiveCorner.y - ray.origin_.y ) * divy;
    }
    else
    {
        tymin = ( positiveCorner.y - ray.origin_.y ) * divy;
        tymax = ( negativeCorner.y - ray.origin_.y ) * divy;
    }

    if ( ( tmin > tymax ) || ( tymin > tmax ) )
        return false;

    if ( tymin > tmin )
        tmin = tymin;

    if ( tymax < tmax )
        tmax = tymax;

    divz = 1.0 / ray.direction_.z;
    if ( divz >= 0.0 )
    {
        tzmin = ( negativeCorner.z - ray.origin_.z ) * divz;
        tzmax = ( positiveCorner.z - ray.origin_.z ) * divz;
    }
    else
    {
        tzmin = ( positiveCorner.z - ray.origin_.z ) * divz;
        tzmax = ( negativeCorner.z - ray.origin_.z ) * divz;
    }

    if ( ( tmin > tzmax ) || ( tzmin > tmax ) )
        return false;

    if ( tzmin > tmin )
        tmin = tzmin;

    if ( tzmax < tmax )
        tmax  = tzmax;

    return true;
}
