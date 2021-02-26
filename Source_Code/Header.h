#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<iostream>
#include<io.h>
#include<fcntl.h> 

#define MAXINODE 50

#define READ 1
#define WRITE 2

#define MAXFILESIZE 2048

#define REGULAR 1
#define SPECIAL 2

#define START 0
#define CURRENT 1
#define END 2

//Declaration of all structures used in project.

typedef struct superblock
{
	int TotalInodes;
	int FreeInode;
}SUPERBLOCK, *PSUPERBLOCK;

typedef struct inode
{
	char FileName[50];
	int InodeNumber;
	int FileSize;
	int FileActualSize;
	int FileType;					// If it is 0 means file is not there.
	char *Buffer;
	int LinkCount;
	int ReferenceCount;
	int permission;
	struct inode *next;
}INODE, *PINODE, **PPINODE;
//typedef struct inode INODE;
//typedef struct inode * PINODE;
//typedef struct inode ** PPINODE;

typedef struct filetable
{
	int readoffset;
	int writeoffset;
	int count;
	int mode;
	PINODE ptrinode;
}FILETABLE, *PFILETABLE;
//typedef struct filetable FILETABLE;
//typedef struct filetable * PFILETABLE;


typedef struct ufdt
{
	PFILETABLE ptrfiletable;
}UFDT;


//Declaration of all functions used in project.

void CreateDILB();

void InitialiseSuperBlock();

int GetFDFromName(char *);

PINODE Get_Inode(char *);

int CreateFile(char *, int);

int OpenFile(char *,int);

int LseekFile(int, int, int);

void DisplayHelp();

void CloseAllFile();

void ls_file();

int fstat_file(int);

void CloseFileByName(int);

int CloseFileByName(char *);

int rm_File(char *);

void man(char *);

int stat_file(char *);

int ReadFile(int, char *, int);

int WriteFile(int, char *, int);

int truncate_File(char *);

void BackupFS();