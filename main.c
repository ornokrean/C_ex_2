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
#define MAX(a, b) ((a)>(b)) ? a : b;


//
//int convert_to_int(const char *to_convert)
//{
//    char *temp = NULL;
//    const int res = (int) strtol(to_convert, &temp, 10);
//    if (strlen(temp))
//    {
//        fprintf(stderr,"ERROR Not a number");
//        // FIXME NOT A NUMBER FREE MEMORYYYYYYY
//        exit(EXIT_FAILURE);
//    }
//    return res;
//}
int calc_cell(int *table, int i, int j)
{

    return 0;

}

int create_table_and_compare(char *str1, char *str2, int gap, int match, int mismatch)
{
    int row = (int) strlen(str1)+1, col = (int) strlen(str2)+1;
    int *table = (int *) malloc((row) * (col) * sizeof(int));
    int last = 0; // FIXME SAVE LAST PLACE
    char *l[100];
    for (int k = 0; k < col; ++k)
    {
        *(table + k) = k*gap;
    }
    for (int k = 0; k < row; ++k)
    {
        *(table + k * col ) = k*gap;

    }



    for (int i = 1; i < row; ++i)
    {
        for (int j = 1; j < col; ++j)
        {
            int up_val = *(table + (i - 1) * col + j) + gap;
            int left_val = *(table + i * col + j - 1) + gap;
            int ul_val = *(table + (i - 1) * col + j - 1);
            ul_val += (str1[j - 1] == str2[i - 1]) ? match : mismatch;
            int max = MAX(up_val,left_val);
            max = MAX(max,ul_val);
            *(table + i * col + j) = max;
            last= max;
        }

    }
    for (int l = 0; l < row; ++l)
    {
        for (int i = 0; i < col; ++i)
        {
            if (*(table + l * col + i)>=0)
            {
                printf("% d ", *(table + l * col + i));
            }else{
                printf("%d ", *(table + l * col + i));

            }
        }
        printf("\n");
    }
    free(table);
    return last;


}


int main()
{
    char *filePath = "/Users/or/Desktop/CS/year2/C/ex2/input.txt";
//    if (argc != 5)
//    {
//        printf("ERROR"); //FIXME ERRROR OF NUM ARGS
//    }
//
//    char *filePath = argv[1];
//    int match = convert_to_int(argv[2]);
//    int mismatch = convert_to_int(argv[3]);
//    int gap = convert_to_int(argv[4]);


    int match = 1;
    int mismatch = 0;
    int gap = -2;

    filePath = "/cs/usr/ornokrean/Desktop/ex2/input.txt";

    FILE *file;
    file = fopen(filePath, "r");
    if (file == NULL)
    {

        //fprintf(stderr, OPENING_FILE_ERR, filePath);
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
        char *pos;
        if ((pos = strchr(currLine, '\n')) != NULL)
            *pos = '\0';


        if (currLine[0] == START_OF_ROW) // new line
        {
            index++;

            names[index] = (char *) malloc(sizeof(char) * strlen(currLine) + 1);
//             get the line name
            if (names[index] != NULL)
            {
                if (currLine != NULL)
                    strncpy(names[index], &currLine[1], strlen(currLine) - 1);
            }
            fgets(currLine, LINE_MAX_LEN, file);
            char *pos;
            if ((pos = strchr(currLine, '\n')) != NULL)
                *pos = '\0';
            if (currLine != NULL)
                currLen = strlen(currLine);

            seqs[index] = (char *) malloc(sizeof(char) * currLen + 1);
            strcpy(seqs[index], &currLine[0]);


        } else // add to current line
        {
            currLen += strlen(currLine);
            seqs[index] = (char *) realloc(seqs[index], sizeof(char) * currLen + 1);


            //FIXME handle no alloc
            strcat(seqs[index], currLine);

            // add the current data
        }
    }
    fclose(file);
//    for (int j = 0; j <= index; ++j)
//    {
//        printf("name: %s\ndata: %s\n\n\n", names[j], seqs[j]);
//    }

    //now we create a table:
    //all the lines in text:
    for (int i = 0; i <= 1; ++i)
    {
        //for all other lines in text
        for (int j = i + 1; j <= index; ++j)
        {
            {
                //create table and compare
               int res= create_table_and_compare(seqs[i],seqs[j],gap,match,mismatch);
               printf("Score for alignment of sequence %s to sequence %s is %d\n",names[i],
                       names[j],res);

            }
        }
    }


    //
    // free all alloc
    for (int i = 0; i <= index; ++i)
    {
        free(names[i]);
        free(seqs[i]);

    }
    return 0;
}


// gcc -Wextra -Wall -Wvla -lm main.c -o ex2

//valgrind --leak-check=full --show-possibly-lost=yes --show-reachable=yes --undef-value-errors=yes ex2