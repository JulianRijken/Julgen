#pragma once

#include <functional>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <optional>
#include <variant>

namespace jul
{
	class GameObject;

    typedef std::optional<std::variant<float,glm::vec2>> InputContext;

	class BaseCommand
	{
	public:

        virtual ~BaseCommand() = default;
        virtual void Execute(InputContext context = std::nullopt) = 0;
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
        void Execute(InputContext context) override;

    private:
        float m_MoveSpeed;
        glm::vec3 m_MoveDirection;
	};

    class UnitMoveCommand : public GameObjectCommand
    {
    public:
        UnitMoveCommand(GameObject* gameObject, const glm::vec3& moveDirection);
        void Execute(InputContext context) override;

    private:
        glm::vec3 m_MoveDirection;
    };

    class StickTestCommand : public BaseCommand
    {
    public:
        void Execute(InputContext context) override;
    };


    class TriggerTestCommand : public BaseCommand
    {
    public:
        void Execute(InputContext context) override;
    };

    class FunctionCommand : public BaseCommand
    {
        typedef std::function<void(InputContext)> InputFunction;

    public:
        template<typename ObjectType>
        FunctionCommand(ObjectType* object,void (ObjectType::*memberFunction)(InputContext))
        {
           m_Function = [=](InputContext context) {
                (object->*memberFunction)(context);
            };
        }
        void Execute(InputContext context) override;

    private:
        InputFunction m_Function;
    };
}
