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


typedef struct Line
{
    char *name;
    char *data;
} Line;

Line *initLine()
{
    Line *line = (Line *) malloc(sizeof(Line));
    line->name = (char *) malloc(sizeof(char));
    line->data = (char *) malloc(sizeof(char));
    return line;
}

void deleteLine(Line *line)
{
    if (line == NULL)
    {
        return;
    }

    if (line->name != NULL)
    {
        free(line->name);

    }
    if (line->data != NULL)
    {
       free(line->data);
    }
    free(line);


}


int main()
{
//    char *filePath = "/Users/or/Desktop/CS/year2/C/ex2/input.txt";
    char *filePath = "/cs/usr/ornokrean/Desktop/ex2/input.txt";


    FILE *file;
    file = fopen(filePath, "r");
    if (file == NULL)
    {
        //printf(stderr, OPENING_FILE_ERR, filePath);
        exit(EXIT_FAILURE);
    }

    Line *line = NULL;
    size_t textSize = sizeof(Line);
    Line *text = (Line*) malloc(textSize);
    char currLine[100];
    size_t currLen = 0;
    int index = 0;
    while (fgets(currLine, LINE_MAX_LEN, file) != NULL)
    {
        if (currLine[0] == START_OF_ROW) // new line
        {
            textSize += sizeof(Line);
            text = (Line*) realloc(text,textSize);
            currLen = 0;
            line = initLine();
            text[index] = *line;
            index++;
            // get the line name
            line->name = (char *) realloc(line->name, (sizeof(char) * strlen(currLine))+1);
            line->data = "";
            if (line->name != NULL)
            {
                strncpy(line->name, &currLine[1], strlen(currLine) - 1);
            }
        }
        else // add to current line
        {
            currLen += strlen(currLine);
            line->data = (char *) realloc(line->data, (sizeof(char) * currLen)+1);
            if (line->data != NULL)
            {
                //FIXME handle no alloc
                char *pos;
                if ((pos=strchr(currLine, '\n')) != NULL)
                    *pos = '\0';
                strcat(line->data, currLine);

            }
            // add the current data
        }
    }
    fclose(file);

    for (int j = 0; j < index; ++j)
    {
        printf("name: %s\ndata: %s\n",text[j].name,text[j].data);
    }
    // free all alloc
//    for (int i = 0; i < index; ++i)
//    {
//        deleteLine(&text[i]);
//    }
//    free(text);

    return 0;
}


// gcc -Wextra -Wall -Wvla -lm main.c -o ex2

//valgrind --leak-check=full --show-possibly-lost=yes --show-reachable=yes --undef-value-errors=yes ex2