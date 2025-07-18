/*-----------------------------------------------------------------
File: mkfs.minix.h
Description: Contains definitions and include files for the 
             mkfs.minix project.
------------------------------------------------------------------*/

/* Include files */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <linux/types.h>
#include <linux/minix_fs.h>

/* Definitions */
#define OK 0
#define ERR1 -1
/***************Disk Layout ***********************************************/
/* Boot 0, SB 1, IMAP 2-3, ZMAP 4-11, ITABLE 12-523, Data ZONES 524-65535 */
/**************************************************************************/
/* Define some basic numbers */
#define BOOTSIZE 512	/* Boot block set to all zero's in first block */
#define BLOCK_SIZE 1024  /* size of blocks */
#define BLOCK_SIZE_BITS (BLOCK_SIZE*8) /* size of blocks in bits */
#define DIRENTRYSIZE (30+2)
#define INODESIZE sizeof(struct minix_inode)
#define IMAPBLOCKS 2  /* number of blocks allocate to Imap */
#define NUMITABLEBLOCKS ((IMAPBLOCKS*BLOCK_SIZE_BITS*INODESIZE)/BLOCK_SIZE)

/* For initialising Super Block */
#define NUMINODES ((BLOCK_SIZE_BITS*IMAPBLOCKS)-1)   /* 2 blocks in IMAP (16383 total) */
#define TOTALBLOCKS ((64*1024)-1)  /* Total number of zones (blocks) - 64 K */
#define NUMIMAPBLOCKS IMAPBLOCKS
#define NUMZMAPBLOCKS 8		/* use 8 blocks - 8 * 8* 1024 64 kbits */
#define FIRSTZONE (1+1+NUMIMAPBLOCKS+NUMZMAPBLOCKS+NUMITABLEBLOCKS) /*First block is 0 boot block*/
#define ZONELOGSIZE 0    /* log2(number of blocks per zone - set to 0, i.e. 1 Block/Zone */
#define MAXFILESIZE ((7+512+512*512)*1024)  /* size of files in bytes */
#define MAGICNUM MINIX_SUPER_MAGIC2	/* V1 - 30 chars per filename */
#define STATE 0x01	/* set bit 0, clear bit 1 */

/* Other useful definitions */
#define NUMZONES (TOTALBLOCKS-FIRSTZONE)


/* Prototypes */
int create_msblk(int);
int create_inodes(int);
int create_mroot(int);
