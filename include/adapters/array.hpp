#ifndef DATA_ADAPTER_ARRAY_HPP_INCLUDED
#define DATA_ADAPTER_ARRAY_HPP_INCLUDED

#include "../data_adapter.hpp"

/**
 *              Notes on the implementation of this:
 *
 *      Firstly, with the constraints given by a static array, my goal was to
 * create an adapter that most closely resembles any STL container.
 *
 *      This means that, if possible, STL algorithms should be used in place of
 * custom implementations. For example, copy, copy_backwards and fill.
 *
 *      Secondly, it was my own principle that the internal members
 * (like used_length, data, and data_size) should never be accessed directly unless
 * absolutely needed to (like in resize, length, capacity, and at). Since many of
 * those are inlined, there is no performance loss, but it does make things more clear
 * and easy to change if need be.
 *
 *      Thirdly, due to the nature of static arrays, almost all insert and erase operations are
 * linear in time since data has to me moved around to create or fill space. That's not so bad, though.
 *
 *      Fourthly, exceptions are all standard library exceptions, usually std::out_of_range,
 * since it made sense to use those. If used correctly, this container should never throw them, though.
 *
 *      Lastly, iterators. The iterators here are full random access iterators. However, they are
 * 'boumd' to their parent containers and should the container be destroyed or resized smaller than the
 * current position the iterator points to, dereferencing the iterator will results in undefined behavior.
 *
 *      Other than that, the iterators are not invalidated when the container changes, since all they store
 * is an offset to the position in the container. Pretty simple stuff.
 *
 */

template <typename T, size_t N>
class DataAdapter<T[N]> : public DataAdapterBase<T[N], T, DataAdapter<T[N]> > {
    public:
        typedef DataAdapterBase<T[N], T, DataAdapter<T[N]> > _Base;

        typedef typename _Base::value_type              value_type;
        typedef typename _Base::element_type            element_type;
        typedef typename _Base::pointer_type            pointer_type;
        typedef typename _Base::size_type               size_type;

        typedef typename _Base::iterator                iterator;
        typedef typename _Base::const_iterator          const_iterator;
        typedef typename _Base::reverse_iterator        reverse_iterator;
        typedef typename _Base::const_reverse_iterator  const_reverse_iterator;

    private:
        value_type data;
        static size_type data_size;
        size_type used_length;

    public:
        DataAdapter() {
            this->clear();
        }

        DataAdapter(size_type n, const element_type& val = element_type( 0x0 )) {
            this->clear();
            this->insert( this->begin(), n, val );
        }

        explicit DataAdapter( const value_type & val ) {
            this->assign( val, val + N );
        }

        DataAdapter(const DataAdapter& a) {
            this->clear();
            this->resize( a.length() );
            std::copy( a.begin(), a.end(), this->begin() );
        }

        DataAdapter& operator=(const DataAdapter& a) {
            this->clear();
            this->resize( a.length() );
            std::copy( a.begin(), a.end(), this->begin() );

            return *this;
        }

        inline size_type capacity() const {
            return DataAdapter::data_size;
        }

        inline size_type length() const {
            return this->used_length;
        }

        void push_back( const element_type & n )    {
            //uses the previous length returned by resize to dictate the position of the last element
            this->at( this->resize( this->length() + 1 ) ) = n;
        }

        void push_front( const element_type & val )   {
            if( !this->full() ) {

                iterator first = this->begin();
                iterator last = this->end();

                this->resize( this->length() + 1 );

                std::copy_backward( first, last, this->end() );

                *first = val;

            } else {
                throw std::out_of_range( "DataAdapter::push_front: Out of Range" );
            }
        }

        element_type pop_back() {
            if( !this->empty() ) {
                element_type ret = this->back();

                this->resize( this->length() - 1 );

                return ret;

            } else {
                return element_type();
            }
        }

        element_type pop_front()    {
            element_type ret = this->front();

            std::copy( this->begin() + 1, this->end() - 1, this->begin() );

            this->resize( this->length() - 1 );

            return ret;
        }

