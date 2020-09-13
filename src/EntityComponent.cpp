#include "EntityComponent.h"

namespace myth
{
    static unsigned s_entity_counter = 0;

    Entity::Entity() : id(++s_entity_counter) {}

    void Entity::addChild(Entity *child)
    {
        if(child)
        {
            m_children.push_back(child);
        }
    }

    Entity *Entity::getChild(const unsigned n)
    {
        if(n < m_children.size())
        {
            return m_children.at(n);
        }

        return nullptr;
    }

    void Entity::addComponent(Component *component)
    {
        if(component != nullptr)
        {
            component->setEntity(this);
            m_components.push_back(component);
        }
    }

    Component* Entity::getComponent(const ComponentType& type)
    {
        for(Component *c : m_components)
        {
            if(c->getType() == type)
            {
                return c;
            }
        }

        return nullptr;
    }

    void Entity::start()
    {
        for(Entity *c : m_children)
        {
            c->start();
        }

        for(Component *c : m_components)
        {
            c->start();
        }
    }

    void Entity::update()
    {
        for(Entity *c : m_children)
        {
            c->update();
        }

        for(Component *c : m_components)
        {
            c->update();
        }
    }

    void Entity::render(RenderingEngine *render)
    {
        for(Entity *c : m_children)
        {
            c->render(render);
        }

        for(Component *c : m_components)
        {
            c->render(render);
        }
    }

    void Entity::fixedUpdate()
    {
        for(Entity *c : m_children)
        {
            c->fixedUpdate();
        }

        for(Component *c : m_components)
        {
            c->fixedUpdate();
        }
    }

    void SceneGraph::dispose()
    {

    }
}
