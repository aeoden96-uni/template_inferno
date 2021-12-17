#include <fmt/format.h>
#include <iostream>
#include <type_traits>

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

//SFINAE

template <typename T, std::size_t N>
std::size_t broj_elemenata(T (&arg)[N]){
    std::puts(__PRETTY_FUNCTION__ );
    return N;
}

template <typename T>
typename T::size_type broj_elemenata(T const & arg){
    std::puts(__PRETTY_FUNCTION__ );
    return arg.size();
}

int broj_elemenata(...){
    return 0;
}


static void test_size(){

    int v[3] = {1,2,3};
    fmt::print("Broj elemenata(v[3]) = {}\n",broj_elemenata(v));
    fmt::print("Broj elemenata('123') = {}\n", broj_elemenata("12345"));
    std::vector vec{1,2,3,4};
    fmt::print("Broj elemenata(std::vec) = {}\n", broj_elemenata(vec));
}



//Kompilacijski if

/**
 * Uvjet se provjerava za vrijeme compilacije.
 * @tparam T
 * @param arg
 * @return
 */
//template <typename T>
//std::string asString(T const & arg){
//    if constexpr(std::is_same_v<T,std::string>)
//        return arg;
//    else if (std::is_arithmetic_v<T>)
//        return std::to_string(arg);
//    else
//        return std::string{arg};
//}
//
//static void test_asString(){
//
//    int x = 8;
//    fmt::print("x = {}\n",asString(x));
//    fmt::print("x = {}\n",asString("aaa"));
//}

template <typename T>
void foo(T t){
    if constexpr(std::is_integral_v<T>)
        fmt::print("Integralan tip.\n");
    else
        nedefinirana_fja(t);
}

static void test_if(){
    foo(4);
}

//tagiranje/oznacavanje
template <typename Iterator,typename Distance>
void advanceImpl(Iterator & it,Distance n,std::random_access_iterator_tag){
    it += n;
}

template <typename Iterator,typename Distance>
void advanceImpl(Iterator & it,Distance n,std::bidirectional_iterator_tag){
    if(n>=0){
        while(n>0){
            ++it; --n;
        }
    }
    else{
        while(n<0){
            --it; ++n;
        }
    }
}

template <typename Iterator,typename Distance>
void advanceImpl(Iterator & it,Distance n,std::input_iterator_tag){
    while(n>0){
        ++it; --n;
    }
}

template <typename Iterator,typename Distance>
void advance(Iterator & it, Distance n){
    using Category = typename std::iterator_traits<Iterator>::iterator_category;
    advanceImpl(it,n,Category{});
}


template <typename Iterator,typename Distance>
void advance1(Iterator & it, Distance n){
    using Category = typename std::iterator_traits<Iterator>::iterator_category;
    if constexpr(std::is_convertible_v<Category, std::random_access_iterator_tag>){
        it += n;
    }
    else if constexpr(std::is_convertible_v<Category, std::bidirectional_iterator_tag>){
        if(n>=0){
            while(n>0){
                ++it; --n;
            }
        }
        else{
            while(n<0){
                --it; ++n;
            }
        }
    }
    else{
        while(n>0){
            ++it; --n;
        }
    }
}


int main(){
    std::vector<int> a{1,2,3,4};
    auto it = a.begin();
    ::advance1(it,2);

    fmt::print("{}\n",*it);



    //test_is_prime();
    //test_is_prime_cpp11();
    //test_is_prime_cpp14();
    //test_application();
    //test_size();
    //test_asString();
    test_if();
    return 0;
}