        inline element_type & at( size_type n ) {
            return this->data[n];
        }

        inline const element_type at( size_type n ) const {
            return this->data[n];
        }

        inline element_type & at( iterator it ) {
            return this->at( it.off );
        }

        inline const element_type at( const_iterator it ) const {
            return this->at( it.off );
        }

        inline element_type front() const {
            return this->at( 0 );
        }

        inline element_type & front() {
            return this->at( 0 );
        }

        element_type back() const {
            if( !this->empty() ) {
                return this->at( this->end() - 1 );

            } else {
                return this->front();
            }
        }

        element_type & back() {
            if( !this->empty() ) {
                return this->at(this->length() - 1);

            } else {
                return this->front();
            }
        }

        iterator sorted_insert( const element_type & n ) {
            //First, add it to the end
            this->push_back( n );

            //Then insert it into the proper place
            for( size_type i = this->length() - 1; i > 0; --i ) {
                if( this->at( i ) < this->at( i - 1 ) ) {
                    std::swap( this->at( i ), this->at( i - 1 ) );

                } else {
                    return this->begin() + i;
                }
            }

            return this->begin();
        }

        //single element
        inline iterator insert( iterator pos, const element_type & val ) {
            return this->insert( pos, 1, val );
        }

        //fill
        iterator insert( iterator pos, size_type n, const element_type & val ) {
            if( n != 0 ) {
                if( pos < this->end() + 1 && this->length() + n <= this->capacity() ) {

                    iterator first = this->begin() + pos;
                    iterator last = this->end();

                    this->resize( this->length() + n );

                    std::copy_backward( first, last, this->end() );
                    std::fill( first, first + n, val );

                    return first;

                } else {
                    throw std::out_of_range( "DataAdapter::insert(fill): Out of Range" );
                }
            } else {
                return this->end();
            }
        }

        //range
        iterator insert( iterator pos, const_iterator first, const_iterator last ) {
            if( first != last && first < last ) {
                if( this->length() + ( last - first ) <= this->capacity() ) {

                    size_type diff = last - first;

                    iterator f = this->begin() + pos;
                    iterator l = this->end();

                    this->resize( this->length() + diff );

                    std::copy_backward( f, l, this->end() );
                    std::copy( first, last, f );

                    return f;

                } else {
                    throw std::out_of_range( "DataAdapter::insert(range): Out of Range" );
                }
            } else {
                return this->end();
            }
        }

        //Clear is unique in that is zeros the memory just in case
        void clear() {
            //Also, this doesn't use iterators as those rely on the used length
            std::fill( this->data, this->data + this->capacity(), element_type( 0x0 ) );
            this->used_length = 0;
        }

        inline size_type resize( size_type n ) {
            return this->resize( n, element_type( 0x0 ) );
        }

        size_type resize( size_type n, const element_type & v ) {
            if( n <= this->capacity() ) {

                size_type ret = this->length();

                if( n < this->length() ) {
                    this->used_length = n;

                } else {
                    size_type ol = this->used_length;

                    this->used_length = n;

                    std::fill( this->begin() + n, this->end(), v );
                }

                return ret;

            } else {
                throw std::out_of_range( "DataAdapter::resize: Out of Range" );
            }
        }

        inline iterator erase( iterator pos )  {
            return this->erase( pos, pos + 1 );
        }

        iterator erase( iterator first, iterator last ) {
            size_type diff = ( last - first );

            if( last <= this->end() && diff <= this->length() ) {

                iterator f = this->begin() + first;
                iterator l = this->begin() + last;

                std::copy( l, this->end(), f );

                this->resize( this->length() - diff );

                return f;

            } else {
                throw std::out_of_range( "DataAdapter::erase(range): Out of Range" );
            }
        }
};

