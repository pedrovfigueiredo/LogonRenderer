//
//  octree.cpp
//  RayTracer
//
//  Created by Pedro Figueirêdo on 12/03/19.
//  Copyright © 2019 Lavid. All rights reserved.
//

#include "octree.h"

Octree::Octree(const std::vector< Primitive::PrimitiveUniquePtr > &primitives):
AccelerationStructure(primitives)
{}

Octree::Octree(const std::vector< Primitive::PrimitiveUniquePtr > &primitives, const unsigned maxDepth):
AccelerationStructure(primitives),
maxDepth_(maxDepth)
{}

void Octree::construct(){
    // Setting up root node
    root_ = new OctreeNode();
    
    root_->box_.negativeCorner = primitives_[0]->negativeCorner_;
    root_->box_.positiveCorner = primitives_[0]->positiveCorner_;
    for(std::size_t i = 1; i < primitives_.size() ; i++){
        root_->box_.negativeCorner = min_components(primitives_[i]->negativeCorner_, root_->box_.negativeCorner);
        root_->box_.positiveCorner = max_components(primitives_[i]->positiveCorner_, root_->box_.positiveCorner);
    }
    root_->box_.center = (root_->box_.negativeCorner + root_->box_.positiveCorner)*0.5f;
    
    //root_->primitives_id_.resize(primitives_.size()*sizeof(int));
    for (int i = 0; i < (long) primitives_.size(); i++)
        root_->primitives_id_.push_back(i);
    
    // Constructing tree from root
    recursiveConstruct(0, root_);
    
    assert(root_);
}

void Octree::recursiveConstruct(unsigned depth, OctreeNode* node){
    if (depth == maxDepth_)
        return;

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++){
                OctreeNode* childNode = new OctreeNode();
                this->computeSubBox(i, j, k, node->box_, childNode->box_);
                
                bool intersects = false;
                for(auto pIndex: node->primitives_id_)
                    if (primitives_[pIndex]->intersect(childNode->box_)) {
                        childNode->primitives_id_.push_back(pIndex);
                        intersects = true;
                    }
                
                if (intersects){
                    node->children_[k + 2*j + 4*i] = childNode;
                    recursiveConstruct(depth + 1, childNode);
                }
            }
}

void Octree::computeSubBox(int i, int j, int k, Bbox& parent, Bbox& child){
    glm::vec3 diagCenter = parent.center - parent.negativeCorner;
    child.negativeCorner = parent.negativeCorner + glm::vec3{k * diagCenter.x,j * diagCenter.y, i * diagCenter.z};
    //child.positiveCorner = parent.center + glm::vec3{k * diagCenter.x,j * diagCenter.y, i * diagCenter.z};
    child.positiveCorner = child.negativeCorner + diagCenter;
    child.center = child.negativeCorner + (child.positiveCorner - child.negativeCorner) * 0.5f;
}

bool Octree::intersect( const Ray &ray,
                            IntersectionRecord &intersection_record ) const{
    return true;
}
