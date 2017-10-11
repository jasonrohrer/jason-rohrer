struct Sprite{
  double x;
  double y;
  int texture;
};

int totalsprites;
int numsprites=0;

Sprite sprite[500];

Uint32 buffer[screenWidth][screenHeight];

//1D Zbuffer
double ZBuffer[screenWidth];

//arrays used to sort the sprites
int spriteOrder[500];
double spriteDistance[500];

//function used to sort the sprites
void combSort(int* order, double* dist, int amount);

void movesprite(double xp, double yp, int t){
    //Move all instances of sprite type t by xp, yp displacement
    for(int i=0; i<numsprites; i++){
		if(sprite[i].texture==t){
			sprite[i].x+=xp;
			sprite[i].y+=yp;
		}
    }
}
double getspritex(int t){
    //returns the x position of the first instance of sprite type t
    for(int i=0; i<numsprites; i++){
		if(sprite[i].texture==t){
			return sprite[i].x;
		}
    }
    return -1;
}
double getspritey(int t){
    //returns the y position of the first instance of sprite type t
    for(int i=0; i<numsprites; i++){
		if(sprite[i].texture==t){
			return sprite[i].y;
		}
    }
    return -1;
}
void raddsprite(double xp, double yp, int t){
    sprite[numsprites].x=(100-xp)+0.5;
    sprite[numsprites].y=yp+0.5;
    sprite[numsprites].texture=t;
    numsprites++;
}
void addsprite(double xp, double yp, int t){
    xp--;
    sprite[numsprites].x=xp;
    sprite[numsprites].y=yp;
    sprite[numsprites].texture=t;
    numsprites++;
}
void copysprite(int a, int b){
    sprite[a].x=sprite[b].x;
    sprite[a].y=sprite[b].y;
    sprite[a].texture=sprite[b].texture;
}
void removespriteat(int t){
    //Remove specific sprite t
    if(t==numsprites-1){
		numsprites--;
    }else{
		for(int i=t; i<numsprites; i++) copysprite(i,i+1);
		numsprites--;
    }
}
void removesprites(int t){
    for(int i=0; i<numsprites; i++){
		if(sprite[i].texture==t){
			removespriteat(i);
			i--;
		}
    }
}
void addflowerpatch(double xp, double yp, int t){
    switch(t){
		case 0:
			addsprite(xp+0.25, yp+0.25, 47); //Flowers
			addsprite(xp+0.75, yp+0.25, 49); //Flowers
			addsprite(xp+0.25, yp+0.75, 49); //Flowers
			addsprite(xp+0.75, yp+0.75, 47); //Flowers
			break;
    }
}
void addtree(double xp, double yp, int t){
    addsprite(xp, yp, t);
    addsprite(xp+3, yp-2, t);
    addsprite(xp+3, yp+2, t);
}
void addroom(const string& t){
    if(t=="treasury"){
		addsprite(20.5,13.5,52);
		addsprite(19.5,13.5,53);
		addsprite(18.5,13.5,54);
		addsprite(19.5,14.5,55);
		addsprite(18.5,14.5,56);
		addsprite(21.5,15.5,57);
		addsprite(21.5,16.5,58);
		addsprite(19.5,16.5,59);
		addsprite(21.5,14.5,61); //Necklace
    }else if(t=="frontgarden"){
		
		
		addflowerpatch(19,34,0);
		addflowerpatch(18,34,0);
		addflowerpatch(17,34,0);
		addflowerpatch(16,34,0);
		addflowerpatch(15,34,0);
		addflowerpatch(14,34,0);
		addflowerpatch(10,34,0);
		addflowerpatch(9,34,0);
		addflowerpatch(8,34,0);
		addflowerpatch(7,34,0);
		addflowerpatch(6,34,0);
		addflowerpatch(5,34,0);
    }else if(t=="garden"){
		addflowerpatch(32,1,0);
		addflowerpatch(31,1,0);
		addflowerpatch(30,1,0);
		addflowerpatch(29,1,0);
		addflowerpatch(28,1,0);
		addflowerpatch(27,1,0);
		addflowerpatch(26,1,0);
		addflowerpatch(25,1,0);
		addflowerpatch(24,1,0);
		addflowerpatch(23,1,0);
		addflowerpatch(22,1,0);
		addflowerpatch(21,1,0);
		addflowerpatch(20,1,0);
		addflowerpatch(19,1,0);
		addflowerpatch(32,2,0);
		addflowerpatch(19,2,0);
		addflowerpatch(32,3,0);
		addflowerpatch(19,3,0);
		addflowerpatch(32,4,0);
		addflowerpatch(19,4,0);
		addflowerpatch(32,5,0);
		addflowerpatch(19,5,0);
		
		addsprite(22.5,4,48);
		addsprite(30.5,5.5,62);
    }else if(t=="kingdom"){
		addtree(84.5,7.5,98);
		addtree(79.5,7.5,98);
		addtree(74.5,7.5,98);
		addtree(69.5,7.5,98);
		addtree(64.5,7.5,98);
		addtree(59.5,7.5,98);
		addtree(54.5,7.5,98);
		addtree(49.5,7.5,98);
		addtree(44.5,8.5,98);
		addtree(40.5,9.5,98);
		addtree(36.5,9.5,98);
		addtree(40.5,17.5,98);
		addtree(36.5,17.5,98);
		addtree(44.5,18.5,98);
		addtree(84.5,19.5,98);
		addtree(79.5,19.5,98);
		addtree(74.5,19.5,98);
		addtree(69.5,19.5,98);
		addtree(64.5,19.5,98);
		addtree(59.5,19.5,98);
		addtree(54.5,19.5,98);
		addtree(49.5,19.5,98);
    }else if(t=="torture"){
      addsprite(3.5,10.5,51);
    }
	  else if (t=="library")
	  {
		  addsprite(10,19,107);  
	  }
}
