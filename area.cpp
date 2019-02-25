#include "area.h"

bool *area::operator[](int xpos)
{
    return tab[xpos];
}

int area::count_around(int xpos, int ypos)
{
    int neighbours=0;
    if(xpos-1 >= 0 && ypos -1 >= 0 && tab[xpos-1][ypos-1] == alive) neighbours++;
    if(ypos -1 >= 0 && tab[xpos][ypos-1] == alive) neighbours++;
    if(xpos+1 < ROWS && ypos -1 >= 0 && tab[xpos+1][ypos-1] == alive) neighbours++;
    if(xpos-1 >=0  && tab[xpos-1][ypos] == alive) neighbours++;
    if(xpos+1 < ROWS && tab[xpos+1][ypos] == alive) neighbours++;
    if(xpos-1 >=0 && ypos+1 < COLS && tab[xpos-1][ypos+1] == alive) neighbours++;
    if(ypos+1 < COLS && tab[xpos][ypos+1] == alive) neighbours++;
    if(xpos+1 < ROWS && ypos+1 < COLS && tab[xpos+1][ypos+1] == alive) neighbours++;

    return neighbours;
}

area area::next_generation()
{
    int neighbours=0;
    area next_city;
    for(int i=0; i<ROWS; i++)
            for(int j=0; j<COLS; j++)
            {
                neighbours = count_around(i,j);

                if(tab[i][j] == alive && (neighbours == 2 || neighbours ==3))
                    next_city.tab[i][j] =  alive;
                else if(tab[i][j] == dead && neighbours == 3)next_city.tab[i][j] = alive;
                else next_city.tab[i][j] = dead;
            }
    return next_city;
}

