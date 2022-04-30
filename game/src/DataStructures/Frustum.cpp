#include "Frustum.h"

Frustum::Frustum()
{

}

Frustum::~Frustum()
{

}    

void Frustum::Create(Plane topFace, Plane bottomFace, Plane leftFace, Plane rightFace, Plane farFace, Plane nearFace)
{
    this->topFace = topFace;
    this->bottomFace = bottomFace;
    this->leftFace = leftFace;
    this->rightFace = rightFace;
    this->farFace = farFace;
    this->nearFace = nearFace;
}