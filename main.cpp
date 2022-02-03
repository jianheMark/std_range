#include <iostream>
#include <ranges>
#include <iterator>
#include <vector>
#include <algorithm>

//template to print out container elements wrapped it with "{" and "}".
template<typename Os, typename V>
Os& operator<< (Os& os, V const& v) {
    os<<"{ ";
    for ( auto const& e: v ) os << e <<' ';
    return os<< "}";
}

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
    std::cout<<"print out int_array"<<int_array;
    std::cout<<"Array backwards: ";
    std::ranges::copy(std::ranges::rbegin(int_array),
                      std::ranges::rend(int_array),
                      std::ostream_iterator<int>(std::cout, " "));
    std::cout<<"\nVector backwards: ";
    std::ranges::copy(std::ranges::rbegin(v),std::ranges::rend(v),
                      std::ostream_iterator<int>(std::cout, " "));
}

int main() {

    std_ranges_rangeAccess();
    return 0;
}
