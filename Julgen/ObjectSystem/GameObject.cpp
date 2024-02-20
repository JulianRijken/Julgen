#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

jul::GameObject::~GameObject() = default;

void jul::GameObject::Update(){}

void jul::GameObject::Render() const
{
	const auto& pos = m_transform.GetPosition();
	Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
}

void jul::GameObject::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void jul::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}
