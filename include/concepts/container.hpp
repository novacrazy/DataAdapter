#ifndef DATA_ADAPTER_CONTAINER_CONCEPT_HPP_INCLUDED
#define DATA_ADAPTER_CONTAINER_CONCEPT_HPP_INCLUDED

#include <iterator>
#include <algorithm>
#include <stdexcept>

template <typename _Base>
class DataAdapter_Container_Concept
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

        virtual iterator begin() = 0;
        virtual iterator end() = 0;

        virtual const_iterator cbegin() const = 0;
        virtual const_iterator cend() const = 0;

        virtual void swap( derived_type & ) = 0;

        virtual size_type size() const = 0;
        virtual size_type max_size() const = 0;

        virtual bool empty() const = 0;
};

#endif // DATA_ADAPTER_CONTAINER_CONCEPT_HPP_INCLUDED
