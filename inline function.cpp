#include <iostream.h>
using namespace std;
inline int Max(int x, int y) //defined function as inlined
{
return (x > y)? x : y;
}
int main( ) // Main function for the program
{
cout << "Max (20,10): " << Max(20,10) << endl;
cout << "Max (0,200): " << Max(0,200) << endl;
cout << "Max (100,1010): " << Max(100,1010) << endl;
return 0;
}