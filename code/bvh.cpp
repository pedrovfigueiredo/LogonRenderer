//
//  bvh.cpp
//  RayTracer
//
//  Created by Pedro Figueirêdo on 12/05/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#include "bvh.h"
#include <fstream>
#include <iostream>


BVH::BVH(const std::vector< Primitive::PrimitiveUniquePtr > &primitives):
primitives_(primitives),
primitivesInserted(0){
    if (primitives_.empty()){
        root = nullptr;
        return;
    }
}

void BVH::constructTree(const SplitMethod& splitMethod){
    for (int i = 0; i < (long) primitives_.size(); i++)
        primitives_id_.push_back(i);

    root = new BVHNode();

    primitivesInserted = 0;

    struct timespec treeBuildingTimeStart, treeBuildingTimeFinish;

    clock_gettime(CLOCK_MONOTONIC, &treeBuildingTimeStart);
    std::thread progressTracker(&BVH::printProgress, this, std::ref(treeBuildingTimeStart));

    switch (splitMethod) {
        case CenterSorting:
            recursiveConstruct(root, 0, (int)primitives_id_.size() - 1);
            break;

        case SAH:
            SAH_recursiveConstruct(root, primitives_id_);
            break;
    }
    clock_gettime(CLOCK_MONOTONIC, &treeBuildingTimeFinish);

    progressTracker.join();

    float duration = treeBuildingTimeFinish.tv_sec - treeBuildingTimeStart.tv_sec;

    std::clog << "\r  BVH construction time: " << ((int)(duration/60)) % 60 << "m " << ((int)round(duration)) % 60 << "s" << std::endl;

    primitives_id_.clear();
}

void BVH::recursiveConstruct(BVHNode* node, int min, int max){

    // Returns when there are 2 primitives or less on the node
    if ((max - min) < 2) {
        if (max == min) {
            primitivesInserted++;
            node->primitives_id_ = {primitives_id_[min]};
            node->box_.positiveCorner = primitives_[primitives_id_[min]]->positiveCorner_;
            node->box_.negativeCorner = primitives_[primitives_id_[min]]->negativeCorner_;
            node->box_.center = primitives_[primitives_id_[min]]->center_;
        }else{
            primitivesInserted += 2;
            node->primitives_id_ = {primitives_id_[min], primitives_id_[max]};
            node->box_.positiveCorner = glm::vec3{std::max(primitives_[primitives_id_[min]]->positiveCorner_.x,primitives_[primitives_id_[max]]->positiveCorner_.x),
                                             std::max(primitives_[primitives_id_[min]]->positiveCorner_.y,primitives_[primitives_id_[max]]->positiveCorner_.y),
                                             std::max(primitives_[primitives_id_[min]]->positiveCorner_.z,primitives_[primitives_id_[max]]->positiveCorner_.z)};
            node->box_.negativeCorner = glm::vec3{std::min(primitives_[primitives_id_[min]]->negativeCorner_.x,primitives_[primitives_id_[max]]->negativeCorner_.x),
                                             std::min(primitives_[primitives_id_[min]]->negativeCorner_.y,primitives_[primitives_id_[max]]->negativeCorner_.y),
                                             std::min(primitives_[primitives_id_[min]]->negativeCorner_.z,primitives_[primitives_id_[max]]->negativeCorner_.z)};
            node->box_.center = (node->box_.positiveCorner + node->box_.negativeCorner) * 0.5f;
        }

        node->left_ = nullptr;
        node->right_ = nullptr;
        return;
    }

    node->box_.negativeCorner = primitives_[primitives_id_[min]]->negativeCorner_;
    node->box_.positiveCorner = primitives_[primitives_id_[min]]->positiveCorner_;

    for(int aux = min + 1; aux <= max ; aux++){
        node->box_.negativeCorner = min_components(primitives_[primitives_id_[aux]]->negativeCorner_, node->box_.negativeCorner);
        node->box_.positiveCorner = max_components(primitives_[primitives_id_[aux]]->positiveCorner_, node->box_.positiveCorner);
    }
    node->box_.center = (node->box_.negativeCorner + node->box_.positiveCorner)*0.5f;

    glm::vec3 bBoxSize = node->box_.positiveCorner - node->box_.negativeCorner;
    float maxDist = std::max({bBoxSize.x, bBoxSize.y, bBoxSize.z});
    int axis;

    // Select which axis will be used for the vector ordenation.
    if (maxDist == bBoxSize.x)
        axis = 0; // X
    else if(maxDist == bBoxSize.y)
        axis = 1; // Y
    else
        axis = 2; // Z

    // Vector "primitives_id_" is rising-ordered, according to each indexed primitive's center.
    std::sort( primitives_id_.begin() + min, primitives_id_.begin() + max + 1, [ & ]( const int& index1, const int& index2 )
    {
        return (primitives_[index1]->center_[axis] < primitives_[index2]->center_[axis]);
    });

    // Two Bbox daughters are created, each one containing half the elements of their parent Bbox.
    node->left_ = new BVHNode;
    recursiveConstruct(node->left_, min, ((max-min)/2) + min);

    node->right_ = new BVHNode;
    recursiveConstruct(node->right_, (((max-min)/2) + 1) + min, max);

}

