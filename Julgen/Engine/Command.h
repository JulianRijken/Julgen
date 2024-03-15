#pragma once

namespace jul
{
	class GameObject;

	class BaseCommand
	{
	public:
		virtual ~BaseCommand() = default;
		virtual void Execute() = 0;
	};


	class GameObjectCommand : public BaseCommand
	{
	public:
		GameObjectCommand(GameObject* gameObject);

	protected:
		GameObject* GetGameObject() const { return m_GameObject; }

	private:
		GameObject* m_GameObject;
	};

    // class Move : public BaseCommand
    // {
    // public:
    // 	GameObjectCommand(GameObject* gameObject);

    // protected:
    // 	GameObject* GetGameObject() const { return m_GameObject; }

    // private:
    // 	GameObject* m_GameObject;
    // };

}
