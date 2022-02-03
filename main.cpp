#include <iostream>
#include <ranges>
#include <iterator>
#include <vector>
#include <algorithm>
#include <cstring>
#include <string>
#include <type_traits>
#include <array>

//template to print out container elements wrapped it with "{" and "}".
template<typename Os, typename V>
Os& operator<< (Os& os, V const& v) {
    os<<"{ ";
    for ( auto const& e: v ) os << e <<' ';
    return os<< "}";
}
template <std::ranges::input_range R>
void print (char id, R&& r)
{
    if (std::ranges::empty(r)) {
        std::cout<<'\t'<<id<<") Empty\n";
        return;
    }
    std::cout<<'\t'<<id<<") elements: ";
    for (const auto& element: r) {
        std::cout<<' '<<element;
    }
    std::cout<<'\n';
}
struct Scanty :private std::vector<int>{
        using std::vector<int>::begin;
        using std::vector<int>::end;
        using std::vector<int>::push_back;
        //empty(),size() are hidden.
    };

void std_ranges_rangeAccess()
{
    //std::ranges::begin Returns an iterator to the first element of the argument.
    std::vector<int> v {3,1,4};
    auto vi = std::ranges::begin(v);
    std::cout<<*vi<<'\n';
    *vi = 42;
    std::cout<<*vi<<'\n';
    if(std::ranges::find(v,5) != std::ranges::end(v))
        std::cout<<"Found a 5 in vector v!\n";
    if(std::ranges::find(v,1) != std::ranges::end(v))
        std::cout<<"Found a 1 in vector v!\n";

    auto vi1 = std::ranges::cbegin(v);
    std::cout<<*vi1
            <<" cbegin, cend: "
              "Returns an iterator to the first/end of the const-qualified argument."<<'\n';
    //Returns a sentinel indicating the end of a const-qualified range.
    auto vi2 = std::ranges::cend(v);
    std::cout<<"using cend() get the last element "<<*(vi2-1)<<'\n';

    if(std::ranges::find(v,4) != std::ranges::cend(v))
        std::cout<<"Using ranges::cend() found 4 in the vector. \n";
    int int_array[] = {-5,10,25};
    auto vi_cr = std::ranges::crbegin(v);
    std::cout<<"reverse sequence with crbegin "<<*vi_cr<<'\n';
//    auto vi_crint = std::ranges::crbegin(a);
    std::cout<<"print out int_array"<<int_array<<'\n';
    std::cout<<"Array backwards: ";
    std::ranges::copy(std::ranges::rbegin(int_array),
                      std::ranges::rend(int_array),
                      std::ostream_iterator<int>(std::cout, " "));
    std::cout<<"Print out vector: "<<v<<'\n';
    std::cout<<"Vector backwards: ";
    std::ranges::copy(std::ranges::rbegin(v),std::ranges::rend(v),
                      std::ostream_iterator<int>(std::cout, " "));
    std::cout<<"print cout ranges::size(array) == "
            <<std::ranges::size(int_array)<<'\n';
    std::cout<<std::boolalpha<<"the return type of ranges::size(v) is_signed: "
                <<std::is_signed_v<decltype(std::ranges::size(int_array))><<'\n';
    //std::ranges::size Returns the size of a range converted to a signed type.
    auto sign_ranges_size = std::ranges::ssize(int_array);
    std::cout<<"Ranges::size(int_array) =  "<< sign_ranges_size<<'\n'
                <<"ranges::ssize is "
                <<(std::is_signed_v<decltype(sign_ranges_size)> ? "signed" : "unsigned")
                <<'\n';
    std::cout<<"Reverse int array as: ";
    for (--sign_ranges_size;sign_ranges_size>=0; --sign_ranges_size)
            std::cout<<int_array[sign_ranges_size]<<' ';
    std::cout<<"\n signed_ranges_size "<<sign_ranges_size<<'\n';
    std::cout<<"\n (1) ranges::empty use std::vector::empty: \n";
    print('a',v);
    v.clear();
    print('b', v);
    {
        std::cout<<"(2) ranges::empty uses ranges::size(initializer_list): \n";
        auto i1 = {7,8,9};
        print('a', i1);
        print('b', std::initializer_list<int>{});
    }
    {
        std::cout<<"(3) ranges::empty on a raw array uses ranges::size:\n";
        int array[] = {4,5,6};
        print('a',array);
    }
    std::cout<<"(3) calling ranges::empty on an object w/o empty()/size(): \n";
    Scanty y;
    print('a',y);
    y.push_back(42);
    print('b',y);
    std::cout<<'\n';
    //std::ranges::data Returns a Pointer to the first element of a contiguous range.
    std::string s{"Hello world!\n"};
//    A C-style string is simply an array of characters that uses a null terminator.
    char a[20];
    std::strcpy(a,std::ranges::data(s));
    std::cout<<a; //bug for print out array.

    std::cout<<"OK\n";
}
void std_ranges_adaptors()
{
    std::vector<int> v{0,1,2,3,4,5};
    for (int n: std::views::all(v) | std::views::take(2)) {
        std::cout<< n<<' ';
    }

    static_assert(
            std::is_same<decltype(std::views::single(42)),
                std::ranges::single_view<int>>{}
            );
    static_assert(
            std::is_same<
                    decltype(std::views::all(v)),
                    std::ranges::ref_view<std::vector<int,std::allocator<int>>>
                    >{}
            );
    int a[] {1,2,3,4};

    static_assert(
            std::is_same<
                    decltype(std::views::all(a)),
                    std::ranges::ref_view<int [4]>
                    >{}
            );
    //todo file:///C:/Users/JIAN%20HE/Downloads/html-book-20220201/reference/en/cpp/ranges/all_view.html
    // the last example.
}

