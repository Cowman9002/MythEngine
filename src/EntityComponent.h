#pragma once

#include <vector>
#include <unordered_map>

#include "Transform.h"

namespace myth
{
    enum class ComponentType
    {
        MeshRenderer,
        Script,
        Size
    };

    class Entity;
    class ResourceManager;
    class RenderingEngine;
    class Component
    {
    private:
        const ComponentType m_type;
        Entity *m_entity;
    protected:
        Component(ComponentType type) : m_type(type), m_entity(nullptr) {};
    public:

        inline ComponentType getType() { return m_type; };

        inline void setEntity(Entity *entity) { m_entity = entity; };
        inline Entity *getEntity() { return m_entity; };

        virtual void start() {};
        virtual void update() {};
        virtual void render(RenderingEngine *render) {};
        virtual void fixedUpdate() {};

    };

    class Entity
    {
    private:
        std::vector<Entity*> m_children;
        std::vector<Component*> m_components;
    public:
        Transform transform;
        const unsigned id;

        Entity();
        //Entity(const Entity& e) = delete;

        void addChild(Entity *child);
        Entity *getChild(const unsigned n);

        void addComponent(Component *component);
        Component* getComponent(const ComponentType& type);

        void start();
        void update();
        void render(RenderingEngine *render);
        void fixedUpdate();
    };

    class SceneGraph
    {
    private:
        Entity m_root;
    public:
        inline void addToScene(Entity *entity) { m_root.addChild(entity); };

        inline void start() { m_root.start(); };
        inline void update() { m_root.update(); };
        inline void render(RenderingEngine *render) { m_root.render(render); };
        inline void fixedUpdate() { m_root.fixedUpdate(); };

        void dispose();
    };
}
