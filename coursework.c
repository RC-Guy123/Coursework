// Austin Jewell, ID:11116019
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define NUMROWS 9     // map size
#define NUMCOLS 9     // map size
#define NUMWALKS 1000 // number of random walks; 1000
#define NUMSTEP 10    // number of steps in each walk
int main(void)
{
    srand(123456);
    FILE *file = fopen("island_map.txt", "r");
    if (file == NULL)
    {
        printf("Error!");
        return 1;
    }
    char map[NUMROWS][NUMCOLS];
    float probarray[NUMROWS][NUMCOLS];
    float meanarray[NUMROWS][NUMCOLS];
    float SDarray[NUMROWS][NUMCOLS][NUMWALKS + 1];
    int i, j, k, l, m;
    for (i = 0; i < NUMROWS; i++)
    {
        for (j = 0; j < NUMCOLS; j++)
        {
            probarray[i][j] = 0;
            meanarray[i][j] = 0;
            for (m = 0; m <= NUMWALKS; m++)
            {
                SDarray[i][j][m] = 0;
            }
        }
    }
    for (j = 0; j < NUMCOLS; j++)
    {
        for (i = 0; i < NUMROWS; i++)
        {
            fscanf(file, " %c", &map[i][j]);
        }
    }
    fclose(file);
    for (j = 0; j < NUMCOLS; j++)
    {
        if (j >= NUMCOLS)
        {
            printf("Error!");
            return 1;
        }
        for (i = 0; i < NUMROWS; i++)
        {
            if (i >= NUMROWS)
            {
                printf("Error!");
                return 1;
            }
            int success = 0;
            double prob = 0;
            double mean = 0;
            float SD = 0;
            char tile = map[i][j];
            if (tile != 'B' && tile != 'W' && tile != 'V' && tile != 'D')
            {
                for (k = 0; k < NUMWALKS; k++)
                {
                    if (k >= NUMWALKS)
                    {
                        printf("Error!");
                        return 1;
                    }
                    int itemp = i;
                    int jtemp = j;
                    int status = 0;
                    for (l = 1; l <= NUMSTEP; l++)
                    {
                        if (l > NUMSTEP)
                        {
                            printf("Error!");
                            return 1;
                        }
                        if (status == 0)
                        {
                            int random = rand() % 8;
                            if (random == 0)
                            {
                                if (jtemp > 0)
                                {
                                    jtemp -= 1;
                                }
                            }
                            else if (random == 1)
                            {
                                if (itemp < NUMROWS && jtemp > 0)
                                {
                                    jtemp -= 1;
                                    itemp += 1;
                                }
                                else if (itemp < NUMROWS)
                                {
                                    itemp += 1;
                                }
                                else if (jtemp > 1)
                                {
                                    jtemp -= 1;
                                }
                            }
                            else if (random == 2)
                            {
                                if (itemp < NUMROWS - 1)
                                {
                                    itemp += 1;
                                }
                            }
                            else if (random == 3)
                            {
                                if (itemp < NUMROWS - 1 && jtemp < NUMCOLS - 1)
                                {
                                    jtemp += 1;
                                    itemp += 1;
                                }
                                else if (itemp < NUMROWS - 1)
                                {
                                    itemp += 1;
                                }
                                else if (jtemp < NUMCOLS - 1)
                                {
                                    jtemp += 1;
                                }
                            }
                            else if (random == 4)
                            {
                                if (jtemp < NUMCOLS - 1)
                                {
                                    jtemp += 1;
                                }
                            }
                            else if (random == 5)
                            {
                                if (itemp > 0 && jtemp < NUMCOLS - 1)
                                {
                                    jtemp += 1;
                                    itemp -= 1;
                                }
                                else if (itemp > 0)
                                {
                                    itemp -= 1;
                                }
                                else if (jtemp < NUMCOLS - 1)
                                {
                                    jtemp += 1;
                                }
                            }
                            else if (random == 6)
                            {
                                if (jtemp > 0)
                                {
                                    itemp -= 1;
                                }
                            }
                            else if (random == 7)
                            {
                                if (itemp > 0 && jtemp > 0)
                                {
                                    jtemp -= 1;
                                    itemp -= 1;
                                }
                                else if (itemp > 0)
                                {
                                    itemp -= 1;
                                }
                                else if (jtemp > 0)
                                {
                                    jtemp -= 1;
                                }
                            }

                            if (map[itemp][jtemp] == 'B')
                            {
                                status = 1;
                                success++;
                                prob++;
                                mean += l;
                                SDarray[i][j][k] = l;
                            }
                            else if (map[itemp][jtemp] == 'W' || map[itemp][jtemp] == 'V' || map[itemp][jtemp] == 'D')
                            {
                                status = 1;
                            }
                        }
                    }
                }
                probarray[i][j] = prob / NUMWALKS * 100;
                if (success <= 0)
                {
                    printf("Error!");
                    return 1;
                }
                meanarray[i][j] = mean / success;
                if (success > 0)
                {
                    for (k = 0; k < NUMWALKS; k++)
                    {
                        if (SDarray[i][j][k] != 0)
                        {
                            SD += pow(SDarray[i][j][k] - meanarray[i][j], 2);
                        }
                    }
                    SDarray[i][j][NUMWALKS] = sqrt(SD / success);
                }
                else
                {
                    SDarray[i][j][NUMWALKS] = 0;
                }
            }
            else if (tile == 'B')
            {
                probarray[i][j] = 100;
                meanarray[i][j] = 0;
                SDarray[i][j][NUMWALKS] = 0;
            }
            else if (tile == 'W' || tile == 'V' || tile == 'D')
            {
                probarray[i][j] = 0;
                meanarray[i][j] = 0;
                SDarray[i][j][NUMWALKS] = 0;
            }
            else
            {
                printf("Error!");
                return 1;
            }
        }
    }
    printf("Map:\n");
    for (j = 0; j < NUMCOLS; j++)
    {
        for (i = 0; i < NUMROWS; i++)
        {
            printf("%c ", map[i][j]);
        }
        printf("\n");
    }
    printf("\nProbability of escape:\n");
    for (j = 0; j < NUMCOLS; j++)
    {
        for (i = 0; i < NUMROWS; i++)
        {
            printf("%.2f ", probarray[i][j]);
        }
        printf("\n");
    }
    printf("\nMean path length:\n");
    for (j = 0; j < NUMCOLS; j++)
    {
        for (i = 0; i < NUMROWS; i++)
        {
            printf("%.2f ", meanarray[i][j]);
        }
        printf("\n");
    }
    printf("\nStandard deviation of path length:\n");
    for (j = 0; j < NUMCOLS; j++)
    {
        for (i = 0; i < NUMROWS; i++)
        {
            printf("%.2f ", SDarray[i][j][NUMWALKS]);
        }
        printf("\n");
    }
    while (1)
    {
    }
}