void BVH::SAH_recursiveConstruct(BVHNode *node, const std::vector< int > &primitives_index){

    // Calculate box dimensions
    node->box_.negativeCorner = primitives_[ primitives_index[0] ]->negativeCorner_;
    node->box_.positiveCorner = primitives_[ primitives_index[0] ]->positiveCorner_;

    for(int prim_id : primitives_index){
        node->box_.negativeCorner = min_components(primitives_[prim_id]->negativeCorner_, node->box_.negativeCorner);
        node->box_.positiveCorner = max_components(primitives_[prim_id]->positiveCorner_, node->box_.positiveCorner);
    }

    node->box_.center = node->box_.negativeCorner;


    // Too few elements, make leaf node were here

    // Select best axis to divide
    glm::vec3 b_size = node->box_.positiveCorner - node->box_.negativeCorner;
    float aux = std::max(b_size.x, std::max(b_size.y, b_size.z));
    int axis;

    if(aux == b_size.x) axis = 0;
    else if(aux == b_size.y) axis = 1;
    else axis = 2;


    // Find best division
    std::vector< int > left_prim;
    std::vector< int > right_prim;

    int trying = 0;
    int init_axis = axis;
    float min_cost = FLT_MAX;
    glm::vec3 best_center;
    int best_axis = init_axis;
    glm::vec3 aux_size;
    float left_area, right_area, total_area;
    float left_cost, right_cost, total_cost, no_div_cost;
    glm::vec3 current_negative_corner;
    glm::vec3 current_positive_corner;

    total_area = (b_size.x*b_size.y + b_size.y*b_size.z + b_size.x*b_size.z) * 2.0f;
    no_div_cost = primitives_index.size();

    do{
        while( trying++ < 32 ){

            for(int prim_id : primitives_index){
                if(primitives_[prim_id]->center_[axis] < node->box_.center[axis])
                    left_prim.push_back(prim_id);
                else
                    right_prim.push_back(prim_id);
            }


            if(left_prim.size() > 0){
                current_negative_corner = primitives_[ left_prim[0] ]->negativeCorner_;
                current_positive_corner = primitives_[ left_prim[0] ]->positiveCorner_;

                for(int prim_id : left_prim){
                    current_negative_corner = min_components(primitives_[prim_id]->negativeCorner_, current_negative_corner);
                    current_positive_corner = max_components(primitives_[prim_id]->positiveCorner_, current_positive_corner);
                }

                aux_size = (current_positive_corner - current_negative_corner);
                left_area = (aux_size.x*aux_size.y + aux_size.y*aux_size.z + aux_size.x*aux_size.z) * 2.0f;
                left_cost = (left_area / total_area) * left_prim.size();
            }
            else{ left_cost = 0.0f; }

            if(right_prim.size() > 0){
                current_negative_corner = primitives_[ right_prim[0] ]->negativeCorner_;
                current_positive_corner = primitives_[ right_prim[0] ]->positiveCorner_;

                for(int prim_id : right_prim){
                    current_negative_corner = min_components(primitives_[prim_id]->negativeCorner_, current_negative_corner);
                    current_positive_corner = max_components(primitives_[prim_id]->positiveCorner_, current_positive_corner);
                }

                aux_size = (current_positive_corner - current_negative_corner);
                right_area = (aux_size.x*aux_size.y + aux_size.y*aux_size.z + aux_size.x*aux_size.z) * 2.0f;
                right_cost = (right_area / total_area) * right_prim.size();
            }
            else{ right_cost = 0.0f; }

            total_cost = 2 + right_cost + left_cost;

            if(min_cost > total_cost){
                min_cost = total_cost;
                best_center = node->box_.center;
                best_axis = axis;
            }

            node->box_.center[axis] = node->box_.negativeCorner[axis] + b_size[axis] * ( trying / 32.0f);

            left_prim.clear();
            right_prim.clear();

        }
        axis = (axis+1)%3;
        trying = 0;
    } while(axis != init_axis);

    if( no_div_cost < min_cost){
        node->primitives_id_ = primitives_index;
        node->left_ = nullptr;
        node->right_ = nullptr;
        primitivesInserted += primitives_index.size();
        return;
    }

    node->box_.center = best_center;
    axis = best_axis;

    for(int prim_id : primitives_index){

        if(primitives_[prim_id]->center_[axis] < node->box_.center[axis])
            left_prim.push_back(prim_id);
        else
            right_prim.push_back(prim_id);
    }


    node->left_ = new BVHNode;
    SAH_recursiveConstruct(node->left_, left_prim);

    node->right_ = new BVHNode;
    SAH_recursiveConstruct(node->right_, right_prim);
}


