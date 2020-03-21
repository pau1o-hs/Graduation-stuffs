# Getting bits from memory card

Every JPEG image starts with the same 4 bits:
0xff 0xd8 0xff 0xe

This means that you can find this byte pattern in an image
whatever, it has a huge chance that the entire contents of a JPEG file.

The program consider that your images are stored continuously in memory (the beginning of one file is exactly the end of another).
And is a 512 bytes block size.

## Input
```
./code < binary-file-with-image
```

## Output

```
000.jpg
001.jpg
002.jpg
...
050.jpg
...
150.jpg
...
```
