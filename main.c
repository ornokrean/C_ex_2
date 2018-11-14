#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * @def LINE_MAX_LEN 100
 * @brief The maximum line length
 */
#define LINE_MAX_LEN 100

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


#define MAX(a, b) ((a)>(b)) ? a : b;


int getBiggerOfThree(int a, int b, int c){
    int max = MAX(a, b);
    max = MAX(max, c);
    return max;
}

void *setMemory(void *oldMemory, size_t newSize)
{
    void *newMemory = (oldMemory == NULL) ? malloc(newSize) : realloc(oldMemory, newSize);
    if (newMemory == NULL)
    {
        fprintf(stderr, "ERROR no memory allocated");
        exit(EXIT_FAILURE);
    }
    return newMemory;
}


void freeMemory(char *names[MAX_SEQUENCES_LEN], char *seqs[MAX_SEQUENCES_LEN], int index)
{
    for (int i = 0; i <= index; ++i)
    {
        free(names[i]);
        free(seqs[i]);
    }
}

void removeChar(const char *currLine, char to_remove)
{
    char *pos;
    if ((pos = strchr(currLine, to_remove)) != NULL)
    {
        *pos = EMPTY_CHAR;
    }
}

void createNewSequence(FILE *file, char **names, char **seqs, char *currLine, size_t *currLen,
                       int *index)
{
    (*index)++;
    names[(*index)] = (char *) setMemory(NULL, sizeof(char) * strlen(currLine) + 1);
    // get the line name
    if (names[(*index)] != NULL)
    {
        strncpy(names[(*index)], &currLine[1], strlen(currLine));
    }
    fgets(currLine, LINE_MAX_LEN, file);
    removeChar(currLine, '\n');
    (*currLen) = strlen(currLine);
    seqs[(*index)] = (char *) setMemory(NULL, sizeof(char) * (*currLen) + 1);
    strcpy(seqs[(*index)], &currLine[0]);
}


int convertToInt(const char *to_convert)
{
    char *temp = NULL;
    const int res = (int) strtol(to_convert, &temp, 10);
    if (strlen(temp))
    {
        fprintf(stderr, "ERROR Not a number");
        exit(EXIT_FAILURE);
    }
    return res;
}


size_t addToSequence(char *seqs[MAX_SEQUENCES_LEN], char *currLine, size_t currLen, int index)
{
    currLen += strlen(currLine);
    seqs[index] = (char *) setMemory(seqs[index], sizeof(char) * currLen + 1);
    strcat(seqs[index], currLine);
    return currLen;
}


int calcCell(const int *table, int i, int j, const char *str1, const char *str2, int col, int gap,
             int match, int mismatch)
{
    int up_val = *(table + (i - 1) * col + j) + gap;
    int left_val = *(table + i * col + j - 1) + gap;
    int ul_val = *(table + (i - 1) * col + j - 1);
    ul_val += (str1[i - 1] == str2[j - 1]) ? match : mismatch;
    int max = getBiggerOfThree(up_val, left_val,ul_val);
    return max;
}


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

int createAndCalcMatches(char *str1, char *str2, int gap, int match, int mismatch)
{
    int row = (int) strlen(str1) + 1, col = (int) strlen(str2) + 1;

    // create the table
    int *table = (int *) setMemory(NULL, (row) * (col) * sizeof(int));
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


void compareSequences(int match, int mismatch, int gap, char *names[MAX_SEQUENCES_LEN],
                      char *seqs[MAX_SEQUENCES_LEN], int index)
{//now we create a table:
//all the lines in text:
    for (int i = 0; i <= 1; ++i)
    {
        //for all other lines in text
        for (int j = i + 1; j <= index; ++j)
        {
            {
                //create table and compare
                int res = createAndCalcMatches(seqs[i], seqs[j], gap, match, mismatch);
                printf("Score for alignment of sequence %s to sequence %s is %d\n", names[i],
                       names[j], res);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        printf("ERROR CompareSequences <path_to_sequences_file> <m> <s> <g>");
        exit(EXIT_FAILURE);
    }

    char *filePath = argv[1];
    int match = convertToInt(argv[2]);
    int mismatch = convertToInt(argv[3]);
    int gap = convertToInt(argv[4]);

    FILE *file;
    file = fopen(filePath, "r");
    if (file == NULL)
    {
        fprintf(stderr, "ERROR opening file %s", filePath);
        exit(EXIT_FAILURE);
    }

    char *names[MAX_SEQUENCES_LEN];
    char *seqs[MAX_SEQUENCES_LEN];
    char currLine[LINE_MAX_LEN + 2];
    size_t currLength = 0;
    int index = -1;
    while (fgets(currLine, LINE_MAX_LEN, file) != NULL)
    {
        removeChar(currLine, '\n');        // remove '\n'

        if (currLine[0] == START_OF_ROW) // new sequence
        {
            createNewSequence(file, names, seqs, currLine, &currLength, &index);
        }
        else // add to current sequence
        {
            currLength = addToSequence(seqs, currLine, currLength, index);
        }
    }
    fclose(file);
    compareSequences(match, mismatch, gap, names, seqs, index);
    // free all alloc
    freeMemory(names, seqs, index);
    return 0;
}








// gcc -Wextra -Wall -Wvla -lm main.c -o ex2

//valgrind --leak-check=full --show-possibly-lost=yes --show-reachable=yes --undef-value-errors=yes ex2 input.txt 1 0 -2