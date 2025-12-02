#pragma once
#include"EntityManager.hpp"
#include "imgui.h"
#include "imgui-sfml.h"
#include "MyEvent.hpp"
#include <functional>

class InputSystem
{
private:

	sf::RenderWindow& window;
	EntityManager& entityManager;
	size_t playerID;

	void HandleKeyPressed(CInput& cInput, const std::optional<sf::Event>& event);
	void HandleKeyReleased(CInput& cInput, const std::optional<sf::Event>& event);
	void HandleMouseBtnPressed(CInput& cInput, const std::optional<sf::Event>& event);
	void HandleMouseBtnReleased(CInput& cInput, const std::optional<sf::Event>& event);
	void HandleWindowClosing(CInput& cInput, const std::optional<sf::Event>& event);
public:

	MyEvent<void(const Vec2f&)> onLeftClick;
	MyEvent<void()> onWindowClose;

	InputSystem(sf::RenderWindow& window, EntityManager& entityManager, size_t playerID): window(window), playerID(playerID),entityManager(entityManager){}

	void HandleInput();
	void ResetPlayer(Entity* player);

};

