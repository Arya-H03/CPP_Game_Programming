#pragma once
#include <functional>
#include <vector>
#include <memory>
#include <utility>

template<typename T>
class MyEvent;

template<typename R, typename ... Args>
class MyEvent<R(Args... args)>
{
private:

	struct BaseListener
	{
		virtual ~BaseListener() = default;
		virtual R Invoke(Args... args) = 0;
		virtual bool Matches(void* obj) const = 0;
	};

	template<typename TObj, R(TObj::*TFunc)(Args...)>
	struct Listener: BaseListener
	{
		TObj* objPtr;

		Listener(TObj* obj) : objPtr(obj){}

		R Invoke(Args... args) override
		{
			return (objPtr->*TFunc)(std::forward<Args>(args)...);
		}

		bool Matches(void* obj)const override
		{
			return objPtr == obj;
		}
	};

	std::vector<std::unique_ptr<BaseListener>> listeners; 

public:

	template<typename TObj, R(TObj::*TFunc)(Args...)>
	void Subscribe(TObj* obj)
	{
		listeners.push_back(std::make_unique<Listener<TObj, TFunc>>(obj));
	}

	template<typename TObj, R(TObj::*TFunc)(Args...)>
	void UnSubscribe(TObj* obj)
	{
		bool matchFound = false;
		int matchIndex = -1;

		for (int i = 0; i < listeners.size(); ++i)
		{
			if (listeners[i]->Matches(obj) && dynamic_cast<Listener<TObj, TFunc>*>(listeners[i].get()))
			{
				matchFound = true;
				matchIndex = i;
				break;
			}
		}

		if (!matchFound) return;

		listeners[matchIndex] = listeners.back();
		listeners.pop_back();
	}

	void Invoke(Args... args)
	{
		for (auto& listener : listeners)
		{
			listener->Invoke(std::forward<Args>(args)...);
		}
	}

	void Clear()
	{
		listeners.clear();
	}

};
