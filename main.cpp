#include <iostream>
#include <ranges>
#include <iterator>
#include <vector>
#include <algorithm>
#include <cstring>
#include <string>

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

int main() {

    std_ranges_rangeAccess();
    return 0;
}