void std_ranges_refview()
{
    //ref_view is a view of elements of some other range. It wraps a reference to that range.
    const std::string s("many world");
    const std::ranges::take_view tv(s,3);
    const std::ranges::ref_view rv(tv);
    std::cout
            <<std::boolalpha
            <<"Call empty(): "<<rv.empty()<<'\n'
            <<" call size(): "<<rv.size()<<'\n'
            <<"call begin(): "<< *rv.begin()<<'\n'
            <<"call end():"<<*(rv.end()-1)<<'\n'
            <<"call data(): " <<rv.data()<<'\n'
            <<"call base() : " <<rv.base().size()<<'\n'
            <<"range-for   : ";
    for (const auto c:rv) {std::cout<<c;}
    std::cout<<'\n';
}
void std_take_while_view(){
    for (int year: std::views::iota(2017)
        | std::views::take_while([] (int y) {return y <= 2022; } ))
        {
        std::cout<<year<<' ';
        }
    std::cout<<'\n';
    const char idea[] {"Today is yesterday's tomorrow!.."};
    /*
     * Range adaptor object. The expression view::take_while(e,f) is expression-equivalent
     * to take_while_view(e,f) any suitable subexpression e and f.
     * //todo understand this part.
     */
    for (char x: std::ranges::take_while_view(idea,[](char c) { return c != '.';}))
        {
        std::cout<<x;
        }
}
void std_drop_view()
{
    constexpr std::array hi{'H','e','l','l','o',',',' ','C','+','+','2','3'};
    std::ranges::for_each(hi,[](const char c) {std::cout<< c;});
    std::cout<<'\n';
    constexpr auto n = std::distance(hi.cbegin(),std::ranges::find(hi,'C'));

    auto cxx = std::ranges::drop_view{hi,n};
    std::ranges::for_each(cxx,[] (const char c) {std::cout<<c;});
    std::cout<<'\n';
}


int main() {
    std_drop_view();
//    std_take_while_view();
//    std_ranges_refview();
//    std_ranges_rangeAccess();
//    std_ranges_adaptors();


    return 0;
}
