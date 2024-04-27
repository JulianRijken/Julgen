#pragma once

namespace jul
{
    class Service
    {
    public:
        Service() = default;
        virtual ~Service() = default;

        Service(const Service&) = delete;
        Service(Service&&) noexcept = delete;
        Service& operator=(const Service&) = delete;
        Service& operator=(Service&&) noexcept = delete;
    };
}  // namespace jul
