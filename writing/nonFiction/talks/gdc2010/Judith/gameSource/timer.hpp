
const int FRAMES_PER_SECOND=30;
//The level.levelparams.timer
class Timer
	{
    private:
		//The clock time when the level.levelparams.timer started
        int startTicks;

		//The ticks stored when the level.levelparams.timer was paused
        int pausedTicks;

		//The level.levelparams.timer status
        bool paused;
        bool started;

    public:
		//Initializes variables
        Timer();

		//The various clock actions
        void start();
        void stop();
        void pause();
        void unpause();

		//Gets the level.levelparams.timer's time
        int get_ticks();

		//Checks the status of the level.levelparams.timer
        bool is_started();
        bool is_paused();
	} fps;

Timer::Timer()
{
	//Initialize the variables
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}


void Timer::start()
{
	//Start the level.levelparams.timer
    started = true;

	//Unpause the level.levelparams.timer
    paused = false;

	//Get the current clock time
    startTicks = SDL_GetTicks();
}



void Timer::stop()
{
	//Stop the level.levelparams.timer
    started = false;

	//Unpause the level.levelparams.timer
    paused = false;
}


void Timer::pause()
{
	//If the level.levelparams.timer is running and isn't already paused
    if( ( started == true ) && ( paused == false ) )
    {
		//Pause the level.levelparams.timer
        paused = true;

		//Calculate the paused ticks
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}


void Timer::unpause()
{
	//If the level.levelparams.timer is paused
    if( paused == true )
    {
		//Unpause the level.levelparams.timer
        paused = false;

		//Reset the starting ticks
        startTicks = SDL_GetTicks() - pausedTicks;

		//Reset the paused ticks
        pausedTicks = 0;
    }
}


int Timer::get_ticks()
{
	//If the level.levelparams.timer is running
    if( started == true )
    {
		//If the level.levelparams.timer is paused
        if( paused == true )
        {
			//Return the number of ticks when the level.levelparams.timer was paused
            return pausedTicks;
        }
        else
        {
			//Return the current time minus the start time
            return SDL_GetTicks() - startTicks;
        }
    }

	//If the level.levelparams.timer isn't running
    return 0;
}


bool Timer::is_started()
{
    return started;
}


bool Timer::is_paused()
{
    return paused;
}


