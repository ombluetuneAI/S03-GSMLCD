/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2012        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "system.h"
#include "flash.h"
/* Definitions of physical drive number for each media */
#define ATA		3
#define MMC		1
#define USB		2
#define SPIFlash 0

u16	    FLASH_SECTOR_COUNT=2048*6;//6M字节,默认为W25Q64
#define FlashBlockSize  		8     //每个BLOCK有8个扇区
#define FlashSectorSize		 	512
/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE drv				/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat;

	switch (drv)
	{

	case SPIFlash:
		SPI_Flash_Init();
		stat=0x0A;
		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0..) */
)
{
	int result;
	
	return result;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..128) */
)
{
	DRESULT res;

	switch (drv) {
	case SPIFlash:
			for(;count>0;count--)
			{
				SPI_Flash_Read(buff,sector*FlashSectorSize,FlashSectorSize);
				sector++;
				buff+=FlashSectorSize;
			}
			res=RES_OK;
			return res;
	}
	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..128) */
)
{
	DRESULT res;

	switch (drv) {
	case SPIFlash:
			for(;count>0;count--)
			{
				//SPI_Flash_Erase_Sector(sector);
				SPI_Flash_Write((u8*)buff,sector*FlashSectorSize,FlashSectorSize);
				sector++;
				buff+=FlashSectorSize;
			}
			res=RES_OK;
		return res;

	}
	return RES_PARERR;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;

	switch (drv) {
	case SPIFlash:
	    switch(ctrl)
	    {
		    case CTRL_SYNC:
				res = RES_OK; 
		        break;	 
		    case GET_SECTOR_SIZE:
		        *(WORD*)buff = FlashSectorSize;
		        res = RES_OK;
		        break;	 
		    case GET_BLOCK_SIZE:
		        *(WORD*)buff = FlashBlockSize;
		        res = RES_OK;
		        break;	 
		    case GET_SECTOR_COUNT:
		        *(DWORD*)buff = FLASH_SECTOR_COUNT;
		        res = RES_OK;
		        break;
		    default:
		        res = RES_PARERR;
		        break;
	    }

		return res;

	case MMC :
		// pre-process here


		// post-process here

		return res;

	case USB :
		// pre-process here



		// post-process here

		return res;
	}
	return RES_PARERR;
}
#endif

DWORD get_fattime (void)
{
	u8 res;
	return res;
}


