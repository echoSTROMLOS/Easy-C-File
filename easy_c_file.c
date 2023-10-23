// easyfile.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef _WIN32
    #include <windows.h>
    #include <io.h>
    #include <direct.h>
    #define mkdir(dir, mode) _mkdir(dir)
    #define rmdir _rmdir
    #define access _access
    #define F_OK 0
#else
    #include <dirent.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <unistd.h>
#endif

int createDirectory(const char *path)
{
#ifdef _WIN32
    if (CreateDirectory(path, NULL) || ERROR_ALREADY_EXISTS == GetLastError())
    {
        return 0;
    }
    return 1;
#else
    if (mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0)
    {
        return 0;
    }
    return 1;
#endif
}

int listFilesInDirectory(const char *directory, char ***fileList, int *numFiles)
{
#ifdef _WIN32
    struct _finddata_t fileInfo;
    intptr_t fileHandle;
    char searchPath[260];
    int count = 0;

    snprintf(searchPath, sizeof(searchPath), "%s/*.*", directory);

    if ((fileHandle = _findfirst(searchPath, &fileInfo)) != -1)
    {
        do
        {
            if (strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0)
            {
                count++;
            }
        } while (_findnext(fileHandle, &fileInfo) == 0);
        _findclose(fileHandle);
    }

    *numFiles = count;
    *fileList = (char **)malloc(count * sizeof(char *));

    if (*fileList == NULL)
    {
        return 1;
    }

    fileHandle = _findfirst(searchPath, &fileInfo);

    if (fileHandle == -1)
    {
        free(*fileList);
        return 1;
    }

    int i = 0;
    do
    {
        if (strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0)
        {
            (*fileList)[i] = (char *)malloc(strlen(fileInfo.name) + 1);

            if ((*fileList)[i] == NULL)
            {
                free(*fileList);
                _findclose(fileHandle);
                return 1;
            }
            strcpy((*fileList)[i], fileInfo.name);
            i++;
        }
    } while (_findnext(fileHandle, &fileInfo) == 0);

    _findclose(fileHandle);

#else
    struct dirent *entry;
    DIR *dir = opendir(directory);

    if (dir == NULL)
    {
        return 1;
    }

    *numFiles = 0;

    while ((entry = readdir(dir)))
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            (*numFiles)++;
        }
    }

    *fileList = (char **)malloc(sizeof(char *) * (*numFiles));

    if (*fileList == NULL)
    {
        closedir(dir);
        return 1;
    }

    rewinddir(dir);

    int i = 0;
    while ((entry = readdir(dir)) && i < *numFiles)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            (*fileList)[i] = (char *)malloc(strlen(entry->d_name) + 1);
            if ((*fileList)[i] == NULL)
            {
                closedir(dir);
                return 1;
            }
            strcpy((*fileList)[i], entry->d_name);
            i++;
        }
    }

    closedir(dir);
#endif

    return 0;
}

int writeFile(const char *filename, const char *data)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        return 1;
    }
    fprintf(file, "%s", data);
    fclose(file);
    return 0;
}

int readFile(const char *filename, char **data)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        return 1;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    *data = (char *)malloc(file_size + 1);
    if (*data == NULL)
    {
        return 1;
    }
    if (fread(*data, 1, file_size, file) != file_size)
    {
        free(*data);
        return 1;
    }
    (*data)[file_size] = '\0';
    fclose(file);
    return 0;
}
int read_binary_file(const char *filename, void **data, size_t *size) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        return 1;
    }

    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    fseek(file, 0, SEEK_SET);

    *data = malloc(*size);
    if (*data == NULL) {
        fclose(file);
        return 1;
    }

    if (fread(*data, 1, *size, file) != *size) {
        free(*data);
        fclose(file);
        return 1;
    }

    fclose(file);
    return 0;
}

int write_binary_file(const char *filename, const void *data, size_t size) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        return 1;
    }

    if (fwrite(data, 1, size, file) != size) {
        fclose(file);
        return 1;
    }

    fclose(file);
    return 0;
}

int appendLine(const char *filename, const char *line)
{
    FILE *file = fopen(filename, "a");
    if (file == NULL)
    {
        return 1;
    }
    fprintf(file, "%s\n", line);
    fclose(file);
    return 0;
}

int deleteFile(const char *filename)
{
    if (remove(filename) != 0)
    {
        return 1;
    }
    return 0;
}

int deleteDirectory(const char *directory)
{
#ifdef _WIN32
    if (RemoveDirectory(directory))
    {
        return 0;
    }
    return 1;
#else
    if (rmdir(directory) == 0)
    {
        return 0;
    }
    return 1;
#endif
}