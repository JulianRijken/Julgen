#pragma once
#include <unordered_set>

#include "Component.h"

namespace jul
{
	class Collider;

	class Behaviour : public Component
	{
		friend class GameObject;

	public:

		Behaviour(const std::string& name = "Behaviour");
		~Behaviour() override;

		Behaviour(Behaviour&&) = delete;
		Behaviour(const Behaviour&) = delete;
		Behaviour& operator=(Behaviour&&) = delete;
		Behaviour& operator=(const Behaviour&) = delete;

		[[nodiscard]] bool IsActiveAndEnabled() const;

		void SetEnabled(bool enabled);

		static void UpdateAll();
		static void LateUpdateAll();

	private:

		virtual void Update() {}
		virtual void LateUpdate() {}

		virtual void OnEnable() {}
		virtual void OnDisable() {}

		virtual void Awake() {}

		bool m_Enabled;

		inline static std::unordered_set<Behaviour*> m_BehaviourPtrs{};

	};

}
