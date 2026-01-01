#pragma once
#include "EntityManager.h"
#include "imgui.h"
#include "imgui-sfml.h"
#include "MyEvent.hpp"
#include <functional>
#include <Components.hpp>

class InputSystem
{
private:

	sf::RenderWindow& window;
	EntityManager& entityManager;

	void HandleKeyPressed(CInput& cInput, const std::optional<sf::Event>& event);
	void HandleKeyReleased(CInput& cInput, const std::optional<sf::Event>& event);
	void HandleMouseBtnPressed(CInput& cInput, const std::optional<sf::Event>& event);
	void HandleMouseBtnReleased(CInput& cInput, const std::optional<sf::Event>& event);
	void HandleWindowClosing(CInput& cInput, const std::optional<sf::Event>& event);
public:

	MyEvent<void(const Vec2f&)> onLeftClick;
	MyEvent<void()> onWindowClose;

	InputSystem(sf::RenderWindow& window, EntityManager& entityManager): window(window),entityManager(entityManager){}

	void HandleInput();

};

