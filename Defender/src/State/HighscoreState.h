#pragma once

#include <SFML/Graphics.hpp>

#include "../Utility/DisplayManager.h"
#include "../Utility/UserInterface/UserInterface.h"

class HighscoreState : public sf::Drawable
{
public:
    HighscoreState();

    virtual ~HighscoreState() override;

    bool tick(double deltatime);
    virtual void
        draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    static void clean();
    static void initialize();
    static void addScore(const char initials[4], int score);
    static bool isHighscore(int score);

private:
    static constexpr int HS_COUNT = 8;

    struct Score
    {
        // I made initials 4 long so we can add a null char and use it as a cstring
        char initials[4] = "   ";
        int  score       = 0;
    };

    static void loadHighscores();
    static void writeHighscores();

    static Score    today[HS_COUNT], allTime[HS_COUNT];
    static sf::Text goatoday, goatime, goatodayTitle, goatimeTitle, hallOfFame,
                    scoreTxt;
    static sf::RectangleShape goatodayUnder, goatimeUnder;
    static sf::Texture *      defenderTex;
    static sf::RectangleShape defender;
    static sf::RenderTexture *shifting;
    static sf::Sprite         shftDra;
};
