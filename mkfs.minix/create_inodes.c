/*-----------------------------------------------------------------
File: create_inodes.c
Description: Contains the code for creating a minix fs inode.
------------------------------------------------------------------*/

#include "mkfs.minix.h"

/*-----------------------------------------------------------------
Routine: create_inodes()

Arguments: int mfd;  - fd for storing the superblock
Description: 
         Creates the inode and zone maps and initialises Inode table.
------------------------------------------------------------------*/

int create_inodes(int mfd)
{
   char *map;  
   int wb;
   size_t mapsize;  /* number of bytes in map */
   int bytnum,bitnum;  /* for setting bits in zone map */
   int i;
  
   /*-----------------INODE MAP---------------------*/
   mapsize = NUMIMAPBLOCKS*BLOCK_SIZE;  
   map = malloc(mapsize);  // Allocates memory for the inode bit map
   if(map == NULL)
   {
     printf("Could not allocate memory for inode map\n");
     return(ERR1);
   }
   memset(map,0,mapsize);  // Sets all bits to zero

   if(lseek(mfd,2*BLOCK_SIZE,SEEK_SET) == -1)  /* go to block 2 on the disk */
   {
      printf("Could not seek to block 2 for inode map\n");
      free(map);
      return(ERR1);
   }
   
   wb = write(mfd,map,mapsize);
   if(wb != mapsize)
   {
      printf("Only wrote %d bytes in inode map\n",wb);
      free(map);
      return(ERR1);
   }

   /*-----------------ZONE MAP---------------------*/
   mapsize = NUMZMAPBLOCKS*BLOCK_SIZE;  /* number of bytes in map */
   map = realloc(map,mapsize);  // Adjusts the allocated memory to size for zone bitmap
   if(map == NULL)
   {
     printf("Could not re-allocate memory for zone map\n");
     free(map);
     return(ERR1);
   }
   memset(map,0,mapsize);  // Sets all bits to zero
   /* need to set last bits to 1 */
   for(i=NUMZONES ; i<TOTALBLOCKS+1 ; i++)  // Want to set unused bits to 1
   {
      /* i/8 - location of byte */
      /* i%8 - bit number to set in byte */
      bytnum = i>>3; /* byte number (map+bnum) */
      bitnum = 1<<(i&0x7);  /* the bit number at the byte */
      map[bytnum] |= bitnum;  /* sets the bit */
   }

    /* no need to seek - just finished writing IMAP above */ 
   wb = write(mfd,map,mapsize); /* writes the ZMAP */
   if(wb != mapsize)
   {
      printf("Only wrote %d bytes in inode map\n",wb);
      free(map);
      return(ERR1);
   }


   /*-----------------INODE TABLE---------------------*/
   mapsize = BLOCK_SIZE;
   map = realloc(map,mapsize);  // Adjusts memory allocated to a BLOCK size
   if(map == NULL)
       { perror("mkfs.minix (realloc itable)"); free(map); return(ERR1); }
   memset(map,0,mapsize);  // Set all bits in the block to zero (i.e. inode entry)

    /* no need to seek - just finished writing ZMAP above */ 
   for(i=0 ; i<NUMITABLEBLOCKS ; i++)
   {
       wb = write(mfd,map,mapsize); /* writes inode block in the INODE TABLE */
       if(wb != mapsize)
           {  perror("mkfs.minix(write imap)"); free(map); return(ERR1); }
   }

    /* now for the zone map */
   return(OK);
}
