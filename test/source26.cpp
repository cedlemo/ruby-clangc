/* Taken from:
 * https://msdn.microsoft.com/en-us/library/f2wbycwh.aspx
 * I dont give a shit of their licence because nobody is reading
 * this.
 * */
class Window
{
public:
   Window();                               // Default constructor.
   Window( int x1, int y1,                 // Constructor specifying
   int x2, int y2 );                       //  window size.
bool SetCaption( const char *szTitle ); // Set window caption.
   const char *GetCaption();               // Get window caption.
   char *szWinCaption;                     // Window caption.
};

// Declare a pointer to the data member szWinCaption.
char * Window::* pwCaption = &Window::szWinCaption;

const char * (Window::*pfnwGC)() = &Window::GetCaption;
bool (Window::*pfnwSC)( const char * ) = &Window::SetCaption;

// virtual_functions.cpp
// compile with: /EHsc
#include <iostream>
using namespace std;

class Base
{
public:
virtual void Print();
};
void (Base ::* bfnPrint)() = &Base :: Print;
void Base :: Print()
{
cout << "Print function for class Base\n";
}

class Derived : public Base
{
public:
void Print();  // Print is still a virtual function.
};

void Derived :: Print()
{
cout << "Print function for class Derived\n";
}

int main()
{
    Base   *bPtr;
    Base    bObject;
Derived dObject;
bPtr = &bObject;    // Set pointer to address of bObject.
(bPtr->*bfnPrint)();
bPtr = &dObject;    // Set pointer to address of dObject.
(bPtr->*bfnPrint)();
}

//Output: Print function for class Base
Print function for class Derived
