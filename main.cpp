#include <iostream>
#include <ranges>
#include <iterator>
#include <vector>
#include <algorithm>
#include <cstring>
#include <string>
#include <type_traits>
#include <array>
#include <utility>
#include <iomanip>
#include <map>
#include <string_view>

//#define lazy_split_view split_view
//#define lazy_split split
//template for
auto print_view = [] (auto const& view) {
    for (std::cout<<"{ "; const auto element : view)
        std::cout<<element<<' ';
    std::cout<<" }";
};
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
    //std::ranges::drop_view default constructor. Value-initializes the underlying view and initializes the count to 0.
    constexpr std::array hi{'H','e','l','l','o',',',' ','C','+','+','2','3'};
    std::ranges::for_each(hi,[](const char c) {std::cout<< c;});
    std::cout<<'\n';
    constexpr auto n = std::distance(hi.cbegin(),std::ranges::find(hi,'C'));

    auto cxx = std::ranges::drop_view{hi,n};
    std::ranges::for_each(cxx,[] (const char c) {std::cout<<c;});
    std::cout<<'\n';
}
void std_range_view()
{
//    A "sentinel" in this context is a special value used to indicate the end of a sequence.
    auto const ints = {0,1,2,3,4,5};
    auto even  = [](int i) {return 0 == i % 2;};
    auto square = [] (int i) {return i * i;};
    //"pipe" syntax of composing the views.
    for (int i: ints | std::views::filter(even) | std::views::transform(square)) {
        std::cout<<i<<' ';
    }
    std::cout<<'\n';

    //a traditional "functional" composing syntax.
    for (int i: std::views::transform(std::views::filter(ints,even),square)) {
        std::cout<<i<<' ';
    }
}

void std_ranges_keys_view()
{
    //Takes a view of tuple-like values,
    // and produces a view with a value-type of the first element of the adapted view's value type.
    const std::vector<std::pair<std::string,double>> quark_mass {
        {"up", 2.3},{"down",4.8},
        {"charm",1275},{"strange",95},
        {"top",173'210},{"bottom",4'180},
    };
    std::cout<<"quark name:   | ";
    for (std::string const& name: std::views::keys(quark_mass))
        std::cout<<std::setw(9) <<name<<" | ";
    std::cout<<"\n mass MeV/c2: | ";
    for (const double mass: std::views::values(quark_mass))
        std::cout<<std::setw(9)<<mass<<" | ";
}
void std_ranges_values_view()
{
    std::map<char, int> map {{'A',1},{'B',2},{'C',3},{'D',4},{'E',5}};
    auto odd = [] (int x) { return 0 != (x & 1);};
    std::cout<<"Odd value in the map: ";
    for (int value :  map | std::views::values | std::views::filter(odd))
        std::cout<<value<<' ';
}
void std_ranges_elements_view()
{
    /*
     * Accepts a view of tuple-like values, and issues a view with
     * value-type of N-th element of the adapted view's value-type
     */
    const std::vector<std::tuple<int,char,std::string>> vt {
            {1,'A',"real"},
            {2,'B',"FAKE"},
            {3,'C',"CMAKE"},
            {4,'D',"packages"},
            {5,'E',"messages"},
    };
    for (int const e: std::views::elements<0>(vt)) {
        std::cout<<e<<' ';
    }
    std::cout<<'\n';
    for (char const e: vt | std::views::elements<1>) {
        std::cout<<e<<' ';
    }
    std::cout<<'\n';

    for (std::string const& e: std::views::elements<2>(vt)) {
        std::cout<<e<<' ';
    }
    std::cout<<'\n';
}
void std_ranges_split_view()
{
//    constexpr std::string_view words{"Hello-_-C++-_-!"};
//    constexpr std::string_view delim {"-_-"};
//    for (const auto word : std::views::split(words, delim)) {
//        std::cout<<
//            std::quoted(std::string_view(word.begin(),word.end())) <<' ';
//    }
}
std::string trimF(std::string_view const in) {
    auto view = in
            | std::views::drop_while(isspace)
            | std::views::reverse
            | std::views::drop_while(isspace)
            | std::views::reverse;
    return {view.begin(),view.end()};
}
void std_ranges_drop_while_view()
{
    const auto s = trimF(" \f\n\t\r\vHello, c++23!\f\n\t\r\v ");
    std::cout<<quoted(s)<<'\n';

    static constexpr auto v = {0,1,2,3,4,5};
    for (int n : v | std::views::drop_while([] (int i) {return i<3;}))
        std::cout<<n<<' ';
}

