/**
 * @file CompareSequences.c
 * @author  Or Nokrean <ornokrean@mail.huji.ac.il>
 * @version 1.0
 * @date 31 Oct 2018
 *
 * @brief this program reads the given file and print the sequence match scores.
 *
 * @section LICENSE
 * This program is not a free software; contact Or Nokrean for license.
 *
 * @section DESCRIPTION
 * this program reads the given file and print the sequence match scores.
 * Input  : sequence file.
 * Process: reading the sequences, saving them and calculating match scores between each sequence
 * Output : printed scores that was analyzed from the given file.
 */
// ------------------------------ includes ------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -------------------------- const definitions -------------------------
/**
* @def FILE_PATH_INDEX 1
* @brief The index of the file path
*/
#define FILE_PATH_INDEX 1


/**
* @def MATCH_INDEX 2
* @brief The index of the match index
*/
#define MATCH_INDEX 2

/**
* @def MISMATCH_INDEX 3
* @brief The index of the mismatch index
*/
#define MISMATCH_INDEX 3


/**
* @def GAP_INDEX 4
* @brief The index of the gap weight
*/
#define GAP_INDEX 4

/**
 * @def LINE_MAX_LEN 100
 * @brief The maximum line length
 */
#define LINE_MAX_LEN 101

/**
 * @def LINE_MAX_LEN 100
 * @brief The maximum atom line length
 */
#define MAX_SEQUENCES_LEN 100

/**
 * @var char START_OF_ROW
 * @brief Start of a valid new line
 */
const char START_OF_ROW = '>';

/**
 * @var char EMPTY_CHAR
 * @brief empty char to replace non wanted char
 */
static const char EMPTY_CHAR = '\0';

/**
 * @var char MEMORY_ERROR[]
 * @brief error message for memory alloc
 */
static const char MEMORY_ERROR[] = "ERROR no memory allocated";


/**
 * @var char NUMBER_ERROR[]
 * @brief error message for wrong number
 */
static const char NUMBER_ERROR[] = "ERROR Not a number";


/**
 * @var char FILE_ERROR[]
 * @brief error message for file read
 */
static const char FILE_ERROR[] = "ERROR opening file %s";


/**
 * @var char ONE_SEQ_ERROR[]
 * @brief error message for only one sequence in file
 */
static const char ONE_SEQ_ERROR[] = "ERROR, Only one sequence in file";


/**
 * @def MAX(a, b)
 * @brief A macro that returns the maximum between a and b
 */

#define MAX(a, b) ((a)>(b)) ? (a) : (b);



// ------------------------------ functions -----------------------------

/**
 * this function returns the maximum of three integer numbers, a, b and c.
 * @param a int, the first number
 * @param b int, the second number
 * @param c int, the three number
 * @return int, the biggest of thee given numbers.
 */
int getBiggerOfThree(int a, int b, int c)
{
    int max = MAX(a, b);
    max = MAX(max, c);
    return max;
}

/**
 * this function sets a new or old memory to given pointer. if ths memory allocation fails, it
 * will exit the program and print a message.
 * @param oldMemory the old memory, if exists, to copy to the new memory
 * @param newSize the new memory size to create.
 * @return a pointer to the new memory if created
 */
void *allocMemory(void *oldMemory, size_t newSize)
{
    void *newMemory = (oldMemory == NULL) ? malloc(newSize) : realloc(oldMemory, newSize);
    if (newMemory == NULL)
    {
        fprintf(stderr, MEMORY_ERROR);
        exit(EXIT_FAILURE);
    }
    return newMemory;
}

/**
 * this function frees all given memory
 * @param names a memory to free
 * @param seqs a memory to free
 * @param n the size of the given memory to free
 */
void freeMemory(char *names[MAX_SEQUENCES_LEN], char *seqs[MAX_SEQUENCES_LEN], int n)
{
    for (int i = 0; i <= n; ++i)
    {
        free(names[i]);
        free(seqs[i]);
    }
}

/**
 * this function removes a given char from the given string, and will replace it with EMPTY_CHAR.
 * @param currLine the line to remove the given char from
 * @param to_remove the char to remove
 */
void removeChar(const char *currLine, char to_remove)
{
    char *pos;
    if ((pos = strchr(currLine, to_remove)) != NULL)
    {
        *pos = EMPTY_CHAR;
    }
}

/**
 * this function creates a new sequence by the given parameters.
 * @param file the file to read from
 * @param names the names array from the file
 * @param seqs the sequences array from the file
 * @param currLine the current line of the file
 * @param currLength the current length of the line from the file, that will be accumulated.
 * @param index the index of the line we are in now
 */
void createNewSequence(FILE *file, char *names[MAX_SEQUENCES_LEN], char *seqs[MAX_SEQUENCES_LEN],
                       char *currLine, size_t *currLength, int *index)
{
    (*index)++;
    names[(*index)] = (char *) allocMemory(NULL, sizeof(char) * strlen(currLine) + 1);
    // get the line name
    if (names[(*index)] != NULL)
    {
        strncpy(names[(*index)], &currLine[1], strlen(currLine));
    }
    fgets(currLine, LINE_MAX_LEN, file);
    removeChar(currLine, '\n');
    (*currLength) = strlen(currLine);
    seqs[(*index)] = (char *) allocMemory(NULL, sizeof(char) * (*currLength) + 1);
    strcpy(seqs[(*index)], &currLine[0]);
}

/**
 * this function converts given string to an int, and prints error on fail
 * @param to_convert the string to convert
 * @return int, the converted string
 */
int convertToInt(const char *to_convert)
{
    char *temp = NULL;
    const int res = (int) strtol(to_convert, &temp, 10);
    if (strlen(temp))
    {
        fprintf(stderr, NUMBER_ERROR);
        exit(EXIT_FAILURE);
    }
    return res;
}


