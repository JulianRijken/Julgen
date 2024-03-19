#pragma once

#include <functional>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#define BIND_MEMBER(thisPtr, func) thisPtr, #func, &func

namespace jul
{
    template<typename... ActionArgs>
    class Action final
    {
        typedef std::function<void(ActionArgs...)> FunctionType;
        typedef std::pair<std::optional<std::pair<void*, std::string>>, FunctionType> OptionalMemberFunctionType;

    public:
        Action() = default;

        ~Action() // Placed inline to avoid extra template
        {
            m_ListenerFunctions.clear();
        }

        Action(Action&&) = delete;
        Action(const Action&) = delete;
        Action& operator=(Action&&) = delete;
        Action& operator=(const Action&) = delete;

        // static function
        void AddListener(FunctionType listener)
        {
            m_ListenerFunctions.push_back({ std::nullopt, listener });
        }

        // member function
        // If you need more arguments than the 20 this allows,
        // be sure to check out C++ core guideline I.23: Keep the number of function arguments low
        template<typename ThisType>
        void AddListener(ThisType* pThis, std::string&& name, void(ThisType::* memberFunc) (ActionArgs...))
        {
            constexpr size_t NUM_ARGS = sizeof...(ActionArgs);
            using PlaceholderTuple = decltype(std::make_tuple(
                std::placeholders::_1 , std::placeholders::_2 , std::placeholders::_3 , std::placeholders::_4 ,
                std::placeholders::_5 , std::placeholders::_6 , std::placeholders::_7 , std::placeholders::_8 ,
                std::placeholders::_9 , std::placeholders::_10, std::placeholders::_11, std::placeholders::_12,
                std::placeholders::_13, std::placeholders::_14, std::placeholders::_15, std::placeholders::_16,
                std::placeholders::_17, std::placeholders::_18, std::placeholders::_19, std::placeholders::_20
            
            ));
            using PlaceholderIndexes = std::make_index_sequence<NUM_ARGS>;

            m_ListenerFunctions.push_back(std::make_pair(
                std::make_pair(reinterpret_cast<void*>(pThis), name),
                BindWithPlaceholders<ThisType, decltype(memberFunc), PlaceholderTuple>(pThis, memberFunc, PlaceholderIndexes{})
            ));
        }

        // static function
        void RemoveListener(FunctionType listener)
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
        void RemoveListener(ThisType* pThis, std::string&& name, void(ThisType::*)(ActionArgs...))
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

        template<typename... Args>
        void Invoke(Args&&... args)
        {
            for (auto& listenerFunction : m_ListenerFunctions)
                listenerFunction.second(args...);
        }

    private:

        template<typename ThisType, typename FunctionType, typename PlaceholderTuple, size_t... Indexes>
        std::function<void(ActionArgs...)> BindWithPlaceholders(
            ThisType* pThis, FunctionType listener, std::index_sequence<Indexes...>)
        {
            return std::bind(listener, pThis, std::get<Indexes>(PlaceholderTuple{})...);
        }

        template<typename T, typename... U>
        size_t GetAddress(std::function<T(U...)> func)
        {
            typedef T(fnType)(U...);
            fnType** fnPointer = func.template target<fnType*>();

            if(fnPointer == nullptr)
                return 0; // uh oh

            return reinterpret_cast<size_t>(*fnPointer);
        }

        std::vector<OptionalMemberFunctionType> m_ListenerFunctions{};
    };
}
