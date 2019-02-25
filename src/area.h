#ifndef AREA
#define AREA

#define alive true
#define dead false
#define ROWS 47
#define COLS 55

using namespace std;

class area 
{
private:
    bool tab[ROWS][COLS];
public:
    bool *operator[]( int xpos);
	area next_generation();	
    int count_around(int xpos, int ypos);

} ;


#endif
