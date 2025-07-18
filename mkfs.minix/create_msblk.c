/*-----------------------------------------------------------------
File: create_msblk.c
Description: Contains the code for creating a minix fs superblock.
------------------------------------------------------------------*/

#include "mkfs.minix.h"


/*-----------------------------------------------------------------
Routine: create_msblk()

Arguments: int mfd;  - fd for storing the superblock
Description: 
         Creates a superblock for the minix filesystem.
------------------------------------------------------------------*/

int create_msblk(int mfd)
{
   struct minix_super_block *sblk_pt;
   int wb;
   
   /* Buffer for superblock */
   sblk_pt = malloc(BLOCK_SIZE); // Allocate a full block, even if not all occupied.
   if(sblk_pt == NULL)
   {
     printf("Could not allocate memory for superblock\n");
     return(ERR1);
   }

   /* Let's set things up */

   memset(sblk_pt,0,BLOCK_SIZE);  /* zero all bytes in block */
   sblk_pt->s_ninodes = NUMINODES;
   sblk_pt->s_nzones = TOTALBLOCKS;
   sblk_pt->s_imap_blocks = NUMIMAPBLOCKS;
   sblk_pt->s_zmap_blocks = NUMZMAPBLOCKS;
   sblk_pt->s_firstdatazone = FIRSTZONE;
   sblk_pt->s_log_zone_size = ZONELOGSIZE;   /* one block per zone */
   sblk_pt->s_max_size = MAXFILESIZE; 
   sblk_pt->s_magic = MAGICNUM;
   sblk_pt->s_state = STATE;

   /* Write to the disk */
   if(lseek(mfd,BLOCK_SIZE,SEEK_SET)==-1)  /* Go go first block */
   {
       printf("Could not seek to superblock\n");
       free(sblk_pt);
       return(ERR1);
   }

   wb = write(mfd,sblk_pt,BLOCK_SIZE);
   if(wb != BLOCK_SIZE)
   {
      perror("mkfs.minix(write sblk)");
      free(sblk_pt);
      return(ERR1);
   }

   free(sblk_pt);
   return(OK);
}
