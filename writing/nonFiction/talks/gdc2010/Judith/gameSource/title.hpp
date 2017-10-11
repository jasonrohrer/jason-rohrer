//returns false if quit game
//true otherwise

bool introloop()
{
	
	
	drawRect(0, 0, 320, 240, RGB_Textbox);
	//printString("Terry Cavanagh and Stephen Lavelle",100,100);
	//printString("present",100,130);
	printcenter("Terry Cavanagh and Stephen Lavelle",100);
	printcenter("present",130);
	
	redraw();
	
	waitFrame(getTime(),1.5);
}



int selectcount;
bool escheld;

bool titleloop()
{
	escheld=true;
	loadcontinue();
	
	stopchannels();
	
	if (continuestate>1)
	{
		selectcount=3;
	}
	else
	{
		selectcount=2;
	}
	
	
	play_effect(0);
    int selected=0;
    SDL_Event event;
    bool exittitle=false;
    while (true)
    {
		fps.start();
		drawRect(0, 0, 320, 240, RGB_Textbox);
        //printString("JUDITH",50,10);
        printcenter("JUDITH",50);
		if (selectcount==2)
		{
			if (selected==0)
			{
				//printString("START",50,100);
				//printString("quit",50,120);
				printcenter("START",100);
				printcenter("quit",120);
			}
			else
			{
				//printString("start",50,100);
				//printString("QUIT",50,120);
				printcenter("start",100);
				printcenter("QUIT",120);
			}
		}
		else
		{
			if (selected==0)
			{
				printcenter("NEW GAME",100);
				printcenter("continue",120);
				printcenter("quit",140);
			}
			else if (selected==1)
			{
				printcenter("new game",100);
				printcenter("CONTINUE",120);
				printcenter("quit",140);
			}
			else
			{
				printcenter("new game",100);
				printcenter("continue",120);
				printcenter("QUIT",140);
			}
		}
		
		
        redraw();
		
        while(SDL_PollEvent(&event))
        {
			if(event.type == SDL_QUIT) end();
			if(event.type == SDL_KEYDOWN)
			{
				switch(event.key.keysym.sym)
				{
					case SDLK_SPACE: case SDLK_z:
						if (selectcount==2)
						{
							if (selected==0)
							{
								fps.stop();
								stop_effect(0);
								savestate(1);
								continuestate=1;
								return true;
							}
							else
							{
								fps.stop();
								stop_effect(0);
								return false;
							}
						}
						else
						{
							if (selected==0)
							{
								fps.stop();
								stop_effect(0);
								continuestate=1;
								savestate(1);
								return true;
							}
							else if(selected==1)
							{
								fps.stop();
								stop_effect(0);
								return true;
							}
							else
							{
								fps.stop();
								stop_effect(0);
								return false;
							}
							
							
						}
						break;
					case SDLK_UP:
						if (selectcount==2)
							selected=(selected+1)%2;
						else
							selected=(selected+2)%3;
						break;
					case SDLK_DOWN:
						if (selectcount==2)
							selected=(selected+1)%2;
						else
							selected=(selected+1)%3;
						
						break;
					case SDLK_ESCAPE:
					{
						return false;
					}
				}
			}
        }
		if (fps.get_ticks() < 1000 / FRAMES_PER_SECOND)
		{
			SDL_Delay((1000 / FRAMES_PER_SECOND) - fps.get_ticks());
		}
    }
}
