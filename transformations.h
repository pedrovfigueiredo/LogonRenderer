//
//  transformations.h
//  RayTracer
//
//  Created by Pedro Figueirêdo on 27/05/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#ifndef transformations_h
#define transformations_h

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "material.h"

class Transformations{
    
public:
    enum Axis{X,Y,Z};
    
    typedef struct Rotation{
        Rotation();
        Rotation(Axis axis, float rotationAngle){
            axis_ = axis; rotationAngle_ = rotationAngle;
        }
        Axis axis_;
        float rotationAngle_;
    }Rotation;
    
    Transformations();
    
    Transformations(float scale, std::vector<Rotation*> rotations, glm::vec3 translation_vector);
    
    glm::vec3 ApplyTransformations(glm::vec3 vector);
    
private:
    
    void rotateInAxis(glm::vec3& vector, Axis axis, float angle);
    
    float scale_;
    glm::vec3 translation_vector_;
    std::vector<Rotation*> rotations_;
    
};


#endif /* transformations_h */
