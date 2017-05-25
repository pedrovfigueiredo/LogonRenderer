//
//  smooth_conductor.h
//  RayTracer
//
//  Created by Pedro Figueirêdo on 22/05/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#ifndef smooth_conductor_h
#define smooth_conductor_h

#include <memory>
#include "material.h"
#include "mirror.h"
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

class RoughConductor : public Material
{
public:
    RoughConductor();
    RoughConductor(float roughness, glm::vec3 color);
    
    glm::vec3 getfr(glm::vec3& w_i, glm::vec3& w_o) const;
    glm::vec3 getEmittance() const;
    glm::vec3 getNewDirection(glm::vec3& w_i);
    
private:
    const float roughness_;
    
    double min(std::vector<double> n) const;
    
};



#endif /* smooth_conductor_h */
