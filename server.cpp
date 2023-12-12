#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#define FILENAME "shared_file.txt"
#define FILESIZE 4096

void executeProjection(int *fd, char **ptr) {
    // Создание файла
    *fd = open(FILENAME, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (*fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    ftruncate(*fd, FILESIZE);
    // Отображение файла в память
    *ptr = (char *)mmap(NULL, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, *fd, 0);
    if (*ptr == MAP_FAILED) {
        perror("Error mapping file to memory");
        exit(EXIT_FAILURE);
    }
}

void writeData(char *ptr) {
    // Запись данных в общую память
    if(ptr) sprintf(ptr, "Hello, shared memory!");
}

void closeProjection(int fd, char *ptr) {
    // Отмена отображения
    if (munmap(ptr, FILESIZE) == -1) {
        perror("Error unmapping file");
    }

    // Удаление файла
    close(fd);
    unlink(FILENAME);
}

int main() {
    int fd;
    char *ptr;

    int choice;
    do {
        printf("\nServer Menu:\n");
        printf("1. Execute Projection\n");
        printf("2. Write Data\n");
        printf("3. Close Projection and Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                executeProjection(&fd, &ptr);
                break;
            case 2:
                writeData(ptr);
                break;
            case 3:
                closeProjection(fd, ptr);
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);

    return 0;
}
