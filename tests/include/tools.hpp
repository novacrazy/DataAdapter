#ifndef DATASTORE_TESTS_TOOLS_HPP_INCLUDED
#define DATASTORE_TESTS_TOOLS_HPP_INCLUDED

namespace DataAdapter_Tests {

    template<class ForwardIt>
    ForwardIt is_sorted_until( ForwardIt first, ForwardIt last ) {
        if ( first != last ) {
            ForwardIt next = first;

            while ( ++next != last ) {
                if ( *next < *first ) {
                    return next;
                }

                first = next;
            }
        }

        return last;
    }

    template<class ForwardIt>
    bool is_sorted( ForwardIt first, ForwardIt last ) {
        return is_sorted_until( first, last ) == last;
    }

    struct true_type {};
    struct false_type {};

    template<class T>
    struct is_array {
        typedef false_type type;
    };

    template<class T>
    struct is_array<T[]> {
        typedef true_type type;
    };

    template<class T, std::size_t N>
    struct is_array<T[N]> {
        typedef true_type type;
    };

#define ASSERT_EQUAL_RANGE(type, f1, l1, f2)    \
    for( type __f1 = f1, __l1 = l1, __f2 = f2;  \
            __f1 != __l1; ++__f1, ++__f2 )          \
    {ASSERT_EQ(*__f1, *__f2);}

}

#endif // DATASTORE_TESTS_TOOLS_HPP_INCLUDED