glm::vec3 BVH::max_components(const glm::vec3 &vecA, const glm::vec3 &vecB){

    glm::vec3 max;

    for(int i = 0; i < 3; i++)
        if(vecA[i] > vecB[i])
            max[i] = vecA[i];
        else
            max[i] = vecB[i];

    return max;
}


glm::vec3 BVH::min_components(const glm::vec3 &vecA, const glm::vec3 &vecB){

    glm::vec3 min;

    for(int i = 0; i < 3; i++)
        if(vecA[i] < vecB[i])
            min[i] = vecA[i];
        else
            min[i] = vecB[i];

    return min;
}


bool BVH::intersect( const Ray &ray,
                    IntersectionRecord &intersection_record ) const{
    return traverse(root, ray, intersection_record);
}


bool BVH::traverse(BVHNode* node, const Ray &ray, IntersectionRecord &intersection_record) const{

    bool intersection_result = false;

    if (node && node->box_.intersect(ray)) {
        if (!node->left_ && !node->right_) { // is a leaf node
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
            if (traverse(node->left_, ray, intersection_record))
                intersection_result = true;
            if (traverse(node->right_, ray, intersection_record))
                intersection_result = true;
        }
    }

    return intersection_result;
}

void BVH::printProgress(struct timespec& begin){

    double elapsed_secs = 0;
    struct timespec finish;
    std::size_t primSize = primitives_.size();

    while (primitivesInserted < primSize) {
        clock_gettime(CLOCK_MONOTONIC, &finish);
        elapsed_secs = double(finish.tv_sec - begin.tv_sec);

        std::stringstream progress_stream;
        progress_stream << "\r  BVH Progress: "
        << std::fixed << std::setw( 6 )
        << std::setprecision( 2 )
        << 100.0 *  primitivesInserted / primitives_.size()
        << "%"
        << " "
        << "Elapsed time: " << ((int)(elapsed_secs/60))/60 << "h " << ((int)(elapsed_secs/60)) % 60 << "m " << ((int)round(elapsed_secs)) % 60 << "s"
        << std::endl;
        std::clog << progress_stream.str();

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
