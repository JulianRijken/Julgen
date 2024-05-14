#pragma once

#include <functional>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "InputContext.h"

namespace jul
{
	class GameObject;

    class BaseCommand
	{
    public:
        BaseCommand(const BaseCommand&) = delete;
        BaseCommand(BaseCommand&&) noexcept = delete;
        BaseCommand& operator=(const BaseCommand&) = delete;
        BaseCommand& operator=(BaseCommand&&) noexcept = delete;

        virtual ~BaseCommand() = default;
        virtual void Execute(const InputContext& context) = 0;

    protected:
        BaseCommand() = default;
    };


    class GameObjectCommand  : public BaseCommand
    {
    protected:

        // We only use this class to inherit from
        GameObjectCommand(GameObject* gameObject);
        [[nodiscard]] GameObject* GetGameObject() const {return m_GameObject; }

    private:

        GameObject* m_GameObject;
    };


    class MoveCommand : public GameObjectCommand
	{
	public:
        MoveCommand(GameObject* gameObject, float moveSpeed, const glm::vec3& moveDirection);
        void Execute(const InputContext& context) override;

    private:
        float m_MoveSpeed;
        glm::vec3 m_MoveDirection;
	};

    class UnitMoveCommand : public GameObjectCommand
    {
    public:
        UnitMoveCommand(GameObject* gameObject, const glm::vec3& moveDirection);
        void Execute(const InputContext& context) override;

    private:
        glm::vec3 m_MoveDirection;
    };

    class MemberFunctionCommand final : public BaseCommand
    {
        using InputFunction = std::function<void(InputContext)>;

    public:
        template<typename ObjectType>
        MemberFunctionCommand(ObjectType* object, void (ObjectType::*memberFunction)(InputContext)) :
            m_Function(
                [object, memberFunction](InputContext context)
                {
                    if(memberFunction == nullptr)
                        return;

                    (object->*memberFunction)(context);
                })
        {}

        void Execute(const InputContext& context) override;

    private:
        InputFunction m_Function;
    };

    class MuteGameCommand final : public BaseCommand
    {
    public:
        void Execute(const InputContext& context) override;
    };
}
