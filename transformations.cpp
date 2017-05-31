//
//  transformations.cpp
//  RayTracer
//
//  Created by Pedro Figueirêdo on 27/05/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#include "transformations.h"
#define GLM_FORCE_RADIANS
#define degreesToRadians(x) x*(3.141592f/180.0f)


Transformations::Transformations():
scale_(1),
translation_vector_({0,0,0}){}

Transformations::Transformations(float scale, std::vector<Rotation*> rotations, glm::vec3 translation_vector):
scale_(scale),
translation_vector_(translation_vector),
rotations_(rotations){}

glm::vec3 Transformations::ApplyTransformations(glm::vec3 vector){
    
    if (!rotations_.empty()) {
        for(auto rotation : rotations_)
            if (rotation->rotationAngle_ != 0)
                rotateInAxis(vector, rotation->axis_, rotation->rotationAngle_);
    }
    
    if (scale_ != 1) {
        vector *= scale_;
    }
    
    if (!(translation_vector_.x == 0 && translation_vector_.y == 0 && translation_vector_.z == 0)) {
        vector += translation_vector_;
    }
    
    return vector;
}

void Transformations::rotateInAxis(glm::vec3& vector, Axis axis, float angle){
    glm::mat3x3 rotationMatrix{{1,0,0}, {0,1,0}, {0,0,1}};
    angle = degreesToRadians(angle);
    
    if (angle) {
        switch (axis) {
            case X:
                rotationMatrix[1] = {0, glm::cos(angle), - glm::sin(angle)};
                rotationMatrix[2] = {0, glm::sin(angle), glm::cos(angle)};
                break;
            case Y:
                rotationMatrix[0] = {glm::cos(angle),0,glm::sin(angle)};
                rotationMatrix[2] = {- glm::sin(angle), 0, glm::cos(angle)};
                break;
            case Z:
                rotationMatrix[0] = {glm::cos(angle), - glm::sin(angle), 0};
                rotationMatrix[1] = {glm::sin(angle), glm::cos(angle), 0};
                break;
            default:
                break;
        }
    }
    
    vector = rotationMatrix * vector;
}
