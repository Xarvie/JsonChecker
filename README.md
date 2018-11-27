# JsonChecker
JSON check tool.
1 thread, 1000 files 100ms.

> ***Environment***
 
> Linux CentOS 6.9 x86_64
 
> Intel(R) Core(TM) i7-7700k CPU @ 4.20GHz

``` jsonc
/* support comments */
{
    "key": 1, // comments
    "key2": /*comments*/ 12
}
```

Supports a variety of configurations, such as: 
``` cpp
/* code/parser.cpp */
int CONFIG_USE_COMMENT = 1;
int CONFIG_TAB_IN_STR = 1;
int CONFIG_ZERO_FIRST = 1;
int CONFIG_DEEP_LIMIT = 1;
```
int CONFIG_USE_COMMENT = 1：
e.g ```/*comments*/``` and ```// comments ```

int CONFIG_TAB_IN_STR = 1：
e.g ```"tab: key": 12``` auto converted to ```"tab:\tkey": 12```

int CONFIG_ZERO_FIRST = 1：
e.g ```"key": 012```


### Build Step (Windows && Linux && OSX)
```shell
mkdir ./build
cd ./build
cmake ..   ##if need X86_64 ver: cmake .. -A x64
cmake --build . --config Release
./Release/main

```
