DataAdapter
===========

Created to use in a school assignment, this C++98 compliant library creates a common interface for data structures, allowing for easy replacement of one with another within a program.

<hr>
####Testing

For the tests, I used the Google Test framework (version 1.7), so that is required to compile the tests. 

Using CMake, you should select the source directory to be DataAdapter/ and the build directory to whatever you want. On the first and maybe second configure run, it will probably fail due to random things. If it fails to find GTest, you should manually enter the CMake variables for `GTEST_INCLUDE_DIR`, `GTEST_LIBRARY` and `GTEST_MAIN_LIBRARY`.

After that it should configure and generate fine. Go into your build directory and run `make` to compile the tests. To run them, either execute DataAdapter.exe or run `make tests`

If you experience any problems, feel free to open an issue or if you're feeling very generous fix them an open a pull request. Any contributions for fixes and new data structures are welcome. 