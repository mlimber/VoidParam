#include <exception>

// As before -- the function we can't change
template <typename T>
T Process(const T& t) { return t; }

// Helper struct that we pass in when we're handed a void param
struct VoidParamDetected {};

// Overload the the function with our helper struct
void Process( VoidParamDetected ) 
{
    throw std::exception("'void' parameter is not allowed");
}

// Overload the comma operator to return the left-hand operand instead of the usual right-hand.
template <typename T>
const T& operator , (const T& t, VoidParamDetected ) 
{ 
    return t; 
}

// Use the comma operator and SFINAE:
//   1. If x is NOT of type void, the comma operator overload is instantiated and returns x, 
//      which is passed to Process.
//   2. If x is of type void, the comma operator fails to instantiate since it is illegal to have a 
//      reference to a void. Thanks to SFINAE, this is not an error and we fall back to the normal
//      comma operator, which evaluates both parameters and returns the right-hand operand, which
//      is an instance of our helper struct. This in turn calls our overload of Process() which
//      throws an exception.
#define PROCESS(x) Process((x, VoidParamDetected()))

// As before
void GetVoid() {}

// As before
void main()
{
    // Process an int
    PROCESS(1); // Success

    PROCESS(GetVoid()); // Runtime exception!
    
    PROCESS(void()); // Runtime exception!
}


