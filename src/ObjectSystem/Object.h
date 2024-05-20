#pragma once
#include <string>


namespace jul
{
    class Object
    {
    public:
        virtual ~Object() = default;

        Object(const Object&) = delete;
        Object(Object&&) noexcept = delete;
        Object& operator=(const Object&) = delete;
        Object& operator=(Object&&) noexcept = delete;

        [[nodiscard]] const std::string& GetName() const { return m_Name; }

        [[nodiscard]] bool IsBeingDestroyed() const { return m_BeingDestroyed; }

        virtual void Destroy();

    protected:
        Object(std::string name = "Object");

    private:
        bool m_BeingDestroyed{false};
        std::string m_Name{};
    };
}  // namespace jul
