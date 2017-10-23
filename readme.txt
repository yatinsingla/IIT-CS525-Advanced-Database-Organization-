/********************************************************/
                   Team Information:
/********************************************************/
Group No: 5 
Members:
1) Manmeet Singh Arora (A20387916)
2) Maneesha Satyanarayanan (A20384410)
3) Naveenkumar Sivasubramanian (A20378088)
4) Yatin Singla (A20392685)

**********************************************************

Steps to execute:

1) Change the directory to /home/class/fall-17/cs525/ysingla/cs525_assignment1_files in the Terminal.
2) Use "make clean" command to remove the existing .o files.
3) Use "make" command to execute all the project files. 
4) Type "make run_test" to run "test_assign1_1.c" file.

**********************************************************


Functions used to manipulate page files:

The below functions are used to create, open and close files.
The file is first created and opened which allows the read and write function to perform for the storage manager followed by the close functionality.


initStorageManager(void)
=========================
•	This function is used to initialize the storage manager. 
•	The global object created for the file stream is assigned to NULL (initialized) in this method.


createPageFile(char *fileName)
===============================
•	Function used to create the file name as in the argument passed to it.
•	The function fopen is used in C for creating a file with two arguments file name followed by the mode. 
•	Here “w+” is used to open the file in read/write mode and truncate to zero length. 
•	If the file is not found we return “RC_FILE_NOT_FOUND” as an acknowledgement of error else we create/allocate the memory and return RC_OK.
•	Inbuilt C library “fseek (FILE *stream, long int offset, int whence)” used to set the file position of the stream to the given offset.
•	The arguments (filepage, PAGE_SIZE, SEEK_SET) are: 
        stream – “filepage “ (This is the pointer to a FILE object that identifies the stream).
        offset – “PAGE_SIZE” (This is the number of bytes to offset from whence).
        whence – “SEEK_SET “ which tends to Beginning of file by default (This is the position from where offset is added. It is specified by one of the following constants).

		
openPageFile(char *fileName, SM_FileHandle *fHandle)
====================================================
•	Opens the file with the fopen() function in read mode.
•	If the file pointer is not equal to null then the file handle's filename is assigned to the current file name and the current position is set to the start of the page.
•	We use the same fseek function as above to set the file position of the stream to the given offset. But the arguments are changed according to the openPage function as “fseek(filePage,0,SEEK_END)” where “0” is the offset and SEEK_END which tends to End of file is the whence.
•	The total number of pages in the file handle’s is calculated using (ftell(filePage) / PAGE_SIZE ) where ftell(filePage) returns the current file position of the given stream.
•	“RC_OK” is returned as a token of success acknowledgement.
•	If the file pointer is equal to null (i.e. else case) “RC_FILE_NOT_FOUND” error is returned.


closePageFile (SM_FileHandle *fHandle)
=======================================
•	Opens the current file pointer and checks for the status of the file.
•	If the file pointer is initialized (not equal to NULL), the initilized file pointer is closed by assigining it to NULL. 
•	Returns RC_OK acknowledgement at the end.


destroyPageFile (char *fileName)
================================
•	If the file pointer is “NULL” then “RC_FILE_NOT_FOUND”  error message is passed.
•	Else if the file exists then the file is removed using the remove() function and “RC_OK” message is passed.


readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
=======================================================================
•	In this block we first check if the page number is greater than the total number of pages or less than 0 (i.e. we check if page number is valid or not). If this is the case then then it returns an error code 'RC_READ_NON_EXISTING_PAGE'.
•	Next we open the file stream in read mode which creates an empty file for reading only.
•	Check if the file was successfully opened i.e. the pointer to the page file is available.
	•	If not return an error 'RC_FILE_NOT_FOUND'
•	Set the pointer position of the file stream to navigate to the given location by seeking(fseek()).
•	Position is calculated by multiplying the page number with page size.
•	Seeking is successful if fseek() is equal to zero, if unsuccessful then we return an error 'RC_READ_NON_EXISTING_PAGE'
•	If successful we read the content from page number and store it in the location pointed by memPage parameter.
•	Then we set the current page position to the pointer position of the file stream.
•	At last we close the file stream so that all the buffers are flushed.


getBlockPos(SM_FileHandle *fHandle)
===================================
•	It returns the current position retrieved from the file handle i.e from the fHandle parameter passed.


readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
==============================================================
•	This function redirects to readBlock() function with first block i.e. with pageNum = 0


readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
=================================================================
•	Here first calculate the current page number by dividing current page postion with page size.
•	Then redirects to readBlock() function with previous block i.e. with pageNum as currentPagePosition - 1


readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
================================================================
•	Here again first calculate the current page number by dividing current page postion with page size.
•	Then redirects to readBlock() function with current block i.e with pageNum as currentPagePosition which was calculated in above step.


readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
=============================================================
•	Here again first calculate the current page number by dividing current page postion with page size.
•	Then redirects to readBlock() function with next block i.e with pageNum as currentPagePosition + 1.


readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
=============================================================
•	Redirects with last block to readBlock() function with pageNum equal to total number of pages.


writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
=======================================================================
•	Checks for the condition if the 'pageNum' parameter is a value less than zero or greater than the total number of pages in the file 'totalNumPages' and returns an error code 'RC_WRITE_FAILED' if the condition is satisfied.
•	Creates a file pointer and opens the file with fopen() in read&write mode.
•	If the file does not exist, i.e file pointer is equal to NULL, an error code 'return RC_FILE_NOT_FOUND' is returned.
•	If the file exists, the file position on the stream is set using the fseek() function where (pageNum * PAGE_SIZE) bytes is the offset and  SEEK_SET is the position from which to offset, which is the file beginning.
•	If the fseek() is successful, the below operations are performed and 'RC_OK' is returned.
	•	The contents from memPage are writen to the file using fwrite() function.
	•	The file handle's 'curPagePos' is updated to the latest position value, that is returned by the ftell() function and the file is closed.	
•	Incase of an unsuccessful fseek, error code 'RC_WRITE_FAILED' is returned.


writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
=================================================================
•	A variable 'currentPage' is initialized to curPagePos / PAGE_SIZE.
•	The writeBlock function is called with 'currentPage' as the parameter passed for 'pageNum'. 


appendEmptyBlock (SM_FileHandle *fHandle)
=========================================
•	An empty block is appended by calling the createPageFile function with the fileName as parameter.
•	The totalNumPages is increased by 1 and the curPagePos is reduced to (totalNumPages - 1) due to adding a new block with a 'RC_OK' returned.


ensureCapacity (int numberOfPages, SM_FileHandle *fHandle)
==========================================================
•	The total number of pages in the file is checked against the numberOfpages(required number of pages).
•	If the total number of pages in the file is less than the required, the below operations are performed, else error 'RC_READ_NON_EXISTING_PAGE' is returned.
	•	The file is opened in the read-write mode and 'newBlocks' - the number of additional pages to be added is computed as numberOfPages - fHandle's totalNumPages.
	•	The File position is offset to (numberOfPages) * PAGE_SIZE) bytes from the file beginning and further offset to ((fHandle->totalNumPages - numberOfPages)*PAGE_SIZE) bytes from the current position.
	•	'PAGE_SIZE'(size of a page) amount of memory corresponding to the number of pages to be added is allocated using the malloc function and written to the file.
	•	The totalnumber of pages in the file's fhandle is updated to the new value and the file is closed and memory is freed. 



