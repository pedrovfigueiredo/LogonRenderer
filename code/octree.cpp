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
                    node->isLeaf = false;
                    recursiveConstruct(depth + 1, childNode);
                }else
                    delete childNode;
            }
    
    // routine for avoiding unneccessary subdivisions
    for(auto& child : node->children_)
        if (!child || child->primitives_id_.size() != node->primitives_id_.size())
            return;
    
    for(int i = 0; i < 8; i++){
        delete node->children_[i];
        node->children_[i] = nullptr;
    }
    
    node->isLeaf = true;
        
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
    return traverse(root_, ray, intersection_record);
}

bool Octree::traverse(OctreeNode* node, const Ray &ray, IntersectionRecord &intersection_record) const{
    bool intersection_result = false;
    
    if (node && node->box_.intersect(ray)) {
        if (node->isLeaf) {
            IntersectionRecord tmp_intersection_record;
            for (std::size_t id_index = 0; id_index < node->primitives_id_.size(); id_index++) {
                if (primitives_[node->primitives_id_[id_index]]->intersect(ray, tmp_intersection_record)) {
                    if ( ( tmp_intersection_record.t_ < intersection_record.t_ ) && ( tmp_intersection_record.t_ > 0.0 ) )
                    {
                        intersection_record = tmp_intersection_record;
                        intersection_result = true; // the ray intersects the primitive!
                    }
                }
            }
        }else{ // is not a leaf node
            for(auto child : node->children_)
                if(traverse(child, ray, intersection_record))
                    intersection_result = true;
        }
    }
    
    return intersection_result;
}
