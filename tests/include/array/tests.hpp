#ifndef DATA_ADAPTER_ARRAY_TESTS_HPP_INCLUDED
#define DATA_ADAPTER_ARRAY_TESTS_HPP_INCLUDED

#include "fixtures.hpp"

namespace DataAdapter_Tests
{

    static const int STATIC_TEST_ARRAY_SIZE = 10;

    typedef DataAdapter_StaticArray_TestFixtureTemplate<int[STATIC_TEST_ARRAY_SIZE]>
    DataAdapter_StaticArray_TestFixture;

    TEST_F( DataAdapter_StaticArray_TestFixture, Construction )
    {

        //Assert that the value_type is the same as T
        ::testing::StaticAssertTypeEq <
        DataAdapter_StaticArray_TestFixture::adapter_t::value_type,
                                            int[STATIC_TEST_ARRAY_SIZE]
                                            > ();

        //assert basic creation parameters are set
        ASSERT_EQ( A.length(), 0 );
        ASSERT_EQ( A.capacity(), STATIC_TEST_ARRAY_SIZE );

        //assign k to A. Nothing should be thrown.
        ASSERT_NO_THROW(
            A.assign( k, k + A.capacity() )
        );

        //Assert that a is now full and that .full works
        ASSERT_EQ( A.length(), A.capacity() );
        ASSERT_TRUE( A.full() );

        DataAdapter_StaticArray_TestFixture::adapter_t C( 3, 0x4 );

        ASSERT_EQ( 3,   C.length() );
        ASSERT_EQ( 0x4, C[0] );
        ASSERT_EQ( 0x4, C[1] );
        ASSERT_EQ( 0x4, C[2] );

        DataAdapter_StaticArray_TestFixture::adapter_t D( 3 );

        ASSERT_EQ( 3,   D.length() );
        ASSERT_EQ( 0x0, D[0] );
        ASSERT_EQ( 0x0, D[1] );
        ASSERT_EQ( 0x0, D[2] );

        {
            SCOPED_TRACE( "copying" );

            D = C;

            ASSERT_EQ( C.length(), D.length() );

            ASSERT_EQUAL_RANGE( DataAdapter_StaticArray_TestFixture::adapter_t::iterator,
                                C.begin(), C.end(), D.begin() );


            DataAdapter_StaticArray_TestFixture::adapter_t E( C );

            ASSERT_EQ( C.length(), E.length() );

            ASSERT_EQUAL_RANGE( DataAdapter_StaticArray_TestFixture::adapter_t::iterator,
                                C.begin(), C.end(), E.begin() );
        }
    }

    TEST_F( DataAdapter_StaticArray_TestFixture, IteratorsBasic )
    {
        DataAdapter_StaticArray_TestFixture::adapter_t::iterator it( &A ), it2;

        //See if the default value is indeed begin.
        ASSERT_EQ( A.begin(), it );

        //See if the default value for nothing is unique
        ASSERT_NE( A.begin(), it2 );

        DataAdapter_StaticArray_TestFixture::adapter_t::const_iterator it3;

        //Compile time test for the conversion from iterator to const_iterator
        it3 = A.begin() + 2;

        //Compile time test for the comparison of iterator and const_iterator
        ASSERT_EQ( it3, A.begin() + 2 );

        //Compile time test for the comparison of iterator and const_iterator
        ASSERT_EQ( A.cbegin(), A.begin() );
    }

