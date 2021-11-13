#include <iostream>

#define fix(x)                  \
    (x)>32 && (x)%16>=8 ?       \
        (x) :                   \
    ((x>=32) ?                  \
        (x-24) :                \
    ((x%16)>=8 ?                \
        (x+24) :                \
        (x)))

int y = fix(5);
int z = fix(33);
    //if x>32 and x%16>8    
//then fix(x) = x
//else
int main()
{
    std::cout << "5 = " << y;
    std::cout << "33 = " << z;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
