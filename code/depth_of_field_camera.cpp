//
//  depth_of_field_camera.cpp
//  RayTracer
//
//  Created by Pedro Figueirêdo on 04/08/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#include "depth_of_field_camera.h"

DepthOfFieldCamera::DepthOfFieldCamera( void )
{}

DepthOfFieldCamera::DepthOfFieldCamera(const float min_x,
                                       const float max_x,
                                       const float min_y,
                                       const float max_y,
                                       const float distance,
                                       const glm::ivec2 &resolution,
                                       const glm::vec3 &position,
                                       const glm::vec3 &up_vector,
                                       const glm::vec3 &look_at,
                                       const float aperture,
                                       const float focal_distance) :
Camera::Camera{ resolution,
    position,
    up_vector,
    look_at },
min_x_{ min_x },
max_x_{ max_x },
min_y_{ min_y },
max_y_{ max_y },
distance_{distance},
aperture_(aperture),
focal_distance_(focal_distance)
{}

Ray DepthOfFieldCamera::getWorldSpaceRay( const glm::vec2 &pixel_coord ) const
{
    float a = max_x_ - min_x_;
    float b = -(max_y_ - min_y_);
    float c = -distance_;

    float u = ((pixel_coord[0])/static_cast< float >(resolution_[0]));
    float v = ((pixel_coord[1])/static_cast< float >(resolution_[1]));

    glm::vec3 s { u*a + min_x_,v*b - min_y_, c };

    Ray primary_ray = Ray{ position_,
        glm::normalize( onb_.getBasisMatrix() * s ) };

    if (aperture_ == 0)
        return primary_ray;
    
    glm::vec3 focal_point = primary_ray.origin_ + ((float)focal_distance_ * primary_ray.direction_);
    
    float theta,r;
    theta = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(2 * Material::PI)));
    r = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(aperture_)));
    
    glm::vec3 randomPoint = primary_ray.origin_ + glm::vec3{r * glm::cos(theta), r * glm::sin(theta) , 0};
    
    return Ray {randomPoint, glm::normalize(focal_point - randomPoint)};
}
