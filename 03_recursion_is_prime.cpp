#include <fmt/format.h>
#include <iostream>

//// Metaprogramming IsPrime implementation - Erwin Unruh 1994

/**
 * Ispituje dali je d djeljivo s d,d-1,d-2...
 */
template <unsigned n,unsigned d>
struct IsPrimeIml{
    static constexpr bool value = (n%d != 0) && IsPrimeIml<n,d-1>::value;
};

/**
 * recursion stop
 */
template <unsigned n>
struct IsPrimeIml<n,2>{
    static constexpr bool value = (n%2 != 0);
};


template <unsigned n>
struct IsPrime {
    static constexpr bool value = IsPrimeIml<n,n/2>::value;
};

template <>
struct IsPrime<3> {
    static constexpr bool value = true;
};

template <>
struct IsPrime<2> {
    static constexpr bool value = true;
};

template <>
struct IsPrime<1> {
    static constexpr bool value = false;
};

template <>
struct IsPrime<0> {
    static constexpr bool value = false;
};

static void test_is_prime(){

    fmt::print("8 is prime: {}\n",IsPrime<8>::value);
    fmt::print("7 is prime: {}\n",IsPrime<7>::value);
    fmt::print("6 is prime: {}\n",IsPrime<6>::value);
    fmt::print("5 is prime: {}\n",IsPrime<5>::value);
}


// C++11 constexpr

constexpr bool isPrimeImpl(unsigned n,unsigned d){
    return (d>2) ? ((n%d != 0) && isPrimeImpl(n,d-1)): (n%2 !=0);
}

constexpr bool isPrime11(unsigned n){
    return (n>3) ? isPrimeImpl(n,n/2): (n>1);
}


static void test_is_prime_cpp11(){

    fmt::print("8 is prime: {}\n",isPrime11(8));
    fmt::print("7 is prime: {}\n",isPrime11(7));
    fmt::print("6 is prime: {}\n",isPrime11(6));
    fmt::print("5 is prime: {}\n",isPrime11(5));
}


// C++14 constexpr more options
constexpr bool isPrime14(unsigned n){
    for (unsigned d = 2; d <= n/2; ++d){
        if( n % d == 0)
            return false;
    }
    return n>1;
}

static void test_is_prime_cpp14(){

    fmt::print("8 is prime: {}\n",isPrime14(8));
    fmt::print("7 is prime: {}\n",isPrime14(7));
    fmt::print("6 is prime: {}\n",isPrime14(6));
    fmt::print("5 is prime: {}\n",isPrime14(5));
}


// Primjena

template <unsigned n,bool = isPrime14(n)>
struct X{
    void info(){fmt::print("Opći predložak.\n"); }
};

template <unsigned n>
struct X<n,true>{
    void info(){fmt::print("Predložak za n prost.\n"); }
};


static void test_application(){

    X<9> x9; x9.info();
    X<7> x7; x7.info();
}



int main(){
    //test_is_prime();
    //test_is_prime_cpp11();
    //test_is_prime_cpp14();
    test_application();

    return 0;
}