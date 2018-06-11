#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <ncurses.h>
#include <unistd.h>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <math.h>

using namespace std;

#define MAX_NBR_OF_knights   10
#define COUNT_MAX  14 

mutex mtx;
condition_variable cv;
bool running = true;
unsigned randTime = rand() % 6 + 1;
int hp_in,def_in,dmg_in;


class Knight
{
public:
    	Knight(int hp, int def, int dmg, int dir_x, int dir_y, int count); //warior params
    	~Knight();
    	int count;    //nie wiem po co jeszcze
	int hp;       //health
	int def;      //def points
	int dmg;      //damage	
	bool inside; //is this warior inside
	bool visible;
    	int getyPosition();
    	int getxPosition();
    	thread moveThread();
	

protected:
private:
    	int yPosition;
    	int xPosition;
    	int dir_x;
    	int dir_y;
   	void move();
};

Knight::Knight(int hp, int def, int dmg, int dir_x, int dir_y, int count)
{
    this->dir_x = dir_x;
    this->dir_y = dir_y;
    this->yPosition = rand()% LINES;  // height changes
    this->xPosition = (std::rand()%2)*COLS; // left or right side
    this->count= count;
    this->hp = hp;
    this->dmg = dmg;
    this->def = def;
    this->visible=true;
    this->inside=false;	

}

Knight::~Knight()
{
    
}

int Knight::getyPosition()
{
    return yPosition;
}

int Knight::getxPosition()
{
    return xPosition;
}

void Knight::move()
{
        
int yPos, xPos;

   while(count<COUNT_MAX && visible)

        {
	    
            	unique_lock<mutex> lock(mtx);
                if(xPosition==COLS && yPosition==LINES && dir_x>0 && dir_y>0)
                {
                    yPosition=LINES;
                    xPosition=COLS;
                }
                else 
                {
                    xPosition+=dir_x;
                    yPosition+=dir_y;
                }
            
                if(xPosition>=COLS-1)
                {
                    xPosition=COLS-1;
                    dir_x*=-1;
                    count++;
                }
                else if(xPosition<=0)
                {
                    xPosition=0;
                    dir_x*=-1;
                    count++;
                } 
                
		if(yPosition>=LINES-1)
                {
                    yPosition=LINES-1;
                    dir_y*=-1;
                    count++;
                }
                else if(yPosition<=0)
                {
                    yPosition=0;
                    dir_y*=-1;
                    count++;
                }
//#####################################################################		
		if(yPosition >= 45 && yPosition <=50 && xPosition >=20  && xPosition <=55)
		{
		
		yPosition=47;
		xPosition=30;
		inside=true;		
		cv.wait(lock);
		}
		
		else if(yPosition >= 4 && yPosition <=9 && xPosition >=70  && xPosition <=105)
		{
		
		yPosition=6;
		xPosition=80;
		inside=true;		
		cv.wait(lock);
		}
		
		else if(yPosition >= 30 && yPosition <=35 && xPosition >=145  && xPosition <=175)
		{
		//hp_in=hp;
		//def_in=def;
		//dmg_in=dmg;
		yPosition=32;
		xPosition=150;
		inside=true;
		cv.wait(lock);
		}

		lock.unlock();
          	usleep(200000);  
       
//#####################################################################			 
	
}
}

thread Knight::moveThread()
{
    return thread(&Knight::move,this);
}

vector<Knight> knights;
vector<thread> knightThreads;


void drawBorders()
{

	while(running == true)
	{
	clear();
        int i;
//#####################################################//	+------------+
	mvprintw(45, 20, "+");    //(y,x)			|45,20  45,55|
        mvprintw(45, 55, "+");    // 				|	     |
	mvprintw(50, 20, "+");	  //				|	     |
	mvprintw(50, 55, "+");	  //				|50,20  50,55|
//#####################################################//       +------------+
	mvprintw(4, 70, "+");    //(y,x)
        mvprintw(4, 105, "+");     
	mvprintw(9, 70, "+");
	mvprintw(9, 105, "+");
//#####################################################//
	mvprintw(30, 145, "+");    //(y,x)
        mvprintw(30, 175, "+");     
	mvprintw(35, 145, "+");
	mvprintw(35, 175, "+");
//#####################################################//       

	mvprintw(44, 19, "CASTLE 1");	
	mvprintw(3, 69, "CASTLE 2");    
	mvprintw(29, 149, "CASTLE 3");  
   
//********* 1 CASTLE ****************************************
		for (i = 46; i <50 ; i++)
		{
		mvprintw(i, 20, "|");
		mvprintw(i, 55, "|");
		}
        	for (i = 21; i < 55; i++)
        	{
            	mvprintw(45, i, "-");
            	mvprintw(50, i, "-");
		}
		
//********* 2 CASTLE ****************************************
		for (i = 5; i <9 ; i++)
		{
		mvprintw(i, 70, "|");
		mvprintw(i, 105, "|");
		}
        	for (i = 71; i < 105; i++)
        	{
            	mvprintw(4, i, "-");
            	mvprintw(9, i, "-");
		}		
//********* 3 CASTLE ****************************************
		for (i = 31; i <35 ; i++)
		{
		mvprintw(i, 145, "|");
		mvprintw(i, 175, "|");
		}
        	for (i = 146; i < 175; i++)
        	{
            	mvprintw(30, i, "-");
            	mvprintw(35, i, "-");
		}

//#####################################################//


		

		for(int i = 0; i < MAX_NBR_OF_knights; i++)
		{
		if(knights.at(i).visible)
            	{	
		mvprintw(knights.at(i).getyPosition(), knights.at(i).getxPosition(),"K %d", i ); //draw moving knights
		if(knights.at(i).inside) // mamy jednego w zamku
            	{
		mvprintw(knights.at(i).getyPosition(), knights.at(i).getxPosition(),"HP:%d DEF:%d DMG:%d",knights.at(i).hp,knights.at(i).def,knights.at(i).dmg);  		
		}
		}
		
		
		}

		refresh();   

		usleep(20000); 
   	} //while

}


void createThreads()
{
   
    	for(int i = 0; i < MAX_NBR_OF_knights; i++)
	{
	Knight Knight((rand()%10)+1, (rand()%3)+1,(rand()%6)+1,(1+(double)rand()/RAND_MAX *(2.5) ),( 1+(double)rand()/RAND_MAX*(2.5) ) ,0);
	knights.push_back(Knight);
	}

	for(int i = 0; i < MAX_NBR_OF_knights; i++)
	{
        knightThreads.push_back(knights.at(i).moveThread());//spawn threads from knights created and lauch thread with function
	
	// Random time in range 500 to 5500 ms until next ball
        usleep(2000000 * randTime); 
	}
}



int main(int argc, char *argv[])
{
    srand(time(NULL));
    // Initialize ncurses
    initscr();
    curs_set(0);
    nodelay(stdscr,TRUE); 
    keypad(stdscr,TRUE); 
    cbreak();
    refresh();

   

thread monitor{drawBorders};	//all drawing
createThreads();                        

   
    for(int i = 0; i < MAX_NBR_OF_knights; i++)
    {
       knightThreads.at(i).join();
    }
    
    //Stop drawing
    sleep(1);
    running = false;
    monitor.join();
    //Close ncurses
    endwin();
    return 0;
}


