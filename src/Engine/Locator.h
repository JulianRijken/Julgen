#pragma once

#include <memory>
#include <typeindex>

#include "Service.h"
#include "unordered_map"

namespace jul
{
    class Locator final
    {
        friend class Julgen;

    public:
        // When using an existing service
        template<typename ServiceType, typename ImplementationType>
        static void Provide(std::unique_ptr<ImplementationType> implementationUPtr)
        {
            g_Services.emplace(typeid(ServiceType), std::move(implementationUPtr));
        }

        // When creating a new service
        template<typename ServiceType, typename ImplementationType, typename... Args>
        static void Provide(Args&&... args)
        {
            g_Services.emplace(typeid(ServiceType), std::make_unique<ImplementationType>(std::forward<Args>(args)...));
        }

        // When creating a new service with no specific implementation
        template<typename ServiceType, typename... Args>
        static void Provide(Args&&... args)
        {
            Provide<ServiceType, ServiceType>(std::forward<Args>(args)...);
        }

        // TODO: Implement null service or error handeling
        template<typename ServiceType>
        static ServiceType& Get()
        {
            auto service{ g_Services.find(typeid(ServiceType)) };
            return *dynamic_cast<ServiceType*>(service->second.get());
        }

        template<typename ServiceType, typename ImplementationType>
        static std::unique_ptr<ImplementationType> Release()
        {
            auto node{ g_Services.extract(typeid(ServiceType)) };

            auto* releasedPtr = dynamic_cast<ImplementationType*>(node.mapped().release());

            return std::unique_ptr<ImplementationType>(releasedPtr);
        }

        template<typename ServiceType>
        static std::unique_ptr<ServiceType> Release()
        {
            return Release<ServiceType, ServiceType>();
        }

    private:
        static inline std::unordered_map<std::type_index, std::unique_ptr<Service>> g_Services{};
    };

}  // namespace jul
