#include <string>
using namespace std;

#define JEFF 0
#define EMILY 1
#define JUDITH 2
#define BLUEBEARD 3
#define PRISONER 4
#define TEXT 4

#define screenWidth 320
#define screenHeight 240
#define texWidth 32
#define texHeight 32
int mapWidth=24;
int mapHeight=24;

double posX = 9.5, posY = 65.5; //x and y start position
//double posX = 51.5, posY = 91.5; //x and y start position
double dirX = -1.0, dirY = 0.0; //initial direction vector
double planeX = 0.0, planeY = 0.66; //the 2d raycaster version of camera plane
double realdir=0; //For easy modification!

double ftime = 0; //time of current frame
double foldTime = 0; //time of previous frame
float lighting=1;

const float PI=3.1415192;
const float PI2=PI*2;

int temp, temp2, tx, ty, tr, tg, tb, ta;
string tempstring, tempstring2;
int textdisplaypara=0;

int optiondelay=0;

int cutscenemode=0;
int csbanners=0;

std::vector<Uint32> texture[200];

void TestWaitFrame(void)
{
	static Uint32 next_tick = 0;
	Uint32 this_tick;

	/* Wait for the next frame */
	this_tick = SDL_GetTicks();
	if ( this_tick < next_tick ) {
		SDL_Delay(next_tick-this_tick);
	}
	next_tick = this_tick + (1000/30);
}



class wallclass{
  public:
  void set(int x, int y){
    xside=x; yside=y;
  }
  int s(int s){
    if(s==0){
      return xside;
    }else return yside;
  }
  int xside, yside;
};

wallclass wall[200];

bool isdoor(int t){
  if((t==8) || (t>=46 && t<=52)) return true;
  return false;
  //return true;
}

void moveto(double xp, double yp){
  posX=xp; posY=yp;
	set_playerpos(posX, posY, dirX, dirY);
	sound_walk(true);
}

bool movex(double xp, double xp2){
  if(xp>0){
    posX=posX+xp;
	  set_playerpos(posX, posY, dirX, dirY);
    if(posX>=xp2)
	{
		sound_walk(false);
		return true;
	}
  }else{
    posX=posX+xp;
	  set_playerpos(posX, posY, dirX, dirY);
	  if(posX<=xp2) 
	  {
		  sound_walk(false);
		  return true;
	  }
  }
	
  sound_walk(true);
  return false;
}

bool movey(double yp, double yp2){
  if(yp>0){
    posY=posY+yp;
	  set_playerpos(posX, posY, dirX, dirY);
    if(posY>=yp2) 
	{
		sound_walk(false);
		return true;
	}
  }else{
    posY=posY+yp;
	  set_playerpos(posX, posY, dirX, dirY);
    if(posY<=yp2)
	{
		sound_walk(false);
		return true;
	}
  }
	sound_walk(true);
  return false;
}

double radfix(double t){
  while(t<0) t+=PI2;
  while(t>PI2) t-=PI2;
  return t;
}

void changedir(double t){
  //both camera direction and camera plane must be rotated
  double oldDirX = dirX;
  dirX = dirX * cos(t) - dirY * sin(t);
  dirY = oldDirX * sin(t) + dirY * cos(t);
  double oldPlaneX = planeX;
  planeX = planeX * cos(t) - planeY * sin(t);
  planeY = oldPlaneX * sin(t) + planeY * cos(t);
  set_playerpos(posX, posY, dirX, dirY);

  realdir=radfix(realdir+t);
}

void forceDir( double d ) {
  //both camera direction and camera plane must be rotated
  double oldDirX = dirX;
  dirX = -1 * cos(d) - 0 * sin(d);
  dirY = -1 * sin(d) + 0 * cos(d);
  double oldPlaneX = planeX;
  planeX = 0 * cos(d) - 0.66 * sin(d);
  planeY = 0 * sin(d) + 0.66 * cos(d);
  set_playerpos(posX, posY, dirX, dirY);

  realdir=radfix(d);
}


