# LZW-compression
Implementation of a compression algorithm, LZW (Lempel-Ziv-Welch Algorithm).

The application supports both compression and decompression. 

|    Command    |       Definition      |
| ------------- | ----------------------|
| -c [FileName] | Compress File         |
| -d            | Decompress File       |
| -e            | Check encoding        |
| -h            | User commands (help)  |
| -q            | Exit the application  |

After finishing compressing, the application specifies the compression rate (percentage) and asks the user for a verbose display of the compression dictionary.
The text after decompression will be shown in the terminal.

It is worthy to note that decompression does not yet work on the given file. It will simply retain the encoding vector of the previous compression iteration.
Therefore, compressing multiple files can work, but only decompressing the last one is viable as of late. 

Decompression based on file input will be viable in the next change.
