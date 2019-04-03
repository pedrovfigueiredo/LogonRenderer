//
//  octree.h
//  RayTracer
//
//  Created by Pedro Figueirêdo on 12/03/19.
//  Copyright © 2019 Lavid. All rights reserved.
//

#ifndef octree_h
#define octree_h

#include <glm/glm.hpp>
#include <vector>

#include "primitive.h"
#include "bbox.h"
#include "ray.h"
#include "intersection_record.h"
#include "acceleration_structure.h"

class Octree : public AccelerationStructure
{
public:
    
    struct OctreeNode
    {
        ~OctreeNode( void ){
            for(auto child: children_)
                delete child;
        }
        
        OctreeNode(){};
        OctreeNode(Bbox& box):box_(box){};
        
        std::vector<int> primitives_id_;
        Bbox box_;
        OctreeNode* children_[8] = {nullptr};   // Pointer to children nodes (if the current node is an inner node).
        
        bool isLeaf = true;
    };
    
    Octree(const std::vector< Primitive::PrimitiveUniquePtr > &primitives);
    
    Octree(const std::vector< Primitive::PrimitiveUniquePtr > &primitives, const unsigned maxDepth);
    
    void construct();
    
    bool intersect( const Ray &ray,
                   IntersectionRecord &intersection_record ) const;
    
private:
    
    void recursiveConstruct(unsigned depth, OctreeNode* node);
    void computeSubBox(int i, int j, int k, Bbox& parent, Bbox& child);
    bool traverse(OctreeNode* node, const Ray &ray, IntersectionRecord &intersection_record) const;
    
    OctreeNode* root_ = nullptr;
    const unsigned maxDepth_ = 5;
};

#endif /* octree_h */
