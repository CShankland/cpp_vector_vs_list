//
// Created by chris on 7/18/2020.
//

#include "Entity.h"

Entity::Entity()
    : mParent(nullptr), mWorldTransform(IdentityMat4()), mLocalTransform(IdentityMat4()) {}

void Entity::update()
{

}

void Entity::updateTransform()
{
    if (mParent)
    {
        mWorldTransform = Mat4Multiply(mParent->mWorldTransform, mLocalTransform);
    }
    else
    {
        mWorldTransform = mLocalTransform;
    }
}

void Entity::draw(Renderer &renderer)
{

}
