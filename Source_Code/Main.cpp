#include "Header.h"

//Enter point function
int main()
{
	
	char *ptr = NULL;
	int ret = 0, fd = 0, count = 0;
	char command[4][80],str[80],arr[1024];
	
	printf("================================================================================================================\n");
	printf("				Customized Virtual File System Application Started\n");
	printf("================================================================================================================\n\n");

	InitialiseSuperBlock();
	CreateDILB();				//function call to create the linked list of inodes.
	
	while(1)
	{
		fflush(stdin);
		strcpy(str,"");
		
		printf("\nMy VFS :>");
		
		fgets(str,80,stdin); //scanf("%[^'\n']s",str);			Accepting command from user
	
		count = sscanf(str,"%s %s %s %s",command[0],command[1],command[2],command[3]);  //Breaking command into tokens
			
			if(count == 1)
			{
				if(strcmp(command[0],"ls")==0)
				{
					ls_file();
				}
				else if(strcmp(command[0],"closeall")==0)
				{
					CloseAllFile();
					printf("All files closed successfully\n");
					continue;
				}
				else if(strcmp(command[0],"clear") == 0)
				{
					system("cls");
					continue;
				}
				else if(strcmp(command[0],"help") == 0)
				{
					DisplayHelp();
					continue;
				}
				else if(strcmp(command[0],"exit") == 0)
				{
					printf("================================================================================================================\n");
					printf("				    Terminating Customized Virtual File System\n");
					printf("================================================================================================================\n\n");
					break;
				}
				else
				{
					printf("\nERROR : Command not found!!\n");
					continue;
				}
			}
			else if(count == 2)
			{
				if(strcmp(command[0],"stat")==0)
				{
					ret = stat_file(command[1]);
					if(ret == -1)
					{
						printf("ERROR : Incorrect parameters\n");
					}
					if(ret == -2)
					{
						printf("ERROR : There is no such file\n");
					}
					continue;
				}
				else if(strcmp(command[0],"fstat")==0)
				{
					ret = fstat_file(atoi(command[1]));
					if(ret == -1)
					{
						printf("ERROR : Incorrect parameters\n");
					}
					if(ret == -2)
					{
						printf("ERROR : There is no such file\n");
					}
					continue;					
				}
				else if(strcmp(command[0],"close")==0)
				{
					ret = CloseFileByName(command[1]);
					if(ret == -1)
					{
						printf("ERROR : There is no such file\n");
					}
					continue;
				}
				else if(strcmp(command[0],"rm")==0)
				{
					ret = rm_File(command[1]);
					if(ret == -1)
					{
						printf("ERROR : There is no such file\n");
					}
					continue;
				}
				else if(strcmp(command[0],"man")==0)
				{
					man(command[1]);
				}
				else if(strcmp(command[0],"write") == 0)
				{
					fd = GetFDFromName(command[1]);
					if(fd == -1)
					{
						printf("Error : Incorrect parameters\n");
						continue;
					}
					printf("Enter the data : \n");
					scanf("%[^\n]",arr);
					
					ret = strlen(arr);
					if(ret == 0)
					{
						printf("Error : Incorrect parameters\n");
						continue;
					}
					ret = WriteFile(fd,arr,ret);
					if(ret == -1)
					{
						printf("ERROR : Permission denied\n");
					}
					if(ret == -2)
					{
						printf("ERROR : There is no sufficient memory to write\n");
					}
					if(ret == -3)
					{
						printf("ERROR : It is not regular file\n");
					}
					if(ret > 0)
					{
						printf("Data succesfully written in the file");
					}
				}
				else if(strcmp(command[0],"truncate")==0)
				{
					ret = truncate_File(command[1]);
					if(ret == -1)
					{
						printf("Error : Incorrect parameters\n");
					}
				}
				else
				{
						printf("\nERROR : Command not found!!\n");
						continue;
				}
			}
			else if(count == 3)
			{
				if(strcmp(command[0],"create") == 0)
				{
					ret = CreateFile(command[1],atoi(command[2]));			//user is giving in string format hence atoi means converting permission from ascii to integer
					if(ret >= 0)
					{
						printf("File is succesfully created with file descriptor : %d\n",ret);
					}
					if(ret == -1)
					{
						printf("ERROR : Incorrect parameters\n");
					}
					if(ret == -2)
					{
						printf("ERROR : There is no inodes\n");
					}
					if(ret == -3)
					{
						printf("ERROR : File already exists\n");
					}
					if(ret == -4)
					{
						printf("ERROR : Memary allocation failure\n");
					}
					continue;
				}
				else if(strcmp(command[0],"open") == 0)
				{
					ret = OpenFile(command[1],atoi(command[2]));
					if(ret >= 0)
					{
						printf("File is succesfully opened with file descriptor : %d\n",ret);
					}
					if(ret == -1)
					{
						printf("ERROR : Incorrect parameters\n");
					}
					if(ret == -2)
					{
						printf("ERROR : File not present\n");
					}
					if(ret == -3)
					{
						printf("ERROR : Permission denied\n");
					}
					continue;
				}
				else if(strcmp(command[0],"read")==0)
				{
					fd = GetFDFromName(command[1]);
					if(fd == -1)
					{
						printf("Error : Incorrect parameter\n");
						continue;
					}
					ptr = (char *)malloc(sizeof(atoi(command[2]))+1);
					if(ptr == NULL)
					{
						printf("Error : Memory allocation failure\n");
						continue;
					}
					ret = ReadFile(fd,ptr,atoi(command[2]));
					if(ret == -1)
					{
						printf("ERROR : File not existing\n");
					}
					if(ret == -2)
					{
						printf("ERROR : Permission denied\n");
					}				
					if(ret == -3)
					{
						printf("ERROR : Reached at end of file\n");
					}
					if(ret == -4)
					{
						printf("ERROR : It is not regular file\n");
					}	
					if(ret == 0)
					{
						printf("ERROR : File empty\n");
					}
					if(ret > 0)
					{
						write(2,ptr,ret);
					}
					continue;
				}
				else
				{
					printf("\nERROR : Command not found!!\n");
					continue;
				}
			}
			else if(count == 4)
			{
				if(strcmp(command[0],"lseek") == 0)
				{
					fd = GetFDFromName(command[1]);
					if(fd == -1)
					{
						printf("Error :Incorrect parameter\n");
						continue;
					}
					ret = LseekFile(fd,atoi(command[2]),atoi(command[3]));
					if(ret == -1)
					{
						printf("Error : Unable to perform lseek\n");
					}
				}
				else
				{
					printf("\nERROR : Command not found!!\n");
					continue;
				}
			}
			else
			{
				printf("\nERROR : Command not found!!\n");
				continue;
			}
	}
	BackupFS();
	return 0;
}