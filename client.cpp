#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#define FILENAME "shared_file.txt"
#define FILESIZE 4096

void executeProjection(int *fd, char **ptr) {
    // Открытие существующего файла
    struct stat fst;
    *fd = open(FILENAME, O_RDWR);
    if (*fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fstat(*fd, &fst);
    // Отображение файла в память
    *ptr = (char*)mmap(NULL, fst.st_size, PROT_READ, MAP_SHARED, *fd, 0);
    if (*ptr == MAP_FAILED) {
        perror("Error mapping file to memory");
        exit(EXIT_FAILURE);
    }
}

void readData(char *ptr) {
    // Чтение данных из общей памяти
    printf("Client received: %s\n", ptr);
}

void closeProjection(int fd, char *ptr) {
    // Отмена отображения
    if (munmap(ptr, FILESIZE) == -1) {
        perror("Error unmapping file");
    }

    // Закрытие файла
    close(fd);
}

int main() {
    int fd;
    char *ptr;

    int choice;
    do {
        printf("\nClient Menu:\n");
        printf("1. Execute Projection\n");
        printf("2. Read Data\n");
        printf("3. Close Projection and Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                executeProjection(&fd, &ptr);
                break;
            case 2:
                readData(ptr);
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
