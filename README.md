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
* File is unformated uniform stream of bytes
* File system is the way of storing and retriving the files into the harddisk.

# Important System calls of File subsystems :
* open() : It is used to open existing file.
* close() : It is used to close already opened file.
* read() : It isused to read the data from file.
* write() : It is used to write data into the file
* lseek() : It is used to change the offset of the file.
* creat() : It is used to create new reular file.




<p align="center">
  <img src="https://github.com/OmkarRatnaparkhi/Customized_Virtual_File_System/blob/main/Assets/Diagram1.png" alt="Diagram1">
</p>
