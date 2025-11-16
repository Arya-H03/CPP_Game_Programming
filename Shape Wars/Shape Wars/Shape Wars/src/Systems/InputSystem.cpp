#include "InputSystem.h"

void InputSystem::HandleInput(sf::RenderWindow& window, CInput& cInput)
{
	while (const std::optional<sf::Event> event = window.pollEvent())
	{
		ImGui::SFML::ProcessEvent(window, *event);

		HandleWindowClosing(cInput, event);
		HandleKeyPressed(cInput, event);
		HandleKeyReleased(cInput, event);
		HandleMouseBtnPressed(cInput, event);
		HandleMouseBtnReleased(cInput, event);
	}
}

void InputSystem::HandleKeyPressed(CInput& cInput, const std::optional<sf::Event>& event)
{
	if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
	{
		switch (keyPressed->scancode)
		{
		case sf::Keyboard::Scancode::W:
			cInput.up = true;
			break;
		case sf::Keyboard::Scancode::S:
			cInput.down = true;
			break;
		case sf::Keyboard::Scancode::A:
			cInput.left = true;
			break;
		case sf::Keyboard::Scancode::D:
			cInput.right = true;
			break;
		default:
			break;
		}
	}
}

void InputSystem::HandleKeyReleased(CInput& cInput, const std::optional<sf::Event>& event)
{
	if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
	{
		switch (keyReleased->scancode)
		{
		case sf::Keyboard::Scancode::W:
			cInput.up = false;
			break;
		case sf::Keyboard::Scancode::S:
			cInput.down = false;
			break;
		case sf::Keyboard::Scancode::A:
			cInput.left = false;
			break;
		case sf::Keyboard::Scancode::D:
			cInput.right = false;
			break;
		default:
			break;
		}
	}
}

void InputSystem::HandleMouseBtnPressed(CInput& cInput, const std::optional<sf::Event>& event)
{
	if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
	{
		if (mousePressed->button == sf::Mouse::Button::Left)
		{
			cInput.shoot = true;
			for (Func<Vec2f>& func : onLeftClick)
			{
				func(Vec2f(mousePressed->position.x, mousePressed->position.y));
			}
		}

	}
}

void InputSystem::HandleMouseBtnReleased(CInput& cInput, const std::optional<sf::Event>& event)
{
	if (const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>())
	{
		if (mouseReleased->button == sf::Mouse::Button::Left)
		{
			cInput.shoot = false;
		}

	}
}

void InputSystem::HandleWindowClosing(CInput& cInput, const std::optional<sf::Event>& event)
{
	if (event->is<sf::Event::Closed>())
	{
		for (Func<void>& func : onWindowClose)
		{
			func();
		}
	}
}

void InputSystem::AddEventToLeftClick(const Func<Vec2f>& func)
{
	onLeftClick.push_back(func);
}

void InputSystem::AddEventToWindowClose(const Func<void>& func)
{
	onWindowClose.push_back(func);
}

