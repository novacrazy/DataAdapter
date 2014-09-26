#include <cstdlib>
#include <ctime>

#include <tests.h>

int main( int argc, char** argv ) {

    //Just in case I use random data in the tests
    //Yes, I know the current time isn't very random, but it's good enough for testing
    srand( time( NULL ) );

    ::testing::InitGoogleTest( &argc, argv );

    return RUN_ALL_TESTS();
}