/**
 * this function adds the new data to the given sequence
 * @param seqs the sequences array
 * @param currLine the current line from the file
 * @param currLength the accumulated length of the current sequence from the file
 * @param index the index of the line in the file
 * @return the new length, accumulated from all of the sequence.
 */
size_t addToSequence(char *seqs[MAX_SEQUENCES_LEN], char *currLine, size_t currLength, int index)
{
    currLength += strlen(currLine);
    seqs[index] = (char *) allocMemory(seqs[index], sizeof(char) * currLength + 1);
    strcat(seqs[index], currLine);
    return currLength;
}

/**
 * this function calculates the cell value according to some conditions.
 * @param table the table to work with
 * @param i the current rows index
 * @param j the current columns index
 * @param str1 the first string
 * @param str2 the second string
 * @param col the number of columns
 * @param gap the gap weight
 * @param match the match weight
 * @param mismatch the mismatch weight
 * @return the value of the cell.
 */
int calcCell(const int *table, int i, int j, const char *str1, const char *str2, int col, int gap,
             int match, int mismatch)
{
    int up_val = *(table + (i - 1) * col + j) + gap;
    int left_val = *(table + i * col + j - 1) + gap;
    int ul_val = *(table + (i - 1) * col + j - 1);
    ul_val += (str1[i - 1] == str2[j - 1]) ? match : mismatch;
    int max = getBiggerOfThree(up_val, left_val, ul_val);
    return max;
}

/**
 * this function initiates a new table
 * @param table the table to work with
 * @param row the number of rows
 * @param col the number of columns
 * @param gap the gap weight
 */
void initiateTable(int *table, int row, int col, int gap)
{
    // fill the first row

    for (int k = 0; k < col; ++k)
    {
        *(table + k) = k * gap;
    }
    // fill the first col
    for (int k = 0; k < row; ++k)
    {
        *(table + k * col) = k * gap;
    }
}

/**
 * this function creates and initiates the table to work with, and calculates all the cells, then
 * returns the match value.
 * @param str1 the first string
 * @param str2 the second string
 * @param gap the gap weight
 * @param match the match weight
 * @param mismatch the mismatch weight
 * @return the result of the matching table.
 */
int createAndCalcMatches(char *str1, char *str2, int gap, int match, int mismatch)
{
    int row = (int) strlen(str1) + 1, col = (int) strlen(str2) + 1;

    // create the table
    int *table = (int *) allocMemory(NULL, (row) * (col) * sizeof(int));
    initiateTable(table, row, col, gap);

    //calc all the cells
    for (int i = 1; i < row; ++i)
    {
        for (int j = 1; j < col; ++j)
        {
            *(table + i * col + j) = calcCell(table, i, j, str1, str2, col, gap, match, mismatch);
        }
    }
    // get the match result
    int result = *(table + (row - 1) * col + col - 1);
    free(table);
    return result; // return the last place
}

/**
 * this function compares sequences two by two and prints each score to the screen
 * @param match the match weight
 * @param mismatch the mismatch weight
 * @param gap the gap weight
 * @param names the names array
 * @param seqs the sequences array
 * @param n the size of the arrays.
 */
void compareSequences(int match, int mismatch, int gap, char *names[MAX_SEQUENCES_LEN],
                      char *seqs[MAX_SEQUENCES_LEN], int n)
{//now we create a table:
//all the lines in text:
    for (int i = 0; i <= n; ++i)
    {
        //for all other lines in text
        for (int j = i + 1; j <= n; ++j)
        {
            {
                //create table and compare
                int res = createAndCalcMatches(seqs[i], seqs[j], gap, match, mismatch);
                printf("Score for alignment of %s to %s is %d\n", names[i], names[j], res);
            }
        }
    }
}

/**
 * this function reads the given file
 * @param file the file to read
 * @return the index - the number of sequences read from the file
 */
int readFile(FILE *file)
{
    char *names[MAX_SEQUENCES_LEN];
    char *seqs[MAX_SEQUENCES_LEN];
    char currLine[LINE_MAX_LEN];
    size_t currLength = 0;
    int index = -1; // we increase index before using, so it will be 0 in first run.
    while (fgets(currLine, LINE_MAX_LEN, file) != NULL)
    {
        removeChar(currLine, '\n');        // remove '\n'

        if (currLine[0] == START_OF_ROW) // new sequence
        {
            currLength = 0;
            createNewSequence(file, names, seqs, currLine, &currLength, &index);
        }
        else // add to current sequence
        {
            currLength = addToSequence(seqs, currLine, currLength, index);
        }
    }
    fclose(file);
    return index;
}


/**
 * this is the main function of the program
 */
int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        printf("ERROR Usage: CompareSequences <path_to_sequences_file> <m> <s> <g>");
        exit(EXIT_FAILURE);
    }

    char *filePath = argv[FILE_PATH_INDEX];
    int match = convertToInt(argv[MATCH_INDEX]);
    int mismatch = convertToInt(argv[MISMATCH_INDEX]);
    int gap = convertToInt(argv[GAP_INDEX]);

    FILE *file;
    file = fopen(filePath, "r");
    if (file == NULL)
    {
        fprintf(stderr, FILE_ERROR, filePath);
        exit(EXIT_FAILURE);
    }

    char *names[100];
    char *seqs[100];
    int index = readFile(file);
    if (index < 1)
    {
        fprintf(stderr, ONE_SEQ_ERROR);
        freeMemory(names, seqs, index);
        exit(EXIT_FAILURE);
    }
    compareSequences(match, mismatch, gap, names, seqs, index);
    // free all alloc
    freeMemory(names, seqs, index);
    return 0;
}
