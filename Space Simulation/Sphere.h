#ifndef SPHERE_H
#define SHERE_H

#include "ObjectRender.h"

class Sphere : public ObjectRender
{
public:
    Sphere(int slices, int stacks, float radius);
};

#endif
