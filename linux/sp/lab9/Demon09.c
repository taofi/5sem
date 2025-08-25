#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <syslog.h>

#define DEFAULT_LOG_DIR "/var/log/demon09d"
#define CONFIG_FILE "/etc/demon09d/config.cfg"

char log_dir[1024] = DEFAULT_LOG_DIR;
char watch_dir[1024] = "";

int srv_log_fd = -1;
int dir_log_fd = -1;
int inotify_fd = -1;
int inotify_wd = -1;

void daemonize();
void watch_directory();
void read_config();
void write_srv_log(const char* message);
void write_dir_log(const char* message);
void get_time_stamp(char* buffer, size_t buffer_size);
void signal_handler(int signum);

int main() {
    signal(SIGTERM, signal_handler);
    signal(SIGHUP, signal_handler);

    read_config();

    struct stat st;
    if (stat(watch_dir, &st) == -1 || !S_ISDIR(st.st_mode)) {
        char message[1024];
        snprintf(message, sizeof(message), "Watch directory does not exist: %s", watch_dir);
        write_srv_log(message);
        exit(EXIT_FAILURE);
    }


    struct stat st1;
    if (stat(log_dir, &st1) == -1) {
        if (mkdir(log_dir, 0700) == 0) {
            char message[1024];
            snprintf(message, sizeof(message), "Demon created catalog %s", log_dir);
            write_srv_log(message);
        }
        else {
            char message[1024];
            snprintf(message, sizeof(message), "Couldnt create catalog %s", log_dir);
            write_srv_log(message);
            exit(EXIT_FAILURE);
        }
    }
    else {
        char message[1024];
        snprintf(message, sizeof(message), "Demon found catalog %s", log_dir);
        write_srv_log(message);
    }


    while (1) {
        watch_directory();
    }

    return 0;
}

void watch_directory() {
    inotify_fd = inotify_init();
    if (inotify_fd < 0) {
        perror("inotify_init");
        char message[1024];
        snprintf(message, sizeof(message), "Error inotify_init");
        write_srv_log(message);
        syslog(LOG_ERR, "inotify_init error");
        exit(EXIT_FAILURE);
    }

    inotify_wd = inotify_add_watch(inotify_fd, watch_dir, IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVED_FROM | IN_MOVED_TO);
    if (inotify_wd == -1) {
        char message[1024];
        snprintf(message, sizeof(message), "Start failed, no such catalog: %s", watch_dir);
        write_srv_log(message);
        syslog(LOG_ERR, "Failed to add inotify watch for directory: %s", watch_dir);
        close(inotify_fd);
        exit(EXIT_FAILURE);
    }
    else {
        char message[1024];
        snprintf(message, sizeof(message), "Demon started with %s %s", watch_dir, log_dir);
        write_srv_log(message);
        syslog(LOG_INFO, "Daemon started with watch directory: %s and log directory: %s", watch_dir, log_dir);
    }

    char buffer[1024];
    int length, i = 0;

    while (1) {
        length = read(inotify_fd, buffer, sizeof(buffer));
        if (length < 0) {
            perror("read");
            syslog(LOG_ERR, "Error reading inotify events");
            break;
        }

        i = 0;
        while (i < length) {
            struct inotify_event* event = (struct inotify_event*)&buffer[i];
            if (event->len) {
                char fullPath[256];
                snprintf(fullPath, 256, "%s/%s", watch_dir, event->name);
                char absolutePath[256];
                realpath(fullPath, absolutePath);

                char log_message[1024];
                if (event->mask & IN_CREATE) {
                    snprintf(log_message, sizeof(log_message), "File created: %s", event->name);

                }
                else if (event->mask & IN_DELETE) {
                    snprintf(log_message, sizeof(log_message), "File deleted: %s", event->name);
                }
                else if (event->mask & IN_MODIFY) {
                    snprintf(log_message, sizeof(log_message), "File edited: %s", event->name);
                }
                else if (event->mask & IN_MOVED_FROM) {
                    snprintf(log_message, sizeof(log_message), "File renamed from: %s", event->name);
                }
                else if (event->mask & IN_MOVED_TO) {
                    snprintf(log_message, sizeof(log_message), "File renamed to: %s", event->name);
                }
                write_dir_log(log_message);
            }
            i += sizeof(struct inotify_event) + event->len;
        }
    }
    inotify_rm_watch(inotify_fd, inotify_wd);
    close(inotify_fd);
}

