#include <iostream>
#include <exception>
#include "./exercises/area_lights.h"

int main()
{
    try
    {
        area_lights_exercise();
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