template <typename T, size_t N>
typename DataAdapter<T[N]>::size_type DataAdapter<T[N]>::data_size =
    sizeof( DataAdapter<T[N]>::value_type ) / sizeof( DataAdapter<T[N]>::element_type );

/*Mutable iterator class template*/
template <typename T, size_t N>
class DataApapterIterator<T[N]>
        : public std::iterator<std::random_access_iterator_tag, T, size_t> {
    public:
        typedef std::iterator<std::random_access_iterator_tag, T> iterator_traits;

        typedef typename iterator_traits::iterator_category     iterator_category;
        typedef typename iterator_traits::value_type            value_type;
        typedef typename iterator_traits::difference_type       difference_type;
        typedef typename iterator_traits::pointer               pointer;
        typedef typename iterator_traits::reference             reference;

        typedef DataAdapter<T[N]> parent_type;

        friend class DataAdapter<T[N]>;

    private:
        //Keep a pointer to the parent
        parent_type * parent;
        difference_type off;

    public:
        DataApapterIterator() : parent( NULL ), off( 0 ) {}
        DataApapterIterator( parent_type * x, difference_type ioff = 0 ) : parent( x ), off( ioff ) {}
        DataApapterIterator( parent_type & x, difference_type ioff = 0 ) : parent( &x ), off( ioff ) {}
        DataApapterIterator( const DataApapterIterator & it ) : parent( it.parent ), off( it.off ) {}

        inline DataApapterIterator & operator=( const DataApapterIterator & it ) {
            this->parent = it.parent;
            this->off = it.off;
            return *this;
        }

        inline bool operator==( const DataApapterIterator & it ) const {
            return this->parent == it.parent && this->off == it.off;
        }

        inline bool operator!=( const DataApapterIterator & it ) const {
            return this->parent != it.parent || this->off != it.off;
        }

        inline reference operator*() {
            return this->parent->at( this->off );
        }

        inline reference operator[]( difference_type n ) {
            return *( *this + n );
        }

        inline pointer operator->() {
            return &( this->parent->at( this->off ) );
        }

        //VERY IMPORTANT for converting and comparing to const_iterators
        inline operator typename parent_type::const_iterator() const {
            return typename parent_type::const_iterator( this->parent ) + this->off;
        }

#define DATA_ADAPTER_ITERATOR_UNARY_OP(op)              \
    inline DataApapterIterator& operator op() {         \
        this->off op;                                   \
        return *this;                                   \
    }                                                   \
    inline DataApapterIterator operator op( int ) {     \
        DataApapterIterator tmp( *this );               \
        this->off op;                                   \
        return tmp;                                     \
    }

        DATA_ADAPTER_ITERATOR_UNARY_OP( ++ )
        DATA_ADAPTER_ITERATOR_UNARY_OP( -- )

        inline DataApapterIterator operator+( const DataApapterIterator & it ) const {
            DataApapterIterator tmp( *this );
            tmp.off += it.off;
            return tmp;
        }
        inline DataApapterIterator operator+( difference_type n ) const {
            DataApapterIterator tmp( *this );
            tmp.off += n;
            return tmp;
        }
        inline DataApapterIterator & operator +=( const DataApapterIterator & it ) {
            this->off += it.off;
            return *this;
        }
        inline DataApapterIterator & operator +=( difference_type n ) {
            this->off += n;
            return *this;
        }

        inline difference_type operator-( const DataApapterIterator & it ) const {
            return this->off - it.off;
        }
        inline DataApapterIterator operator-( difference_type n ) const {
            DataApapterIterator tmp( *this );
            tmp.off -= n;
            return tmp;
        }
        inline DataApapterIterator & operator -=( const DataApapterIterator & it ) {
            this->off -= it.off;
            return *this;
        }
        inline DataApapterIterator & operator -=( difference_type n ) {
            this->off -= n;
            return *this;
        }

#define DATA_ADAPTER_ITERATOR_COMP_OP(op)                               \
    inline bool operator op( const DataApapterIterator& it ) const {    \
        return this->off op it.off;                                     \
    }

        DATA_ADAPTER_ITERATOR_COMP_OP( < )
        DATA_ADAPTER_ITERATOR_COMP_OP( > )
        DATA_ADAPTER_ITERATOR_COMP_OP( <= )
        DATA_ADAPTER_ITERATOR_COMP_OP( >= )
};


