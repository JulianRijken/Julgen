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
        template<typename ServiceType, typename... Args>
        static void Provide(Args... args)
        {
            g_Services.emplace(typeid(ServiceType), std::make_unique<ServiceType>(args...));
        }

        template<typename ServiceType>
        static ServiceType& Get()
        {
            auto service = g_Services.find(typeid(ServiceType));

            Service* test = service->second.get();
            ServiceType* result = dynamic_cast<ServiceType*>(test);

            return *result;
        }

        // template<typename ServiceType>
        // static ServiceType* Release()
        // {
        //     auto node = g_Services.extract(typeid(ServiceType));
        //     return std::any_cast<Service<ServiceType>>(node.mapped()).Release();
        // }

    private:
        static inline std::unordered_map<std::type_index, std::unique_ptr<Service>> g_Services{};
    };

}  // namespace jul
