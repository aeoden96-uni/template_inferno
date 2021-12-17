#include <iostream>
//#include <fmt/format.h>
#include <type_traits>

template <typename T>
struct TypeIdentity{ using type = T; };

//template <typename T>
//using TypeIdentity_t = typename TypeIdentity<T>::type;


template <auto v>
struct ValueIdentity{ static constexpr auto value = v;};

template <auto v>
const auto ValueIdentity_v = ValueIdentity<v>::value;

//// 2.predavanje //////////////////////////////////////////////

// std::integral_constant<T,v>
// std::bool_constant<v> = std::integral_constant<bool,v>
// std::true_type = std::bool_constant<true>
// std::false_type = std::bool_constant<false>


template <typename T>
struct RemoveConst : TypeIdentity<T> { };

template <typename T>
struct RemoveConst<T const> : TypeIdentity<T> { };

template <typename T>
using RemoveConst_t = typename RemoveConst<T>::type;


static void test_const(){
    static_assert( std::is_same_v<RemoveConst_t<const int>, int> );
    static_assert( std::is_same_v<RemoveConst_t<  int const volatile>, volatile int> );
    static_assert( std::is_same_v<RemoveConst_t<const int * const>, const int * > );

}


template <typename T>
struct RemoveVolatile : TypeIdentity<T> { };

template <typename T>
struct RemoveVolatile<T volatile> : TypeIdentity<T> { };

template <typename T>
using RemoveVolatile_t = typename RemoveVolatile<T>::type;

static void test_volatile(){
    static_assert( std::is_same_v<RemoveVolatile_t<  int volatile>, int> );
    static_assert( std::is_same_v<RemoveVolatile_t<  int const volatile>, int const> );

}


// std::conditional<b, T1, T2>
template <bool B, typename T1, typename T2>
struct Conditional{ using type = T1; };

template <typename T1, typename T2>
struct Conditional<false,T1,T2>{ using type = T2; };

template <bool B, typename T1, typename T2>
using Conditional_t = typename Conditional<B,T1,T2>::type;


static void test_conditional(){

    static_assert(std::is_same_v<Conditional_t<true,int,double>, int>);
    static_assert(std::is_same_v<std::conditional_t<true,int,double>, int>);

    static_assert(std::is_same_v<Conditional_t<false,int,double>, double>);
    static_assert(std::is_same_v<std::conditional_t<false,int,double>, double>);

}

template <typename T>
struct RemoveCV : RemoveConst<RemoveVolatile_t<T>> {};

template <typename T>
using RemoveCV_t  = typename RemoveCV<T>::type;


static void test_cv(){
    static_assert(std::is_same_v<RemoveCV_t<volatile int * const>,volatile int * >);
    static_assert(std::is_same_v<std::remove_cv_t<volatile int * const>,volatile int * >);
}

// std::is_same
template <typename T1, typename T2>
struct IsSame : std::false_type{};

template <typename T>
struct IsSame<T,T> : std::true_type{};

template <typename T1, typename T2>
inline constexpr bool IsSame_v = IsSame<T1,T2>::value;

static void test_isSame(){
    static_assert(std::is_same_v<double, double>);
    static_assert(IsSame_v<double, double>);
}


template <typename T1, typename T2>
struct IsRawSame : IsSame<RemoveCV_t<T1>, RemoveCV_t<T2>> {};

template <typename T1, typename T2>
inline constexpr bool IsRawSame_v = IsRawSame<T1,T2>::value;


static void test_isRawSame(){
    static_assert(IsRawSame_v  < const int volatile, int   >);
}

// std::is_floating_point

template <typename T>
struct IsFloatingPoint{
    constexpr static bool value = IsRawSame_v<T, float> ||
                                  IsRawSame_v<T, double> ||
                                  IsRawSame_v<T, long double>;
};

template <typename T>
inline constexpr bool IsFloatingPoint_v = IsFloatingPoint<T>::value;

static void test_floatingPoint(){
    static_assert(IsFloatingPoint_v<const double>);
    static_assert(IsFloatingPoint_v<volatile float>);
    static_assert(!IsFloatingPoint_v<volatile int>);
}


template <typename T>
struct Is_pointer{
    constexpr static bool value = false;
};

template <typename T>
struct Is_pointer<T*>{
    constexpr static bool value = true;
};

template <typename T>
inline constexpr bool Is_pointer_v = Is_pointer<T>::value;

