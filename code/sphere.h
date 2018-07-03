#ifndef SPHERE_H_
#define SPHERE_H_

#include <glm/glm.hpp>
#include "primitive.h"
#include "intersection_record.h"
#include "ray.h"

class Sphere : public Primitive
{
public:

    Sphere( void );

    Sphere( Material* material, const glm::vec3 &center,
            float radius );

    bool intersect( const Ray &ray,
                    IntersectionRecord &intersection_record ) const;

    float radius_= 1.0f;
};

#endif /* SPHERE_H_ */
