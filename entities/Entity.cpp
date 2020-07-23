//
// Created by chris on 7/18/2020.
//

#include "Entity.h"

Entity::Entity(mat4* worldTransform, mat4* localTransform)
    : mParent(nullptr), mWorldTransform(worldTransform), mLocalTransform(localTransform) {}

void Entity::update()
{

}

void Entity::updateTransform()
{
    if (mParent)
    {
        *mWorldTransform = Mat4Multiply(*mParent->mWorldTransform, *mLocalTransform);
    }
    else
    {
        *mWorldTransform = *mLocalTransform;
    }
}

void Entity::draw(Renderer &renderer)
{

}
