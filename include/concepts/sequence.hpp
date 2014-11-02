#ifndef DATA_ADAPTER_SEQUENCE_CONCEPT_HPP_INCLUDED
#define DATA_ADAPTER_SEQUENCE_CONCEPT_HPP_INCLUDED

#include "container.hpp"

template <typename _Base>
class DataAdapter_Sequence_Concept : public DataAdapter_Container_Concent<_Base>
{
    public:
        typedef typename _Base::element_type        value_type;
        typedef typename _Base::reference           reference;
        typedef typename _Base::const_reference     const_reference;
        typedef typename _Base::iterator            iterator;
        typedef typename _Base::const_iterator      const_iterator;
        typedef typename _Base::difference_type     difference_type;
        typedef typename _Base::size_type           size_type;
        typedef typename _Base::derived_type        derived_type;

        virtual iterator insert( iterator, const value_type & ) = 0;
        virtual iterator insert( iterator, size_type, const value_type & ) = 0;
        virtual iterator insert( iterator, const_iterator, const_iterator ) = 0;

        virtual void clear() = 0;

        virtual iterator erase( iterator ) = 0;
        virtual iterator erase( iterator, iterator ) = 0;

        virtual void assign( iterator, iterator ) = 0;
        virtual void assign( size_type, const value_type & ) = 0;
};

#endif // DATA_ADAPTER_SEQUENCE_CONCEPT_HPP_INCLUDED
