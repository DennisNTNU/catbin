# catbin

A small command line program to print the contents of a file in a hexadecimal format to the console.

## Features

Choose whether the file should be printed in big-endian (Most significan Byte first) or little-endian (Least significant Byte first) format.

### Examples

```$ catbin <file path>```

prints the file given by `<file path>` in big-endian format in 3 four Byte wide columns.

```$ catbin -l <file path>```

does the same except prints in little-endian format.

```$ catbin <file path> -c <columns>```

changes the number of four Byte wide columns to <columns>
