

cl /std:c++20 dynamic_array_test.cpp /link /LIBPATH:"../../build" CKit.lib
dynamic_array_test
:: cl /std:c++20 main.cpp