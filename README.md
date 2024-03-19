# Battery Monitor

A command line utility to monitor the battery status and notify the user in all open `tty` sessions about the low battery status.

![demo](https://raw.githubusercontent.com/kasramp/battery_monitor/master/assets/demo.gif)

## Build

Make sure you have `libacpi` installed. 

```bash
$ sudo apt install libacpi-dev libacpi0
```

Then run:

```bash
$ make
```

## Download

If you want to quickly test the utility without compiling, or have a portable version, go to [the release page](https://github.com/kasramp/battery_monitor/releases) and download the latest version. 
After that, give the +x permission to the downloaded file,

```bash
$ chmod +x battery_monitor
```

Keep in mind that the executable is only for `x86_64` platform. For other architecture, you need to build manually. See the next step.

## Installation

After running the build, run:

```bash
$ sudo make install
```

That installs the `battery_monitor` utility on your device.

## Removal

To uninstall the programm, run:

```bash
$ sudo make uninstall
```

## Code Format

The project uses Linux code formatting. To format the code automatically, ensure `indent` is installed. Then run,

```bash
$ make format
```

## Man page

After changing the man page content, run,

```bash
$ cd man && man -l battery_monitor.1
```

To ensure the man page format is correct.
