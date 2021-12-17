#include <iostream>
#include <fmt/format.h>
#include <type_traits>


template <typename T>
struct Size{
    static constexpr int value = sizeof(T);
};

template <typename T>
struct TypeIdentity{
    using type = T;
};

template <typename T>
using TypeIdentity_t = typename TypeIdentity<T>::type;


template <typename T, T v>
struct ValueIdentity_deprecated{
    static constexpr T value = v;
};

template <auto v>
struct ValueIdentity{
    static constexpr auto value = v;
};

template <auto v>
const auto ValueIdentity_v = ValueIdentity<v>::value;

static void test_basic(){
    static_assert(Size<int>::value == 4);
    static_assert(Size<long>::value == 8);

    using new_type =  TypeIdentity<int>::type;
    static_assert( std::is_same_v< new_type, int>);

    static_assert(ValueIdentity<3>::value == 3);
    static_assert(ValueIdentity_v<3> == 3);

}


template <typename X, typename Y>
constexpr auto fun(X x, Y y){
    return x+y;
}

template <auto x, auto y>
struct Fun{
    static constexpr auto value = x+y;
};

template <auto x, auto y>
inline constexpr auto Fun_v = Fun<x,y>::value;


static void test_functions() {
    static_assert(fun<int, int>(1, 2) == 3);
    static_assert(Fun_v<1, 2> == 3);
}

template <typename T>
struct IsVoid{
    static constexpr bool value = false;
};

template <>
struct IsVoid<void>{
    static constexpr bool value = true;
};

template <typename T>
inline constexpr auto IsVoid_v = IsVoid<T>::value;


template <typename T>
struct IsVoid1 : std::false_type {};

template <>
struct IsVoid1<void> : std::true_type {};

template <typename T>
inline constexpr auto IsVoid1_v = IsVoid1<T>::value;

static void test_void(){
    static_assert(std::is_void_v<int>   == false);
    static_assert(std::is_void_v<void*> == false);
    static_assert(std::is_void_v<void> == true);

    static_assert(IsVoid_v<void> == true);
    static_assert(IsVoid_v<void*> == false);

    static_assert(IsVoid1_v<void> == true);
    static_assert(IsVoid1_v<void*> == false);
}



int main() {
    test_basic();
    test_functions();
    test_void();

    return 0;
}
