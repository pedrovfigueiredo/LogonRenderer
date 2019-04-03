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

void Triangle::project(const std::vector<glm::vec3>& vec, const glm::vec3 axis, float& min, float& max) const{
    min = FLT_MAX;
    max = FLT_MIN;
    for(auto& v : vec){
        float d = glm::dot(v,axis);
        if (d < min) min = d;
        if (d > max) max = d;
    }
}

// Using SAT theorem
bool Triangle::intersect( const Bbox &bbox) const{
    // min and max variables
    float minTriangle, maxTriangle, minBbox, maxBbox;
    
    // Triangle vertices
    const std::vector<glm::vec3> vTriangle {a_,b_,c_};
    
    // Bbox vertices
    const glm::vec3 v1 = bbox.negativeCorner;
    const glm::vec3 v2 = bbox.positiveCorner;
    const glm::vec3 v3 = glm::vec3{v1.x, v1.y, v2.z};
    const glm::vec3 v4 = glm::vec3{v1.x, v2.y, v1.z};
    const glm::vec3 v5 = glm::vec3{v2.x, v1.y, v1.z};
    const glm::vec3 v6 = glm::vec3{v1.x, v2.y, v2.z};
    const glm::vec3 v7 = glm::vec3{v2.x, v1.y, v2.z};
    const glm::vec3 v8 = glm::vec3{v2.x, v2.y, v1.z};
    const std::vector<glm::vec3> vBbox {v1,v2,v3,v4,v5,v6,v7,v8};
    
    // Test the box normals (x, y and z axes)
    glm::vec3 boxNormals[] {glm::vec3{1,0,0}, glm::vec3{0,1,0}, glm::vec3{0,0,1}};
    for (int i = 0; i < 3; i++) {
        project(vTriangle, boxNormals[i], minTriangle, maxTriangle);
        if (maxTriangle < v1[i] || minTriangle > v2[i])
            return false;
    }
    
    // Test the triangle normal
    glm::vec3 n = glm::cross(a_ - b_, c_ - b_);
    float triangleOffset = glm::dot(n, a_);
    project(vBbox, n, minBbox, maxBbox);
    if (maxBbox < triangleOffset || minBbox > triangleOffset)
        return false;
    
    // Test the nine edge cross-products
    glm::vec3 tEdges[] {a_ - c_, c_ - b_, b_ - a_};
    
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++){
            // The box normals are the same as it's edge tangents
            glm::vec3 axis = glm::cross(tEdges[i], boxNormals[j]);
            project(vBbox, axis, minBbox, maxBbox);
            project(vTriangle, axis, minTriangle, maxTriangle);
            if (maxBbox < minTriangle || minBbox > maxTriangle)
                return false;
        }
    
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
