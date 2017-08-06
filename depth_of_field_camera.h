//
//  depth_of_field_camera.h
//  RayTracer
//
//  Created by Pedro Figueirêdo on 04/08/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#ifndef depth_of_field_camera_h
#define depth_of_field_camera_h


#include "camera.h"
#include "ray.h"
#include "material.h"

class DepthOfFieldCamera : public Camera {
public:
    
    DepthOfFieldCamera( void );
    
    DepthOfFieldCamera( const float min_x,
                  const float max_x,
                  const float min_y,
                  const float max_y,
                  const float distance,
                  const glm::ivec2 &resolution,
                  const glm::vec3 &position,
                  const glm::vec3 &up_vector,
                  const glm::vec3 &look_at,
                  const float aperture,
                  const float focal_distance);
    
    Ray getWorldSpaceRay( const glm::vec2 &pixel_coord ) const;
    
    float min_x_;
    
    float max_x_;
    
    float min_y_;
    
    float max_y_;
    
    float distance_;
    
    float aperture_;
    
    float focal_distance_;
    
    glm::vec3 focal_point_;
};


#endif /* depth_of_field_camera_h */
