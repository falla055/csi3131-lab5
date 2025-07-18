/*-----------------------------------------------------------------
File: mkfs.minix.c
Description: This file contains the main function for creating the 
             minix file system. The main() function checks the
	     argument (i.e. can open the partition file), and then
	     calls create_mfs to create the file system.
------------------------------------------------------------------*/

#include "mkfs.minix.h"
int create_mfs(int mfd); // Prototype

/*-----------------------------------------------------------------
Routine: main()

Arguments: int argc - number of command line arguments
	   char **argv - pointers to command line arguments
Description: 
	Command synopsis: mkfs.minix <file>
	<file> is the filename of the hard drive partition where
	       the physical file system is to be created.
	The file will be opened and its fd passed to create_mfs
------------------------------------------------------------------*/

int main(int argc, char **argv)
{
   int fd;
   
   if(argc != 2)
   {
      printf("Usage: mkfs.minix <device>\n");
      return(ERR1);
   }
   
   fd = open(argv[1],O_RDWR);  /* open for reading and writing */
   if(fd == -1)
   {
      printf("Could not open %s\n",argv[1]);
      return(ERR1);
   }

   if(create_mfs(fd)== ERR1) 
        printf("Error occured in creating MINIX fs on %s\n",argv[1]);

   close(fd);
   return(OK);
}


/*-----------------------------------------------------------------
Routine: create_mfs()

Arguments: int mfd - File descriptor to partition where fs is
           to be created.

Description: Use the calls to create_msblk, create_inodes, create_mroot
             to create the file system.  Each of these routines
	     are used to create the superblock, inode and zone maps
	     (initialise inode table), and root directory entry.

-----------------------------------------------------------------*/

int create_mfs(int mfd)
{
   char boot[BOOTSIZE];

       /* Setup boot block */
   memset(boot,0,BOOTSIZE);  /* set to all zeros */
   if(lseek(mfd,0,SEEK_SET)==-1)  /* go to start */
   {
      perror("mkfs.minix");
      return(ERR1);
   }
   if(write(mfd,boot,BOOTSIZE) != BOOTSIZE)
   {
      printf("mkfs.minix (write boot)");
      return(ERR1);
   }
   if(create_msblk(mfd) == ERR1) return(ERR1); // Creates superblock
   if(create_inodes(mfd) == ERR1) return(ERR1);  // Creates inodes and bitmaps
   if(create_mroot(mfd) == ERR1) return(ERR1); // Creates root directory

   return(OK);
}