    TEST_F( DataAdapter_StaticArray_TestFixture, InsertionBasic )
    {
        DataAdapter_StaticArray_TestFixture::adapter_t::iterator it;

        {
            SCOPED_TRACE( "push_back" );

            B.push_back( 0x1234 );

            ASSERT_EQ( 0x1234, B.back() );
        }

        {
            SCOPED_TRACE( "push_front" );

            B.push_front( 0x4321 );

            ASSERT_EQ( 0x4321, B.front() );
        }

        ASSERT_EQ( 2,        B.length() );
        ASSERT_EQ( 0x4321,   B.front() );
        ASSERT_EQ( 0x1234,   B.back() );

        {
            SCOPED_TRACE( "sorted_insert" );

            it = B.sorted_insert( 0x0 );
            ASSERT_EQ( 3, B.length() );
            ASSERT_EQ( B.begin(), it );
            ASSERT_EQ( 0x0,     B[0] );
            ASSERT_EQ( 0x4321,  B[1] );
            ASSERT_EQ( 0x1234,  B[2] );
        }

        {
            SCOPED_TRACE( "push_back 2" );

            B.push_back( 0x1111 );

            ASSERT_EQ( 4,        B.length() );
            ASSERT_EQ( 0x1111,   B.back() );
        }
    }

    TEST_F( DataAdapter_StaticArray_TestFixture, InsertionNormal )
    {
        DataAdapter_StaticArray_TestFixture::adapter_t::iterator it;

        {
            SCOPED_TRACE( "setup" );

            A.push_back( 0x0 );
            A.push_back( 0x2 );
            A.push_back( 0x16 );
            A.push_back( 0x128 );

            ASSERT_EQ( 4, A.length() );
        }

        {
            SCOPED_TRACE( "insert(1) at begin() + 2" );

            it = A.insert( A.begin() + 2, 0x4 );

            ASSERT_EQ( A.begin() + 2, it );
            ASSERT_EQ( 0x4,          *it );
            ASSERT_EQ( 0x4,          A[2] );
        }

        {
            SCOPED_TRACE( "clear" );

            A.clear();

            ASSERT_EQ( 0, A.length() );
        }

        {
            SCOPED_TRACE( "insert(1) at begin()" );

            it = A.insert( A.begin(), 0x12 );

            ASSERT_EQ( A.begin(), it );
            ASSERT_EQ( 1,        A.length() );
            ASSERT_EQ( 0x12,     *it );
            ASSERT_EQ( 0x12,     A[0] );
            ASSERT_EQ( 0x12,     A.front() );
        }
    }

    TEST_F( DataAdapter_StaticArray_TestFixture, InsertionAdvanced )
    {
        DataAdapter_StaticArray_TestFixture::adapter_t::iterator it;

        {
            SCOPED_TRACE( "setup" );

            A.push_back( 0x0 );
            A.push_back( 0x2 );
            A.push_back( 0x16 );
            A.push_back( 0x128 );

            ASSERT_EQ( 4, A.length() );
        }

        {
            SCOPED_TRACE( "insert(3) at begin()" );

            it = A.insert( A.begin(), 3, 0x11 );

            ASSERT_EQ( A.begin(),   it );
            ASSERT_EQ( 4 + 3,       A.length() );
            ASSERT_EQ( 0x11,        A[0] );
            ASSERT_EQ( 0x11,        A[1] );
            ASSERT_EQ( 0x11,        A[2] );
        }

        {
            SCOPED_TRACE( "insert(range) at begin()" );

            it = B.insert( B.begin(), A.cbegin(), A.cend() );

            ASSERT_EQ( B.begin(),    it );
            ASSERT_EQ( A.length(),   B.length() );

            {
                SCOPED_TRACE( "range equal" );

                ASSERT_EQUAL_RANGE( DataAdapter_StaticArray_TestFixture::adapter_t::iterator,
                                    A.begin(), A.end(), B.begin() );

            }
        }

        {
            SCOPED_TRACE( "insert(range) at begin() + 2" );

            it = B.insert( B.begin() + 2, A.cend() - 2, A.cend() );

            ASSERT_EQ( B.begin() + 2,    it );
            ASSERT_EQ( A.length() + 2,   B.length() );

            {
                SCOPED_TRACE( "range_equal" );

                ASSERT_EQUAL_RANGE( DataAdapter_StaticArray_TestFixture::adapter_t::iterator,
                                    A.end() - 2, A.end(), B.begin() + 2 );
            }
        }
    }

