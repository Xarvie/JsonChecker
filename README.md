# JsonChecker
The JSON check tool used in the work.
Scan 1000 files, about ***100ms***.

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
support ```/*comments*/``` and ```// comments ```

int CONFIG_TAB_IN_STR = 1：
support ```"{tab-> }key": 12```

int CONFIG_ZERO_FIRST = 1：
support ```"key": 012```

---

### Build Step
```shell
cd ./build
cmake ..
```
