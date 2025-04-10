// Copyright (c) 2025 Michael Dello
//
// This software is provided under the MIT License.
// See LICENSE file for details.

#ifndef INCLUDE_ST_ENUM_OPS_H__
#define INCLUDE_ST_ENUM_OPS_H__
//------------------------------------------------------------------------------
//
// This header provides is a generalized solution for allowing strongly typed
// enums to seamlessly work as operands for various operations.
//
// Notable usage features and characteristics:
//
//     1. Operands must be enum class types
//     2. While the operands can be different enum class types, their underlying
//        types must be the same
//     3. Operations are carried out on the underlying type
//     4. Operations return a value of the underlying type to avoid accidental 
//        narrowing or ambiguity
//     5. A header-only implementation to avoid required explicit instantiation
//        for different types
//     6. This implementation acknowledges the trade-off between rigidly
//        adhering to enum domain separateion and flexibility/convenience
//     7. Underlying type safety is maintained as the highest priority in any
//        tradeoffs
//
//------------------------------------------------------------------------------

#include <type_traits>
#include <cstdint>

namespace StronglyTypedEnumOps
{
    //--------------------------------------------------------------------------
    // Compile-time Checks
    //--------------------------------------------------------------------------

    // Compile-time constraint to ensure both types are enum and share the same
    // underlying type
    template <typename E1, typename E2>
    concept CompatibleEnums = 
        std::is_enum_v<E1> && std::is_enum_v<E2> &&
        std::is_same_v<std::underlying_type_t<E1>, std::underlying_type_t<E2>>;
    
    // Underlying type accessor using a compile-time assertion
    // No exception thrown at run-time due to being correctly resolved at
    // compile time
    template <typename E>
    constexpr auto to_underlying(E e) noexcept
    {
        static_assert(std::is_enum_v<E>, "to_underlying requires enum type");
        return static_cast<std::underlying_type_t<E>>(e);
    }

    //--------------------------------------------------------------------------
    // Equality / Inequality
    //--------------------------------------------------------------------------

    template <typename E1, typename E2>
    requires CompatibleEnums<E1, E2>
    constexpr bool operator==(E1 lhs, E2 rhs) noexcept
    {
        return to_underlying(lhs) == to_underlying(rhs);
    }

    template <typename E1, typename E2>
    requires CompatibleEnums<E1, E2>
    constexpr bool operator!=(E1 lhs, E2 rhs) noexcept
    {
        return to_underlying(lhs) != to_underlying(rhs);
    }

    //--------------------------------------------------------------------------
    // Bitwise Operations
    //--------------------------------------------------------------------------

    template <typename E1, typename E2>
    requires CompatibleEnums<E1, E2>
    constexpr E1 operator|(E1 lhs, E2 rhs) noexcept
    {
        return static_cast<E1>(to_underlying(lhs) | to_underlying(rhs));
    }

    template <typename E1, typename E2>
    requires CompatibleEnums<E1, E2>
    constexpr E1 operator&(E1 lhs, E2 rhs) noexcept
    {
        return static_cast<E1>(to_underlying(lhs) & to_underlying(rhs));
    }

    //--------------------------------------------------------------------------
    // Arithmetic Operations
    //--------------------------------------------------------------------------

    template <typename E1, typename E2>
    requires CompatibleEnums<E1, E2>
    constexpr E1 operator%(E1 lhs, E2 rhs) noexcept
    {
        return static_cast<E1>(to_underlying(lhs) % to_underlying(rhs));
    }

    //--------------------------------------------------------------------------
    // Assignment Operations
    //--------------------------------------------------------------------------

    template <typename E1, typename E2>
    requires CompatibleEnums<E1, E2>
    constexpr E1& operator&=(E1& lhs, E2 rhs) noexcept
    {
        using U = std::underlying_type_t<E1>;
        lhs = static_cast<E1>(static_cast<U>(lhs) & static_cast<U>(rhs));
        return lhs;
    }

    template <typename E1, typename E2>
    requires CompatibleEnums<E1, E2>
    constexpr E1& operator|=(E1& lhs, E2 rhs) noexcept
    {
        using U = std::underlying_type_t<E1>;
        lhs = static_cast<E1>(static_cast<U>(lhs) | static_cast<U>(rhs));
        return lhs;
    }

    //--------------------------------------------------------------------------
    // Conversion
    //--------------------------------------------------------------------------

    template <typename From, typename To>
    constexpr To enum_cast(From from)
    {
        static_assert(std::is_enum_v<From> && std::is_enum_v<To>);
        static_assert(
          std::is_same_v<std::underlying_type_t<From>,
                         std::underlying_type_t<To>>);
        return static_cast<To>(static_cast<std::underlying_type_t<From>>(from));
    }

} // namespace StronglyTypedEnumOps

#endif // INCLUDE_ST_ENUM_OPS_H__