    TEST_F( DataAdapter_StaticArray_TestFixture, ManipulationBasic )
    {
        typedef typename DataAdapter_StaticArray_TestFixture::adapter_t::element_type element;

        DataAdapter_StaticArray_TestFixture::adapter_t::iterator it;

        {
            SCOPED_TRACE( "setup" );

            A.assign( k, k + STATIC_TEST_ARRAY_SIZE );

            ASSERT_EQ( A.length(), STATIC_TEST_ARRAY_SIZE );
        }

        {
            SCOPED_TRACE( "pop_back" );

            element i = A.pop_back();

            ASSERT_EQ( 0x10, i );
            ASSERT_EQ( 9,    A.length() );
        }

        {
            SCOPED_TRACE( "push_front" );

            A.push_front( 0x10 );

            ASSERT_EQ( 0x10, A.front() );
            ASSERT_EQ( 10,   A.length() );
            ASSERT_EQ( 0x5,  A.at( 5 ) );
        }
    }

    TEST_F( DataAdapter_StaticArray_TestFixture, ManipulationAdvanced )
    {
        DataAdapter_StaticArray_TestFixture::adapter_t::iterator it, f, l;

        {
            SCOPED_TRACE( "setup" );

            A.assign( k, k + STATIC_TEST_ARRAY_SIZE );

            ASSERT_EQ( A.length(), STATIC_TEST_ARRAY_SIZE );
        }

        {
            SCOPED_TRACE( "erase(1)" );

            it = A.erase( A.begin() + 4 );

            ASSERT_EQ( A.begin() + 4, it );
            ASSERT_EQ( 0x6,          *it );
            ASSERT_EQ( 9,            A.length() );
        }

        {
            SCOPED_TRACE( "erase(range)" );

            //Erase all the the first and last two elements, leaving four total
            f = A.begin() + 2;
            l = A.end() - 2;

            it = A.erase( f, l );

            ASSERT_EQ( f,    it );
            ASSERT_EQ( 4,    A.length() );
            ASSERT_EQ( 0x1,  A.at( 0 ) );
            ASSERT_EQ( 0x2,  A.at( 1 ) );
            ASSERT_EQ( 0x9,  A.at( 2 ) );
            ASSERT_EQ( 0x10, A.at( 3 ) );
        }

        {
            SCOPED_TRACE( "erase(range) all" );

            f = A.begin();
            l = A.end();

            it = A.erase( f, l );

            ASSERT_EQ( f, it );
            ASSERT_TRUE( A.empty() );
        }
    }

    TEST_F( DataAdapter_StaticArray_TestFixture, BinarySearch )
    {
        DataAdapter_StaticArray_TestFixture::adapter_t::iterator it;

        {
            SCOPED_TRACE( "setup" );

            A.assign( k, k + STATIC_TEST_ARRAY_SIZE );

            ASSERT_EQ( A.length(), STATIC_TEST_ARRAY_SIZE );
            ASSERT_TRUE( is_sorted( A.begin(), A.end() ) );

            A.resize( 5 );

            ASSERT_EQ( A.length(), 5 );
            ASSERT_TRUE( is_sorted( A.begin(), A.end() ) );
        }

        {
            SCOPED_TRACE( "binary_search" );

            it = A.find_sorted( 0x4 );

            ASSERT_EQ( A.begin() + 3, it );
            ASSERT_EQ( 0x4, *it );
        }

        {
            SCOPED_TRACE( "insert from previous binary_search" );

            it = A.insert( it, 0x4 );

            ASSERT_EQ( 6, A.length() );
            ASSERT_EQ( A.begin() + 3, it );
            ASSERT_TRUE( is_sorted( A.begin(), A.end() ) );
        }

    }
}

#endif // DATA_ADAPTER_ARRAY_TESTS_HPP_INCLUDED
