#pragma once

#include <SFML/Graphics.hpp>

class AttractState : public sf::Drawable
{
	struct WilliamsNode
	{
		sf::Vector2<uint8_t> targetD;
		uint8_t size;

		// Bresenham's Line Algorithm
		bool draw(sf::Vector2<uint16_t>& start, sf::Vector2<uint16_t>& pos, sf::Image& image, double deltatime_)
		{
			static double deltatime = 0;
			constexpr double rate = 1. / 10.;

			// Calculate target position
			sf::Vector2<uint16_t> p2 = {
				static_cast<uint16_t>(start.x + targetD.x),
				static_cast<uint16_t>(start.y + targetD.y)
			};

			int16_t dx = abs(p2.x - start.x);
			int16_t dy = abs(p2.y - start.y);
			int8_t sx = (start.x < p2.x) ? 1 : -1;
			int8_t sy = (start.y < p2.y) ? 1 : -1;
			int err = dx - dy;

			// Update delta time
			deltatime += deltatime_;

			while (deltatime - rate > 0)
			{
				deltatime -= rate;

				// Draw to image
				/* Odd/Even ranges relative to pos
				 * Pos = (0,0)
				 * _________________________________
				 * SIZE |  RANGE  |     VALUES
				 * -----+---------+-----------------
				 * 1    | [0]     | [0]
				 * 2    | [-1, 0] | [-1,  0]
				 * 3    | [-1, 1] | [-1,  0,  1]
				 * 4    | [-2, 1] | [-2, -1,  0,  1]
				 */
				for (int8_t i = 0 - size / 2; i < size / 2 - (size % 2 == 0 ? 1 : 0); ++i)
					for (int8_t j = 0 - size / 2; j < size / 2 - (size % 2 == 0 ? 1 : 0); ++j)
						image.setPixel(pos.x + i, pos.y + j, sf::Color::White);

				// Check if line is complete
				if (pos.x == p2.x && pos.y == p2.y)
					return true;

				int e2 = 2 * err;
				if (e2 > -dy) { err -= dy; pos.x += sx; }
				if (e2 < dx) { err += dx; pos.y += sy; }
			}

			return false;
		}
	};
public:

	AttractState();

	~AttractState();

	bool tick(double deltatime);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		switch (stage)
		{
		case 3: // Copyright
		case 2: // Defender
		case 1: // Electronics Inc
		case 0: // Williams
			target.draw(willVert, willPos * 4, sf::Quads, states);
		}
	}

private:

	struct WillNode
	{
		float x, y;
		short delay;
		short flags; // 1 start, 2 end

		WillNode(float x = 0, float y = 0, short delay = 0, short flags = 0):
			x(x), y(y), delay(delay), flags(flags) {}
	};

	int willPos = 0;

	static WillNode willNodes[];
	sf::Vertex willVert[100 * 4];

	int stage = 0;
	
};

