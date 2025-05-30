#pragma once

#include <SFML/System/Vector2.hpp>
#include <cstdint>


class GameObject
{
	public:

		GameObject() = default;
		GameObject(float x, float y) : m_position{ x,y } {}
		virtual ~GameObject() = default;

		const sf::Vector2f& getPosition() const { return m_position; }
		sf::Vector2f& getPosition() { return m_position; }
		void setPosition(const sf::Vector2f& newPosition) { m_position = newPosition; }

		virtual void update(uint32_t deltaMilliseconds) = 0;
		virtual bool isMarkedForRemoval() const { return m_markedForRemoval; }

		unsigned int getId() const { return m_id; }
    	void setId(unsigned int id) { m_id = id; }

	protected:

		sf::Vector2f m_position{ .0f, .0f };
		bool m_markedForRemoval{false};
		unsigned int m_id{0}; 
};