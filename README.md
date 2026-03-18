Easy "clone and go" repository for a libopencm3 based project using VScode as an IDE.

# Instructions
 1. git clone https://github.com/yspacedev/libopencm3-vscode-template.git your-project
 2. cd your-project
 3. ln -s /path/to/libopencm3
 4. modify `Makefile` to use your specific microcontroller
 4. make

If you want libopencm3 installed in this repository instead of symlinking it, use `--recurse_submodules` when cloning or `git submodule update --init` after the clone. If you are downoading libopencm3 for the first time, compile it with `make` to build the binaries and generate code files (like nvic.h)

# Using with your microcontroller
- edit `OOCD`, `DEVICE`, `OOCD_TARGET`, `OOCD_INTERFACE` in `Makefile` to make compilation work properly
- edit `defines` in `.vscode/c_cpp_properties.json` to make IntelliSense work properly
- edit `configFiles` and `svdFile` in `.vscode/launch.json` to enable VScode debugging with GDB (you will also need the extension Cortex-Debug for this)
- import the SVD file for your microcontroller if you want register definitions

# Features
- Clock config, systick, and queued printf debug over UART implemented
- Default program blinks LED on C4
- Planning to add semihosting option and inputs over semihosting and UART

# Directories
* root contains the application, makefiles, compiled binaries, and linker script

# As a template
You should replace this with your _own_ README if you are using this as a template.
