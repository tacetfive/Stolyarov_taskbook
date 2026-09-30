#include <sys/stat.h>
#include <sys/sysmacros.h> /* major(), minor() for device ID */
#include <stdio.h>
#include <stdint.h> /* uintmax_t */
#include <time.h>   /* ctime() localtime() */
#include <stdlib.h> /* exit() */
#include <errno.h>

enum { S_IFPERM = 0x0FFF };

void print_file_info(const struct stat *sb)
{
    printf("File type:                ");
    switch (sb->st_mode & S_IFMT) { /* S_IFMT == 0xF000, mask 4 major bits */
        case S_IFBLK:  printf("block device\n");        break;
        case S_IFCHR:  printf("character device\n");    break;
        case S_IFDIR:  printf("directory\n");           break;
        case S_IFIFO:  printf("FIFO/pipe\n");           break;
        case S_IFLNK:  printf("symlink\n");             break;
        case S_IFREG:  printf("regular file\n");        break;
        case S_IFSOCK: printf("socket\n");              break;
        default:       printf("unknown type\n");        break;
    }
    printf("ID of containing device:  [%x,%x]\n",
                major(sb->st_dev),
                minor(sb->st_dev));
    printf("I-node number:            %ju\n", (uintmax_t)sb->st_ino);
    printf("Permissions:              %jo (octal)\n",
            ( (uintmax_t)sb->st_mode & S_IFPERM ));
    printf("Link count:               %ju\n", (uintmax_t)sb->st_nlink);
    printf("Ownership:                UID = %ju   GID = %ju\n", 
                        (uintmax_t)sb->st_uid, (uintmax_t)sb->st_gid);
    printf("Preferred I/O block size: %jd bytes\n", (uintmax_t)sb->st_blksize);
    printf("File size:                %jd bytes\n", (uintmax_t)sb->st_size);
    printf("Blocks allocated:         %jd\n", (uintmax_t)sb->st_blocks);

    printf("Last status change:       %s", ctime(&sb->st_ctime)); 
    printf("Last file access:         %s", ctime(&sb->st_atime));
    printf("Last file modification:   %s", ctime(&sb->st_mtime));
/* ctime() interprets time in seconds to sb.st_ctime field to D/M/Y format. */
    return;
}

int main(int argc, char **argv)
{
    char *filename;
    struct stat sb; /* not a struct stat* to avoid initialization */
    if (argc != 2) {
        fprintf(stderr, "invalid arguments\n");
        exit(1);
    }
    filename = argv[1];

    if ( (lstat(filename, &sb)) == -1 ) {
        perror("lstat()");
        return -1;
    }

    print_file_info(&sb);

    if ( (sb.st_mode & S_IFMT) == S_IFLNK ) { /* print both file & link info */
        if ( (stat(filename, &sb)) == -1 ) {
            if ( errno == 2 ) {
                printf("dangling\n");
                return -1;
            }
            perror("stat()");
            return -1;
        }
        printf("\n>>> Information about file the lynk refers to: <<<\n");
        print_file_info(&sb);
    }
    return 0;
}






