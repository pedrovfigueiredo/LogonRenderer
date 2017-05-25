//
//  smooth_dieletric.h
//  RayTracer
//
//  Created by Pedro Figueirêdo on 02/05/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#ifndef smooth_dieletric_h
#define smooth_dieletric_h

#include <memory>
#include "material.h"
#include "mirror.h"
#include <glm/glm.hpp>
#include <iostream>

class SmoothDieletric : public Material
{
public:
    SmoothDieletric();
    SmoothDieletric(float IOR);
    
    glm::vec3 getfr(glm::vec3& w_i, glm::vec3& w_o) const;
    glm::vec3 getEmittance() const;
    glm::vec3 getNewDirection(glm::vec3& w_i);
    
    double rShclick2(const glm::vec3& w_i, const glm::vec3& normal , double& n1, double& n2);
    glm::vec3 refract(const glm::vec3& w_i,const glm::vec3 normal, const float n1, const float n2);
    
private:
    const float IOR_;
    
};


#endif /* smooth_dieletric_h */
