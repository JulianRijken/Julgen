#pragma once

#include <glm/vec3.hpp>

namespace jul
{
	class GameObject;

	class BaseCommand
	{
	public:
		virtual ~BaseCommand() = default;
		virtual void Execute() = 0;

    protected:
        BaseCommand() = default;
    };


    class GameObjectCommand : public BaseCommand
    {
    public:
        void Execute() = 0;

    protected:
        // We only use this class to inharit from
        GameObjectCommand(GameObject* gameObject);
        GameObject* GetGameObject() {return m_GameObject; }

    private:

        GameObject* m_GameObject;
    };


    class MoveCommand : public GameObjectCommand
	{
	public:
        MoveCommand(GameObject* gameObject, float moveSpeed, glm::vec3 moveDirection);
        void Execute();

    private:
        float m_MoveSpeed;
        glm::vec3 m_MoveDirection;
	};

    class UnitMoveCommand : public GameObjectCommand
    {
    public:
        UnitMoveCommand(GameObject* gameObject, glm::vec3 moveDirection);
        void Execute();

    private:
        glm::vec3 m_MoveDirection;
    };
}
