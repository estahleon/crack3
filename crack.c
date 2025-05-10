#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "md5.h"

#if __has_include("fileutil.h")
#include "fileutil.h"
#endif

#define PASS_LEN 50     // Maximum length any password will be.
#define HASH_LEN 33     // Length of hash plus one for null.

char **loadFileAA(char *filename, int *size)
{
    FILE *in = fopen(filename, "r");
    if (!in)
    {
        perror("Can't open file");
        exit(1);
    }

    int CAPACITY = 10;
    char **arr = malloc(CAPACITY * sizeof(char *));
    *size = 0;

    char buffer[1000];

    while (fgets(buffer, 1000, in))
    {
        if (*size >= CAPACITY)
        {
            CAPACITY *= 2;
            arr = realloc(arr, CAPACITY * sizeof(char *));
        }

        char *nl = strchr(buffer, '\n');
        if (nl) *nl = '\0';

        int len = strlen(buffer);
        char *str = malloc((len + 1) * sizeof(char));
        strcpy(str, buffer);
        arr[*size] = str;
        (*size)++;
    }
    fclose(in);
    return arr;
}

void freeAA(char **arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        free(arr[i]);
    }
    free(arr);
}

int search(char **arr, int size, char *target)
{
    for (int i = 0; i < size; i++)
    {
        if (strcmp(arr[i], target) == 0)
        {
            return i;
        }
    }
    return -1;
}

int main(int argc, char *argv[])
{
    if (argc < 3) 
    {
        printf("Usage: %s hash_file dictionary_file\n", argv[0]);
        exit(1);
    }

    // TODO: Read the hashes file into an array.
    //   Use either a 2D array or an array of arrays.
    //   Use the loadFile function from fileutil.c
    //   Uncomment the appropriate statement.
    int size;
    //char (*hashes)[HASH_LEN] = loadFile(argv[1], &size);
    char **hashes = loadFileAA(argv[1], &size);
    if (!hashes)
    {
        perror("Error loading hashes");
        exit(1);
    }
    
    // CHALLENGE1: Sort the hashes using qsort.
    
    // TODO
    // Open the password file for reading.
    FILE *dict = fopen(argv[2], "r");
    if (!dict)
    {
        perror("Can't open dictionary file");
        for (int i = 0; i < size; i++)
            free(hashes[i]);
        free(hashes);
        exit(1);
    }

    // TODO
    // For each password, hash it, then use the array search
    // function from fileutil.h to find the hash.
    // If you find it, display the password and the hash.
    // Keep track of how many hashes were found.
    char buffer[1000];
    int found = 0;

     while (fgets(buffer, sizeof(buffer), dict))
    {
        char *nl = strchr(buffer, '\n');
        if (nl) *nl = '\0';

        int index = search(hashes, size, buffer);
        if (index != -1)
        {
            printf("Match: %s\n", buffer);
            found++;
        }
    }

    // CHALLENGE1: Use binary search instead of linear search.

    // TODO
    // When done with the file:
    //   Close the file
    //   Display the number of hashes found.
    //   Free up memory.
    fclose(dict);

    printf("Cracked %d hashes.\n", found);

    for (int i = 0; i < size; i++)
        free(hashes[i]);
    free(hashes);
    return 0;
}

