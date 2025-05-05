#include <iostream>
#include <exception>
#include "./exercises/sky_box.h"
#include "triangle.h"
#include "bounding_box.h"

int main()
{
    try
    {
        sky_box_exercise();
        std::cout << "Total triangle intersection tests: " << triangle_tests << "\n";
        std::cout << "Total bbox intersection tests: " << bbox_tests << "\n";
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

