# Battery Monitor

A command line utility to monitor the battery status and notify the user in all open `tty` sessions about the low battery status.

![demo](https://raw.githubusercontent.com/kasramp/battery_monitor/master/assets/demo.gif)

## Build

Make sure you have `libacpi` installed. Then run:

```bash
$ make
```

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
