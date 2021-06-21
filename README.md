Changed some files for source (https://pallini.di.uniroma1.it/index.html) Version 2.7R1 (MAY 19 2019):
- naututil.h:
    For msvc use, change `<sys/time.h>` to `"time.h"` and with `"times.h"`
- gtools.h:
    For msvc use, change `<sys/wait.h>` to `"wait.h"`
- New `CMakeLists.txt` instead of `makefile`.
- All `boolean` type code are changed to `_Boolean`
- Change a Macro in `nauty.h`:
    `FIRSTBITNZ(x)` to `msc_bsr_xx(x)` where call `unsigned long p` instead of `unsigned long *p`

File `time.h` and `wait.h` is from `https://github.com/win32ports`, By MIT License. Some necessary changed is with them.
File `times.h` is used for necessary function and types of `time.h`