#pragma once

#include"Components.hpp"
#include<string>
#include<tuple>

class EntityManager;

using ComponentTuple = std::tuple<

	CTransform,
	CShape,
	CCollision,
	CScore,
	CLifeSpan,
	CInput,
	CCoord,
	CText
>;

class Entity
{
	friend class EntityManager;

	ComponentTuple m_components;
	bool m_active = true;
	std::string m_tag = "default";
	size_t m_id = 0;

	Entity(const size_t& id, const std::string &tag)
		:m_tag(tag), m_id(id) { }

public:

	bool IsActive() const
	{
		return m_active;
	}

	void Destroy()
	{
		m_active = false;
	}

	size_t Id() const
	{
		return m_id;
	}

	const std::string& Tag() const
	{
		return m_tag;
	}

	template <typename T>
	T& Get()
	{
		return std::get<T>(m_components);
	}

	template <typename T>
	const T& Get() const
	{
		return std::get<T>(m_components);
	}

	template <typename T>
	void Remove()
	{
		Get<T>() = T();
	}

	template<typename T,typename... TArgs>
	T& Add(TArgs&&... mArgs)
	{
		auto& component = Get<T>();
		component = T(std::forward<TArgs>(mArgs)...);
		component.exists = true;
		return component;
	}

	template <typename T>
	bool Has() const
	{
		return Get<T>().exists;
	}
};