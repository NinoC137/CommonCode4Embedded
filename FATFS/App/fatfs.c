/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
#include "fatfs.h"

uint8_t retSD;    /* Return value for SD */
char SDPath[4];   /* SD logical drive path */
FATFS SDFatFS;    /* File system object for SD logical drive */
FIL SDFile;       /* File object for SD */

/* USER CODE BEGIN Variables */
/* 用于测试读写速度 */
#define TEST_FILE_LEN            (2*1024*1024)    /* 用于测试的文件长�?? */
#define BUF_SIZE                (4*1024)        /* 每次读写SD卡的�??大数据长�?? */
/* 变量 */
FATFS fs;
FIL file;
BYTE work[4096];
char FsReadBuf[1024];
char FsWriteBuf[1024] = {"Nino's Calling \r\n I Love Rick From C137 \r\n"};
uint8_t g_TestBuf[BUF_SIZE];

DIR DirInf;
FILINFO FileInf;
char DiskPathSPI[4]; /* SD卡�?�辑驱动路径，比盘符0，就�??"0:/" */
char DiskPathSD[4]; /* SD卡�?�辑驱动路径，比盘符0，就�??"0:/" */

static const char *FR_Table[] =
        {
                "FR_OK",                                             /* (0) Succeeded */
                "FR_DISK_ERR",                             /* (1) A hard error occurred in the low level disk I/O layer */
                "FR_INT_ERR",                                     /* (2) Assertion failed */
                "FR_NOT_READY",                         /* (3) The physical drive cannot work */
                "FR_NO_FILE",                                 /* (4) Could not find the file */
                "FR_NO_PATH",                                 /* (5) Could not find the path */
                "FR_INVALID_NAME",                             /* (6) The path name format is invalid */
                "FR_DENIED",         /* (7) Access denied due to prohibited access or directory full */
                "FR_EXIST",                                 /* (8) Access denied due to prohibited access */
                "FR_INVALID_OBJECT",                 /* (9) The file/directory object is invalid */
                "FR_WRITE_PROTECTED",                     /* (10) The physical drive is write protected */
                "FR_INVALID_DRIVE�??",                         /* (11) The logical drive number is invalid */
                "FR_NOT_ENABLED",                             /* (12) The volume has no work area */
                "FR_NO_FILESYSTEM",                     /* (13) There is no valid FAT volume */
                "FR_MKFS_ABORTED",             /* (14) The f_mkfs() aborted due to any parameter error */
                "FR_TIMEOUT",         /* (15) Could not get a grant to access the volume within defined period */
                "FR_LOCKED",                 /* (16) The operation is rejected according to the file sharing policy */
                "FR_NOT_ENOUGH_CORE",             /* (17) LFN working buffer could not be allocated */
                "FR_TOO_MANY_OPEN_FILES", /* (18) Number of open files > _FS_SHARE */
                "FR_INVALID_PARAMETER"                         /* (19) Given parameter is invalid */
        };
/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the SD driver ###########################*/
  retSD = FATFS_LinkDriver(&SD_Driver, SDPath);

  /* USER CODE BEGIN Init */
  /* additional user code for init */

  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
  return 0;
  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */
void CreateNewFile(char *_path, char* _fileName, char* _content)
{
    FRESULT result;
    UINT bw;
    char path[32];

    if(strcmp(_path, "") == 0)
    {
        _path = "0:/";
    }

    /* 挂载文件系统 */
//    result = f_mount(&fs, _path, 0);			/* Mount a logical drive */
//    if (result != FR_OK)
//    {
//        uart_printf("mount fail (%s)\r\n", FR_Table[result]);
//    }

    /* 打开文件 */
    sprintf(path, "%s%s", _path, _fileName);
    result = f_open(&file, path, FA_CREATE_ALWAYS | FA_WRITE);
    if (result == FR_OK)
    {
        uart_printf("%s file open success\r\n", _fileName);
    }
    else
    {
        uart_printf("%s file open fail  (%s)\r\n", _fileName, FR_Table[result]);
    }

    /* 写一串数�?? */
    result = f_write(&file, _content, strlen(_content), &bw);
    if (result == FR_OK)
    {
        uart_printf("%s file write success\r\n", _fileName);
    }
    else
    {
        uart_printf("%s file write fail  (%s)\r\n", _fileName, FR_Table[result]);
    }

    /* 关闭文件*/
    f_close(&file);

    /* 卸载文件系统 */
//    f_mount(NULL, _path, 0);
}

void ReadFileData(char *_path, char* _fileName)
{
    FRESULT result;
    UINT bw;
    char path[64];

    if(strcmp(_path, "") == 0)
    {
        _path = "0:/";
    }

    /* 挂载文件系统 */
//    result = f_mount(&fs, _path, 0);			/* Mount a logical drive */
//    if (result != FR_OK)
//    {
//        uart_printf("mount fail (%s)\r\n", FR_Table[result]);
//    }

    /* 打开文件 */
    sprintf(path, "%s%s", _path, _fileName);
    result = f_open(&file, path, FA_OPEN_EXISTING | FA_READ);
    if (result !=  FR_OK)
    {
        uart_printf("Don't Find File : %s\r\n", _fileName);
        return;
    }

    /* 读取文件 */
    result = f_read(&file, FsReadBuf, sizeof(FsReadBuf), &bw);
    if (bw > 0)
    {
        FsReadBuf[bw] = 0;
        uart_printf("\r\n%s : \r\n%s\r\n", _fileName, FsReadBuf);
    }
    else
    {
        uart_printf("\r\n%s : \r\n", _fileName);
    }

    /* 关闭文件*/
    f_close(&file);

    /* 卸载文件系统 */
//    f_mount(NULL, _path, 0);
}