/*Immutable iterator class template*/
template <typename T, size_t N>
class DataApapterIterator<const T[N]> : public std::iterator<std::random_access_iterator_tag, T, size_t> {
    public:
        typedef std::iterator<std::random_access_iterator_tag, T> iterator_traits;

        typedef typename iterator_traits::iterator_category     iterator_category;
        typedef typename iterator_traits::value_type            value_type;
        typedef typename iterator_traits::difference_type       difference_type;
        typedef typename iterator_traits::pointer               pointer;
        typedef typename iterator_traits::reference             reference;

        typedef DataAdapter<T[N]> parent_type;

        friend class DataAdapter<T[N]>;

    private:
        //Keep a pointer to the parent
        const parent_type * parent;
        difference_type off;

    public:
        DataApapterIterator() : parent( NULL ), off( 0 ) {}
        DataApapterIterator( const parent_type * x, difference_type ioff = 0 ) : parent( x ), off( ioff ) {}
        DataApapterIterator( const parent_type & x, difference_type ioff = 0 ) : parent( &x ), off( ioff ) {}
        DataApapterIterator( const DataApapterIterator & it ) : parent( it.parent ), off( it.off ) {}

        inline DataApapterIterator & operator=( const DataApapterIterator & it ) {
            this->parent = it.parent;
            this->off = it.off;
            return *this;
        }

        inline bool operator==( const DataApapterIterator & it ) const {
            return this->parent == it.parent && this->off == it.off;
        }

        inline bool operator!=( const DataApapterIterator & it ) const {
            return this->parent != it.parent || this->off != it.off;
        }

        inline value_type operator*() {
            return this->parent->at( this->off );
        }

        inline value_type operator[]( difference_type n ) {
            return *( *this + n );
        }

        DATA_ADAPTER_ITERATOR_UNARY_OP( ++ )
        DATA_ADAPTER_ITERATOR_UNARY_OP( -- )

        inline DataApapterIterator operator+( const DataApapterIterator & it ) const {
            DataApapterIterator tmp( *this );
            tmp.off += it.off;
            return tmp;
        }
        inline DataApapterIterator operator+( difference_type n ) const {
            DataApapterIterator tmp( *this );
            tmp.off += n;
            return tmp;
        }
        inline DataApapterIterator & operator +=( const DataApapterIterator & it ) {
            this->off += it.off;
            return *this;
        }
        inline DataApapterIterator & operator +=( difference_type n ) {
            this->off += n;
            return *this;
        }

        inline difference_type operator-( const DataApapterIterator & it ) const {
            return this->off - it.off;
        }
        inline DataApapterIterator operator-( difference_type n ) const {
            DataApapterIterator tmp( *this );
            tmp.off -= n;
            return tmp;
        }
        inline DataApapterIterator & operator -=( const DataApapterIterator & it ) {
            this->off -= it.off;
            return *this;
        }
        inline DataApapterIterator & operator -=( difference_type n ) {
            this->off -= n;
            return *this;
        }

        DATA_ADAPTER_ITERATOR_COMP_OP( < )
        DATA_ADAPTER_ITERATOR_COMP_OP( > )
        DATA_ADAPTER_ITERATOR_COMP_OP( <= )
        DATA_ADAPTER_ITERATOR_COMP_OP( >= )
};

#undef DATA_ADAPTER_ITERATOR_UNARY_OP
#undef DATA_ADAPTER_ITERATOR_BINARY_OP
#undef DATA_ADAPTER_ITERATOR_COMP_OP

#endif // DATA_ADAPTER_ARRAY_HPP_INCLUDED

