#include <iostream>
#include <data_adapter>

using namespace std;

int main()
{
    DataAdapter<int[20]> my_container;

    my_container.push_front( 0x4321 );
    my_container.push_back( 0x1234 );
    my_container.insert( my_container.begin() + 1, 3, 0xAF );

    DataAdapter<int[20]>::iterator it = my_container.begin();

    for( ; it != my_container.end(); ++it )
    {
        cout << hex << uppercase << "0x" << *it << ' ';
    }

    return 0;
}
