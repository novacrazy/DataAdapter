#ifndef DATA_ADAPTER_ASSOCIATIVE_CONCEPT_HPP_INCLUDED
#define DATA_ADAPTER_ASSOCIATIVE_CONCEPT_HPP_INCLUDED

#include "container.hpp"

#include <map>

template <typename _Base, typename _Key, typename _Compare = std::less<_Key> >
class DataAdapter_Associative_Concept : public DataAdapter_Container_Concent<_Base>
{
    public:
        typedef typename _Base::element_type        value_type;
        typedef _Key                                key_type;
        typedef _Compare                            key_compare;
        typedef typename _Base::reference           reference;
        typedef typename _Base::const_reference     const_reference;
        typedef typename _Base::iterator            iterator;
        typedef typename _Base::const_iterator      const_iterator;
        typedef typename _Base::difference_type     difference_type;
        typedef typename _Base::size_type           size_type;
        typedef typename _Base::derived_type        derived_type;

        //Stolen from stl_map.h
        class value_compare : public std::binary_function<value_type, value_type, bool>
        {
                friend class DataAdapter_Associative_Concept<_Base, _Key, _Compare>;

            protected:
                _Compare comp;

                value_compare( _Compare __c )
                    : comp( __c ) { }

            public:
                bool operator()( const value_type &__x, const value_type &__y ) const
                {
                    return comp( __x.first, __y.first );
                }
        };

        virtual key_compare key_comp() const = 0;

        virtual value_compare value_comp() const
        {
            return value_comp( this->key_comp() );
        }
};

#endif // DATA_ADAPTER_ASSOCIATIVE_CONCEPT_HPP_INCLUDED
