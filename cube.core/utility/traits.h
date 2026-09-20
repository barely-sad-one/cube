#pragma once

namespace cube::utility
{

struct TrivialComponent { float x, y, z; };
struct NonTrivialComponent
{
  char* string_ptr;
  ~NonTrivialComponent() {}
};

template <typename T, T v>
struct IntegralConstant
{
  using value_type = T;
  using type = IntegralConstant;

  static constexpr T value = v;
  constexpr operator value_type() const { return value; }
  constexpr value_type operator()() const { return value; }
};

template <bool v>
using BoolConstant = IntegralConstant<bool, v>;

using true_type  = BoolConstant<true>;
using false_type = BoolConstant<false>;

template <typename T, typename U>
struct IsSameHelper : false_type {};

template <typename T>
struct IsSameHelper<T, T> : true_type {};

template <bool Cond, typename T, typename F> struct Conditional              { using type = T; };
template <           typename T, typename F> struct Conditional<false, T, F> { using type = F; };
template <bool Cond, typename T, typename F>
using conditional_t = typename Conditional<Cond, T, F>::type;

template <typename T> struct RemoveConst          { using type = T; };
template <typename T> struct RemoveConst<const T> { using type = T; };
template <typename T>
using remove_const_t = typename RemoveConst<T>::type;

template <typename T> struct RemoveVolatile             { using type = T; };
template <typename T> struct RemoveVolatile<volatile T> { using type = T; };
template <typename T>
using remove_volatile_t = typename RemoveVolatile<T>::type;

template <typename T>
using remove_cv_t = remove_const_t<remove_volatile_t<T>>;

template <typename T, typename U>
inline constexpr bool is_same_v = IsSameHelper<T, U>::value;

template <typename T>
inline constexpr bool is_empty_v = __is_empty(T);

inline constexpr bool is_const_evaluated() { return  __builtin_is_constant_evaluated(); }

template <typename T>
inline constexpr bool is_enum_v = __is_enum(T);

template <typename T>
inline constexpr bool is_union_v = __is_union(T);

template <typename T> struct IsIntegralHelper                     : false_type {};
template <>           struct IsIntegralHelper<bool>               : true_type {};
template <>           struct IsIntegralHelper<char>               : true_type {};
template <>           struct IsIntegralHelper<signed char>        : true_type {};
template <>           struct IsIntegralHelper<unsigned char>      : true_type {};
template <>           struct IsIntegralHelper<wchar_t>            : true_type {};
template <>           struct IsIntegralHelper<char16_t>           : true_type {};
template <>           struct IsIntegralHelper<char32_t>           : true_type {};
template <>           struct IsIntegralHelper<short>              : true_type {};
template <>           struct IsIntegralHelper<unsigned short>     : true_type {};
template <>           struct IsIntegralHelper<int>                : true_type {};
template <>           struct IsIntegralHelper<unsigned int>       : true_type {};
template <>           struct IsIntegralHelper<long>               : true_type {};
template <>           struct IsIntegralHelper<unsigned long>      : true_type {};
template <>           struct IsIntegralHelper<long long>          : true_type {};
template <>           struct IsIntegralHelper<unsigned long long> : true_type {};
template <typename T>
inline constexpr bool is_integral_v = IsIntegralHelper<T>::value;

template <typename T> struct IsFloatingPointHelper              : false_type {};
template <>           struct IsFloatingPointHelper<float>       : true_type {};
template <>           struct IsFloatingPointHelper<double>      : true_type {};
template <>           struct IsFloatingPointHelper<long double> : true_type {};
template <typename T>
inline constexpr bool is_floating_point_v = IsFloatingPointHelper<T>::value;

template <typename T>
inline constexpr bool is_arithmetic_v = is_integral_v<T> || is_floating_point_v<T>;

template <typename T>
inline constexpr bool is_trivially_constructible_v = __is_trivially_constructible(T);

template <typename T>
inline constexpr bool is_trivially_destructible_v = __is_trivially_destructible(T);

template <typename T>
inline constexpr bool is_trivially_copyable_v = __is_trivially_copyable(T);

template <typename T>
concept trivial = is_trivially_copyable_v<remove_cv_t<T>> && is_trivially_destructible_v<remove_cv_t<T>>;

template <typename T>
concept non_trivial = !(is_trivially_copyable_v<remove_cv_t<T>> && is_trivially_destructible_v<remove_cv_t<T>>);

}
