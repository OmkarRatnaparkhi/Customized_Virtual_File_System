# Customized_Virtual_File_System

**Description :**
* This project is used to create customized file system which can work on Windows NT platform. We are going to implement unix file system. This project is used to emulate all functionalities provided by File systems.
* Known as customized virtual files system:-<br />
Customised : Because its designed is dependent on the users requirement<br />
Virtual : Because it is used in the RAM. <br />
File System : Way of storing and retriving the files. <br />

**Platform required :**
Windows NT platform OR Linux Distributions

**Hardware requirement :**
Minimum 512 MB RAM

**Architectural requirement :**
Intel 32 bit processor

**User Interface :**
Command User Interface

**SDK used :**
None

**Technology used :**
C programming

# About Virtual File System :
* In this project we emulate all data structures which are used by operating system to
manage File system oriented tasks.
* As the name suggest its virtual because we maintain all records in Primary storage.
* In this project we create all data structures which required for File Subsystems as
Inode Inode Table, File Table, UAREA, User File Descriptor Table, Super block, Disk
Inode List Block, Data Block, Boot Block etc.
* We provide all implementations of necessary system calls and commands of File
subsystem as Open, Close, Read, Write, Lseek, Create, RM, LS, Stat, Fstat etc.
* While providing the implementations of all above functionality we use our own data
structures by referring Algorithms of UNNIX operating system.
* By using this project we can get overview of UFS (UNIX File System) on any platform.

# What is File and File System ?
* File is unformated uniform stream of bytes.
* File system is the way of storing and retriving the files into the harddisk.

# Important System calls of File subsystems :
* open() : It is used to open existing file.
* close() : It is used to close already opened file.
* read() : It isused to read the data from file.
* write() : It is used to write data into the file
* lseek() : It is used to change the offset of the file.
* creat() : It is used to create new reular file.

# Diagramatic representation of File System 

<p align="center">
  <img src="https://github.com/OmkarRatnaparkhi/Customized_Virtual_File_System/blob/main/Assets/Diagram1.png" alt="Diagram1">
</p>

# RAM Section :
- **u AREA :**<br />
The u area contains information specific to the process (e.g., open files, current directory, signal actions, accounting information) and a system stack segment for process use. u Area is such region by using which kernel can access any process.

 - **File descriptors :**<br />
In Unix and related computer operating systems, a file descriptor is an abstract indicator used to access a file or other input/output resourc. A file descriptor is a non-negative integer, generally represented in the C programming language as the type int (negative values being reserved to indicate "no value" or an error condition).Normally, a process inherits three open file descriptors when it is started by the shell: descriptor 0 is standard input, the file from which the process takes its input; descriptor 1 is standard output, the file to which the process writes its output; and descriptor 2 is standard error, the file to which the process writes error messages and notification of exceptional or abnormal conditions. In an interactive shell or program, these three descriptors are normally connected to the terminal. In the stdio library, these descriptors correspond to the file streams stdin, stdout, and stderr.

 - **UFDT :**<br />
There is a File Descriptor table per process in the u area. A file descriptor entry contains a pointer to a file table entry.

 - **File Table :**<br />
The system needs to keep track of each "connection" to a file. Since the same file may have many open connections, this "connection" is truly distinct from the concept of a file. Since distinct processes may even share a connections (say two processes wanted to write to the same log file, each adding new data to the end of the file), the "connection" information cannot be kept inside the process table. So the kernel keeps a data structure called the system open-file table which has an entry for each connection. Each entry contains the connection status, e.g. read or write, the current offset in the file, and a pointer to a vnode, which is the OS's structure representing the file, irrespective of where in the file you may currently be looking.

 - **Inode :**<br />
Inode is structure which holds information about file such as the access mode (read, write, execute permissions), ownership, file type, file size, group, number of links, etc. Each inode is identified by an integer number. An inode is assigned to a file when it is created. Whenever a file is opened, its corresponding Inode is copied in main memory from the disk. So the inode residing on the disk is called disk inode & the one on main memory is called Incore inode.

 - **Incore Inode Table :**<br />
The kernel keeps yet another table, this table contains a copy of the inode for the file and refernce to that file.

# Hard Disk Section :

<p align="center">
  <img src="https://github.com/OmkarRatnaparkhi/Customized_Virtual_File_System/blob/main/Assets/Diagram2.png">
</p>

 - **Boot Block :**<br />
A dedicated block usually at the beginning (first block) of a storage medium that holds special data used to start a system. The boot block contains the initial bootstrap program used to load the operating system.
 
 - **Super block :**<br />
A super block describes the state of the file system: the total size of the partition, the block size, list of free blocks, the inode number of the root directory.
A superblock is a collection of metadata used to show the properties of file systems. It keeps record of empty blocks present on harddisk , how many inode are filled with data etc.

 - **Disk inode list block :**<br />
It is a linear linked list of inodes. There is a one to one mapping of files to inodes and vice versa. An inode is identified by its "inode number", which contains the information needed to find the inode itself on the disk. Thus, while users think of files in terms of file names, Unix thinks of files in terms of inodes.

 - **Data blocks:**<br />
data blocks contains the actual contents os data of files.

# Diagramatic representation of Customized Virtual File System

<p align="center">
  <img src="https://github.com/OmkarRatnaparkhi/Customized_Virtual_File_System/blob/main/Assets/Diagram3.png" alt="Diagram1">
</p>
