#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/inotify.h>

void listDirectory(const char *path)
{
    struct dirent *entry;
    DIR *dp = opendir(path);
    if (dp == NULL)
    {
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }

    printf("Содержимое каталога '%s':\n", path);
    while ((entry = readdir(dp)) != NULL)
    {
        if (entry->d_name[0] != '.')
        {
            char newPath[256];
            snprintf(newPath, sizeof(newPath), "%s/%s", path, entry->d_name);

            struct stat statbuf;
            if (stat(newPath, &statbuf) == 0)
            {
                printf("%s\n", newPath);

                if (S_ISDIR(statbuf.st_mode))
                {
                    listDirectory(newPath);
                }
            }
            else
            {
                perror("stat failed");
            }
        }
    }
    closedir(dp);
}

void handleEvent(int fd)
{
    char buffer[1024];
    int length = read(fd, buffer, sizeof(buffer));
    if (length < 0)
    {
        perror("Error reading inotify events");
        return;
    }

    for (int i = 0; i < length;)
    {
        struct inotify_event *event = (struct inotify_event *)&buffer[i];
        if (event->mask & IN_CREATE)
        {
            printf("Создано: %s\n", event->name);
        }
        if (event->mask & IN_DELETE)
        {
            printf("Удалено: %s\n", event->name);
        }
        if (event->mask & IN_MODIFY)
        {
            printf("Изменено: %s\n", event->name);
        }
        if (event->mask & IN_MOVED_FROM)
        {
            printf("Перемещено из: %s\n", event->name);
        }
        if (event->mask & IN_MOVED_TO)
        {
            printf("Перемещено в: %s\n", event->name);
        }
        i += sizeof(struct inotify_event) + event->len;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Использование: %s <путь к каталогу>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *directoryPath = argv[1];

    listDirectory(directoryPath);

    int fd = inotify_init();
    if (fd < 0)
    {
        perror("inotify_init failed");
        return EXIT_FAILURE;
    }

    int wd = inotify_add_watch(fd, directoryPath, IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVED_FROM | IN_MOVED_TO);
    if (wd < 0)
    {
        perror("inotify_add_watch failed");
        close(fd);
        return EXIT_FAILURE;
    }

    printf("Отслеживание изменений в каталоге '%s'. Нажмите Ctrl+C для выхода.\n", directoryPath);

    while (1)
    {
        handleEvent(fd);
    }

    inotify_rm_watch(fd, wd);
    close(fd);

    return EXIT_SUCCESS;
}