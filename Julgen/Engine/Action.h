#pragma once

#include <functional>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#define ACTION_BIND(classname, func) this, #func, &classname::func

namespace jul
{
template<typename... ActionArgs>
class Action
{
    typedef std::function<void(ActionArgs...)> FunctionType;
    typedef std::pair<std::optional<std::pair<void*, std::string>>, FunctionType> MaybeBoundType;
public:
    Action() = default;

    inline ~Action() // Placed inine to avoid extra template
    {
        m_ListenerFunctions.clear();
    }

    inline void AddListener(FunctionType listener)
    {
        m_ListenerFunctions.push_back({ std::nullopt, listener });
    }

    template<typename ThisType, typename FunctionType, typename PlaceholderTuple, size_t... Indexes>
    inline std::function<void(ActionArgs...)> bindWithPlaceholders(ThisType* pThis, FunctionType listener, std::index_sequence<Indexes...>) {
        return std::bind(listener, pThis, std::get<Indexes>(PlaceholderTuple{})...);
    }

    template<typename ThisType>
    inline void AddListener(ThisType* pThis, std::string&& name, void(ThisType::* memberFunc) (ActionArgs...))
    {
        constexpr size_t NumArgs = sizeof...(ActionArgs);
        using PlaceholderTuple = decltype(std::make_tuple(std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)); // TODO: fill more of these
        using PlaceholderIndexes = std::make_index_sequence<NumArgs>;

        m_ListenerFunctions.push_back(std::make_pair(
            std::make_pair(reinterpret_cast<void*>(pThis), name),
            bindWithPlaceholders<ThisType, decltype(memberFunc), PlaceholderTuple>(pThis, memberFunc, PlaceholderIndexes{})
        ));
    }

    // static function
    inline void RemoveListener(FunctionType listener)
    {
        std::erase_if(m_ListenerFunctions, [&](auto&& element)
        {
            if(element.first.has_value()) // is a member function
                return false;

            return GetAddress(element.second) == GetAddress(listener);
        });
    }

    // member function
    template<typename ThisType>
    inline void RemoveListener(ThisType* pThis, std::string&& name, void(ThisType::*)(ActionArgs...))
    {
        std::erase_if(m_ListenerFunctions, [&](auto&& element)
                      {
                          if(not element.first.has_value()) // is a static function
                              return false;

                          const auto& value{ element.first.value() };
                          return value.first == reinterpret_cast<void*>(pThis) and
                                 value.second == name;
                      });

    }

    inline void operator+=(const FunctionType& listener)
    {
        AddListener(listener);
    }

    inline void operator-=(const FunctionType& listener)
    {
        RemoveListener(listener);
    }

    template<typename... Args>
    inline void Invoke(Args&&... args)
    {
        for (auto& listenerFunction : m_ListenerFunctions)
        {
            listenerFunction.second(args...);
        }
    }


private:

    template<typename T, typename... U>
    size_t GetAddress(std::function<T(U...)> func)
    {
        typedef T(fnType)(U...);
        fnType** fnPointer = func.template target<fnType*>();

        if(fnPointer == nullptr)
            return 0; // uh oh

        return reinterpret_cast<size_t>(*fnPointer);
    }

    std::vector<MaybeBoundType> m_ListenerFunctions{};
};
}
