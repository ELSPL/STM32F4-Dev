/******************************************************************//**
* @file		newlib_stubs.c
* @brief	Stubs for Code coverage
* @version  v1
* @date		May 20, 2016
* @author	Dwijay Bane (Edutech Learning Solutions)
**********************************************************************/


/* Includes ------------------------------------------------------------------- */
#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include "stm32f4xx.h"
//#include "coverage_stubs.h"


//#if DO_COVERAGE

extern int errno;

char *__env[1] = { 0 };
char **environ = __env;

int _write(int file, char *ptr, int len);

void _exit(int status)
{
 __gcov_flush();
}

int _close(int file)
{
    return -1;
}

int _fstat(int file, struct stat *st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

int _getpid()
{
    return 1;
}

int _isatty(int file)
{
    switch (file)
    {
    case STDOUT_FILENO:
    case STDERR_FILENO:
    case STDIN_FILENO:
        return 1;
    default:
        errno = EBADF;
        return 0;
    }
}

int _kill(int pid, int sig)
{
    errno = EINVAL;
    return (-1);
}

int _lseek(int file, int ptr, int dir)
{
    return 0;
}

void *_sbrk(int incr)
{
    extern char _ebss; // Defined by the linker
    static char *heap_end;
    char *prev_heap_end;

    if (heap_end == 0)
    {
        heap_end = &_ebss;
    }
    prev_heap_end = heap_end;

    char * stack = (char*) __get_MSP();

     if (heap_end + incr >  stack)
     {
         _write (STDERR_FILENO, "Heap and stack collision\n", 25);
         errno = ENOMEM;
         return  (void *) -1;
     }

    heap_end += incr;
    return (void *) prev_heap_end;
}

int _read(int file, char *ptr, int len)
{
 return 0;
}

int _unlink(char *name)
{
    errno = ENOENT;
    return -1;
}


//const unsigned char *fileName; /* file name used for _open() */

int _write(int file, char *ptr, int len)
{
  static unsigned char gdb_cmd[128];
  (void)file;

  trace_printf("dump binary memory 0x%08x 0x%08x",(uint32_t)ptr,(uint32_t)(ptr+len));
  return 0;
}

int _open (const char *ptr, int mode)
{
  (void)mode;
//  fileName = (const unsigned char*)ptr; /* store file name for _write() */
  return 0;
}


//#endif



 /* --------------------------------- End Of File ------------------------------ */
