#pragma once

#include <cstddef>
#include <string>

class EntityMemoryPool; 

class Entity
{
    friend class EntityManager;

    size_t m_id = 0;

public:
    Entity() = default;
    explicit Entity(size_t id);

    bool IsActive() const;
    void Destroy() const;

    size_t Id() const;
    const std::string& Tag() const;

    template <typename T>
    T& GetComponent()
    {
        return EntityMemoryPool::Instance().GetComponent<T>(m_id);
    }

    template <typename T>
    bool HasComponent()
    {
        return EntityMemoryPool::Instance().GetComponent<T>(m_id).exists;
    }

    template <typename T>
    T& Add()
    {
        auto& component = GetComponent<T>();
        component.exists = true;
        return component;
    }

    template <typename T>
    void Remove()
    {
        auto& component = GetComponent<T>();
        component.exists = false;
    }
};
