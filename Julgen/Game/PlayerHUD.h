#pragma once
#include <TextRenderer.h>

#include "Component.h"
#include "Player.h"

namespace bb
{
class PlayerHUD : public jul::Component
    {
    public:
        PlayerHUD(jul::GameObject* parentPtr, Player* player, TextRenderer* scoreText, TextRenderer* livesText, const SDL_Color& color);

    private:
        void UpdateScore(int score);
        void UpdateLives(int lives);

        TextRenderer* m_ScoreText;
        TextRenderer* m_LivesText;
    };
}