void read_config() {
    FILE* file = fopen(CONFIG_FILE, "r");
    if (file == NULL) {
        perror("Could not open configuration file");
        syslog(LOG_ERR, "Failed to open configuration file: %s", CONFIG_FILE);
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (sscanf(buffer, "log_dir=%1023s", log_dir) == 1) continue;
        if (sscanf(buffer, "watch_dir=%1023s", watch_dir) == 1) continue;
    }

    fclose(file);
    syslog(LOG_INFO, "Configuration file %s read successfully", CONFIG_FILE);
}

void write_srv_log(const char* message) {
    if (srv_log_fd == -1) {
        char log_file[1024];
        char datetime[64];
        get_time_stamp(datetime, sizeof(datetime));
        snprintf(log_file, sizeof(log_file), "%s/%s-srv.log", log_dir, datetime);

        struct stat st;
        if (stat(log_dir, &st) == -1) {
            if (mkdir(log_dir, 0700) != 0) {
                perror("Could not create log directory");
                syslog(LOG_ERR, "Failed to create log directory: %s", log_dir);
                exit(EXIT_FAILURE);
            }
        }

        srv_log_fd = open(log_file, O_RDWR | O_CREAT | O_APPEND, 0600);
        if (srv_log_fd == -1) {
            perror("Could not create srv.log");
            syslog(LOG_ERR, "Failed to open srv.log");
            exit(EXIT_FAILURE);
        }
        syslog(LOG_INFO, "Service log file %s created successfully", log_file);
    }

    char time_stamp[64];
    get_time_stamp(time_stamp, sizeof(time_stamp));
    dprintf(srv_log_fd, "[%s] %s\n", time_stamp, message);
}

void write_dir_log(const char* message) {
    if (dir_log_fd == -1) {
        char log_file[1024];
        char datetime[64];
        get_time_stamp(datetime, sizeof(datetime));
        snprintf(log_file, sizeof(log_file), "%s/%s-dir.log", log_dir, datetime);

        struct stat st;
        if (stat(log_dir, &st) == -1) {
            if (mkdir(log_dir, 0700) != 0) {
                perror("Could not create log directory");
                syslog(LOG_ERR, "Failed to create log directory: %s", log_dir);
                exit(EXIT_FAILURE);
            }
        }

        dir_log_fd = open(log_file, O_RDWR | O_CREAT | O_APPEND, 0600);
        if (dir_log_fd == -1) {
            perror("Не удалось открыть dir.log");
            syslog(LOG_ERR, "Failed to open dir.log");
            exit(EXIT_FAILURE);
        }
        syslog(LOG_INFO, "Directory log file %s created successfully", log_file);
    }

    char time_stamp[64];
    get_time_stamp(time_stamp, sizeof(time_stamp));
    dprintf(dir_log_fd, "[%s] %s\n", time_stamp, message);
    fsync(dir_log_fd);
}

void get_time_stamp(char* buffer, size_t buffer_size) {
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    strftime(buffer, buffer_size, "%Y-%m-%d_%H-%M-%S", t);
}

void signal_handler(int signum) {
    struct stat st;
    switch (signum) {
    case SIGTERM:
        write_srv_log("Daemon stopped");
        syslog(LOG_INFO, "Daemon is shutting down");

        if (inotify_fd != -1) {
            if (inotify_rm_watch(inotify_fd, inotify_wd) == -1) {
                syslog(LOG_ERR, "Failed to remove inotify watch: %m");
            }
            close(inotify_fd);
            inotify_fd = -1;
        }

        if (srv_log_fd != -1) {
            close(srv_log_fd);
            srv_log_fd = -1;
        }
        if (dir_log_fd != -1) {
            close(dir_log_fd);
            dir_log_fd = -1;
        }
        exit(EXIT_SUCCESS);
        break;

    case SIGHUP:
        write_srv_log("Demon restarted");
        syslog(LOG_INFO, "Daemon is reloading configuration");
        read_config();
        if (stat(watch_dir, &st) == -1 || !S_ISDIR(st.st_mode)) {
            char message[1024];
            snprintf(message, sizeof(message), "Could not find watching directory %s", watch_dir);
            write_srv_log(message);
            syslog(LOG_ERR, "Watch directory does not exist: %s", watch_dir);
            exit(EXIT_FAILURE);
        }
        else {
            char message[1024];
            snprintf(message, sizeof(message), "Demon works with params %s %s", watch_dir, log_dir);
            write_srv_log(message);
            syslog(LOG_INFO, "Daemon is now working with parameters watch_dir: %s, log_dir: %s", watch_dir, log_dir);
        }
        break;
    }
}
