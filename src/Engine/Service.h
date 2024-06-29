#ifndef SERVICE_H
#define SERVICE_H

namespace jul
{
    class Service
    {
    public:
        explicit Service() = default;
        virtual ~Service() = default;

        Service(const Service&) = delete;
        Service(Service&&) noexcept = delete;
        Service& operator=(const Service&) = delete;
        Service& operator=(Service&&) noexcept = delete;
    };
}  // namespace jul
#endif // SERVICE_H