//
//  triangle.cpp
//  RayTracer
//
//  Created by Pedro Figueirêdo on 10/02/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#include "triangle.h"

const float Triangle::kIntersectionTestEpsilon_ = 0.000001;

Triangle::Triangle( void ){}

Triangle::Triangle( Material* material, const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c ) :
Primitive::Primitive{material,                                                                         // material
                    {0,0,0},                   // center
                    {std::min({a.x, b.x, c.x}), std::min({a.y, b.y, c.y}), std::min({a.z, b.z, c.z})}, // negativeCorner
                    {std::max({a.x, b.x, c.x}), std::max({a.y, b.y, c.y}), std::max({a.z, b.z, c.z})}},// positiveCorner
a_{ a },
b_{ b },
c_{ c }
{center_ = (positiveCorner_ + negativeCorner_) * 0.5f;}

bool Triangle::intersect( const Bbox &bbox) const{
    
    // helper function to decide whether intervals overlap a->b overlaps c->d?
    
    // function that projects vertices of the triangle to axis x,y,z and returns interval
        // Triangle: x(1,0,0),y(0,1,0),z(0,0,1), cross(c-b,a-b) = n, cross(n,x), cross(n,y), cross(n,z)
    
    // if one is false, false
    // if all intersect, true
    
    
    
    return true;
}

// Ray-Triangle Intersection based on the paper "Fast, Minimum Storage Ray/Triangle Intersection", by Tomas Moller and  Ben Trumbore.
// The non-culling approach was chosen.

bool Triangle::intersect( const Ray &ray, IntersectionRecord &intersection_record ) const{

    glm::vec3 edge1 = b_ - a_;
    glm::vec3 edge2 = c_ - a_;
    glm::vec3 pvec = glm::cross( ray.direction_, edge2 );

    double det = glm::dot( edge1, pvec );

    if ( ( det > -Triangle::kIntersectionTestEpsilon_ ) && ( det < Triangle::kIntersectionTestEpsilon_ ) )
        return false;

    double inv_det = 1.0 / det;

    glm::vec3 tvec{ ray.origin_ - a_ };

    double u = glm::dot( tvec, pvec ) * inv_det;

    if ( ( u < 0.0 ) || ( u > 1.0 ) )
        return false;

    glm::vec3 qvec{ glm::cross( tvec, edge1 ) };

    double v = glm::dot( ray.direction_, qvec ) * inv_det;

    if ( ( v < 0.0 ) || ( ( u + v ) > 1.0 ) )
        return false;

    double t = glm::dot( edge2, qvec ) * inv_det;


    intersection_record.t_ = t;
    intersection_record.position_ = ray.origin_ + intersection_record.t_ * ray.direction_;
    intersection_record.normal_ = glm::normalize( glm::cross(b_ - a_, c_ - a_));
    intersection_record.material = material_;

    return true;
}
