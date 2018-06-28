//
//  smooth_dieletric.cpp
//  RayTracer
//
//  Created by Pedro Figueirêdo on 02/05/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#include "smooth_dieletric.h"

SmoothDieletric::SmoothDieletric(void):
Material::Material{{0,0,0}, {0,0,0}},
IOR_(1.5),
absorbance_({0,0,0}){type_ = type::SmoothDieletric;};

SmoothDieletric::SmoothDieletric( float IOR, glm::vec3 absorbance):
Material::Material({0,0,0}, {0,0,0}),
IOR_(IOR),
absorbance_(absorbance){type_ = type::SmoothDieletric;}

glm::vec3 SmoothDieletric::getEmittance() const{
    return emittance_;
}

glm::vec3 SmoothDieletric::getNewDirection(glm::vec3& w_i){
    double ni,no;
    glm::vec3 invertedDirection = -w_i;
    double fresnel;
    glm::vec3 normal = {0,1,0};

    if (glm::dot(w_i, normal) > 0) { // ray comes from outside
        ni = 1.0f;
        no = this->IOR_;
    }else{ // ray comes from inside
        ni = this->IOR_;
        no = 1.0f;
        normal = {0, -1, 0};
    }

    fresnel = rShclick2(invertedDirection,normal, ni, no);

    double random = (double)rand()/(RAND_MAX + 1.0f);

    if (random < fresnel) // incoming ray is reflected
        return glm::vec3{-w_i.x, w_i.y, -w_i.z};
    else
        return refract(invertedDirection,normal, ni, no);

}

glm::vec3 SmoothDieletric::getfr(glm::vec3& w_i, glm::vec3& w_o, float pathLength, float& distanceInObject) const{
    double ni,no;
    glm::vec3 invertedDirection = -w_i;
    glm::vec3 normal = {0,1,0};

    if (glm::dot(w_i, normal) > 0) { // ray comes from outside
        ni = 1.0f;
        no = this->IOR_;
    }else{ // ray comes from inside
        ni = this->IOR_;
        no = 1.0f;
        normal = {0, -1, 0};
        distanceInObject += pathLength;
    }

    double fresnel = rShclick2(invertedDirection,normal, ni, no);

    double random = (double)rand()/(RAND_MAX + 1.0f);


    if (random < fresnel) // incoming ray is reflected
        return glm::vec3{1,1,1};
    else{ // incoming ray is refracted
        glm::vec3 refracted_color = glm::exp((-this->absorbance_) * distanceInObject);
        distanceInObject = 0;
        return refracted_color;
    }
}

double SmoothDieletric::rShclick2( const glm::vec3& w_i, const glm::vec3& normal , double& n1, double& n2) const{
    double r0 = (n1 - n2) /(n1 + n2);
    r0 *= r0;
    double cosX = -glm::dot(normal, w_i);
    if (n1 > n2) {
        const double n = n1/n2;
        const double sinT2 = n * n * (1.0 - cosX * cosX);
        if(sinT2 > 1.0) return 1.0; //TIR
        cosX = sqrt(1.0 - sinT2);
    }
    const double x = 1.0 - cosX;
    return r0 + ( 1.0 - r0 ) * x * x * x * x * x;
}

glm::vec3 SmoothDieletric::refract(const glm::vec3& w_i, const glm::vec3 normal, const float n1, const float n2) const {
    const float n = n1/n2;
    const float cosI = -glm::dot(normal, w_i);
    const double sinT2 = n * n * (1.0 - cosI * cosI);
    if (sinT2 > 1.0) return glm::vec3{0,0,0}; // TIR
    const float cosT = glm::sqrt(1.0 - sinT2);
    return n * w_i + (n * cosI - cosT) * normal;
}
