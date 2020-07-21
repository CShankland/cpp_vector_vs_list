//
// Created by chris on 7/18/2020.
//

#include "Container.h"

void Container::addChild(Entity* entity)
{
    entity->mParent = this;

    mChildren.push_back(entity);
}

void Container::update()
{
    Entity::update();

    for (auto& child : mChildren)
    {
        child->update();
    }
}

void Container::updateTransform()
{
    Entity::updateTransform();

    for (auto& child : mChildren)
    {
        child->updateTransform();
    }
}

void Container::draw(Renderer &renderer)
{
    Entity::draw(renderer);

    for (auto& child : mChildren)
    {
        child->draw(renderer);
    }
}
