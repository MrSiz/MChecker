### MChecker
This tool is created for detecting memory leaks. There's no doubt it's far worse than valgrind.

#### Usage
1. `include "mchecker.h`
2. `g++ your_file.cc mchecker.h -std=c++03 -o target`
3. Run `./target`

**Note**
1. Error will be reported if you compile with `std=c++11` or later version, because `=delete` will be intrepreted as `=deletep(__FILE__, __LINE__), delete`. In order to avoid this problem, `#define delete deletep(__FILE__, __LINE__), delete`  shoule be commented out.
2. The order of dependent libraries will affect the final effect.`delete()` defined in `mchecker.cc` may not be called. 

##### License
MIT