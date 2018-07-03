#include "primitive.h"

Primitive::Primitive(void)
{}

Primitive::Primitive( Material* material, glm::vec3 center, glm::vec3 negativeCorner, glm::vec3 positiveCorner ):
center_(center),
negativeCorner_(negativeCorner),
positiveCorner_(positiveCorner),
material_(material)
{}

Primitive::~Primitive( void )
{}
