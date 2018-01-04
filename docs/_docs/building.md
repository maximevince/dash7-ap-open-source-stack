---
title: Building
permalink: /docs/building/
---


# Get prerequisites

- OSS-7 code. The code is hosted on [github](https://github.com/mosaic-lopow/dash7-ap-open-source-stack/), so either fork or clone the repository.
- [CMake](http://www.cmake.org/) (v2.8.12 or greater) as a flexible build system
- a GCC-based toolchain matching the target platform, for example [GNU ARM Embedded](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads) for ARM Cortex-M based platforms. By default the build system assumes the GNU ARM Embedded toolchain is located in the PATH environment variable (meaning you can run `arm-none-eabi-gcc` without specifying the full path).
- [JLinkExe](https://www.segger.com/downloads/jlink) (included in 'J-Link Software and Documentation Pack') if you are using a JLink probe to flash/debug your target

# Run cmake

We will create a build directory and run cmake to generate the buildscript:

	$ git clone https://github.com/MOSAIC-LoPoW/dash7-ap-open-source-stack.git
	$ mkdir build
	$ cd build
	$ cmake ../dash7-ap-open-source-stack/stack/ -DAPP_GATEWAY=y -DAPP_SENSOR_PUSH=y
	-- Cross-compiling using gcc-arm-embedded toolchain
	-- Cross-compiling using gcc-arm-embedded toolchain
	-- The C compiler identification is GNU 4.9.3
	-- The CXX compiler identification is GNU 4.9.3
	-- Detecting C compiler ABI info
	-- Detecting C compiler ABI info - failed
	-- Detecting C compile features
	-- Detecting C compile features - failed
	-- Detecting CXX compiler ABI info
	-- Detecting CXX compiler ABI info - failed
	-- Detecting CXX compile features
	-- Detecting CXX compile features - failed
        -- detected supported platforms: B_L072Z_LRWAN1 EFM32GG_STK3700 EZR32LG_USB01 EZR32LG_WSTK6200A EZRPi NUCLEO_L073RZ
	-- selected platform: B_L072Z_LRWAN1
	-- The ASM compiler identification is GNU
	-- Found assembler: /home/glenn/bin/toolchains/gcc-arm-none-eabi-4_9-2014q4/bin/arm-none-eabi-gcc
	-- Added chip stm32l0xx
	-- Added chip sx127x
	-- Configuring done
	-- Generating done
	-- Build files have been written to: /home/glenn/projects/build


A quick run-down of what happens:
* We point cmake to the open source stack implementation: `../dash7-ap-open-source-stack/stack/` in this case
* We are using the default gcc-arm-embedded toolchain, which is in our path. If you prefer not to add the toolchain directory to the PATH you have to specify the path by passing this to cmake as in this example: `-DTOOLCHAIN_DIR=../gcc-arm-none-eabi-4_9-2015q3/`. If you would want to use another toolchain you have to pass the `-DCMAKE_TOOLCHAIN_FILE=...` option to point to the cmake configuration for the cross compiler.
* Based on the toolchain a number of supported platform options are available. By default the `B_L072Z_LRWAN1` platform is selected. If you want another platform you can specify this using `-DPLATFORM=<platform-name>`. Each subdirectory beneath `stack/framework/hal/platforms` contains a different platform, and the platform name to use is equal to the name of the subdirectory.
* A platform is a combination of one or more chips (MCU or RF) and the wiring between them. Based on the platform a number of chips will be added to the build, in this example the `stm32l0xx` MCU and the `sx127x` RF chip.
* Applications can be added by setting `-DAPP_<name>=y`. The name of the application is the name of a subdirectory of stack/apps, but uppercased. In this example we enabled the sensor and gateway application.

# Build it!

If your toolchain is setup correctly you should be able to build the stack and the configured application(s) now. An example of (shortened) output is below:

	$ make
	[ 18%] Built target d7ap
	[ 20%] Built target CHIP_SX127X
	[ 58%] Built target CHIP_STM32L0XX
	[ 65%] Built target PLATFORM
	[ 66%] Built target FRAMEWORK_COMPONENT_timer
	[ 70%] Built target FRAMEWORK_COMPONENT_aes
	[ 71%] Built target FRAMEWORK_COMPONENT_cli
	[ 73%] Built target FRAMEWORK_COMPONENT_compress
	[ 75%] Built target FRAMEWORK_COMPONENT_console
	[ 76%] Built target FRAMEWORK_COMPONENT_crc
	[ 78%] Built target FRAMEWORK_COMPONENT_fec
	[ 80%] Built target FRAMEWORK_COMPONENT_fifo
	[ 81%] Built target FRAMEWORK_COMPONENT_log
	[ 83%] Built target FRAMEWORK_COMPONENT_node_globals
	[ 85%] Built target FRAMEWORK_COMPONENT_pn9
	[ 86%] Built target FRAMEWORK_COMPONENT_random
	[ 88%] Built target FRAMEWORK_COMPONENT_scheduler
	[ 90%] Built target FRAMEWORK_COMPONENT_segger_rtt
	[ 91%] Built target FRAMEWORK_COMPONENT_shell
	[ 95%] Built target framework
	[100%] Built target sensor_push.elf

Note that this builds the chip drivers, the platform, the framework and the DASH7 stack implementation as a static library and links the applications.

# Build options

More build options to for example tweak parameters of the stack or platform specific settings can be configured through cmake. This can be done by passing -D options on the commandline (as above) or using the ccmake interactive console interface or the cmake-gui interface as shown below. The current values of all options can be displayed by executing `cmake -L`.

# Flashing

cmake will generate targets for flashing each application using JLink, by running `make flash-<appname>`.
If you are not using a JLink adapter you can of course flash the binary manually. For instance, if you want to
use the embedded ST-LINK on the B_L072Z_LRWAN1 you can copy over the generated bin file from `<build-dir>/apps/<app-name>/<app-name>.bin` to the mass storage device, as explained in the [B_L072Z_LRWAN1 platform notes]({{ site.baseurl }}{% link _docs/platform-lrwan1.md %}).
Otherwise, if you are using JLink just use the make target like this:

	$ make flash-sensor_push
	[100%] Built target sensor_push.elf
	Scanning dependencies of target flash-sensor_push
	SEGGER J-Link Commander V6.18a (Compiled Aug 11 2017 17:53:58)
	<snip>
	Downloading file [sensor_push.bin]...
	Comparing flash   [100%] Done.
	Erasing flash     [100%] Done.
	Programming flash [100%] Done.
	Verifying flash   [100%] Done.
	<snip>
	[100%] Built target flash-sensor_push

If all went well the application should be running on the target.

# Running

Please refer to the [running the examples section]({{ site.baseurl }}{% link _docs/running-examples.md %}), but it is advised to read the next section first, which gives [an introduction to DASH7 Alliance Protocol]({{ site.baseurl }}{% link _docs/D7AP-intro.md %}).


# MS Windows support

While the above is written for Unix OS's (GNU/Linux and Mac OS X) it works on MS Windows as well. On MS Windows you should install the mingw32 compiler and use the "MinGW32 Makefiles" generator option when running cmake. Finally, you should use the `mingw32-make` command instead of `make`.
