#ifndef PRIMITIVE_H_
#define PRIMITIVE_H_

#include <memory>
#include <glm/glm.hpp>

#include "ray.h"
#include "material.h"
#include "intersection_record.h"

class Primitive
{
public:

    typedef std::unique_ptr< Primitive > PrimitiveUniquePtr;

    Primitive (void);
    
    Primitive( Material* material );

    virtual ~Primitive( void );

    virtual bool intersect( const Ray &ray,
                            IntersectionRecord &intersection_record ) const = 0;
    
    Material* material_;
};

#endif /* PRIMITIVE_H_ */

