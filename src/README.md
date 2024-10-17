## Development setup
`sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib picotool` - apt

Arch:
`sudo pacman -Syy cmake`
`yay -S arm-none-eabi-gcc arm-none-eabi-newlib libstdc++-arm-none-eabi-newlib picotool` or yaourt or whatever the fuck Mihai is using


Then,
`git pull --recurse-submodules --jobs=10`

When building time comes, run `./build.sh`


## CMakeLists

Add each module in CMakeLists, e.g. Hello world:
```c
#include <stdio.h>
#include "pico/stdlib.h"

int main() {
    setup_default_uart();
    printf("Hello, world!\n");
    return 0;
}
```

and add the following to your CMakeLists.txt:
```
add_executable(hello_world
    hello_world.c
)
target_link_libraries(hello_world pico_stdlib)

pico_add_extra_outputs(hello_world)
```

## Monitoring outputs
- Install `minicom`
- Or find the location of the serial port (Run `lsblk`)
- Run `minicom -b 115200 -o -D /dev/<PORT>`
