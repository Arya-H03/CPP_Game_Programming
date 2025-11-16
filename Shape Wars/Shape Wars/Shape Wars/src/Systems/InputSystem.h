#pragma once
#include"Entity.hpp"
#include "imgui.h"
#include "imgui-sfml.h"
#include <functional>
class InputSystem
{
private:

	template<typename T>
	using Func = std::function<void(T)>;

	std::vector<Func<Vec2f>> onLeftClick;
	std::vector<Func<void>> onWindowClose;

	void HandleKeyPressed(CInput& cInput, const std::optional<sf::Event>& event);
	void HandleKeyReleased(CInput& cInput, const std::optional<sf::Event>& event);
	void HandleMouseBtnPressed(CInput& cInput, const std::optional<sf::Event>& event);
	void HandleMouseBtnReleased(CInput& cInput, const std::optional<sf::Event>& event);
	void HandleWindowClosing(CInput& cInput, const std::optional<sf::Event>& event);
public:

	InputSystem() = default;

	void HandleInput(sf::RenderWindow& window,CInput& cInput);

	void AddEventToLeftClick(const Func<Vec2f>& func);
	void AddEventToWindowClose(const Func<void>& func);
};

