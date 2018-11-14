#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * @def LINE_MAX_LEN 80
 * @brief The maximum atom line length
 */
#define LINE_MAX_LEN 100


/**
 * @var char START_OF_ROW
 * @brief Start of a valid new line
 */
const char START_OF_ROW = '>';

void
create_new_sequence(const FILE *file, char *const *names, char *const *seqs, const char *currLine,
                    size_t *currLen, int *index);

#define MAX(a, b) ((a)>(b)) ? a : b;


int convert_to_int(const char *to_convert)
{
    char *temp = NULL;
    const int res = (int) strtol(to_convert, &temp, 10);
    if (strlen(temp))
    {
        fprintf(stderr, "ERROR Not a number");
        // FIXME NOT A NUMBER FREE MEMORYYYYYYY
        exit(EXIT_FAILURE);
    }
    return res;
}

int calc_cell(int *table, int i, int j, char *str1, char *str2, int col, int gap, int match, int
mismatch)
{
    int up_val = *(table + (i - 1) * col + j) + gap;
    int left_val = *(table + i * col + j - 1) + gap;
    int ul_val = *(table + (i - 1) * col + j - 1);
    ul_val += (str1[i - 1] == str2[j - 1]) ? match : mismatch;
    int max = MAX(up_val, left_val);
    max = MAX(max, ul_val);
    return max;
}

void initiate_table(int *table, int row, int col, int gap)
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

int create_table_and_compare(char *str1, char *str2, int gap, int match, int mismatch)
{
    int row = (int) strlen(str1) + 1, col = (int) strlen(str2) + 1;

    // create the table
    int *table = (int *) malloc((row) * (col) * sizeof(int));
    initiate_table(table, row, col, gap);

    //calc all the cells
    for (int i = 1; i < row; ++i)
    {
        for (int j = 1; j < col; ++j)
        {
            *(table + i * col + j) = calc_cell(table, i, j, str1, str2, col, gap, match, mismatch);
        }
    }
    // get the match result
    int result = *(table + (row - 1) * col + col - 1);
    free(table);
    return result; // return the last place
}

void remove_char(const char *currLine, char to_remove)
{
    char *pos;
    if ((pos = strchr(currLine, to_remove)) != NULL)
        *pos = '\0';
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        fprintf(stderr,"ERROR"); //FIXME ERRROR OF NUM ARGS
        exit(EXIT_FAILURE);
    }

    char *filePath = argv[1];
    int match = convert_to_int(argv[2]);
    int mismatch = convert_to_int(argv[3]);
    int gap = convert_to_int(argv[4]);

    FILE *file;
    file = fopen(filePath, "r");
    if (file == NULL)
    {
        fprintf(stderr, "ERROR opening file %s", filePath);
        exit(EXIT_FAILURE);
    }

    char *names[100];
    char *seqs[100];
    char currLine[102];
    size_t currLen = 0;
    int index = -1;
    while (fgets(currLine, LINE_MAX_LEN, file) != NULL)
    {
        // remove '\n'
        remove_char(currLine,'\n');

        if (currLine[0] == START_OF_ROW) // new line
        {
            create_new_sequence(file, names, seqs, currLine, &currLen, &index);
        }
        else // add to current line
        {
            currLen += strlen(currLine);
            seqs[index] = (char *) realloc(seqs[index], sizeof(char) * currLen + 1);
            //FIXME handle no alloc
            strcat(seqs[index], currLine);

            // add the current data
        }
    }
    fclose(file);

    //now we create a table:
    //all the lines in text:
    for (int i = 0; i <= 1; ++i)
    {
        //for all other lines in text
        for (int j = i + 1; j <= index; ++j)
        {
            {
                //create table and compare
                int res = create_table_and_compare(seqs[i], seqs[j], gap, match, mismatch);
                printf("Score for alignment of sequence %s to sequence %s is %d\n", names[i],
                       names[j], res);

            }
        }
    }

    // free all alloc
    for (int i = 0; i <= index; ++i)
    {
        free(names[i]);
        free(seqs[i]);

    }
    return 0;
}

void
create_new_sequence(const FILE *file, char *names[100], char *seqs[100], const char *currLine,
                    size_t *currLen, int *index)
{
    (*index)++;
    names[(*index)] = (char *) malloc(sizeof(char) * strlen(currLine) + 1);
//             get the line name
    if (names[(*index)] != NULL)
            {
                strncpy(names[(*index)], &currLine[1], strlen(currLine));
            }
    fgets(currLine, LINE_MAX_LEN, file);

    remove_char(currLine,'\n');

    (*currLen) = strlen(currLine);
    seqs[(*index)] = (char *) malloc(sizeof(char) * (*currLen) + 1);
    strcpy(seqs[(*index)], &currLine[0]);
}




// gcc -Wextra -Wall -Wvla -lm main.c -o ex2

//valgrind --leak-check=full --show-possibly-lost=yes --show-reachable=yes --undef-value-errors=yes ex2