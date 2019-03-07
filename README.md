# ringandkey::rpn (reverse polish notation)

It is a C++ header only class for converting arithmetic expressions of given arithmetic operations into reverse Polish notation and calculating.

## how to use

Simply include the header file.

Calculate by passing the formula as a string to ringandkey::rpn::calc(std::string txt).

```cpp
#include <iostream>
#include "ringandkey/rpn.h"

int main(int argc, char** argv) {
    std::cout << ringandkey::rpn::calc("100*(1+2)/10") << std::endl;

    return 0;
}
```

Save the above code as "sample.cpp", then build with g++.

```sh
> g++ -std=c++11 -o sample test.cpp -I/path/to/include
```

running sample, the result is "30".

```sh
> ./sample
30
```