bool face(double t){
  double tp=radfix(realdir-t);
  double tp2=radfix(t-realdir);

  if(fabs(realdir-t)<=PI/16){
    changedir(-realdir);
    changedir(t);
    return true;
  }else{
    if(tp<tp2){
      changedir(-PI/24);
    }else{
      changedir(PI/24);
    }
  }

  if(fabs(realdir-t)<=PI/24){
    changedir(-realdir);
    changedir(t);
	sound_walk(false);
    return true;
  }

  sound_walk(true);
  return false;
}

int glow=0, quickglow=0;
int glowphase=0, quickglowphase=0;
int qglow=0;
int qglowdir=0;

void process_glow(){
  if(glowphase==0){
    glow++;
    if(glow>=100){
      glow=100;
      glowphase=1;
    }
  }else if(glowphase==1){
    glow--;
    if(glow<=0){
      glow=0;
      glowphase=0;
    }
  }

  if(quickglowphase==0){
    quickglow++;
    if(quickglow>=50){
      quickglow=50;
      quickglowphase=1;
    }
  }else if(quickglowphase==1){
    quickglow--;
    if(quickglow<=0){
      quickglow=0;
      quickglowphase=0;
    }
  }

  if(qglowdir==0){
    qglow+=4;
    if(qglow>=255){
      qglow=255;
      qglowdir=1;
    }
  }else{
    qglow-=4;
    if(qglow<0){
      qglow=0;
      qglowdir=0;
    }
  }
}

//sort algorithm
void combSort(int* order, double* dist, int amount)
{
  int gap = amount;
  bool swapped = false;
  while(gap > 1 || swapped)
  {
    //shrink factor 1.3
    gap = (gap * 10) / 13;
    if(gap == 9 || gap == 10) gap = 11;
    if (gap < 1) gap = 1;
    swapped = false;
    for (int i = 0; i < amount - gap; i++)
    {
      int j = i + gap;
      if (dist[i] < dist[j])
      {
        std::swap(dist[i], dist[j]);
        std::swap(order[i], order[j]);
        swapped = true;
      }
    }
  }
}

string its(int t, int s=0){
  string te;
  ostringstream *its_temp;
  its_temp = new ostringstream;
  (*its_temp) << t;
  te = (*its_temp).str();
  delete its_temp;
  if(s==0){ return te;
  }else{
    int d;
    d=s-te.length();
    if(d<0){
      return te;
    }else{
      while(d>0){
        te = "0" + te;
        d--;
      }
      return te;
    }
  }
}

int sti(string s){
  istringstream stringbuf(s);
  int t;
  stringbuf >> t;
  return t;
}

string itsmil(int t){
  tempstring=its(t);
  tempstring2="";
  temp=0; temp2=tempstring.length();

  for(int i=0; i<temp2; i++){
    tempstring2=tempstring[(temp2-1)-i]+tempstring2;
    if((temp==2 && temp2!=3) || (temp==5 && temp2!=6) || (temp==8 && temp2!=9))
      tempstring2=","+tempstring2;
    temp++;
  }
  return tempstring2;
}

int nmod(int t, int s){
  while(t<0) t+=s;
  return t%s;
}

string lowercase(string s){
  std::transform(s.begin(), s.end(), s.begin(),
               (int(*)(int)) toupper);

  return s;
}

bool inbox(int x1, int y1, int x2, int y2, int tx, int ty){
  if(tx>=x1&&tx<=x2){
    if(ty>=y1&&ty<=y2){
      return true;
    }
  }
  return false;
}

bool inboxw(int x1, int y1, int w, int h, int tx, int ty){
  if(tx>=x1&&tx<=x1+w){
    if(ty>=y1&&ty<=y1+h){
      return true;
    }
  }
  return false;
}