static void test_isPointer(){
    static_assert(Is_pointer_v<const double> == false);
    static_assert(Is_pointer_v<double *> == true);
    static_assert(Is_pointer_v<void *> == true);

    static_assert(std::is_pointer_v<const double> == false);
    static_assert(std::is_pointer_v<double *> == true);
    static_assert(std::is_pointer_v<void *> == true);
}

template <typename T>
struct Is_union{
    constexpr static bool value = false;
};

template <typename T>
struct Is_union< T* >{
    constexpr static bool value = true;
};

template <typename T>
inline constexpr bool Is_union_v = Is_union<T>::value;

static void test_isUnion(){
    typedef union {
        int a;
        float b;
    } B;

    static_assert(Is_union_v<const double> == false);
    static_assert(Is_union_v<void> == false);

    static_assert(std::is_union_v<B> == true);
    static_assert(std::is_union_v<double> == false);

}

namespace Impl{
    std::true_type  IsNullPtrImpl(std::nullptr_t);
    std::false_type IsNullPtrImpl(...);
}

template <typename T>
using IsNullPtrT = decltype(Impl::IsNullPtrImpl(std::declval<T>()));

template <typename T>
inline constexpr bool IsNullPtrT_v = IsNullPtrT<T>::value;

static void test_nullptr(){

    static_assert(  IsNullPtrT_v<decltype(nullptr)> == true );
    static_assert(  IsNullPtrT_v<std::nullptr_t> == true );
    static_assert(  IsNullPtrT_v<int> == false );

    static_assert(  std::is_null_pointer_v<decltype(nullptr)> == true );
    static_assert(  std::is_null_pointer_v<std::nullptr_t> == true );
    static_assert(  std::is_null_pointer_v<int> == false );

}


namespace Impl{
    using YES = int;
    using NO = long;

    template <typename B, typename = decltype(B())>
    static YES test_def_ctr(B *) {}

    template <typename T>
    static NO test_def_ctr(...) {}
}

template <typename T>
struct IsDefaultConstructable{

    //D* p;
    //                 return_val = Impl::test_child<B,D>(p);

    inline static auto return_val = Impl::test_def_ctr<T>(nullptr);

    constexpr static bool value = std::is_same_v<decltype(return_val), Impl::YES>;
};

template <typename T>
inline constexpr bool IsDefaultConstructable_v = IsDefaultConstructable<T>::value;


static void test_IsDefaultConstructable(){
    struct X{
        int n_;
        explicit X(int n) : n_(n) {}
    };

    static_assert(IsDefaultConstructable_v<std::string> == true);
    static_assert(IsDefaultConstructable_v<X>  == false);

    static_assert(std::is_default_constructible_v<std::string> == true);
    static_assert(std::is_default_constructible_v<X> == false);
}


namespace Impl{
    template <typename T>
    using IS_NOT_UNION = std::bool_constant< !std::is_union_v<T> >;
    using IS_UNION  = std::false_type;

    template <typename T>
    IS_NOT_UNION<T> test_not_union(int T::*){}
    template <typename T>
    [[maybe_unused]] IS_UNION test_not_union(...){}
}

template <typename T>
using IsClass =  decltype(Impl::test_not_union<T>(nullptr));

template <typename T>
inline constexpr bool IsClass_v = IsClass<T>::value;


static void test_isClass(){
    class C{};

    static_assert(IsClass_v<float> == false);
    static_assert(IsClass_v<C> == true);

    static_assert(std::is_class_v<float> == false);
    static_assert(std::is_class_v<C> == true);

}


namespace Impl{

    template <typename B,typename D, typename = decltype((B)D())>
    static YES test_child(B *) {}

    template <typename B,typename D>
    static NO test_child(...) {}

}

template <typename B, typename D>
struct is_base_of
{
    static const bool value = std::is_same_v<decltype(Impl::test_child<B,D>(nullptr)),Impl::YES>;
};

template <typename B, typename D>
inline constexpr bool is_base_of_v = is_base_of<B,D>::value;


static void test_isChild(){
    class B{};
    class D : public B {};
    class C{};

    static_assert(is_base_of_v<B,C> == false);
    static_assert(is_base_of_v<B,D> == true);

    static_assert(std::is_base_of_v<B,C> == false);
    static_assert(std::is_base_of_v<B,D> == true);

}
int main() {
    test_const();
    test_volatile();
    test_conditional();
    test_cv();

    test_floatingPoint();
    test_isPointer();
    test_isSame();
    test_isRawSame();

    test_nullptr();
    test_IsDefaultConstructable();
    test_isUnion();
    test_isClass();
    test_isChild();
    return 0;
}
