#pragma once
#include"Entity.hpp"
#include "imgui.h"
#include "imgui-sfml.h"
#include "MyEvent.hpp"
#include <functional>

class InputSystem
{
private:

	template<typename T>
	using Func = std::function<void(T)>;

	

	void HandleKeyPressed(CInput& cInput, const std::optional<sf::Event>& event);
	void HandleKeyReleased(CInput& cInput, const std::optional<sf::Event>& event);
	void HandleMouseBtnPressed(CInput& cInput, const std::optional<sf::Event>& event);
	void HandleMouseBtnReleased(CInput& cInput, const std::optional<sf::Event>& event);
	void HandleWindowClosing(CInput& cInput, const std::optional<sf::Event>& event);
public:

	MyEvent<void(const Vec2f&)> onLeftClick;
	MyEvent<void()> onWindowClose;

	InputSystem() = default;

	void HandleInput(sf::RenderWindow& window,Entity* player);

};

