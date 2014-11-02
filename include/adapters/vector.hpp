#ifndef DATA_ADAPTER_VECTOR_HPP_INCLUDED
#define DATA_ADAPTER_VECTOR_HPP_INCLUDED

#include <data_adapter.hpp>

template <typename T>
class DataAdapter<T[]> : public DataAdapterBase<T[], T, DataAdapter<T[]> >
{
    private:
        const float phi = 1.618033988749895;

    public:
        typedef DataAdapterBase<T[], T, DataAdapter<T[]> > _Base;

        typedef typename _Base::value_type              value_type;
        typedef typename _Base::pointer_type            pointer_type;
        typedef typename _Base::size_type               size_type;

        typedef typename _Base::iterator                iterator;
        typedef typename _Base::const_iterator          const_iterator;
        typedef typename _Base::reverse_iterator        reverse_iterator;
        typedef typename _Base::const_reverse_iterator  const_reverse_iterator;

    private:
        value_type *data;
        size_type data_capacity, used_length;

    public:
        DataAdapter()
        {
            this->clear();
        }

        DataAdapter( size_type n, const value_type &val = value_type() )
        {
            this->clear();
            this->insert( this->begin(), n, val );
        }

        DataAdapter( const DataAdapter &a )
        {
            this->clear();
            this->resize( a.length() );
            std::copy( a.begin(), a.end(), this->begin() );
        }

        DataAdapter &operator=( const DataAdapter &a )
        {
            this->clear();
            this->resize( a.length() );
            std::copy( a.begin(), a.end(), this->begin() );

            return *this;
        }

        inline bool operator==( const DataAdapter &da ) const
        {
            return std::lexicographical_compare( this->begin(), this->end(), da.begin(), da.end() ) == false;
        }

        inline bool operator<( const DataAdapter &da ) const
        {
            return std::lexicographical_compare( this->begin(), this->end(), da.begin(), da.end() ) == true;
        }

        inline size_type capacity() const
        {
            return this->data_capacity;
        }

        inline size_type length() const
        {
            return this->used_length;
        }

        void push_back( const value_type &n = value_type() )
        {
            //uses the previous length returned by resize to dictate the position of the last element
            this->at( this->resize( this->length() + 1 ) ) = n;
        }

        inline value_type &at( size_type n )
        {
            return this->data[n];
        }

        inline const value_type at( size_type n ) const
        {
            return this->data[n];
        }

        inline value_type &at( iterator it )
        {
            return this->at( it.off );
        }

        inline const value_type at( const_iterator it ) const
        {
            return this->at( it.off );
        }

        inline value_type front() const
        {
            return this->at( 0 );
        }

        inline value_type &front()
        {
            return this->at( 0 );
        }

        value_type back() const
        {
            if( !this->empty() )
            {
                return this->at( this->end() - 1 );
            }
            else
            {
                return this->front();
            }
        }

        value_type &back()
        {
            if( !this->empty() )
            {
                return this->at( this->length() - 1 );
            }
            else
            {
                return this->front();
            }
        }

        iterator sorted_insert( const value_type &n )
        {
            //First, add it to the end
            this->push_back( n );

            //Then insert it into the proper place
            for( size_type i = this->length() - 1; i > 0; --i )
            {
                if( this->at( i ) < this->at( i - 1 ) )
                {
                    std::swap( this->at( i ), this->at( i - 1 ) );
                }
                else
                {
                    return this->begin() + i;
                }
            }

            return this->begin();
        }
};

