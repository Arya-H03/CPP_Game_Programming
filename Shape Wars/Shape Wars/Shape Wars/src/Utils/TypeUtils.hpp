#pragma once

template<typename T>
struct TypeInfo
{
	static constexpr const char* Name = "Unknown";
	static constexpr size_t Size = sizeof(T);
};

#define REGISTER_TYPE(T) \
template<> struct TypeInfo<T> \
{ \
	static inline constexpr const char* Name = #T; \
	static inline constexpr size_t Size = sizeof(T); \
};

template<typename T>
constexpr const char* GetTypeName()
{
	using Clean = std::remove_cv_t<std::remove_pointer_t<std::remove_reference_t<T>>>;
	return TypeInfo<Clean>::Name;
}

template<typename T>
constexpr size_t GetTypeSize()
{
	using Clean = std::remove_cv_t<std::remove_pointer_t<std::remove_reference_t<T>>>;
	return TypeInfo<Clean>::Size;
}