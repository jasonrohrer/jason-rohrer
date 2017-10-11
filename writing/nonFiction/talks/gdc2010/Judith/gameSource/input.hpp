bool gototitle;

inline bool canmove(int x, int y)
{
	return (worldMap[x][y]==false && floorMap[x][y]!=38 /*&& floorMap[x][y]!=109*/ );
}

int jumpStates[] = { 
    1,    6001, 6002, 6003, 6004, 
    6005, 6006, 6007, 6008, 6009, 
    6010, 6011, 6012 
    };
int jumpIndex = 0;
int lastJumpIndex = 12;

// avoid double-presses
char jumpKeyDown = false;


void jumpinput() {
    // Jason's hacks for a slide show:
    
    char anyPressed = false;

    if( keyPressed( SDLK_a ) ) {
        // back
        if( jumpIndex > 0 ) {
            jumpIndex--;
            }
        game.state = jumpStates[ jumpIndex ];
        anyPressed = true;
        }
    if( keyPressed( SDLK_f ) ) {
        // forward
        if( jumpIndex < lastJumpIndex ) {
            jumpIndex++;
            }
        game.state = jumpStates[ jumpIndex ];
        anyPressed = true;
        }
    

    /*    
  if( keyDown( SDLK_a ) ) {
      game.state = 1;
      anyPressed = true;
      }
  if( keyDown( SDLK_s ) ) {
      game.state = 6001;
      anyPressed = true;
      }
  if( keyDown( SDLK_h ) ) {
      game.state = 6020;
      anyPressed = true;
      }
    */
  if( anyPressed ) {
      // clear any stale textbox
      game.removetextbox();
      }
  
  if( keyDown( SDLK_p ) ) {
      //print position information
      //printf( "Player pos(%f, %f), rot %f\n", posX, posY, realdir );
      printf( "Player pos:  moveto( %f, %f ); forceDir( %f );\n",
              posX, posY, realdir );
      }
  
    }



void gameinput(){
  //speed modifiers
  double frameTime = (ftime - foldTime) / 1000.0; //frametime is the time this frame has taken, in seconds
  double moveSpeed = frameTime * 4.0; //the constant value is in squares/second
  double rotSpeed = frameTime * 2.0; //the constant value is in radians/second

  //Check for objects in view of the player, handled here so that we can use moveSpeed...
  if(game.objecton>0) game.objecton--;
  for(int i=0; i<5; i++)
  game.checkobject(int(posX + (dirX * i) * 0.5),int(posY + (dirY * i) * 0.5));

  readKeys();
	
	bool walking=false;


  //move forward if no wall in front of you
 if (keyDown(SDLK_UP))
 {
	 walking=true;
    if(canmove(int(posX + dirX * moveSpeed),int(posY))){
      if(canmove(int(posX + dirX * (moveSpeed*2)),int(posY)) ){
        if(canmove(int(posX + dirX * (moveSpeed*3)),int(posY)) ){
          posX += dirX * moveSpeed;
        }
      }
    }
    if(canmove(int(posX),int(posY + dirY * moveSpeed)) ){
      if(canmove(int(posX),int(posY + dirY *( moveSpeed*2))) ){
        if(canmove(int(posX),int(posY + dirY *( moveSpeed*3))) ){
          posY += dirY * moveSpeed;
        }
      }
    }
    set_playerpos(posX, posY, dirX, dirY);
 }
  //move backwards if no wall behind you
  if (keyDown(SDLK_DOWN))
  {
	  walking=true;
    if(canmove(int(posX - dirX * moveSpeed),int(posY)) ){
      if(canmove(int(posX - dirX * (moveSpeed*2)),int(posY)) ){
        if(canmove(int(posX - dirX * (moveSpeed*3)),int(posY)) ){
          posX -= dirX * moveSpeed;
        }
      }
    }
    if(canmove(int(posX),int(posY - dirY * moveSpeed)) ){
      if(canmove(int(posX),int(posY - dirY * (moveSpeed*2))) ){
        if(canmove(int(posX),int(posY - dirY * (moveSpeed*3))) ){
          posY -= dirY * moveSpeed;
        }
      }
    }
    set_playerpos(posX, posY, dirX, dirY);
  }
  //rotate to the right
  if (keyDown(SDLK_RIGHT))
  {
	  walking=true;
    //both camera direction and camera plane must be rotated
    double oldDirX = dirX;
    dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
    dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
    double oldPlaneX = planeX;
    planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
    planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
    set_playerpos(posX, posY, dirX, dirY);

    realdir=radfix(realdir-rotSpeed);
  }
  //rotate to the left
  if (keyDown(SDLK_LEFT))
  {
	  walking=true;
    //both camera direction and camera plane must be rotated
    double oldDirX = dirX;
    dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
    dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
    double oldPlaneX = planeX;
    planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
    planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
    set_playerpos(posX, posY, dirX, dirY);

    realdir=radfix(realdir+rotSpeed);
  }
  /*if (keyDown(SDLK_z)){
    //Open a door if there's one ahead
    if(isdoor(worldMap[int(posX + (dirX * 5) * moveSpeed)][int(posY + (dirY * 5)* moveSpeed)])){
      worldMap[int(posX + (dirX * 5) * moveSpeed)][int(posY + (dirY * 5) * moveSpeed)]=0;
    }
    if(isdoor(worldMap[int(posX + (dirX * 10) * moveSpeed)][int(posY + (dirY * 10)* moveSpeed)])){
      worldMap[int(posX + (dirX * 10) * moveSpeed)][int(posY + (dirY * 10) * moveSpeed)]=0;
    }
  }*/


  // Jason's hack
  jumpinput();
  



  if(keyPressed(SDLK_z)||keyPressed(SDLK_SPACE)){
    //if we're looking at an object, run that state!
    if(game.objecton>0){
      game.state=game.objectaction;
    }
  }
	
	sound_walk(walking);
}

void textboxinput(){
	sound_walk(false);

  readKeys();


  // Jason's hack
  jumpinput();


  if(keyDown(SDLK_z)||keyDown(SDLK_SPACE)){
    if(game.textmode==2 && game.textincomplete){
      game.textpara+=8;
    }
  }
	if((keyPressed(SDLK_z)||keyPressed(SDLK_SPACE) )&& game.textbox){
		//Depending on where the textbox is:
		if(game.textmode==4){
			game.removetextbox();
			if(game.currentoption==0){
				game.state=game.nextstate;
			}else {  game.state=game.nostate; if(game.nostate==0) game.controlstate=0; }
		}else if(game.textmode==5){
			game.removetextbox();
			game.state=game.nextstate;
		}else{
			if(game.nextstate>-1 && rendermode!=rm_fade && game.statedelay==0){
				if(!game.textincomplete){
					game.removetextbox();
					game.state=game.nextstate;
				}
			}
		}
	}
  if(game.textmode==4){
    optiondelay--;
    if(optiondelay<=0){
      optiondelay=0;
      if (keyPressed(SDLK_DOWN) || keyPressed(SDLK_UP)){
        game.currentoption=(game.currentoption+1)%2;
        optiondelay=3;
      }
    }
  }
}
