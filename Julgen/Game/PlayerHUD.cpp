#include "PlayerHUD.h"

#include <fmt/format.h>

#include "GameObject.h"

bb::PlayerHUD::PlayerHUD(GameObject* parentPtr,Player* player, TextRenderer* scoreText, TextRenderer* livesText, const SDL_Color& color) :
      jul::Component(parentPtr, "ScoreDisplay"),
      m_ScoreText(scoreText),
      m_LivesText(livesText)
{
    assert(m_ScoreText);
    assert(m_LivesText);

    m_ScoreText->SetColor(color);
    m_LivesText->SetColor(color);

    player->GetOnScoreChange().AddListener(this,&PlayerHUD::UpdateScore);
    player->GetOnDeath().AddListener(this,&PlayerHUD::UpdateLives);


    UpdateScore(0);
    UpdateLives(3); // TODO lives hardcoded should ideally be changed
}

void bb::PlayerHUD::UpdateScore(int score)
{
    m_ScoreText->SetText(fmt::format("SCORE: {}",score));
}

void bb::PlayerHUD::UpdateLives(int lives)
{
    m_LivesText->SetText(fmt::format("LIVES: {}",lives));
}
