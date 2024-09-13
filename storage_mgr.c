#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage_mgr.h"
#include "dberror.h"

#define PAGE_SIZE 4096


void initStorageManager(void) {  
}


RC createPageFile(char *fileName) {
    FILE *file = fopen(fileName, "w+");
    if (file == NULL) return RC_FILE_NOT_FOUND;

    char *page = (char *)malloc(PAGE_SIZE);
    if (page == NULL) {
        fclose(file);
        return RC_WRITE_FAILED;
    }
    memset(page, 0, PAGE_SIZE);

    if (fwrite(page, 1, PAGE_SIZE, file) < PAGE_SIZE) {
        fclose(file);
        free(page);
        return RC_WRITE_FAILED;
    }

    free(page);
    fclose(file);
    return RC_OK;
}


RC openPageFile(char *fileName, SM_FileHandle *fHandle) {
    FILE *file = fopen(fileName, "r+");
    if (file == NULL) return RC_FILE_NOT_FOUND;

    fHandle->fileName = fileName;
    fHandle->mgmtInfo = file;

    fseek(file, 0, SEEK_END);
    long fileLength = ftell(file);
    fHandle->totalNumPages = fileLength / PAGE_SIZE;
    fHandle->curPagePos = 0;

    return RC_OK;
}


RC closePageFile(SM_FileHandle *fHandle) {
    FILE *file = (FILE *)fHandle->mgmtInfo;
    if (fclose(file) != 0) return RC_FILE_HANDLE_NOT_INIT;
    return RC_OK;
}


RC destroyPageFile(char *fileName) {
    if (remove(fileName) != 0) return RC_FILE_NOT_FOUND;
    return RC_OK;
}


RC readBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage) {
    if (pageNum < 0 || pageNum >= fHandle->totalNumPages) return RC_READ_NON_EXISTING_PAGE;

    FILE *file = (FILE *)fHandle->mgmtInfo;
    fseek(file, pageNum * PAGE_SIZE, SEEK_SET);

    size_t bytesRead = fread(memPage, 1, PAGE_SIZE, file);
    if (bytesRead < PAGE_SIZE) {
        return RC_READ_NON_EXISTING_PAGE;
    }

    fHandle->curPagePos = pageNum;
    return RC_OK;
}

int getBlockPos(SM_FileHandle *fHandle) {
    return fHandle->curPagePos;
}


RC readFirstBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) {
    return readBlock(0, fHandle, memPage);
}


RC readPreviousBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) {
    if (fHandle->curPagePos <= 0) return RC_READ_NON_EXISTING_PAGE;
    return readBlock(fHandle->curPagePos - 1, fHandle, memPage);
}

RC readCurrentBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) {
    return readBlock(fHandle->curPagePos, fHandle, memPage);
}


RC readNextBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) {
    if (fHandle->curPagePos >= fHandle->totalNumPages - 1) return RC_READ_NON_EXISTING_PAGE;
    return readBlock(fHandle->curPagePos + 1, fHandle, memPage);
}


