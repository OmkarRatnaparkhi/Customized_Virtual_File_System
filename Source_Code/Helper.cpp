#include "Header.h"

UFDT UFDTArr[50];					//Array of ufdt(object of struct ufdt) created and each element of ufdt array is pointer which points to particular filetable.

SUPERBLOCK SUPERBLOCKobj;			//object of superblock struct created

PINODE head = NULL;					//Global pointer of inode


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function name : CreateDILB
//  Description :   It is used to create linked list of Disk inode list block
//  Autor : OMKAR NARENDRA RATNAPARKHI
//  Date :  3rd  November 2020
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CreateDILB()					//Creating linked list of inodes
{
	int i = 1;
	PINODE newn = NULL;
	PINODE temp = head;
	
	while (i<=MAXINODE)				//Loop iterates till specified inode[50] are created
	{
		newn = (PINODE)malloc(sizeof(INODE));
		
		newn->LinkCount = 0;
		newn->ReferenceCount = 0;
		newn->FileType = 0;
		newn->FileSize = 0;
		
		newn->Buffer = NULL;
		newn->next = NULL;
		
		newn->InodeNumber = i;
		
		if(temp == NULL)			//For first inode 
		{
			head = newn;
			temp = head;
		}
		else						//Second inode onwards
		{
			temp->next = newn;
			temp = temp->next;
		}
		i++;
	}
	printf("-----------------------------------------\n");
	printf("|	DILB created succesfully	|\n");
	printf("-----------------------------------------\n");
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function name : InitialiseSuperBlock
//  Description :   It is used to Initialise SuperBlock and Array of UFDT.
//  Autor : OMKAR NARENDRA RATNAPARKHI
//  Date :  3rd  November 2020
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InitialiseSuperBlock()
{
	int i = 0;

	SUPERBLOCKobj.TotalInodes = MAXINODE;						//initializing SuperBlock
	SUPERBLOCKobj.FreeInode = MAXINODE;
	printf("-----------------------------------------\n");
	printf("|	SuperBlock created succesfully	|\n");
	printf("-----------------------------------------\n");		
	
	while(i<MAXINODE)
	{
		UFDTArr[i].ptrfiletable = NULL;							//UFDTArr initialized with null
		i++;
	}
	printf("-----------------------------------------\n");
	printf("|	UFDT Array created succesfully	|\n");
	printf("-----------------------------------------\n");	
	
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function name : GetFDFromName
//  Description :   It is used to get file descriptor of that particular file.
//  Autor : OMKAR NARENDRA RATNAPARKHI
//  Date :  3rd  November 2020
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int GetFDFromName(char *name)
{
	int i = 0;
	
	while(i<50)
	{
		if(UFDTArr[i].ptrfiletable != NULL)
		{
			if(strcmp((UFDTArr[i].ptrfiletable->ptrinode->FileName),name)==0)			//comparing strings
			{
				break;
			}
			i++;		
		}
	}
	
	if(i == 50)
	{
		return -1;
	}
	else
	{
		return i;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function name : Get_Inode
//  Description :   It is used to get inode of file.
//  Autor : OMKAR NARENDRA RATNAPARKHI
//  Date :  3rd  November 2020
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PINODE Get_Inode(char *name)
{
	PINODE temp = head;
	int i =0;
	
	if(name == NULL)
	{
		return NULL;
	}
	
	while(temp != NULL)							//It is used to check whether the given file is already present or not
	{
		if(strcmp(name,temp->FileName) == 0)
		{
			break;
		}
		temp = temp->next;
	}
	
	return temp;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function name : CreateFile
//  Description :   It is used to create file with given permissions by user.
//  Autor : OMKAR NARENDRA RATNAPARKHI
//  Date :  4th  November 2020
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CreateFile(char *name,int permission)
{
	int i = 0;
	PINODE temp = head;
	
	if((name == NULL) || (permission == 0) || (permission>3))
	{
		return -1;
	}
	
	if(SUPERBLOCKobj.FreeInode == 0)
	{
		return -2;
	}
	
	(SUPERBLOCKobj.FreeInode)--;
	
	if(Get_Inode(name) != NULL)
	{
		return -3;
	}
	
	while(temp != NULL)														//Search empty inode
	{
		if(temp->FileType == 0)
		{
			break;
		}
		temp=temp->next; 
	}
	
	while(i<50)																//Search for empty entry from UFDT
	{
		if(UFDTArr[i].ptrfiletable == NULL)
		{
			break;
		}
		i++;
	}
	
	UFDTArr[i].ptrfiletable = (PFILETABLE)malloc(sizeof(FILETABLE));		//Memory allocation for filetable for particular file.
	
	//Initialization of filetable
	UFDTArr[i].ptrfiletable->count = 1;
	UFDTArr[i].ptrfiletable->mode = permission;
	UFDTArr[i].ptrfiletable->readoffset = 0;
	UFDTArr[i].ptrfiletable->writeoffset = 0;
	
	UFDTArr[i].ptrfiletable->ptrinode = temp;
	
	strcpy(UFDTArr[i].ptrfiletable->ptrinode->FileName,name);				//Memory allocation for inode
	UFDTArr[i].ptrfiletable->ptrinode->FileType = REGULAR;
	UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount = 1;
	UFDTArr[i].ptrfiletable->ptrinode->LinkCount = 1;
	UFDTArr[i].ptrfiletable->ptrinode->FileSize = MAXFILESIZE;
	UFDTArr[i].ptrfiletable->ptrinode->FileActualSize = 0;
	UFDTArr[i].ptrfiletable->ptrinode->permission = permission;
	UFDTArr[i].ptrfiletable->ptrinode->Buffer = (char*)malloc(MAXFILESIZE);
		
	return i;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function name : OpenFile
//  Description :   It is used to open file in particular mode.
//  Autor : OMKAR NARENDRA RATNAPARKHI
//  Date :  4th  November 2020
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int OpenFile(char *name,int mode)
{
	int i = 0;
	PINODE temp = NULL;
	
	if(name == NULL || mode <= 0)
	{
		return -1;
	}
	
	temp = Get_Inode(name);
	if(temp == NULL)
	{
		return -2;
	}
	
	if(temp->permission < mode)
	{
		return -3;
	}
	
	while(i<50)
	{
		if(UFDTArr[i].ptrfiletable == NULL)
		{
			break;
		}
		i++;
	}
	
	UFDTArr[i].ptrfiletable = (PFILETABLE)malloc(sizeof(FILETABLE));
	if(UFDTArr[i].ptrfiletable == NULL)
	{
		return -1;
	}
	UFDTArr[i].ptrfiletable->count = 1;
	UFDTArr[i].ptrfiletable->mode = mode;
	if(mode == READ + WRITE)
	{
		UFDTArr[i].ptrfiletable->readoffset = 0;
		UFDTArr[i].ptrfiletable->writeoffset = 0;
	}
	else if(mode == READ)
	{
		UFDTArr[i].ptrfiletable->readoffset = 0;
	}
	else if(mode == WRITE)
	{
		UFDTArr[i].ptrfiletable->writeoffset = 0;	
	}
	UFDTArr[i].ptrfiletable->ptrinode = temp;
	(UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)++;
	
	return i;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function name : LseekFile
//  Description :   It is used to change offset of the file.
//  Autor : OMKAR NARENDRA RATNAPARKHI
//  Date :  10th  November 2020
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int LseekFile(int fd, int size , int from)
{
	if((fd<0) || (from > 2))
	{
		return -1;
	}
	if(UFDTArr[fd].ptrfiletable == NULL)
	{
		return -1;
	}
	if((UFDTArr[fd].ptrfiletable->mode  == READ) || (UFDTArr[fd].ptrfiletable->mode == READ+WRITE))
	{
		if(from == CURRENT)
		{
			if(((UFDTArr[fd].ptrfiletable->readoffset) + size) > UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)
			{
				return -1;
			}
			if(((UFDTArr[fd].ptrfiletable->readoffset) + size) < 0)
			{
				return -1;
			}
			(UFDTArr[fd].ptrfiletable->readoffset) = (UFDTArr[fd].ptrfiletable->readoffset) + size; 
		}
		else if(from == START)
		{
			if(size > (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize))
			{
				return -1;
			}
			if(size < 0)
			{
				return -1;
			}
			(UFDTArr[fd].ptrfiletable->readoffset) = size;
		}
		else if(from == END)
		{
			if((UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size > MAXFILESIZE)
			{
				return -1;
			}
			if(((UFDTArr[fd].ptrfiletable->writeoffset) + size) < 0)
			{
				return -1;
			}
			(UFDTArr[fd].ptrfiletable->writeoffset) = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size;
		}
	}
	
	return 0;/////////////
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function name : DisplayHelp
//  Description :   It is used display help for user.
//  Autor : OMKAR NARENDRA RATNAPARKHI
//  Date :  1st  November 2020
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DisplayHelp()
{
	printf("------------------------------------------------------------------------\n");
	printf("ls : To List out all files\n");
	printf("clear : To clear console\n");
	printf("open : To open the file\n");
	printf("create : To create the file\n");
	printf("close : To close the file\n");
	printf("closeall : To close all opened files\n");
	printf("read : To Read the contents from file\n");
	printf("write : To Write contents into file\n");
	printf("exit : To Terminate file system\n");
	printf("stat : To Display information of file using name\n");
	printf("fstat :To Display information of file using file descriptor\n");
	printf("truncate : To Remove all data from file\n");
	printf("rm : To Delete the file\n");
	printf("------------------------------------------------------------------------\n");
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function name : CloseAllFile
//  Description :   It is used close all files which are opened.
//  Autor : OMKAR NARENDRA RATNAPARKHI
//  Date :  5th November 2020
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CloseAllFile()
{
	int i = 0;
	while(i<50)
	{
		if(UFDTArr[i].ptrfiletable != NULL)
		{
			UFDTArr[i].ptrfiletable->readoffset = 0;
			UFDTArr[i].ptrfiletable->writeoffset = 0;
			(UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)--;
			break;
		}
		i++;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function name : ls_file
//  Description :   It is used list all files.
//  Autor : OMKAR NARENDRA RATNAPARKHI
//  Date :  6th November 2020
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ls_file()
{
	int i = 0;
	PINODE temp = head;
	
	if(SUPERBLOCKobj.FreeInode == MAXINODE)
	{
		printf("Error : There are no files\n");
		return; 
	}
	
	printf("\nFile Name\tInode number\tFile size\tLink count\n");
	printf("--------------------------------------------------------\n");
	while(temp != NULL)
	{
		if(temp->FileType != 0)
		{
			printf("%s\t\t%d\t\t%d\n",temp->FileName,temp->InodeNumber,temp->FileActualSize,temp->LinkCount);
		}
		temp = temp->next;
	}
	printf("--------------------------------------------------------\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function name : fstat_file
//  Description :   It is used to give information about a file when file descriptor is given.
//  Autor : OMKAR NARENDRA RATNAPARKHI
//  Date :  6th November 2020
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int fstat_file(int fd)
{
	PINODE temp = head;
	int i = 0;
	
	if(fd<0)
	{
		return -1;
	}
	
	if(UFDTArr[fd].ptrfiletable == NULL)
	{
		return -2;
	}
	
	temp = UFDTArr[fd].ptrfiletable->ptrinode;
	
	printf("\n------------Statistical Information about file------------\n\n");
	printf("File name : %s\n",temp->FileName);
	printf("Inode Number : %d\n",temp->InodeNumber);
	printf("File size : %d\n",temp->FileSize);
	printf("Actual File size : %d\n",temp->FileActualSize);
	printf("Link count : %d\n",temp->LinkCount);
	printf("Reference Count : %d\n",temp->ReferenceCount);	

	if(temp->permission == 1)
	{
		printf("File Permission : Read Only\n");
	}
	else if(temp->permission == 2)
	{
		printf("File Permission : Write\n");
	}
	else if(temp->permission == 3)
	{
		printf("File Permission : Read & Write\n");
	}
	printf("-------------------------------------------------\n\n");
	
	return 0;		
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function name : CloseFileByName
//  Description :   It is used to close particular file when file descriptor is given.
//  Autor : OMKAR NARENDRA RATNAPARKHI
//  Date :  6th November 2020
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CloseFileByName(int fd)
{
	UFDTArr[fd].ptrfiletable->readoffset = 0;
	UFDTArr[fd].ptrfiletable->writeoffset = 0;
	(UFDTArr[fd].ptrfiletable->ptrinode->ReferenceCount)--;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function name : CloseFileByName
//  Description :   It is used to close particular file when file name is given.
//  Autor : OMKAR NARENDRA RATNAPARKHI
//  Date :  6th November 2020
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CloseFileByName(char *name)
{
	int i = 0;
	i = GetFDFromName(name);
	if(i == -1)
	{
		return -1;
	}
	
	UFDTArr[i].ptrfiletable->readoffset = 0;
	UFDTArr[i].ptrfiletable->writeoffset = 0;
	(UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)--;
	
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function name : rm_File
//  Description :   It is used to delete file.
//  Autor : OMKAR NARENDRA RATNAPARKHI
//  Date :  10th November 2020
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//rm_File("Demo.txt")
int rm_File(char *name)
{
	int fd = 0;
	
	fd = GetFDFromName(name);
	if(fd == -1)
	{
		return -1;
	}
	
	(UFDTArr[fd].ptrfiletable->ptrinode->LinkCount)--;
	
	if(UFDTArr[fd].ptrfiletable->ptrinode->LinkCount == 0)
	{
		UFDTArr[fd].ptrfiletable->ptrinode->FileType = 0;
		//free(UFDTArr[fd].ptrfiletable->ptrinode->Buffer);
		free(UFDTArr[fd].ptrfiletable);
	}
	
	UFDTArr[fd].ptrfiletable = NULL;
	(SUPERBLOCKobj.FreeInode)++;
	
	return 0;/////////////
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function name : man
//  Description :   It is used to display manuals of commands.
//  Autor : OMKAR NARENDRA RATNAPARKHI
//  Date :  15th November 2020
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void man(char *name)
{
	if(name == NULL)
	{
		return; 
	}
	
	if(strcmp(name,"create")==0)
	{
		printf("Description : Used to create new regular file\n");
		printf("Permission 1 to read\t");
		printf("Permission 2 to write\t");
		printf("Permission 3 to read&write\n");
		printf("Usage : create File_name Permission\n");
	}
	else if(strcmp(name,"read") == 0)
	{
		printf("Description : Used to read data from regular file\n");
		printf("Usage : read File_name No_Of_Bytes_To_Read\n");
	}
	else if(strcmp(name,"write") == 0)
	{
		printf("Description : Used to write into regular file\n");
		printf("Usage : write File_name\n After this enter the data that we want to write\n");
	}
	else if(strcmp(name,"ls") == 0)
	{
		printf("Description : Used to list all information of files\n");
		printf("Usage : ls\n");
	}
	else if(strcmp(name,"stat") == 0)
	{
		printf("Description : Used to display information of file\n");
		printf("Usage : stat File_name\n");
	}
	else if(strcmp(name,"fstat") == 0)
	{
		printf("Description : Used to display information of file\n");
		printf("Usage : fstat File_Descriptor\n");
	}
	else if(strcmp(name,"truncate") == 0)
	{
		printf("Description : Used to remove data from file\n");
		printf("Usage : truncate File_name\n");
	}	
	else if(strcmp(name,"open") == 0)
	{
		printf("Description : Used to open existing file\n");
		printf("Usage : open File_name mode\n");		
	}
	else if(strcmp(name,"close")==0)
	{
		printf("Description : Used to close opened file\n");
		printf("Usage : close File_name\n");
	}
	else if(strcmp(name,"closeall")==0)
	{
		printf("Description : Used to close all opened file\n");
		printf("Usage : closeall\n");
	}
	else if(strcmp(name,"lseek") == 0)
	{
		printf("Description : Used to change file offset\n");
		printf("Usage : lseek File_Name ChangeInOffset StartPoint\n");		
	}
	else if(strcmp(name,"rm")==0)
	{
		printf("Description : Used to delete the file");
		printf("Usage : rm File_Name\n");
	}
	else
	{
		printf("ERROR : No manual entry available");
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function name : stat_file
//  Description :   It is used to give information about a file when file name is given.
//  Autor : OMKAR NARENDRA RATNAPARKHI
//  Date :  11th November 2020
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int stat_file(char *name)
{
	PINODE temp = head;
	int i = 0;
	
	if(name == NULL)
	{
		return -1;
	}
	
	while(temp != NULL)
	{
		if(strcmp(name,temp->FileName) == 0)
		{
			break;
		}
		temp = temp->next;
	}
	
	if(temp == NULL)
	{
		return -2;
	}
	
	printf("\n------------Statistical Information about file------------\n\n");
	printf("File name : %s\n",temp->FileName);
	printf("Inode Number : %d\n",temp->InodeNumber);
	printf("File size : %d\n",temp->FileSize);
	printf("Actual File size : %d\n",temp->FileActualSize);
	printf("Link count : %d\n",temp->LinkCount);
	printf("Reference Count : %d\n",temp->ReferenceCount);
	
	if(temp->permission == 1)
	{
		printf("File Permission : Read Only\n");
	}
	else if(temp->permission == 2)
	{
		printf("File Permission : Write\n");
	}
	else if(temp->permission == 3)
	{
		printf("File Permission : Read & Write\n");
	}
	printf("-------------------------------------------------\n\n");
	
	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function name : ReadFile
//  Description :   It is used to read contents of file.
//  Autor : OMKAR NARENDRA RATNAPARKHI
//  Date :  19th November 2020
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int ReadFile(int fd, char *arr,int isize)
{
	int read_size = 0;
	
	if(UFDTArr[fd].ptrfiletable == NULL)
	{
		return -1;
	}
	
	if(UFDTArr[fd].ptrfiletable->mode != READ && UFDTArr[fd].ptrfiletable->mode != READ+WRITE)
	{
		return -2;
	}

	if(UFDTArr[fd].ptrfiletable->readoffset == UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)
	{
		return -3;
	}	
	
	if(UFDTArr[fd].ptrfiletable->ptrinode->FileType != REGULAR)
	{
		return -4;
	}
	
	read_size = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) - (UFDTArr[fd].ptrfiletable->readoffset); ////////nahi kalali hi line
	
	if(read_size < isize)
	{
		strncpy(arr,(UFDTArr[fd].ptrfiletable->ptrinode->Buffer) +(UFDTArr[fd].ptrfiletable->readoffset),read_size);
	
		UFDTArr[fd].ptrfiletable->readoffset = UFDTArr[fd].ptrfiletable->readoffset + read_size;
	}
	else
	{
		strncpy(arr,(UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->readoffset),isize);
		
		(UFDTArr[fd].ptrfiletable->readoffset) = (UFDTArr[fd].ptrfiletable->readoffset) + isize;	
	}
	
	return isize;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function name : WriteFile
//  Description :   It is used to write contents in the file.
//  Autor : OMKAR NARENDRA RATNAPARKHI
//  Date :  23 November 2020
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int WriteFile(int fd , char *arr, int isize)
{
	if(((UFDTArr[fd].ptrfiletable->mode) != WRITE) && ((UFDTArr[fd].ptrfiletable->mode) != READ+WRITE))
	{
		return -1;
	}
	
	if(((UFDTArr[fd].ptrfiletable->ptrinode->permission) != WRITE) && ((UFDTArr[fd].ptrfiletable->ptrinode->permission) != READ+WRITE))
	{
		return -1;
	}
	
	if((UFDTArr[fd].ptrfiletable->writeoffset) == MAXFILESIZE)
	{
		return -2;
	}
	
	if((UFDTArr[fd].ptrfiletable->ptrinode->FileType) != REGULAR) 
	{
		return -3;
	}
	
	//Data copied into buffer
	strncpy((UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->writeoffset),arr,isize);
	
	(UFDTArr[fd].ptrfiletable->writeoffset) = (UFDTArr[fd].ptrfiletable->writeoffset)+ isize;
	
	(UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + isize;
	
	return isize;
}	


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function name : truncate_File
//  Description :   It is used to truncate particular file.
//  Autor : OMKAR NARENDRA RATNAPARKHI
//  Date :  23 November 2020
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int truncate_File(char *name)
{
	int fd = GetFDFromName(name);
	if(fd == -1)
	{
		return -1;
	}
	
	memset(UFDTArr[fd].ptrfiletable->ptrinode->Buffer,0,1024);
	UFDTArr[fd].ptrfiletable->readoffset = 0;
	UFDTArr[fd].ptrfiletable->writeoffset = 0;
	UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize = 0;

	return 0;/////////////
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function name : BackupFS
//  Description :   It is used to take backup of this customized virtual file system.
//  Autor : OMKAR NARENDRA RATNAPARKHI
//  Date :  15 January 2021
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BackupFS()
{
    int fd = 0;
    // Store the address if linkelist head
    PINODE temp = head;
    
    // Create new file for backup
    fd = creat("Backup_of_CVFS.txt",0777);
    if(fd == -1)
    {
        printf("Unable to create the file");
        return;
    }
    
    // Travel the inodes linkedlist
    while(temp != NULL)
    {
        // Check whether file is existing
        if(temp->FileType != 0)
        {
            // Write the inode into the backp files
            write(fd,temp,sizeof(INODE));
            // Write the data of file into the backup file
            write(fd,temp->Buffer, MAXFILESIZE);
        }
        // Incerment the pointer by one
        temp = temp->next;
    }
}


