#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<math.h>

#include "storage_mgr.h"

FILE *filePointer;
void initStorageManager (void) {
	// Initialising file pointer i.e. storage manager.
	filePointer = NULL;
}

RC createPageFile (char *fileName) {

	FILE *fp;
	fp = fopen(fileName, "w+");
	if(fp == NULL) {
		//Return fail message.
		return RC_FILE_CREATION_FAILED;
	} else {

         //inbuilt C library used to set the file position of the stream to the given offset.
		 fseek(fp,PAGE_SIZE,SEEK_SET);
		 fputc('\0',fp);

		 //Buffers are flushed using the close function.
		 fclose(fp);
		 //Returns OK is success.
		 return RC_OK;
	}
}

RC openPageFile (char *fileName, SM_FileHandle *fHandle) {

	FILE *fp;
	fp = fopen(fileName, "rb+");
	if(fp != NULL)
	{
		//set's fhandle filename to the current filename.
		fHandle->fileName = fileName;
		//set's fhandle current page position to start (0).
		fHandle->curPagePos = 0;
		//gets the file position from the given offset.
		fseek(fp,0,SEEK_END);
		//set's total number of pages
		fHandle->totalNumPages = (ftell(fp) / PAGE_SIZE );
		fclose(fp);
		//return success message.
		return RC_OK;
	}
	else{
		//return error message.
		return RC_FILE_NOT_FOUND;
	}
}

RC closePageFile (SM_FileHandle *fHandle) {
	//checks file status and close the file
	FILE *fp = fopen(fHandle->fileName,"r");
		int status = fseek(fp,0,SEEK_SET);
		if(status == 0)
		{
			fclose(fp);
			fp = NULL;
		return RC_OK;
		}
		return RC_FILE_NOT_FOUND;
}


RC destroyPageFile (char *fileName) {
	FILE *fp;
	 //opens the file in read mode.
	fp = fopen(fileName, "r");

	//If null then returns error message.
	if(fp == NULL){
		fclose(fp);
		return RC_FILE_NOT_FOUND;
	}
	else{
		fclose(fp);
		//removes the file
		int deleteStatus = remove(fileName);
		if(deleteStatus == 0){
		   return RC_OK;
	    }
		else{
			return RC_FILE_NOT_FOUND;
		}

	}
}

RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage) {
    if (pageNum > fHandle->totalNumPages || pageNum < 0){
        return RC_READ_NON_EXISTING_PAGE;
    }
    FILE *fp;
    fp = fopen(fHandle->fileName,"r");
    if (fp==NULL){
        fclose(fp);
        return RC_FILE_NOT_FOUND;
    } else {
        int isSeekSuccess = fseek(fp, (pageNum * PAGE_SIZE), SEEK_SET);
        if(isSeekSuccess == 0) {
		fread(memPage, PAGE_SIZE, sizeof(char), fp);
                fHandle->curPagePos = ftell(fp);
                fclose(fp);
    	        return RC_OK;
        } else {
            return RC_READ_NON_EXISTING_PAGE;
        }
    }
}

int getBlockPos(SM_FileHandle *fHandle)
{
	return fHandle->curPagePos;
}

RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	return readBlock(0, fHandle, memPage);
}

RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage) {
	        int previousBlock = getBlockPos(fHandle) - 1;
	        return readBlock(previousBlock,fHandle,memPage);
}

RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
    int currentBlock = fHandle->curPagePos / PAGE_SIZE;
	return readBlock(currentBlock,fHandle,memPage);
}

RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
    int nextBlock = getBlockPos(fHandle) + 1;
	return readBlock(nextBlock,fHandle,memPage);
}

RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
    int lastBlock= fHandle->totalNumPages -1;
	return readBlock(lastBlock,fHandle,memPage);
}

RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage) {
	FILE *fp;
	if (pageNum > fHandle->totalNumPages || pageNum < 0)
        return RC_WRITE_FAILED;
	fp = fopen(fHandle->fileName, "r+");
	if(fp == NULL)
		return RC_FILE_NOT_FOUND;
	int isSeekSuccess = fseek(fp, (pageNum * PAGE_SIZE), SEEK_SET);
	if(isSeekSuccess == 0) {
		fwrite(memPage, sizeof(char), strlen(memPage), fp);
		fHandle->curPagePos = ftell(fp);
		fclose(fp);
	} else {
		return RC_WRITE_FAILED;
	}
	return RC_OK;
}

RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage) {
	int currentPage = fHandle->curPagePos / PAGE_SIZE;
	fHandle->totalNumPages++;
	return writeBlock(currentPage, fHandle, memPage);
}


RC appendEmptyBlock (SM_FileHandle *fHandle) {
	    createPageFile(fHandle->fileName);
		fHandle->totalNumPages = fHandle->totalNumPages + 1;
		fHandle->curPagePos = fHandle->totalNumPages - 1;
		return RC_OK;
}

RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle) {
	    FILE *fp;
	    int newBlocks;
	    char *temp_memory;
	    if(numberOfPages>fHandle->totalNumPages)
	    {
	        fp=fopen(fHandle->fileName,"r+");
	        newBlocks = numberOfPages - fHandle->totalNumPages;
	        fseek(fp,((numberOfPages) * PAGE_SIZE),SEEK_SET);
	        fseek(fp,((fHandle->totalNumPages - numberOfPages)*PAGE_SIZE),SEEK_CUR);
	        temp_memory = malloc(((newBlocks) * PAGE_SIZE));
	        fwrite(temp_memory,1,((newBlocks) * PAGE_SIZE) ,fp);
	        fHandle->totalNumPages = fHandle->totalNumPages + newBlocks;
	        fclose(fp);
	        free(temp_memory);
	        return RC_OK;
	    }
	    else{
	        return RC_READ_NON_EXISTING_PAGE;
	    }
}
