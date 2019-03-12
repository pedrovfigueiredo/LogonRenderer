//
//  bvh.h
//  RayTracer
//
//  Created by Pedro Figueirêdo on 12/05/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#ifndef bvh_h
#define bvh_h

#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include "primitive.h"
#include "ray.h"
#include "intersection_record.h"
#include "bbox.h"

class BVH
{
public:
    
    struct BVHNode
    {
        ~BVHNode( void )
        {
            if ( left_ )
            {
                delete left_;
                left_ = nullptr;
            }
            
            if ( right_ )
            {
                delete right_;
                right_ = nullptr;
            }
        }
        
        std::vector<int> primitives_id_;
        Bbox box_;
        BVHNode* left_ = nullptr;                 // Pointer to the left child node (if the current node is a inner node).
        BVHNode* right_ = nullptr;                // Pointer to right inner node (if the current node is a inner node).
    };

    enum SplitMethod {CenterSorting , SAH};

    BVH(const std::vector< Primitive::PrimitiveUniquePtr > &primitives);

    void constructTree(const SplitMethod& splitMethod);

    bool intersect( const Ray &ray,
                   IntersectionRecord &intersection_record ) const;

private:
    void recursiveConstruct(BVHNode* node, int min, int max);
    void SAH_recursiveConstruct(BVHNode *node, const std::vector< int > &primitives_index);
    glm::vec3 min_components(const glm::vec3 &vecA, const glm::vec3 &vecB);
    glm::vec3 max_components(const glm::vec3 &vecA, const glm::vec3 &vecB);
    bool traverse(BVHNode* node, const Ray &ray, IntersectionRecord &intersection_record) const;
    void printProgress(struct timespec& begin);

    const std::vector< Primitive::PrimitiveUniquePtr > &primitives_;
    std::vector<int> primitives_id_;
    BVHNode* root;
    std::size_t primitivesInserted;
};


#endif /* bvh_h */
