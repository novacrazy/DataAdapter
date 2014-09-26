#ifndef DATA_ADAPTER_ARRAY_TEST_FIXTURES_HPP_INCLUDED
#define DATA_ADAPTER_ARRAY_TEST_FIXTURES_HPP_INCLUDED

#include <gtest/gtest.h>

#include "../tools.hpp"
#include "../../adapters/array.hpp"

namespace DataAdapter_Tests {

    template <typename T>
    class DataAdapter_StaticArray_TestFixtureTemplate : public ::testing::Test {
        public:
            typedef DataAdapter<T> adapter_t;

            virtual void SetUp() {
                testing::StaticAssertTypeEq<typename is_array<T>::type, true_type>();
            }

            typename adapter_t::value_type k = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0x10};

            adapter_t A, B;
    };

}

#endif // DATA_ADAPTER_ARRAY_TEST_FIXTURES_HPP_INCLUDED