/*Mutable iterator class template*/
template <typename T>
class DataApapterIterator<T[]>
    : public std::iterator<std::random_access_iterator_tag, T>
{
    public:
        typedef std::iterator<std::random_access_iterator_tag, T> iterator_traits;

        typedef typename iterator_traits::iterator_category     iterator_category;
        typedef typename iterator_traits::value_type            value_type;
        typedef typename iterator_traits::difference_type       difference_type;
        typedef typename iterator_traits::pointer               pointer;
        typedef typename iterator_traits::reference             reference;

        typedef DataAdapter<T[]> parent_type;

        friend class DataAdapter<T[]>;

    private:
        //Keep a pointer to the parent
        parent_type *parent;
        difference_type off;

    public:
        DataApapterIterator() : parent( NULL ), off( 0 ) {}
        DataApapterIterator( parent_type *x, difference_type ioff = 0 ) : parent( x ), off( ioff ) {}
        DataApapterIterator( parent_type &x, difference_type ioff = 0 ) : parent( &x ), off( ioff ) {}
        DataApapterIterator( const DataApapterIterator &it ) : parent( it.parent ), off( it.off ) {}

        inline DataApapterIterator &operator=( const DataApapterIterator &it )
        {
            this->parent = it.parent;
            this->off = it.off;
            return *this;
        }

        inline bool operator==( const DataApapterIterator &it ) const
        {
            return this->parent == it.parent && this->off == it.off;
        }

        inline bool operator!=( const DataApapterIterator &it ) const
        {
            return this->parent != it.parent || this->off != it.off;
        }

        inline reference operator*()
        {
            return this->parent->at( this->off );
        }

        inline reference operator[]( difference_type n )
        {
            return *( *this + n );
        }

        inline pointer operator->()
        {
            return &( this->parent->at( this->off ) );
        }

        //VERY IMPORTANT for converting and comparing to const_iterators
        inline operator typename parent_type::const_iterator() const
        {
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

        inline DataApapterIterator operator+( const DataApapterIterator &it ) const
        {
            DataApapterIterator tmp( *this );
            tmp.off += it.off;
            return tmp;
        }
        inline DataApapterIterator operator+( difference_type n ) const
        {
            DataApapterIterator tmp( *this );
            tmp.off += n;
            return tmp;
        }
        inline DataApapterIterator &operator +=( const DataApapterIterator &it )
        {
            this->off += it.off;
            return *this;
        }
        inline DataApapterIterator &operator +=( difference_type n )
        {
            this->off += n;
            return *this;
        }

        inline difference_type operator-( const DataApapterIterator &it ) const
        {
            return this->off - it.off;
        }
        inline DataApapterIterator operator-( difference_type n ) const
        {
            DataApapterIterator tmp( *this );
            tmp.off -= n;
            return tmp;
        }
        inline DataApapterIterator &operator -=( const DataApapterIterator &it )
        {
            this->off -= it.off;
            return *this;
        }
        inline DataApapterIterator &operator -=( difference_type n )
        {
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
template <typename T>
class DataApapterIterator<const T[]> : public std::iterator<std::random_access_iterator_tag, T>
{
    public:
        typedef std::iterator<std::random_access_iterator_tag, T> iterator_traits;

        typedef typename iterator_traits::iterator_category     iterator_category;
        typedef typename iterator_traits::value_type            value_type;
        typedef typename iterator_traits::difference_type       difference_type;
        typedef typename iterator_traits::pointer               pointer;
        typedef typename iterator_traits::reference             reference;

        typedef DataAdapter<T[]> parent_type;

        friend class DataAdapter<T[]>;

    private:
        //Keep a pointer to the parent
        const parent_type *parent;
        difference_type off;

    public:
        DataApapterIterator() : parent( NULL ), off( 0 ) {}
        DataApapterIterator( const parent_type *x, difference_type ioff = 0 ) : parent( x ), off( ioff ) {}
        DataApapterIterator( const parent_type &x, difference_type ioff = 0 ) : parent( &x ), off( ioff ) {}
        DataApapterIterator( const DataApapterIterator &it ) : parent( it.parent ), off( it.off ) {}

        inline DataApapterIterator &operator=( const DataApapterIterator &it )
        {
            this->parent = it.parent;
            this->off = it.off;
            return *this;
        }

        inline bool operator==( const DataApapterIterator &it ) const
        {
            return this->parent == it.parent && this->off == it.off;
        }

        inline bool operator!=( const DataApapterIterator &it ) const
        {
            return this->parent != it.parent || this->off != it.off;
        }

        inline value_type operator*()
        {
            return this->parent->at( this->off );
        }

        inline value_type operator[]( difference_type n )
        {
            return *( *this + n );
        }

        DATA_ADAPTER_ITERATOR_UNARY_OP( ++ )
        DATA_ADAPTER_ITERATOR_UNARY_OP( -- )

        inline DataApapterIterator operator+( const DataApapterIterator &it ) const
        {
            DataApapterIterator tmp( *this );
            tmp.off += it.off;
            return tmp;
        }
        inline DataApapterIterator operator+( difference_type n ) const
        {
            DataApapterIterator tmp( *this );
            tmp.off += n;
            return tmp;
        }
        inline DataApapterIterator &operator +=( const DataApapterIterator &it )
        {
            this->off += it.off;
            return *this;
        }
        inline DataApapterIterator &operator +=( difference_type n )
        {
            this->off += n;
            return *this;
        }

        inline difference_type operator-( const DataApapterIterator &it ) const
        {
            return this->off - it.off;
        }
        inline DataApapterIterator operator-( difference_type n ) const
        {
            DataApapterIterator tmp( *this );
            tmp.off -= n;
            return tmp;
        }
        inline DataApapterIterator &operator -=( const DataApapterIterator &it )
        {
            this->off -= it.off;
            return *this;
        }
        inline DataApapterIterator &operator -=( difference_type n )
        {
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

#endif // DATA_ADAPTER_VECTOR_HPP_INCLUDED
