# File Comments
## File differences with source library codes
Changed some files from source (https://pallini.di.uniroma1.it/index.html) Version 2.7R1 (MAY 19 2019):
- naututil.h:
    For msvc use, change `<sys/time.h>` to `"times.h"` and `times.c` with Macro defined for arch chioce.
- gtools.h:
    For msvc use, change `<sys/wait.h>` to `"wait.h"`
- showg.c:
    Add a compiler choice `ASLIB`

# Code structure and function enhancement information
- New `CMakeLists.txt` instead of `makefile`.
- All `boolean` type code are changed to `_Boolean`
- Change a Macro in `nauty.h`:
    `FIRSTBITNZ(x)` to `msc_bsr_xx(x)` where call `unsigned long p` instead of `unsigned long *p`

File and `wait.h` is from `https://github.com/win32ports`, By MIT License. Some necessary changed is with them.
File `times.h` is used for necessary function and types.
# Other info
This repo is try to use `nauty` for some chemical research and create some new API. It's still on going.
Source COPYRIGHT License is [COPYRIGHT](libnauty\COPYRIGHT)