1. createPageFile(char *fileName)
   File Creation: A file is opened using fopen with the "w+" mode, which allows for both reading and writing.
   Memory Allocation: A memory buffer is allocated using malloc to create a single page of size 4096 bytes.
   Initialize Page: The page is initialized to zero using memset.
   File Write: The page is written to the file using fwrite, making the file contain exactly one empty page.
   After writing, free is called to release the allocated memory, and fclose is used to close the file.

2. openPageFile(char *fileName, SM_FileHandle *fHandle)
    File Open: Opens the specified file using fopen in "r+" mode (read/write).
    File Length Calculation: Uses fseek to move to the end of the file and ftell to determine the file's size in bytes, which is then divided by PAGE_SIZE to get the total number of pages.

3. closePageFile(SM_FileHandle *fHandle)
    File Close: Closes the file using fclose to ensure all operations are complete and resources are released.

4. destroyPageFile(char *fileName)
   File Removal: Deletes the file from the file system using the remove function.

5. readBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
   Positioning in File: Uses fseek to move to the specific page in the file by calculating the byte offset as pageNum * PAGE_SIZE.
   File Read: Reads the page data into the memory buffer (memPage) using fread.

6. getBlockPos(SM_FileHandle *fHandle)
   Returns the current page position stored in curPagePos.

7. readFirstBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
   Reads the first page of the file by calling readBlock(0, fHandle, memPage).

8. readPreviousBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
  Reads the previous page relative to the current page position by calling readBlock(fHandle->curPagePos - 1, fHandle, memPage).

9. readCurrentBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
   Reads the block at the current page position stored in curPagePos using readBlock.


10. readNextBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
    Reads the next block relative to the current page position by calling readBlock(fHandle->curPagePos + 1, fHandle, memPage).


Functions used:
fopen: Opens a file for reading/writing or creates a new file.
malloc: Allocates memory dynamically, used for allocating space for pages.
memset: Initializes a memory block with a specific value (used to zero-fill pages).
fwrite: Writes data from memory into a file.
free: Releases dynamically allocated memory to prevent memory leaks.
fseek: Moves the file pointer to a specified position, used for navigating to the correct page in the file.
fclose: Closes the file, ensuring all file operations are complete.
remove: Deletes a file from the file system.
ftell: Returns the current file position, used to calculate the file length.
fread: Reads data from a file into memory.








