#include <iostream>
#include <exception>
#include "./exercises/triangles.h"

int main()
{
    try
    {
        triangles_exercise();
        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return 1; // non-zero return indicates error
    }
    catch (...)
    {
        std::cerr << "Unknown exception caught!" << std::endl;
        return 2;
    }
}

