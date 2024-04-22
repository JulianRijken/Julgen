#pragma once

#include <any>
#include <memory>
#include <typeindex>

#include "unordered_map"

class Locator final
{
public:
    template<typename Service, typename... Args>
    static void Provide(Args... args)
    {
        g_Services.emplace(typeid(Service), std::make_shared<Service>(args...));
    }

    // template<typename Service>
    // static Service* Release()
    // {
    //     auto node = g_Services.extract(typeid(Service));
    //     return node.mapped().
    // }

    template<typename Service>
    static Service& Get()
    {
        auto service = g_Services.find(typeid(Service));

        return *std::any_cast<std::shared_ptr<Service>>(service->second);
    }

private:
    static inline std::unordered_map<std::type_index, std::any> g_Services{};
};

// TODO: Implement null service
// if(service == g_Services.end())
// {
//     // Return null
//     return;
// }
