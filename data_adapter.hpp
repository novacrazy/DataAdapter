#ifndef DATA_ADAPTER_HPP_INCLUDED
#define DATA_ADAPTER_HPP_INCLUDED

//C Libraries
#include <cstdlib>
#include <cstring>

//C++ Libraries
#include <iterator>
#include <algorithm>
#include <stdexcept>

//This will house specialized iterator functionality for each specialization of DataAdapter
template <typename T>
class DataApapterIterator {};

#if _MSC_VER && !__INTEL_COMPILER && !defined(_SCL_SECURE_NO_WARNINGS)
/*
    This works with MSVC 13+S3, but I'm unsure before that.

    Although the equivalent could be accomplished by defining _SCL_SECURE_NO_WARNINGS,
    this prevents errors from springing up from the library when a user tries to compile
*/
namespace std {
    template<class T>
    struct _Is_checked_helper<DataApapterIterator<T> >
            : public true_type {
        // mark back_insert_iterator as checked in MSVC, even though it's not. Heh. Bloody compile errors.
    };
}
#endif // _MSC_VER

/*
    This is a base class that defines commonly used operations on data structures
    and provide a single interface for working with differing data structures.
*/

template <typename T, typename K, typename _Derived>
class DataAdapterBase {
    public:
        //Common typedefs that help in maintaining consistency
        typedef T                                           value_type;
        typedef K                                           element_type;
        typedef T                     *                     pointer_type;
        typedef size_t                                      size_type;
        typedef DataApapterIterator<value_type>             iterator;
        typedef DataApapterIterator<const value_type>       const_iterator;
        typedef std::reverse_iterator<iterator>             reverse_iterator;
        typedef std::reverse_iterator<const_iterator>       const_reverse_iterator;
        typedef _Derived                                    derived_type;

        template <typename _ForwardIterator>
        void assign( _ForwardIterator first, _ForwardIterator last ) {
            this->resize( last - first );
            std::copy( first, last, this->begin() );
        }

        virtual size_type length() const = 0;
        virtual size_type capacity() const = 0;

        virtual inline bool empty() const {
            return this->length() == 0;
        }

        virtual inline bool full() const {
            return this->length() == this->capacity();
        }

        virtual void push_back( const element_type & )    = 0;
        virtual void push_front( const element_type & )   = 0;
        virtual element_type pop_back()     = 0;
        virtual element_type pop_front()    = 0;

        virtual element_type & at( size_type ) = 0;
        virtual const element_type at( size_type ) const = 0;
        virtual element_type & at( iterator ) = 0;
        virtual const element_type at( const_iterator ) const = 0;

        inline element_type & operator[]( size_type n ) {
            return this->at( n );
        }

        inline const element_type operator[]( size_type n ) const {
            return this->at( n );
        }

        virtual iterator insert( iterator, const element_type & ) = 0;
        virtual iterator insert( iterator, size_type, const element_type & ) = 0;
        virtual iterator insert( iterator, const_iterator, const_iterator ) = 0;

        virtual iterator sorted_insert( const element_type & ) = 0;

        virtual void clear() = 0;

        virtual size_type resize( size_type ) = 0;
        virtual size_type resize( size_type, const element_type & ) = 0;

        virtual iterator erase( iterator ) = 0;
        virtual iterator erase( iterator, iterator ) = 0;

        virtual iterator begin() {
            return iterator( dynamic_cast<derived_type *>( this ) );
        }

        virtual iterator end() {
            iterator tmp( dynamic_cast<derived_type *>( this ) );
            std::advance( tmp, this->length() );
            return tmp;
        }

        virtual const_iterator cbegin() const {
            return const_iterator( dynamic_cast<const derived_type *>( this ) );
        }

        virtual const_iterator cend() const {
            const_iterator tmp( dynamic_cast<const derived_type *>( this ) );
            std::advance( tmp, this->length() );
            return tmp;
        }

        inline const_iterator begin() const {
            return this->cbegin();
        }

        inline const_iterator end() const {
            return this->cend();
        }

        inline reverse_iterator rbegin() {
            return reverse_iterator( this->end() );
        }

        inline reverse_iterator rend() {
            return reverse_iterator( this->begin() );
        }

        inline const_reverse_iterator crbegin() const {
            return const_reverse_iterator( this->cend() );
        }

        inline const_reverse_iterator crend() const {
            return const_reverse_iterator( this->cbegin() );
        }

        virtual element_type back() const   = 0;
        virtual element_type front() const  = 0;
        virtual element_type & back()  = 0;
        virtual element_type & front() = 0;

        //These are implementation defined, as alternatives exist for varying data structures
        virtual inline void sort() {
            std::sort( this->begin(), this->end() );
        }

        virtual inline void stable_sort() {
            std::stable_sort( this->begin(), this->end() );
        }

        virtual inline iterator find( const element_type & n ) {
            return std::find( this->begin(), this->end(), n );
        }

        virtual iterator find_sorted( const element_type & n ) {
            iterator it = std::lower_bound( this->begin(), this->end(), n );

            if( it != this->end() && *it == n ) {
                return it;

            } else {
                return this->end();
            }
        }

        virtual ~DataAdapterBase() {}
};

//Initial declaration, not specialized, does nothing.
//Will actually cause a compiler error if used because it's still an abstract class.
template <typename T>
class DataAdapter : public DataAdapterBase<T, T, DataAdapter<T> > {};

//Using iterators, this should be able to print out any implementation of DataAdapter
template <typename T>
std::ostream & operator<<( std::ostream & out, const DataAdapter<T> & d ) {
    for( typename DataAdapter<T>::const_iterator it = d.cbegin(); it != d.cend(); ++it ) {
        out << *it << ' ';
    }

    return out;
}

#endif // DATA_ADAPTER_HPP_INCLUDED

