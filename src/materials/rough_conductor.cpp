//
//  smooth_conductor.cpp
//  RayTracer
//
//  Created by Pedro Figueirêdo on 22/05/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#include "rough_conductor.h"

RoughConductor::RoughConductor(void):
Material::Material{{0,0,0}, {0,0,0}},
roughness_(0.8){type_ = type::RoughConductor;};

RoughConductor::RoughConductor(float roughness, glm::vec3 color):
roughness_(roughness){
    type_ = type::RoughConductor;
    emittance_ = {0,0,0};

    if (color.x > 1 || color.y > 1 || color.z > 1)
        brdf_ = {(color.x/255), (color.y/255), (color.z/255)};
    else
        brdf_ = {color.x, color.y, color.z};


}

glm::vec3 RoughConductor::getEmittance() const{
    return emittance_;
}

glm::vec3 RoughConductor::getfr(glm::vec3& w_i, glm::vec3& w_o, float pathLength, float& distanceInObject) const{

    if ( w_o.y < 0.0 )
        return glm::vec3{ 0.0, 0.0, 0.0 };

    glm::vec3 n = {0,1,0};
    glm::vec3 h = glm::normalize((w_o + w_i)/2.0f);

    double nh = std::abs( h.y );
    double nwo = std::abs( w_o.y );
    double nwi = std::abs( w_i.y );
    double hwo = glm::abs(glm::dot( h , w_o ));
    double hwi = glm::abs(glm::dot( h , w_i ));

    // Beckmann
    double nh2 = nh*nh;
    double m2 = roughness_ * roughness_;
    double d1 = 1 / (PI * m2 * nh2 * nh2);
    double d2 = (nh2 - 1) / (m2 * nh2);
    double D = d1 * glm::exp(d2);

    // Geometric term
    double g1 = 2 * nh / hwo;
    double G = min({1, g1 * nwo, g1 * nwi});

    // Fresnel term (Schlick's)
    glm::dvec3 r0 = this->brdf_;
    const double x = 1.0 - hwi;
    glm::dvec3 F = r0 + ( 1.0 - r0 ) * x * x * x * x * x;

    //PDF
    double pdf = ( D * nh ) / ( 4 * hwi );
    //double pdf = 1/(2*PI);

    return (( F * D * G ) / ( 4 * nwo * nwi ) * (double) w_o.y) / pdf;


}

glm::vec3 RoughConductor::getNewDirection(glm::vec3& w_i){
    double r1 = ((double)rand()/(RAND_MAX));
    double r2 = ((double)rand()/(RAND_MAX));
    double phi = 2 * PI * r2;
    double theta = glm::atan( glm::sqrt( -( roughness_ * roughness_) * glm::log ( 1 - r1 ) ) );
    glm::vec3 m = glm::vec3{ glm::cos(phi)* glm::sin(theta), glm::cos(theta), glm::sin(phi) * glm::sin(theta) };
    // generate the light ray by reflecting wo about m
    return 2.0f * m * glm::dot( m, w_i ) - w_i;
}

double RoughConductor::min(std::vector<double> n) const
{
    double min = n[0];

    for(auto &&ns : n){
        if (ns < min) {
            min = ns;
        }
    }

    return min;
}
