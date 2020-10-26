# hdump

A unix hex-dump utility written in C.

## Features

* sane defaults: 16 columns, no ascii view
```sh
~ $ hdump some_file.txt
0000  74 68 69 73  20 69 73 20   6a 75 73 74  20 61 6e 20   
0010  65 78 61 6d  70 6c 65 2c   20 77 68 79  20 61 72 65   
0020  20 79 6f 75  20 6c 6f 6f   6b 69 6e 67  20 73 6f 20   
0030  63 6c 6f 73  65 6c 79 0a   00
...
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

* Calculates sane amount of padding for the position, so a small file *won't* have `000000010` etc in the first column, and there will always be enough padding to fir the size.
