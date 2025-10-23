#ifndef SPHERE_H
#define SPHERE_H

#include "ObjectRender.h"

class Sphere : public ObjectRender
{
public:
    Sphere(float radius, int stacks, int slices);
};

#endif
