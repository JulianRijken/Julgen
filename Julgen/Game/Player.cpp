#include "Player.h"
#include "GameObject.h"

#include <GameTime.h>

bb::Player::Player(GameObject* parentPtr,
                   Animator* animator,
                   SpriteRenderer* spriteRenderer)
    : Component(parentPtr, "PlayerController"), m_AnimatorPtr(animator),m_SpriteRenderer(spriteRenderer)
{
    if(m_AnimatorPtr == nullptr)
        m_AnimatorPtr = parentPtr->GetComponent<Animator>();

    if(m_SpriteRenderer == nullptr)
        m_SpriteRenderer = parentPtr->GetComponent<SpriteRenderer>();

    assert(m_AnimatorPtr);
    assert(m_SpriteRenderer);

    m_AnimatorPtr->PlayAnimation("Idle",true);    
}

void bb::Player::Kill()
{
    if(m_IsDead)
        return;

    assert(m_AnimatorPtr);
    m_AnimatorPtr->PlayAnimation(m_DeathAnimationName);

    m_Lives--;

    m_OnDeathEvent.Invoke(m_Lives);

    if(m_Lives == 0)
        m_IsDead = true;
}

void bb::Player::AddScore()
{
    m_Score += 100;
    m_OnScoreChangeEvent.Invoke(m_Score);
}

void bb::Player::Move(float input)
{
    if(m_IsDead)
        return;

    Transform().Translate(glm::vec3{input * GameTime::GetDeltaTimeF() * 10.0f,0,0});

    if(input > 0)
        m_SpriteRenderer->m_FlipX = false;
    else
    if(input < 0)
        m_SpriteRenderer->m_FlipX = true;
}

void bb::Player::OnTestScoreInput(InputContext)
{
    AddScore();
}

void bb::Player::OnTestLivesInput(InputContext)
{
    Kill();
}


void bb::Player::OnMoveLeftInput(InputContext)
{
    Move(-1.0f);
}

void bb::Player::OnMoveRightInput(InputContext)
{
    Move(1.0f);
}

void bb::Player::OnMoveStickInput(InputContext context)
{
    Move(std::get<float>(context.value()));
}

void bb::Player::Update()
{
    if(m_IsDead)
        return;

    if(not m_AnimatorPtr->IsPlaying())
    {

        m_AnimatorPtr->PlayAnimation(m_IdleAnimationName);
    }
}

