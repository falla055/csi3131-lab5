/*-----------------------------------------------------------------
File: create_mroot.c
Description: Contains the code for creating a minix root directory.
------------------------------------------------------------------*/

#include "mkfs.minix.h"
#include <time.h>
#include <unistd.h>
#include <string.h>

typedef unsigned short u16;
typedef unsigned char u8;
typedef unsigned int u32;

#define START_INODE_MAP (2 * BLOCK_SIZE)
#define START_ZONE_MAP (4 * BLOCK_SIZE)
#define START_INODE_TBL (12 * BLOCK_SIZE)

/*-----------------------------------------------------------------
Routine: create_mroot()

Arguments: int mfd;  - fd for storing the superblock
Description: 
         Creates the root directory using inode 1 and block 1.
------------------------------------------------------------------*/

int create_mroot(int mfd)
{
  struct minix_inode root_inode;
  unsigned char imap_byte;
  unsigned char zmap_byte;
  char rootd[BLOCK_SIZE] = {0};
  time_t now = time(NULL);

   /* lets create the inode for the directory */
   root_inode.i_mode = S_IFDIR | 0755;
   root_inode.i_uid = getuid();
   root_inode.i_size = 2 * DIRENTRYSIZE;
   root_inode.i_time = now;
   root_inode.i_gid = getgid();
   root_inode.i_nlinks = 2;
   root_inode.i_zone[0] = FIRSTZONE;
   for (int i=1; i < 9; i++)
    root_inode.i_zone[i] = 0;

   /* Store inode on disk  */
   lseek(mfd, START_INODE_TBL, SEEK_SET);
   write(mfd, &root_inode, sizeof(root_inode));

   /* Set bit in imap */
   imap_byte = 0x3;
   lseek(mfd, START_INODE_MAP, SEEK_SET);
   write(mfd, &imap_byte, 1);
   
   /* lets fill directory table w 2 entries "." and ".." */
   *(u16 *)rootd = 1;
   strncpy(rootd + 2, ".", 30);
   
   *(u16 *)(rootd + 32) = 1;
   strncpy(rootd + 34, "..", 30);

   /* store directory table on disk */
   lseek(mfd, FIRSTZONE * BLOCK_SIZE, SEEK_SET);
   write(mfd, rootd, BLOCK_SIZE);

   /* Set bit in zmap to indicate first zone is used*/
   int block_offset = (FIRSTZONE -1) / 8;
   int bit_position = (FIRSTZONE -1) % 8;

    if (lseek(mfd, START_ZONE_MAP + block_offset, SEEK_SET) == -1) {
        perror("lseek zone map");
        return(ERR1);
    }
    if (read(mfd, &zmap_byte, 1) != 1) {
        perror("read zone map");
        return(ERR1);
    }
    zmap_byte |= (1 << bit_position);
    if (lseek(mfd, START_ZONE_MAP + block_offset, SEEK_SET) == -1) {
        perror("lseek zone map write");
        return(ERR1);
    }
    if (write(mfd, &zmap_byte, 1) != 1) {
        perror("write zone map");
        return(ERR1);
    }
  
   return(OK);
}
