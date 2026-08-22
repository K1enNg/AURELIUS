// 0 — Standard Input (STDIN_FILENO): Reads input from the keyboard/terminal.
// 1 — Standard Output (STDOUT_FILENO): Writes output to the terminal.
// 2 — Standard Error (STDERR_FILENO): Writes error messages to the terminal.

// #include <fcntl.h>    For open() flags (O_RDONLY, O_WRONLY, O_CREAT, etc.)
// #include <unistd.h>   For read(), write(), close()
// #include <sys/stat.h> For file permissions (S_IRUSR, S_IWUSR)

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024

int main() {
    const char *sourceFile = "source.txt";
    const char *destFile = "destination.txt";

    // create a dummy source file using low-level write
    int src_fd = open(sourceFile, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (src_fd < 0) {
        perror("error create source file");
        close(src_fd);
        return 1;
    }

    const char *initialData = "from low-level system call.\n direct kernel.";
    write(src_fd, initialData, 58);
    close(src_fd);

    // open source for reading
    src_fd = open(sourceFile, O_RDONLY);
    if(src_fd < 0) {
        perror("error opening source file");
        return 1;
    }

    // open destination for writing
    int dest_fd = open(destFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd < 0) {
        perror("error opening destination file");
        close(src_fd);
        return 1;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;

    while ((bytesRead = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
        write(dest_fd, buffer, bytesRead);
    }

    const char *msg = "file copy completed successfully.\n";
    write(STDOUT_FILENO, msg, 44);

    close(src_fd);
    close(dest_fd);

    return 0;
}