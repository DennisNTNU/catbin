# hexdumb

A small command line program to print the contents of a file in a hexadecimal format to the console. My own implementation of hexdump.

### Features

Choose whether the file should be printed in big-endian (Most significan Byte first) or little-endian (Least significant Byte first) format.

### Examples

```$ hexdumb <file path>```

prints the file given by `<file path>` in big-endian format in 3 four Byte wide columns.

```$ hexdumb -l <file path>```

does the same except prints in little-endian format.

```$ hexdumb <file path> -c <columns>```

changes the number of four Byte wide columns to `<columns>`

### Install

Navigate inside the repository folder (same directory that contains `makefile`) and compile the code with 

```$ make```

Optionally copy the executable to your current user's bin folder `~/.local/bin` with

```$ make install```
