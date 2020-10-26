# hdump

A unix hex-dump utility written in C.

## How to Build

### Requirements
* an OS with `<unistd.h>` and `<sys/stat.h>`
* a C compiler supporting C17
* `cmake`
* `make`
* (optional) `git`

### Building, step by step
*(if you have all requirements you can use the all-in-one command below)*

1. Clone this repository, with git or manually (git shown here)
```
$ git clone https://github.com/lionkor/hdump
$ cd hdump
```
2. run `cmake`, feel free to add any other common options
```
$ cmake -S . -B bin
```
3. run `make` - substitute the `5` with the number of cores make should use
```
$ make -j 5 -C bin
```
4. 
```
$ cd ..
```

Next up: [Installing](#installing)

### Building, all in one
copy, paste and run:
```
git clone https://github.com/lionkor/hdump && cd hdump && cmake -S . -B bin && make -j 5 -C bin && cd ..
```

Next up: [Installing](#installing)

### Installing

To install, consult your OS's documentation on the best way to do this. 
For most systems, symlinking or copying the file `hdump/bin/hdump` to `/bin`, `/usr/bin` or something similar is fine.

Example:

```
# ln -s /home/lion/src/hdump/bin/hdump /usr/bin/hdump
```

## Features

* sane defaults: 16 columns, no ascii view
```sh
~ $ hdump some_file.txt
0000  74 68 69 73  20 69 73 20   6a 75 73 74  20 61 6e 20   
0010  65 78 61 6d  70 6c 65 2c   20 77 68 79  20 61 72 65   
0020  20 79 6f 75  20 6c 6f 6f   6b 69 6e 67  20 73 6f 20   
0030  63 6c 6f 73  65 6c 79 0a   00
```

* setting column count, example with 8 columns, `-c <value>` or `--columns <value>`:
```sh
~ $ hdump -c 8 some_file.txt
0000  74 68 69 73  20 69 73 20   
0008  6a 75 73 74  20 61 6e 20   
0010  65 78 61 6d  70 6c 65 2c   
0018  20 77 68 79  20 61 72 65   
0020  20 79 6f 75  20 6c 6f 6f   
0028  6b 69 6e 67  20 73 6f 20   
0030  63 6c 6f 73  65 6c 79 0a   
0038  00  
```

* optional flag for ascii display, `-a` or `--ascii`:
```sh
~ $ hdump -a some_file.txt
0000  69 27 6d 20  61 6e 20 65   78 61 6d 70  6c 65 20 74    i'm an example t 
0010  65 78 74 2e  20 68 65 72   65 20 69 73  20 61 20 6c    ext. here is a l 
0020  69 6e 65 62  72 65 61 6b   3a 20 0a 61  6e 64 20 6e    inebreak:  and n 
0030  6f 77 20 73  6f 6d 65 20   6d 6f 72 65  20 74 65 78    ow some more tex 
0040  74 2e 20 79  61 79 21 0a   00                          t. yay!  
```

* sanitisation of special characters in ascii-view, they just silently turn into spaces.

* Calculates sane amount of padding for the position, so a small file *won't* have `000000010` etc in the first column, and there will always be enough padding to for the size.

## Examples

This 360 MB file example shows the padding for the positional column adjusting properly:

```
0000000000  2f 2a 20 41  20 73 70 6c   61 79 2d 74  72 65 65 20   
0000000010  64 61 74 61  74 79 70 65   2e 0a 20 20  20 43 6f 70   
0000000020  79 72 69 67  68 74 20 28   43 29 20 31  39 39 38 2d   
0000000030  32 30 32 30  20 46 72 65   65 20 53 6f  66 74 77 61 
...
0001426700  61 74 20 69  74 20 77 69   6c 6c 20 62  65 20 75 73   
0001426710  65 66 75 6c  2c 20 62 75   74 0a 57 49  54 48 4f 55   
...
```
etc.

Tiny file example, hdump was invoked with `hdump -a -c 8 file.txt`:
```
000  68 65 6c 6c  6f 20 77 6f    hello wo 
008  72 6c 64 0a  00             rld 
```
