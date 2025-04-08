#include <sstream>
#include <string>
#include "gtest/gtest.h"
#include "../canvas.h"
#include "../colour.h"
#include "../ppm.h"

/*
Scenario: Constructing the PPM header
  Given c ← canvas(5, 3)
  When ppm ← ppm(c)
  Then lines 1-3 of ppm are
    """
    P3
    5 3
    255
    """
*/
TEST(ppm, should_write_ppm_header)
{
    const canvas_t c{ 5, 3 };
    const ppm_t ppm{ c };
    std::istringstream stream(ppm.data);
    std::string line;
    int num_lines_to_read = 3;  // Change this value to read the desired number of lines
    int lines_read = 0;

    // Read up to numLinesToRead lines
    while (lines_read < num_lines_to_read && std::getline(stream, line)) {
        if (lines_read == 0)
        {
            EXPECT_STREQ(line.c_str(), "P3");
        }
        else if (lines_read == 1)
        {
            EXPECT_STREQ(line.c_str(), "5 3");
        }
        else if (lines_read == 2)
        {
            EXPECT_STREQ(line.c_str(), "255");
        }
        lines_read++;
    }
}

/*
Scenario: Constructing the PPM pixel data
  Given c ← canvas(5, 3)
    And c1 ← color(1.5, 0, 0)
    And c2 ← color(0, 0.5, 0)
    And c3 ← color(-0.5, 0, 1)
  When write_pixel(c, 0, 0, c1)
    And write_pixel(c, 2, 1, c2)
    And write_pixel(c, 4, 2, c3)
    And ppm ← ppm(c)
  Then lines 4-6 of ppm are
    """
    255 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    0 0 0 0 0 0 0 128 0 0 0 0 0 0 0
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 255
    """
*/
TEST(ppm, should_write_ppm_pixel_data_from_canvas)
{
    canvas_t c{ 5, 3 };
    const colour_t c1{ 1.5, 0, 0 };
    const colour_t c2{ 0, 0.5, 0 };
    const colour_t c3{ -0.5, 0, 1 };
    c.write_pixel(0, 0, c1);
    c.write_pixel(2, 1, c2);
    c.write_pixel(4, 2, c3);
    const ppm_t ppm{ c };
    std::istringstream stream(ppm.data);
    std::string line;
    int num_lines_to_read = 6;  // Change this value to read the desired number of lines
    int lines_read = 0;

    // Read up to numLinesToRead lines
    while (lines_read < num_lines_to_read && std::getline(stream, line)) {
        if (lines_read == 3)
        {
            EXPECT_STREQ(line.c_str(), "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0");
        }
        else if (lines_read == 4)
        {
            EXPECT_STREQ(line.c_str(), "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0");
        }
        else if (lines_read == 5)
        {
            EXPECT_STREQ(line.c_str(), "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255");
        }
        lines_read++;
    }
}

/*
Scenario: Splitting long lines in PPM files
  Given c ← canvas(10, 2)
  When every pixel of c is set to color(1, 0.8, 0.6)
    And ppm ← ppm(c)
  Then lines 4-7 of ppm are
    """
    255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204
    153 255 204 153 255 204 153 255 204 153 255 204 153
    255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204
    153 255 204 153 255 204 153 255 204 153 255 204 153
    """
*/
TEST(ppm, should_split_colour_data_lines_at_70_chars)
{
    canvas_t c{ 10, 2 };
    const colour_t colour{ 1, 0.8, 0.6 };
    c.fill(colour);
    const ppm_t ppm{ c };
    std::istringstream stream(ppm.data);
    std::string line;
    int num_lines_to_read = 7;  // Change this value to read the desired number of lines
    int lines_read = 0;

    // Read up to numLinesToRead lines
    while (lines_read < num_lines_to_read && std::getline(stream, line)) {
        if (lines_read == 3)
        {
            EXPECT_STREQ(line.c_str(), "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204");
        }
        else if (lines_read == 4)
        {
            EXPECT_STREQ(line.c_str(), "153 255 204 153 255 204 153 255 204 153 255 204 153");
        }
        else if (lines_read == 5)
        {
            EXPECT_STREQ(line.c_str(), "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204");
        }
        else if (lines_read == 6)
        {
            EXPECT_STREQ(line.c_str(), "153 255 204 153 255 204 153 255 204 153 255 204 153");
        }
        lines_read++;
    }
}

/*
Scenario: PPM files are terminated by a newline character
  Given c ← canvas(5, 3)
    And ppm ← ppm(c)
  Then ppm ends with a newline character
*/
TEST(ppm, should_end_in_a_newline_char)
{
    canvas_t c{ 5,  3 };
    const ppm_t ppm{ c };
    std::istringstream stream(ppm.data);
    std::string line;
    int num_lines_to_read = 4;  // Change this value to read the desired number of lines
    int lines_read = 0;

    // Read up to numLinesToRead lines
    while (lines_read < num_lines_to_read && std::getline(stream, line)) {
        if (lines_read == 4)
        {
            EXPECT_STREQ(line.c_str(), "\n");
        }
        lines_read++;
    }
}

/*
Scenario: PPM can be written to disk
  Given c ← canvas(5, 3)
    And c1 ← colour(1, 0, 0)
    And c.fill(c1)
    And ppm ← ppm(c)
    And ppm.write_to_file("./out.ppm")
  Then out.ppm is a readable ppm image file
*/
TEST(ppm, should_be_able_to_write_ppm_to_disk)
{
    canvas_t c{ 5, 3 };
    const colour_t red{ 1, 0, 0 };
    c.fill(red);
    const ppm_t ppm{ c };
    ppm.write_to_file("./out.ppm");
    const ppm_t out_ppm{ ".\\out.ppm" };
    EXPECT_STREQ(ppm.data.c_str(), out_ppm.data.c_str());
}

/*
Scenario: PPM can be read from file
  Given c ← canvas(5, 3)
    And c1 ← colour(1, 0, 0)
    And c.fill(c1)
    And ppm ← ppm(c)
    And in_ppm ← in_ppm("..\\..\\tests\\in.ppm")
  Then ppm.data = in_ppm.data
*/
TEST(ppm, should_be_able_to_read_ppm_file_from_disk)
{
    canvas_t c{ 5, 3 };
    const colour_t red{ 1, 0, 0 };
    c.fill(red);
    const ppm_t ppm{ c };
    const ppm_t in_ppm{ "..\\..\\tests\\in.ppm" };
    EXPECT_STREQ(ppm.data.c_str(), in_ppm.data.c_str());
}