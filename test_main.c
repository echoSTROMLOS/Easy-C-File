#include "easy_c_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    const char *directoryName = "my_directory";

    if (createDirectory(directoryName) == 0) {
        printf("Directory created: %s\n", directoryName);
    }

    char **fileList = NULL;  // Initialize fileList to NULL
    int numFiles = 0;

    const char *filename = "binary_file.bin";

    // Data to be written to the binary file
    unsigned char binaryData[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    size_t dataSize = sizeof(binaryData);

    char filepath[256]; // Adjust the size as needed
    snprintf(filepath, sizeof(filepath), "%s/%s", directoryName, filename);

    // Write binary data to the file
    if (write_binary_file(filepath, binaryData, dataSize) == 0) {
        printf("Binary data written to %s\n", filepath);
    }

    // You should allocate memory for fileList before adding elements
    fileList = (char **)malloc(sizeof(char *));
    if (fileList == NULL) {
        return 1;  // Handle allocation error
    }

    fileList[0] = (char *)malloc(strlen(filename) + 1);
    if (fileList[0] == NULL) {
        return 1;  // Handle allocation error
    }

    strcpy(fileList[0], filename);
    numFiles++;

    // Read binary data from the file
    void *readData;
    size_t readSize;

    if (read_binary_file(filepath, &readData, &readSize) == 0) {
        printf("Read %zu bytes from %s\n", readSize, filepath);
        free(readData);
    } else {
        printf("Failed to read binary data from %s\n", filepath);
    }

    if (listFilesInDirectory(directoryName, &fileList, &numFiles) == 0) {
        printf("Files in directory:\n");
        for (int i = 0; i < numFiles; i++) {
            printf("%s\n", fileList[i]);
            free(fileList[i]);
        }
        free(fileList);
    }

    return 0;
}
