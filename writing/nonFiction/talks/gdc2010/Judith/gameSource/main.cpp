//Engine Includes
using namespace std;
#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include <string>
#include <cstdio>
#include <sstream>
#include <fstream>
#include <math.h>
#include "quickcg.h"
#include "musicback.hpp"

using namespace QuickCG;

int continuestate;

void loadcontinue()
{
	continuestate=0;
	ifstream fp_in;  
	fp_in.open("data/save.dat",ifstream::in); 
	fp_in>>continuestate;
	fp_in.close();   // close the streams
	std::cout<<"CONTINUESTATE\n\n\n\n"<<continuestate<<"\n\n\n\n";
}

void savestate(int s)
{
	std::cout<<"SAVING AT STATE "<<s<<std::endl;
	continuestate=s;
	ofstream fp_out;
	fp_out.open("data/save.dat");
	fp_out<<s;
	fp_out.close(); 
}


//Game includes
#include "misc.hpp"
#include "map.hpp"
#include "sprites.hpp"
#include "game.hpp"

#include "render.hpp"
#include "timer.hpp"
#include "input.hpp"
#include "logic.hpp"
#include "textureblocks.hpp"
#include "title.hpp"



int main(int /*argc*/, char */*argv*/[]){


  screen(screenWidth,screenHeight, 1, "Judith");

  redraw();

  //sound initialization:
  init_sound();
  place_sound(0,33.5, 51.5);
  set_reverb(rt_plain);
  //activate_sound(0);
  set_playerpos(posX, posY, dirX, dirY);

  redraw();

 

  redraw();

  for(int i = 0; i < texturecount; i++) texture[i].resize(texWidth * texHeight);
  //load some textures
  unsigned long tw, th, error = 0;

  for (int i=0;i<texturecount;i++)
  {
    error |= loadImage(texture[i], tw, th, texturenames[i]);
    std::cout<<i<<"\n";
    if(error) { std::cout << "error loading images" << std::endl; return 1; }

 }

  for (int i=0;i<blockcount;i++)
      wall[i].set(blocks[i].a,blocks[i].b); //B: Floor

  game.nextstate=-1;
  //For release, comment out
  //game.state=2;

	SDL_Event event;
	
	//gobble input
	while(SDL_PollEvent(&event)){}
	
  //start the title screen
	

  introloop();

  title:
  gototitle=false;
  if (!titleloop())
    goto quitgame;
	
	 loadmap("level1");

	if (continuestate==0)
		continuestate=1;
	game.state=continuestate;

	if (game.state==1)
	{
		//Initalise a few things for the script
		backdrop=bg_outside;
		rendermode=rm_normal;
		//changedir(-PI/4);
		game.controlstate=2;
	}
	else
	{//subsequent scenes indoors
		//Initalise a few things for the script
		backdrop=bg_black;
		rendermode=rm_black;
		//changedir(-PI/4);
		game.controlstate=2;
		
		
	}
  //start the main loop
  while(!done()){

	fps.start();

	  if(game.controlstate==3){
		  drawRect(0, 0, 320, 240, RGB_Textbox);
		  printcenter("The end", 115, game.gettextcol(qglow/4), 0, "The end");
		  redraw();
	  }else{
		  gamerender();
	  }


    if(game.controlstate==0){
      gameinput();
    }else if(game.controlstate==1){
      textboxinput();
    }

    gamelogic();
    //Ok, I know this is a bit mental but there is a CPU usage issue with this game
    //and this totaly over the top delay is the only thing that seems to fix it...
	if (fps.get_ticks() < 1000 / FRAMES_PER_SECOND)
	{
		SDL_Delay((1000 / FRAMES_PER_SECOND) - fps.get_ticks());
	}
  }
	goto title;

  quitgame:

  release_sound();
  return 0;
}

/*

The raycaster used in this game is based on the code from Lode Vandevenne's
SDL raycaster tutorial. Copyright notice from that program follows:

Copyright (c) 2004-2007, Lode Vandevenne

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