void std_ranges_views_counted()
{
    //A counted view presents a view of elements of the counted range[i,n) for some iterator i and non-negative integer n.
    const auto i1 = {1,2,3,4,5};
    for (auto i : std::views::counted(i1.begin()+1,4))
        std::cout<<i <<' ';
    std::cout<<'\n';
}
void std_ranges_join_view()
{
    /*std::ranges::join_view.
     * A Range adaptor that represents view consisting of the sequence obtained from flattening a view of range.
    */
    using namespace std::literals;
    const auto bits = {"reference/"sv,"en/"sv,"ranges/"sv,"join_view"sv,"./"sv,"html"sv};
    for (char const c: bits | std::views::join)
        std::cout<<c;
    std::cout<<'\n';
}

void std_lazy_split_view()
{
    //lazy_split_view takes a view and a delimiter, and a splits the view into subranges on delimiter.
    constexpr static auto source = {0,1,0,2,3,0, 4,5,6,0, 7,8,9 };
    constexpr int delimiter {0};
    constexpr std::ranges::split_view outer_view(source, delimiter);

    std::cout<<"Splits["<<std::ranges::distance(outer_view) <<"]: ";
    for (auto const& innner_view: outer_view)
        print_view(innner_view);
    constexpr std::string_view hello {"Hello C++ 23 !"};
    std::cout<<"\n substring: ";
    std::ranges::for_each(hello | std::views::split(' '), print_view);
}
void std_ranges_dangling()
{
    //todo reference/en/cpp/ranges/borrowed_range.html
    auto get_array_by_value = [] { return std::array{0,1,0,1}; };
    auto dangling_iter  = std::ranges::max_element(get_array_by_value());

    static_assert(std::is_same_v<std::ranges::dangling,
                                decltype(dangling_iter)>);
//    std::cout<<*dangling_iter;
    auto get_persistent_array = [] () -> const std::array<int, 4>& {
        static constexpr std::array a{0,1,0,1};
        return a;
    };
    auto valid_iter = std::ranges::max_element(get_persistent_array());
//    std::cout<<decltype(valid_iter);
    static_assert(
            !std::is_same_v<std::ranges::dangling, decltype(valid_iter)>
            );
    std::cout<<*valid_iter<<' ';

    auto get_string_view = [] { return std::string_view{"alpha"};};
    auto valid_iter2 = std::ranges::min_element(get_string_view());
    static_assert(
            !std::is_same_v<std::ranges::dangling, decltype(valid_iter2)>
            );
    std::cout<<'\''<<*valid_iter2<<'\''<<'\n';

}

const int& getRef(const int* p) { return *p;}
auto getRefFwdBad(const int* p) { return getRef(p);}
void std_decltype_specifier()
{
    //todo https://en.cppreference.com/w/cpp/language/decltype
    static_assert(
            std::is_same_v<decltype(getRef), const int&(const int*)>
                    );
    static_assert(
            std::is_same_v<decltype(getRefFwdBad),int(const int*)>,
                "Just returning auto isn't prefect forwarding."
            );
}


int main() {
    /* In computer programming, a sentinel value
     (also referred to as a flag value, trip value, rogue value, signal value, or dummy data)[1]
    is a special value in the context of an algorithm which uses its presence as a condition of termination,
    typically in a loop or recursive algorithm.
    */
     std_ranges_dangling();
//    std_decltype_specifier();
//    std_lazy_split_view();
//    std_ranges_join_view();
//    std_ranges_views_counted();
//    std_ranges_drop_while_view();
//    std_ranges_split_view();
//    std_ranges_elements_view();
//    std_ranges_values_view();
//    std_ranges_keys_view();
//    std_range_view();
    //    std_drop_view();
//    std_take_while_view();
//    std_ranges_refview();
//    std_ranges_rangeAccess();
//    std_ranges_adaptors();
    std::cout<<"OK\n";return 0;
}