void CreateDir(char* _path, char* _dirName)
{
    FRESULT result;
    char path[64];

    /* 挂载文件系统 */
//    result = f_mount(&fs, _path, 0);            /* Mount a logical drive */
//    if (result != FR_OK)
//    {
//        uart_printf("挂载文件系统失败 (%s)\r\n", FR_Table[result]);
//    }

    if(strcmp(_path, "") == 0)
    {
        _path = "0:/";
    }

    /* 创建目录/Dir1 */
    sprintf(path, "%s%s", _path, _dirName);
    result = f_mkdir(path);
    if (result == FR_OK)
    {
        //uart_printf("f_mkdir %s OK\r\n", _dirName);
    }
    else if (result == FR_EXIST)
    {
        uart_printf("%s exist    (%s)\r\n", _dirName, FR_Table[result]);
    }
    else
    {
        uart_printf("f_mkdir %s fail (%s)\r\n", _dirName, FR_Table[result]);
        return;
    }

    /* 卸载文件系统 */
//    f_mount(NULL, _path, 0);
}

void DeleteDirFile(char* _path, char* _fileName)
{
    FRESULT result;
    uint8_t i;
    char path[64];

    /* 挂载文件系统 */
//    result = f_mount(&fs, _path, 0);            /* Mount a logical drive */
//    if (result != FR_OK)
//    {
//        uart_printf("挂载文件系统失败 (%s)\r\n", FR_Table[result]);
//    }

    if(strcmp(_path, "") == 0)
    {
        _path = "0:/";
    }

    /* 删除目录fileName */
    sprintf(path, "%s%s", _path, _fileName);
    result = f_unlink(path);
    if (result == FR_OK)
    {
        uart_printf("\r\nDelete %s success", _fileName);
    }
    else if (result == FR_NO_FILE)
    {
        uart_printf("\r\nWithout this file :%s", _fileName);
    }
    else
    {
        uart_printf("\r\nDelete %s fail\t (%s)", FR_Table[result]);
    }

    /* 卸载文件系统 */
//    f_mount(NULL, _path, 0);
}

void FileFormat(char *_path){
    uint8_t result;
    /* 挂载文件系统 */
    result = f_mount(&fs, _path, 1);
    if (result != FR_OK)
    {
        uart_printf("mount fail (%s)\r\n", FR_Table[result]);
    }
    else
    {
        uart_printf("mount success (%s)\r\n", FR_Table[result]);
    }

    /* 第一次使用必须进行格式化 */
    //result = f_mkfs(DiskPath, 0, 0);
    result = f_mkfs(_path, FM_FAT32, 512, work, sizeof work);
    if (result != FR_OK)
    {
        uart_printf("mkfs fail (%s)\r\n", FR_Table[result]);
    }
    else
    {
        uart_printf("mkfs success (%s)\r\n", FR_Table[result]);
    }

    /* 卸载文件系统 */
    result  = f_mount(NULL, _path, 0);
    if (result != FR_OK)
    {
        uart_printf("demount fail (%s)\r\n", FR_Table[result]);
    }
    else
    {
        uart_printf("demount success (%s)\r\n", FR_Table[result]);
    }
    //重新挂载fs系统
    f_mount(&fs, _path, 0);
}

void ViewRootDir(char *_path)
{
    FRESULT result;
    uint32_t cnt = 0;
    FILINFO fno;

    if(strcmp(_path, "") == 0)
    {
        _path = "0:/";
    }

    uart_printf("\r\n");
    uart_printf("Open Dir:\t%s\r\n", _path);

    /* 挂载文件系统 */
//    result = f_mount(&fs, _path, 0);	/* Mount a logical drive */
//    if (result != FR_OK)
//    {
//        uart_printf("mount fail (%s)\r\n", FR_Table[result]);
//    }

    /* 打开根文件夹 */
    result = f_opendir(&DirInf, _path); /* 如果不带参数，则从当前目录开�?? */
    if (result != FR_OK) {
        uart_printf("open root fail  (%s)\r\n", FR_Table[result]);
        return;
    }

    uart_printf("attribute|\tFileSize|\tName(S)|\tName(L)\r\n");
    for (cnt = 0;; cnt++) {
        result = f_readdir(&DirInf, &FileInf);        /* 读取目录项，索引会自动下�?? */
        if (result != FR_OK || FileInf.fname[0] == 0) {
            break;
        }

        if (FileInf.fname[0] == '.') {
            continue;
        }

        /* 判断是文件还是子目录 */
        if (FileInf.fattrib & AM_DIR)
        {
            uart_printf("(0x%02d)Menu  ", FileInf.fattrib);
        }
        else
        {
            uart_printf("(0x%02d)File\t", FileInf.fattrib);
        }

        f_stat(FileInf.fname, &fno);

        /* 打印文件大小, �??�??4G */
        uart_printf("%10d\t", (int)fno.fsize);


        uart_printf("%s\r\n", (char *)FileInf.fname);	/* 长文件名 */
    }

    /* 卸载文件系统 */
//    f_mount(NULL, _path, 0);
}


/* USER CODE END Application */
