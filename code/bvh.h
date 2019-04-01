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
#include "acceleration_structure.h"

class BVH : public AccelerationStructure
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
    
    BVH(const std::vector< Primitive::PrimitiveUniquePtr > &primitives, const SplitMethod &splitMethod);

    void construct();

    bool intersect( const Ray &ray,
                   IntersectionRecord &intersection_record ) const;
    
private:
    void recursiveConstruct(BVHNode* node, int min, int max);
    void SAH_recursiveConstruct(BVHNode *node, const std::vector< int > &primitives_index);
    bool traverse(BVHNode* node, const Ray &ray, IntersectionRecord &intersection_record) const;
    void printProgress(struct timespec& begin);
    
    std::vector<int> primitives_id_;
    BVHNode* root;
    std::size_t primitivesInserted;
    SplitMethod sp;
};


#endif /* bvh_h */
