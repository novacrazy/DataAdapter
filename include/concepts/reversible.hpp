#ifndef DATA_ADAPTER_REVERSIBLE_CONCEPT_HPP_INCLUDED
#define DATA_ADAPTER_REVERSIBLE_CONCEPT_HPP_INCLUDED

#include "container.hpp"

template <typename _Base>
class DataAdapter_Reversible_Concept : public DataAdapter_Container_Concept<_Base>
{
    public:
        typedef typename _Base::reverse_iterator            reverse_iterator;
        typedef typename _Base::const_reverse_iterator      const_reverse_iterator;

        virtual reverse_iterator rbegin() = 0;
        virtual reverse_iterator rend() = 0;
        virtual const_reverse_iterator crbegin() const = 0;
        virtual const_reverse_iterator crend() const = 0;
};

#endif // DATA_ADAPTER_REVERSIBLE_CONCEPT_HPP_INCLUDED
