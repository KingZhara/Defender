#include "../AttractState.h"

AttractState::WillNode AttractState::willNodes[]
{
	WillNode(10,  10, 10, 1),
	WillNode(30, 100, 10),
	WillNode(50,  10, 10),

	WillNode(100, 10, 10, 2),
};


AttractState::AttractState()
{
}

AttractState::~AttractState()
{
}

bool AttractState::tick(double deltatime)
{
	static WillNode lastWill;
	static int timer = 0;

	if (timer)
	{
		timer--;
		return false;
	}

	switch (stage)
	{
	case 0: // Williams
	{
		timer += willNodes[willPos].delay;

		// https://github.com/SFML/SFML/wiki/Source:-Line-segment-with-thickness

		float thickness = 5.f;

		sf::Vector2f point2(willNodes[willPos].x, willNodes[willPos].y);
		sf::Vector2f point1;
		if (willNodes[willPos].flags & 1)
			point1 = point2;
		else
			point1 = sf::Vector2f(lastWill.x, lastWill.y);

		sf::Vector2f dir = point2 - point1;
		sf::Vector2f unitDir = dir / std::sqrt(dir.x * dir.x + dir.y * dir.y);

		sf::Vector2f offset = (thickness / 2.f) * sf::Vector2f(-unitDir.y, unitDir.x);

		willVert[willPos * 4 + 0].position = point1 + offset;
		willVert[willPos * 4 + 1].position = point2 + offset;
		willVert[willPos * 4 + 2].position = point2 - offset;
		willVert[willPos * 4 + 3].position = point1 - offset;

		lastWill = willNodes[willPos];

		if (willNodes[willPos].flags & 2)
			stage++;

		willPos++;

		break;
	}
	}

	return false;
}