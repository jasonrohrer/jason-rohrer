#include "rendermodes.hpp"

//added movesprite function
//added getspritex and getspritey functions

enum Locations {L_INSIDE, L_OUTSIDE, L_LAKE};
enum Characters {C_JEFF, C_EMILY};

class triggertileclass{
  public:
  triggertileclass(){
    x=0; y=0; action=0;
  }
  int x, y;
  int action;
  string name;
};


class gameclass{
  public:
  gameclass(){
    shakyrun=0;

    state=1;
    statedelay=0;
    objecton=0; //Don't show object text at the start
  }
  //First up, triggers and states
  void copytrigger(int a, int b){
    triggertile[a].x=triggertile[b].x;
    triggertile[a].y=triggertile[b].y;
    triggertile[a].action=triggertile[b].action;
    triggertile[a].name=triggertile[b].name;
  }
  void addtrigger(int xp, int yp, int t){
    triggertile[numtriggers].x=xp;
    triggertile[numtriggers].y=yp;
    triggertile[numtriggers].action=t;
    triggertile[numtriggers].name="trigger";
    numtriggers++;
  }
  void removetriggerat(int t){
    //Remove specific trigger t
    if(t==numtriggers-1){
      numtriggers--;
    }else{
      for(int i=t; i<numtriggers; i++) copytrigger(i,i+1);
      numtriggers--;
    }
  }
  void removetrigger(int t){
    for(int i=0; i<numtriggers; i++){
      if(triggertile[i].action==t){
        removetriggerat(i);
        i--;
      }
    }
  }
  void removetriggers(int t, int s){
    //Removes triggers for actions between t and s.
    for(int i=0; i<numtriggers; i++){
      if(triggertile[i].action>=t && triggertile[i].action<=s){
        removetriggerat(i);
        i--;
      }
    }
  }
  bool checktrigger(int xp, int yp){
    for(int i=0; i<numtriggers; i++){
      if(triggertile[i].x==int(xp)){
        if(triggertile[i].y==int(yp)){
          state=triggertile[i].action;
        }
      }
    }
  }

  //Ok, now "objects", which are triggered when the hero is looking at them
  //and selects "z".
  void copyobject(int a, int b){
    objecttile[a].x=objecttile[b].x;
    objecttile[a].y=objecttile[b].y;
    objecttile[a].action=objecttile[b].action;
    objecttile[a].name=objecttile[b].name;
  }
  void removeobjectat(int t){
    //Remove specific trigger t
    if(t==numobjects-1){
      numobjects--;
    }else{
      for(int i=t; i<numobjects; i++) copyobject(i,i+1);
      numobjects--;
    }
  }
  void removeobjectatpos(int xloc, int yloc)
  {
	  for(int i=0; i<numobjects; i++){
		  if(objecttile[i].x==xloc && objecttile[i].y==yloc)
		  {
			  removeobjectat(i);
			  i--;
		  }
	  }

  }
  void removeobject(int t){
    for(int i=0; i<numobjects; i++){
      if(objecttile[i].action==t){
        removeobjectat(i);
        i--;
      }
    }
    objecton=0;
  }
	void addobject(int xp, int yp, int t, string n){
		//step 1: remove any other objects at that position
		removeobjectatpos(xp,yp);

		objecttile[numobjects].x=xp;
		objecttile[numobjects].y=yp;
		objecttile[numobjects].action=t;
		objecttile[numobjects].name=n;
		numobjects++;
	}
  void checkobject(int xp, int yp){
    //If there is an object at this location, show the prompt
    for(int i=0; i<numobjects; i++){
      if(objecttile[i].x==xp){
        if(objecttile[i].y==yp){
          objecton=2;
          objectdes=objecttile[i].name;
          objectaction=objecttile[i].action;
        }
      }
    }
  }



	void removelockeddoors()
	{
		removeobject(411);
	}


	void addlockeddoor(int x, int y, int t)
	{
		worldMap[x][y]=t;
		addobject(x,y,411,"Open Door");
	}

	enum Doors{
		d_front=0,
		d_library=1,
		d_bedroom=2,
		d_treasury=3,
		d_torture=4,
		d_armory=5,
		d_garden=6,
		d_midway=7,
		d_kingdom=8,
		d_lake=9,
		d_final=10
	};

	struct DoorPos
	{
		int x;
		int y;
	};

	DoorPos getDoor(int i)
	{
		switch (i)
		{
			case d_front:
				return (DoorPos) {11,32};
				break;
			case d_library:
				return (DoorPos) {14,23};
				break;
			case d_bedroom:
				return (DoorPos) {19,23};
				break;
			case d_treasury:
				return (DoorPos) {15,15};
				break;
			case d_torture:
				return (DoorPos) {11,12};
				break;
			case d_armory:
				return (DoorPos) {13,6};
				break;
			case d_garden:
				return (DoorPos) {21,6};
				break;
			case d_midway:
				return (DoorPos) {24,8};
				break;
			case d_kingdom:
				return (DoorPos) {28,13};
				break;
			case d_lake:
				return (DoorPos) {28,30};
				break;
			case d_final:
				return (DoorPos) {26,42};
				break;
		}
	};

	int doorTile(int i, bool locked)
	{
		switch (i)
		{
			case d_front:
				if (!locked)
					return 8;
				else
					return 8;//front door always looks the same
				break;
			case d_library:
				if (!locked)
					return 8;
				else
					return 9;
				break;
			case d_bedroom:
				if (!locked)
					return 8;
				else
					return 9;
				break;
			case d_treasury:
				if (!locked)
					return 47;
				else
					return 55;
				break;
			case d_torture:
				if (!locked)
					return 48;
				else
					return 56;
				break;
			case d_armory:
				if (!locked)
					return 49;
				else
					return 57;
				break;
			case d_garden:
				if (!locked)
					return 50;
				else
					return 58;
				break;
			case d_midway:
				if (!locked)
					return 54;
				else
					return 54;
				break;
			case d_kingdom:
				if (!locked)
					return 51;
				else
					return 59;
				break;
			case d_lake:
				if (!locked)
					return 52;
				else
					return 60;
				break;
			case d_final:
				if (!locked)
					return 53;
				else
					return 61;
				break;
		}
	}

	/*
	 o - open; removes scripts at that square as well
	 c - closed; need to add scripts manually
	 l - locks; adds generic 'door locked' script

	 BOOKCASE IS NOT POSITIONED HERE
	 */

	void placeDoors(const string& s)//"ooooclocloo"
	{
		for (int i=0;i<11;i++)
		{
			if (s[i]=='o')
			{
				worldMap[getDoor(i).x][getDoor(i).y]=0;
				removeobjectatpos(getDoor(i).x,getDoor(i).y);
			}
			else if (s[i]=='c')
			{

				worldMap[getDoor(i).x][getDoor(i).y]=doorTile(i,false);
				removeobjectatpos(getDoor(i).x,getDoor(i).y);
				if (i==d_midway)
				{
					addobject(getDoor(i).x,getDoor(i).y,114+i-3,"Open Gate");
				}
				else if (i>=3)
				{
					addobject(getDoor(i).x,getDoor(i).y,114+i-3,"Open Door");
				}
			}
			else if (s[i]=='l')
			{
				worldMap[getDoor(i).x][getDoor(i).y]=doorTile(i,true);
				addobject(getDoor(i).x,getDoor(i).y,411,"Open Door");
			}
		}
	}


	void opendoor(int d)
	{
		place_sound(2,getDoor(d).x, getDoor(d).y); activate_sound(2);
		worldMap[getDoor(d).x][getDoor(d).y]=0;
	}


	void judithpos()
	{
		set_player(1);	controlstate=1; changedir(-realdir); moveto(23.5,19.5);
		
	}

  void updatestate();

  int state, statedelay;
  int nextstate;
  triggertileclass triggertile[1000];
  triggertileclass objecttile[1000];
  int numtriggers, numobjects;
  int controlstate;
  int nostate; //Quick little holder for selecting the second option

  int objecton;
  int objectaction;
  string objectdes;
  //Textbox stuff
  void removetextbox(){
    textmode=0;
    textbox=false;
  }
  void settextbox(string t, int c, int t2, int t3=0, bool setstate=false){
    if (setstate)
      controlstate=1;

    currentoption=0;

    line[0]=t;
    numlines=1;
    textbox=true;

    textmode=t2;
    if(t2==1 || t2==2) textpara=0;
    if(t2==2) textlim=t3;
    if(t2==3) textpara=t3;
    if(t2==4) { textpara=0; }
    if(t2==5) { textpara=0; }
    //0 is simple display
    //1, 2 is typed out
    //3 is timed
    //4 is an option
    //5 is an "option" option
    textcolor=c;

    nextstate=state+1; state=0;
  }
  void addline(string t){
    line[numlines]=t;
    numlines++;
  }
  ColorRGB getcol(int off){
    if(textcolor==1) return ColorRGB(255-off,128,128);
    if(textcolor==2) return ColorRGB(128,255-off,128);
    if(textcolor==3) return ColorRGB(128,128,255-off);
	  if(textcolor==4) return ColorRGB(128-off+(rand()%128),0,0);
    if(textcolor==5) return ColorRGB(255-off,64-off,255-off);
    return ColorRGB(255-off,255-off,255-off);
  }
  ColorRGB gettextcol(int off){ return ColorRGB(196-off,196-off,255-off); }
  ColorRGB getyescol(int off){  return ColorRGB(255-off,96-off,96-off); }
  ColorRGB getnocol(int off){   return ColorRGB(128-off,128-off,128-off); }
  int textmode;
  int textpara, textlim;
  int textcolor;
  bool textincomplete;
  int currentoption;

  int shakyrun;

  string line[20];
  int numlines;
  bool textbox;
};

gameclass game;

void gameclass::updatestate(){
  if(textmode==1 || textmode==2){
    textpara++;
    if(textmode==1){
      if(textpara>textlim) removetextbox();
    }
  }else if(textmode==3){
    textpara--;
    if(textpara<=0) removetextbox();
  }

  statedelay--; if(statedelay<=0) statedelay=0;
  //i'm so sorry
  //this is awful
  //i promise never to code like this again
  if(statedelay<=0){

      if( state != 0 ) {
          printf( "Game state %d\n", state );
          }
      
    switch(state){
      case 0:
        //Do nothing here! Standard game state
      break;file://localhost/Volumes/NO%20NAME/judith%2011th%20april%20MK%202/musicback.hpp
      case 1:
			//Game initilisation
			backdrop=bg_outside;
			placeDoors("ccccccccccc");
			addobject(14,23,5,"Open Door");
			addobject(19,23,6,"Open Door");

			changedir(-realdir); changedir(0); 
			moveto(9.5,65.5);
			
			state=130;

			set_reverb(rt_plain);
	break;
      case 2:
        //Testing game initilisation: Skip ahead to the part of the game
        //I'm working on.

        //200   - Judith: Treasury
        //400   - Jeff [Starts from Judith's talk with BB]
        //700   - Judith: Torture
        //1100  - Jeff [Starts from Judith's talk with BB]
        //1300  - Judith: Armory
        //1500  - Jeff [Starts from Judith's talk with BB]
        //1700  - Judith: Garden
        //2100  - Jeff: Garden [Starts from Judith's talk with BB]
        //2400  - Judith: Kingdom
        //2600  - Judith: The lake and final cutscene

        //All the script "sections" are grouped together now!
        state=2600;
      break;

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//

  Everything before 200 is part of the opening cutscenes with Jeff and Emily, plus
  a few misc. scripts like opening doors.

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/

      case 5: worldMap[14][23]=0; removeobject(5); state=0;
			place_sound(2, 14, 23); activate_sound(2); break;
		case 6: worldMap[19][23]=0; removeobject(6); state=0;
			place_sound(2, 19, 23); activate_sound(2);break;

      case 7:
        removeobject(7);
        controlstate=1;
        backdrop=bg_black;
        settextbox("Emily? Are you there?", JEFF, 2, 180);
      break;
      case 8:
			do_fadeout();
      break;
      case 9: state=200; break;

      case 10:
        //Special: Restore the doors!
        numobjects=0; numtriggers=0;
        addobject(13,18,3,"Examine Bookcase"); worldMap[13][18]=40;
        addobject(14,23,5,"Open Door"); worldMap[14][23]=8;
        addobject(19,23,6,"Open Door"); worldMap[19][23]=8;
        worldMap[11][32]=8;
        state=0;
      break;


      case 100:
        controlstate=2;
        removetrigger(100);
        state++;// cutscenemode=1;
      break;
      case 101:
        if(face(PI/2)) state++;
      break;
      case 102: controlstate=1; settextbox("Jeff!", EMILY, 2, 180); break;
      case 103: settextbox("Emily! You look beautiful.", JEFF, 2, 180); break;
      case 104: settextbox("I've missed you so much...", EMILY, 2, 180); break;
      case 105:
        settextbox("I keep thinking about the last", JEFF, 2, 180);
           addline("time we met, when I gave");
           addline("you that necklace.");
      break;
      case 106:
        settextbox("Hah! My husband was a little", EMILY, 2, 180);
           addline("suspicious about it.");
      break;
      case 107:
        settextbox("I told him that I'd bought", EMILY, 2, 180);
           addline("it for myself as a treat.");
      break;
      case 108:
        settextbox("I hate that we have to", JEFF, 2, 180);
           addline("be so secretive...");
      break;
      case 109:
        settextbox("I know, me too... it's better", EMILY, 2, 180);
           addline("for everyone this way, though.");
      break;
      case 110: settextbox("Shall we go inside?", EMILY, 2, 180); break;
      case 111:
        controlstate=2;
        removesprites(45);
        addsprite(12.5,50.5,46); state++;
        play_effect(2);
      break;
      case 112:
        controlstate=0;
        movesprite(0,-0.25,46);
        //moveto(posX, posY-0.1);
        if(getspritey(46)<33.6) state++;
      break;
      case 113:
        removesprites(46);
        addsprite(12.5,33.6,45);
        controlstate=0; //cutscenemode=0;
        state=0;
      break;


			/*generic door openings*/

		case 114: removeobject(114); state=0;
			opendoor(3); break;

		case 115: removeobject(115); state=0;
			opendoor(4); break;

		case 116: removeobject(116); state=0;
			opendoor(5); break;

		case 117: removeobject(117); state=0;
			opendoor(6); break;

		case 118: removeobject(118); state=0;
			opendoor(7); break;

		case 119: removeobject(119); state=0;
			opendoor(8); break;

		case 120: removeobject(120); state=0;
			opendoor(9); break;

		case 121: removeobject(121); state=2800;
			opendoor(10); break;



			/*

			 OPENING CUTSCENE!!!!!


			 */

      case 130:
        //Ok! This is very important - it's the opening cutscene!
        //First of all, we set things up so that you're in the the
        //"front garden" block. Then, we position the player and
        //show the opening monologue
        controlstate=1;
        numsprites=0;
        removetriggers(20,49); //Remove every trigger between 21 and 49 inclusive
        state++;
      break;
      case 131:
        settextbox("I'm meeting Emily today.", JEFF, 2, 180);
      break;
      case 132:
        settextbox("We can't meet very often. We're", JEFF, 2, 180);
           addline("both married, but not to each other.");
      break;
      case 133:
        settextbox("So when we do get to see each", JEFF, 2, 180);
           addline("other, we try to make it special.");
      break;
      case 134:
        settextbox("It's been four months", JEFF, 2, 180);
           addline("since we last met.");
      break;
      case 135: controlstate=2; play_effect(1); state++; break;
      case 136:
        addtrigger(12,52,100);
        addtrigger(10,52,100);
        addtrigger(11,52,100);

        addtrigger(10,34,140);
        addtrigger(11,34,140);
        addtrigger(12,34,140);

        addtrigger(2,56,139);

        worldMap[10][65]=5;


        addroom("frontgarden");
        controlstate=0;
        state=0;// statedelay=15; //Start in front garden block!
      break;
      case 139:
        removetrigger(139);
        addsprite(12.5,50.5,45);
        state=0;
        //just adding the Emily sprite
      break;

      case 140:
        //This the cutscene with Jeff and Emily at the front door.
        controlstate=2;
        removetrigger(140);
        state++;
      break;
      case 141:
        if(face(PI/2)) state++;
      break;
      case 142:
        controlstate=1;
        settextbox("Are you sure this place", EMILY, 2, 180);
           addline("is private enough?");
      break;
      case 143:
        settextbox("Oh, I think so - according to", JEFF, 2, 180);
           addline("the company I rented it from,");
           addline("it's been abandoned for years.");
      break;
      case 144:
        settextbox("The locals are really", JEFF, 2, 180);
           addline("superstitious about it.");
      break;
      case 145: settextbox("Ohhh! How exciting!", EMILY, 2, 180); break;
      case 146:
        worldMap[11][32]=0; state++; statedelay=15;
		place_sound(2, 11, 32); activate_sound(2);
      break;
      case 147:
        settextbox("Let's have a look around!", EMILY, 2, 180);
      break;
      case 148:
        controlstate=2;
        removesprites(45);
        addsprite(12.5,33.6,46); state++;
        play_effect(3);
      break;
      case 149:
		controlstate=2;
		movesprite(0,-0.35,46);
		if(getspritey(46)<25.5) state++;
	  break;
      case 150:
        movesprite(0.1,0,46);
        if(getspritex(46)>12.5) state++;
      break;
      case 151:
			removesprites(46);
			addtrigger(11,30,5020);
			controlstate=0; state=0;
      break;


/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//

  200 - 400 : Judith, Treasury. Lines 300-> are for prisoner.

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/

      case 200:
			savestate(state);
        controlstate=1; changedir(-realdir); moveto(23.5,19.5);
		set_player(1);
        settextbox("I had the strangest dream last night.", JUDITH, 2, 180);
      break;
      case 201: settextbox("Hmm?", BLUEBEARD, 2, 180); break;
      case 202:
        settextbox("It was of a man, sitting with", JUDITH, 2, 180,1);
           addline("a piece of jewellery in his");
           addline("hand. He seemed so sad.");
      break;
      case 203: settextbox("What happened after that?", BLUEBEARD, 2, 180); break;
      case 204:
        settextbox("I can't remember.", JUDITH, 2, 180);
      break;
		case 205: do_fadein(); break;
      case 206: play_effect(4); backdrop=bg_outside; state=210; controlstate=0; break;

      case 210:
        //Special: Restore the doors/triggers for this section!
        numobjects=0; numtriggers=0;
		placeDoors("lccclllllll");
        addobject(13,18,215,"Examine Bookcase"); worldMap[13][18]=40;
        addobject(14,23,5,"Open Door");
        addobject(19,23,6,"Open Door");
        state=0;

        addobject(11,12,213,"Open Door");
        addobject(13,6,211,"Open Door");
        addobject(21,6,211,"Open Door");
        addobject(24,8,211,"Open Door");

        addobject(20,14,230,"Examine Necklace");
			addtrigger(17, 14, 226);
			addtrigger(17, 15, 226);

			addtrigger(17, 16, 226);

      break;

      case 211:
        controlstate=1;
        settextbox("This door seems to be locked.", JUDITH, 2, 180);
      break;
      case 212: state=0; controlstate=0; break;

      case 213:
        controlstate=1;
        settextbox("This door seems to be locked.", JUDITH, 2, 180);
           addline("I think... I can hear");
           addline("something behind it?");
      break;
      case 214: state=0; controlstate=0; break;

      case 215:
        controlstate=1;
        settextbox("One of these books looks", JUDITH, 2, 180);
           addline("oddly out of place...");
      break;
      case 216:
        settextbox("Move the book?", JUDITH, 4, 180);
           addline("YES");addobject(13,18,215,"Examine Bookcase");
           addline("NO"); nostate=0;
      break;
      case 217:
        removeobject(215);
        worldMap[13][18]=0;
		place_sound(3, 13, 18); activate_sound(3);
        state++; statedelay=10;
      break;
      case 218:
        controlstate=1;
        settextbox("Oh!", JUDITH, 2, 180);
      break;
      case 219:
        controlstate=1;
        settextbox("What is this?", JUDITH, 2, 180);
        addroom("treasury"); addobject(getDoor(d_treasury).x, getDoor(d_treasury).y, 221,"Open Door");

        addtrigger(13,17,238);
      break;
      case 220: state=0; controlstate=0; break;

      case 221:
        //Entering the treasury for the first time
		opendoor(d_treasury);
		removeobject(221);
        state++;
      break;
      case 222:
        if(face(PI)) state++;
      break;
      case 223:
        controlstate=1;
        settextbox("I don't believe it! My husband", JUDITH, 2, 180);
           addline("keeps a treasury back here!");
      break;
      case 224:
        settextbox("He is such a secretive man;", JUDITH, 2, 180);
           addline("look at all of this gold!");
      break;
      case 225: state=0; controlstate=0; break;

      case 226:
        controlstate=1;
        removetrigger(226);
        settextbox("Wait... is this... blood?", JUDITH, 2, 180);
      break;
      case 227: state=0; controlstate=0; break;

      case 230:
        controlstate=1;
        settextbox("That necklace looks very pretty!", JUDITH, 2, 180);
      break;
      case 231:
        controlstate=1;
        removetrigger(240); addtrigger(13,19,240);
        settextbox("I don't suppose my husband will mind", JUDITH, 2, 180);
           addline("if I take it? He probably won't");
           addline("even notice that it's missing...");
      break;
      case 232:
        settextbox("Take the necklace?", JUDITH, 4, 180);
           addline("YES");
           addline("NO"); nostate=236;
      break;
      case 233:
        removesprites(61);
        removeobject(230); removeobject(231);
        state++; statedelay=10;
      break;
      case 234:
        controlstate=1;
        settextbox("It fits perfectly.", JUDITH, 2, 180);
		addtrigger(13,25,250);
		addtrigger(15,25,250);
        removetrigger(260); removetrigger(270);
      break;
		case 235: state=0; controlstate=0; play_effect(5); break;


      case 236:
        removeobject(230);
        addobject(20,14,231,"Examine Necklace");
        controlstate=1;
        settextbox("No, I should probably leave it...", JUDITH, 2, 180);

        addtrigger(13,19,260);
        removetrigger(240); addtrigger(13,19,240);

			addtrigger(13,25,270);
			addtrigger(15,25,270);
      break;
		case 237: state=0; controlstate=0; play_effect(6); break;


      case 238:
        //Trigger to close bookcase behind you if you re-enter library
        removetrigger(238);
        addtrigger(13,19,240);
        state=0; controlstate=0;
      break;

      case 240:
        //bookcase closing behind you
        removetrigger(240);
        controlstate=2;
        state++;
      break;
      case 241: if(face(PI/2)) state++; break;
      case 242:
        moveto(posX, posY+0.1);
        if(posY>20.5)
		{
			sound_walk(false);
			state++;
		}
      break;
      case 243:
        addobject(13,18,245,"Open secret passage");
        addtrigger(13,17,238);
			worldMap[13][18]=40;
			place_sound(3, 13, 18); activate_sound(3);
        state++; statedelay=10;
      break;
      case 244: state=0; controlstate=0; break;


		case 245: worldMap[13][18]=0;
			place_sound(3, 13, 18); activate_sound(3); removeobject(245); state=0; break;

      case 250:
        //Return to bed, having taken the necklace
        removetrigger(250);
        controlstate=1;
        settextbox("No... I really shouldn't have", JUDITH, 2, 180);
           addline("taken this necklace...");
      break;
      case 251:
        settextbox("I don't feel right about it.", JUDITH, 2, 180);
           addline("I should leave it back.");
        addobject(20,14,255, "Return Necklace");
      break;
		case 252: state=0; controlstate=0; play_effect(13); break;

      case 255:
        //Put the necklace back in the treasury
        removeobject(255);
        controlstate=1;
        addsprite(21.5,14.5,61);
			addtrigger(13,25,270);
			addtrigger(15,25,270);
        addtrigger(13,19,260); removetrigger(240);
        settextbox("Ok... I guess I feel", JUDITH, 2, 180);
           addline("better now.");
      break;
		case 256: state=0; controlstate=0;play_effect(14); break;

      case 260:
        //bookcase closing behind you (in final case)
        removetrigger(260);
        controlstate=2;
        state++;
      break;
      case 261: if(face(PI/2)) state++; break;
      case 262:
        moveto(posX, posY+0.1);
        if(posY>20.5)
		{
			sound_walk(false);
			state++;
		}
      break;
      case 263:
			worldMap[13][18]=40;
			place_sound(3, 13, 18); activate_sound(3);
        state++; statedelay=10;
      break;
      case 264:
        controlstate=1;
        settextbox("My husband must have this", JUDITH, 2, 180);
           addline("part of the castle hidden");
           addline("away for a reason...");
      break;
      case 265:
        settextbox("I probably shouldn't even", JUDITH, 2, 180);
           addline("tell him I was here.");
        backdrop=bg_black;
      break;
      case 266: state=0; controlstate=0; break;


      case 270:
        //Return to bed, end of day.
        removetrigger(270);
        controlstate=1;
			do_fadeout();
      break;
      case 271: state=400; break;


      case 300:
        removeobject(300);
        controlstate=1;
        settextbox("Who...who are you?", JUDITH, 2, 180,1);  break;
      case 301:
        settextbox("...please...please help me...", PRISONER, 2, 180,1); break;
      case 302:
        settextbox("What are you doing here?", JUDITH, 2, 180,1);  break;
      case 303:
        settextbox("I ... have been a prisoner", PRISONER, 2, 180,1);
           addline("here for four years now.");
      break;
      case 304:
        settextbox("I don't understand! Why?", JUDITH, 2, 180,1); break;
      case 305:
        settextbox("Please... just kill me...", PRISONER, 2, 180,1); break;
      case 306:
        settextbox("What? No! I couldn't do that.", JUDITH, 2, 180,1); break;
      case 307:
        settextbox("Please, I can't bear", PRISONER, 2, 180,1);
           addline("any more of this...");
      break;
      case 308:
        settextbox("I can't believe my husband", JUDITH, 2, 180,1);
           addline("would do this...");
      break;
      case 309:
        settextbox("If you don't end my suffering,", PRISONER, 2, 180,1);
           addline("I'll tell him you found me.");
			break;
		case 310:
		settextbox("What will he do to you when he", PRISONER, 2, 180,1);
		   addline("finds out where you've been?");
			break;
		case 311:
		settextbox("Maybe you'll end up here like me", PRISONER, 2, 180,1);
		   addline("begging his next wife for mercy!");
			break;
      case 312:
        settextbox("N... No!", JUDITH, 2, 180,1); break;
		case 313:
			removetrigger(240);
			addtrigger(13,19,5000);

			addtrigger(13,25,720);
			addtrigger(15,25,720);
			addobject(2,10,800,"Talk to prisoner");
			state=0; controlstate=0;
			play_effect(34);
			break;

			/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//

  400 - 700 : Jeff, Treasury. (all Jeff's scenes start from Judith's night time talk)

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/


		case 400:
			savestate(state);
			judithpos();
        controlstate=1; 
        settextbox("What did you do today, Judith?", BLUEBEARD, 2, 180,1);break;
      case 401:
        settextbox("Oh... I was just mending", JUDITH, 2, 180,1);
           addline("some clothes, mostly...");
      break;
      case 402:
        settextbox("You shouldn't trouble yourself", BLUEBEARD, 2, 180,1);
           addline("with such tasks. That's what");
           addline("the servents are for.");
      break;
      case 403:
        settextbox("Yes, you're right... I'm", JUDITH, 2, 180,1);
           addline("still adjusting to life");
           addline("in a castle.");
      break;
      case 404:
        settextbox("My sweet; rest yourself tonight.", BLUEBEARD, 2, 180,1);
			addline("These things take time.");
			break;
      case 405:
        settextbox("You're right, my love.", JUDITH, 2, 180,1);
			addline("Good night.");
			moveto(19.5,21);changedir(-realdir); changedir(PI); 
			set_player(0);
			break;
		case 406:do_fadein();break;
      case 407:
        backdrop=bg_outside;
        settextbox("Oh... The bed's empty.", JEFF, 2, 180,1);
      break;
      case 408:
        settextbox("That's strange. I saw Emily", JEFF, 2, 180,1);
           addline("come down here... I wonder");
           addline("where she's gone?");
      break;
      case 409:
			play_effect(7);
        state=410; controlstate=0;
      break;

      case 410:
        //Special: Restore the doors/triggers for this section!
        //Jeff find the necklace
        numobjects=0; numtriggers=0; numsprites=0;
        addobject(13,18,415,"Examine Bookcase"); worldMap[13][18]=40;
		placeDoors("lccclllllll");
        addobject(14,23,5,"Open Door");
        addobject(19,23,6,"Open Door");
        state=0;

        addobject(11,12,411,"Open Door");
        addobject(13,6,411,"Open Door");
        addobject(21,6,411,"Open Door");
        addobject(24,8,411,"Open Door");

        addobject(20,14,445,"Examine Necklace");

        addobject(16,23,417,"Examine Bookcase");

        addobject(16,18,431,"Examine Bookcase");
        addobject(15,18,431,"Examine Bookcase");

        addobject(11,18,433,"Examine Bookcase");
        addobject(10,18,433,"Examine Bookcase");

        addobject(11,23,435,"Examine Bookcase");
        addobject(10,23,435,"Examine Bookcase");

        addobject(8,19,437,"Examine Bookcase");
        addobject(8,20,437,"Examine Bookcase");
        addobject(8,21,439,"Examine Bookcase");
        addobject(8,22,439,"Examine Bookcase");
      break;

      case 411:
        controlstate=1;
        settextbox("Looks like this door is locked.", JEFF, 2, 180);
      break;
      case 412: state=0; controlstate=0; break;


      case 415:
        controlstate=1;
        settextbox("Seems to contain books", JEFF, 2, 180);
           addline("on military history.");
      break;
      case 416: state=0; controlstate=0; break;

      case 417:
        controlstate=1;
        settextbox("This looks like a fiction", JEFF, 2, 180);
           addline("section. I see a number of");
           addline("fairytale collections.");
      break;
      case 418:
        controlstate=1;
        settextbox("There's one book that seems out", JEFF, 2, 180);
           addline("of place though - a heavy leather");
           addline("bound book called CIVIL WAR.");
      break;
      case 419:
        settextbox("Take the book?", JEFF, 4, 180);
           addline("YES");
           addline("NO"); nostate=0;
      break;
      case 420:
        removeobject(417);
        removeobject(415);

        addobject(16,23,422,"Examine Bookcase");
        addobject(13,18,424,"Examine Bookcase");
        state++;
      break;
      case 421: state=0; controlstate=0; break;

      case 422:
        controlstate=1;
        settextbox("This looks like a fiction", JEFF, 2, 180);
           addline("section. I see a number");
           addline("of fairytale collections.");
      break;
      case 423: state=0; controlstate=0; break;

      case 424:
        controlstate=1;
        settextbox("Seems to contain books", JEFF, 2, 180);
           addline("on military history.");
      break;
      case 425:
        controlstate=1;
        settextbox("Could be the right place", JEFF, 2, 180);
           addline("for that book I found.");
      break;
      case 426:
        settextbox("Put the book on the shelf?", JEFF, 4, 180);
           addline("YES");
           addline("NO"); nostate=0;
      break;
      case 427:
        removeobject(215);
			worldMap[13][18]=0;
			place_sound(3, 13, 18); activate_sound(3);
        state++; statedelay=10;
      break;
      case 428:
        controlstate=1;

        addsprite(21.5,14.5,61); //Necklace
        settextbox("Ah! A secret passage!", JEFF, 2, 180);
      break;
      case 429:
        controlstate=1;

        removeobject(424);
        settextbox("I wonder if Emily found", JEFF, 2, 180);
           addline("this and wandered in?");
			play_effect(9);
      break;
      case 430: state=0; controlstate=0; break;

      case 431:
        controlstate=1;
        settextbox("There are a lot of religious", JEFF, 2, 180);
           addline("books in this section.");
      break;
      case 432: state=0; controlstate=0; break;

      case 433:
        controlstate=1;
        settextbox("Mostly books on legal matters,", JEFF, 2, 180);
           addline("as far as I can see.");
      break;
      case 434: state=0; controlstate=0; break;

      case 435:
        controlstate=1;
        settextbox("Periodicals, I think.", JEFF, 2, 180);
      break;
      case 436: state=0; controlstate=0; break;

      case 437:
        controlstate=1;
        settextbox("Books on mathematics", JEFF, 2, 180);
           addline("and engineering.");
      break;
      case 438: state=0; controlstate=0; break;

      case 439:
        controlstate=1;
        settextbox("Books on various scientific topics.", JEFF, 2, 180);
           addline("They seem to be quite old.");
      break;
      case 440: state=0; controlstate=0; break;


      case 445:
        controlstate=1;
        settextbox("This necklace...", JEFF, 2, 180);
      break;
      case 446:
        settextbox("This belongs to Emily...", JEFF, 2, 180);
      break;
      case 447:
        removesprites(61); state++; statedelay=10; break;
      case 448:
        backdrop=bg_black;
			play_effect(10);
        settextbox("Where is she?", JEFF, 2, 180);
      break;
      case 449:
        //End of this section
			do_fadeout();
      break;
      case 450: state=700; break;



      // book case puzzle
      case 500:
        settextbox("Where did she go?", JEFF, 2, 180,1);break;
      case 501:
        settextbox("I can't see any way out.", JEFF, 2, 180,1);break;
      case 502: state=0; controlstate=0; break;

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//

  700 - 800 : Judith, Prisoner. Actual Prisoner conversation is at 300 (due to room
                                movement after we wrote the script)

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/



		case 700:
			savestate(state);
        //Day 2 with Judith; Finding the prisoner
        controlstate=1; changedir(-realdir); moveto(23.5,19.5);
        numobjects=0; numtriggers=0; numsprites=0;
		set_player(1);
        settextbox("I had another terrible", JUDITH, 2, 180,1);
           addline("dream last night.");
      break;
      case 701:
        settextbox("Of what?", BLUEBEARD, 2, 180,1);break;
      case 702:
        settextbox("I dreamt of the same man;", JUDITH, 2, 180,1);
           addline("he feared his wife dead.");
      break;
      case 703:
        settextbox("It's just a dream. Don't", BLUEBEARD, 2, 180,1);
           addline("let it upset you.");
      break;
		case 704: do_fadein(); break;
      case 705: backdrop=bg_outside; state=710; controlstate=0;
			play_effect(15); break;


      case 706:
        controlstate=1;
        settextbox("I wonder... What else has my", JUDITH, 2, 180);
           addline("husband hidden back here?");
      break;
      case 707:
        removeobject(706);
			worldMap[13][18]=0;
			place_sound(3, 13, 18); activate_sound(3);
        state=0; controlstate=0;
      break;

      case 708:
        controlstate=1; removeobject(708);
			  opendoor(d_torture);
        settextbox("Wasn't this door", JUDITH, 2, 180);
           addline("locked yesterday?");
      break;
      case 709: state=0; controlstate=0;
			  //opendoor(d_torture);
			break;


      case 710:
        //Special: Restore the doors/triggers for this section!

        //Judith discovers the prisoner
        numobjects=0; numtriggers=0; numsprites=0;


		placeDoors("lccccllllll");
		addobject(getDoor(d_torture).x,getDoor(d_torture).y,708,"Open Door");

        worldMap[13][18]=40;
        addobject(14,23,5,"Open Door"); worldMap[14][23]=8;
        addobject(19,23,6,"Open Door"); worldMap[19][23]=8;
        worldMap[11][32]=8;
        state=0;

//        worldMap[11][12]=0;
        addobject(13,6,211,"Open Door");
        addobject(21,6,211,"Open Door");
        addobject(24,8,211,"Open Door");

        addobject(13,18,706,"Open secret passage");
        addtrigger(13,17,238);

        addroom("treasury");
        addroom("torture");

        //addtrigger(11,12,708);
        addtrigger(5,10,711);
        addtrigger(6,10,711);
        addobject(2,10,300,"Talk to Prisoner");
			place_sound(0,2,10);
		activate_sound(0);
      break;

      case 711:
        removetrigger(711);
        controlstate=2;
        if(face(0)) state++;
      break;
      case 712:
        controlstate=1;
        settextbox("What... what is this?", JUDITH, 2, 180);
      break;
      case 713:
        settextbox("I think I'm going to be sick...", JUDITH, 2, 180);
      break;
      case 714: state=0; controlstate=0; break;

      case 720:
        //Return to bed, end of day.
        removetrigger(720);
        backdrop=bg_black; state++;
        controlstate=1;
      break;
      case 721:
			do_fadeout();
      break;
		case 722:
			deactivate_sound(0);
			state=1100;
		break;


      case 800://comes in without knife
        settextbox("Are you here to", PRISONER, 2, 180,1);
           addline("taunt me again?");
      break;
      case 801: state=0; controlstate=0; break;


/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//

  1100 - 1300 : Jeff, Torture chamber.

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/

		case 1100://night
			savestate(state);
			judithpos();
        settextbox("What did you do today, Judith?", BLUEBEARD, 2, 180,1);break;
      case 1101:
        settextbox("I received a letter", JUDITH, 2, 180,1);
           addline("from my sister.");
      break;
      case 1102:
        settextbox("How is she?", BLUEBEARD, 2, 180,1);break;
      case 1103:
        settextbox("Happy in all things.", JUDITH, 2, 180,1);break;
      case 1104:
        settextbox("As are we. I hope you have", BLUEBEARD, 2, 180,1);
           addline("a pleasant day tomorrow.");
      break;
      case 1105:
        numobjects=0; numtriggers=0; numsprites=0;
        settextbox("Yes... goodnight.", JUDITH, 2, 180,1);
			changedir(-realdir); changedir(PI);
			
			set_player(0);
			moveto(19.5, 14.5);
			break;
      case 1106:
			do_fadein();
      break;
		case 1107: backdrop=bg_outside; state=1110; controlstate=0; play_effect(17); break;


      case 1110:
        //Special: Restore the doors/triggers for this section!

        //Very short section with Jeff: Simply enter the prisoner room.
        numobjects=0; numtriggers=0; numsprites=0;

		placeDoors("lccccllllll");
//        worldMap[13][18]=0;
        addobject(14,23,5,"Open Door"); //worldMap[14][23]=8;
        addobject(19,23,6,"Open Door"); //worldMap[19][23]=8;
//        worldMap[11][32]=8;
        state=0;

//        worldMap[11][12]=0;
        addobject(13,6,411,"Open Door");
        addobject(21,6,411,"Open Door");
        addobject(24,8,411,"Open Door");

        addtrigger(11,12,1112); //Enters torture room
        addobject(9,8,1115,"Examine Bloody Floor");
        addobject(7,8,1115,"Examine Bloody Floor");
        addobject(7,11,1115,"Examine Bloody Floor");
        addobject(6,12,1115,"Examine Bloody Floor");
        addobject(3,10,1115,"Examine Bloody Floor");
        addobject(2,11,1115,"Examine Bloody Floor");
      break;

      case 1112:
        controlstate=1; removetrigger(1112);
        settextbox("This door wasn't", JEFF, 2, 180);
           addline("unlocked earlier.");
      break;
      case 1113:
        settextbox("Emily?", JEFF, 2, 180);
      break;
      case 1114: state=0; controlstate=0; break;

      case 1115:
        controlstate=1; removeobject(1105);
        settextbox("Whose blood is this?", JEFF, 2, 180);
      break;
      case 1116:
        settextbox("...", JEFF, 2, 180);
      break;
      case 1117:
        backdrop=bg_black;
        settextbox("Where are you, Emily?", JEFF, 2, 180);
      break;
      case 1118:
			do_fadeout();
      break;
      case 1119:
        state=1300;
      break;



/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//

  1300 - 1500 : Judith, finds knife and kills prisoner.

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/

		case 1300:
			savestate(state);
        controlstate=1; changedir(-realdir); moveto(23.5,19.5);
			set_player(1);
        settextbox("I had that dream again.", JUDITH, 2, 180,1);break;
      case 1301:
        settextbox("With the man who'd lost his wife?", BLUEBEARD, 2, 180,1);break;
      case 1302:
        settextbox("He was weeping tears of blood.", JUDITH, 2, 180,1);
      break;
	  case 1303:
			settextbox("Some day, I hope, you will",BLUEBEARD,2,180,1);
			addline("wake up happy by my side.");
			break;
      case 1304:
			do_fadein();
      break;
      case 1305: state=1310; controlstate=0;
			activate_sound(0); play_effect(19); break;

      case 1310:
        //Special: Restore the doors/triggers for this section!

        //Judith finds the knife in the armory
        numobjects=0; numtriggers=0; numsprites=0;
        worldMap[13][18]=40;
        addobject(14,23,5,"Open Door"); worldMap[14][23]=8;
        addobject(19,23,6,"Open Door"); worldMap[19][23]=8;
        worldMap[11][32]=8;
        state=0;

        worldMap[11][12]=0;
        worldMap[13][6]=0;
        addobject(21,6,211,"Open Door");
        addobject(24,8,211,"Open Door");

        addobject(13,18,245,"Open secret passage");
        addtrigger(13,17,238);

        addroom("treasury");
        addroom("torture");

        //Just add the knife for the armory
        addsprite(12,2,66);
        addobject(11,2,1320,"Take Dagger and Kill Prisoner");

        addtrigger(13,6,1350);
        addobject(2,10,800,"Talk to Prisoner");
      break;

      case 1320:
        controlstate=1; removeobject(1320); removesprites(66);
        removeobject(800);
        settextbox("I... can't stop thinking about", JUDITH, 2, 180);
          addline ("that poor prisoner...");
      break;
      case 1321:
        settextbox("Has he really been there", JUDITH, 2, 180);
          addline ("for four long years?");
      break;
      case 1322:
        settextbox("Death would be a release for him.", JUDITH, 2, 180);
      break;
      case 1323:
        controlstate=2;
        if(face(PI)) state++;
      break;
      case 1324:
        moveto(posX+0.15, posY);
        if(posX>=13.5)
		{
			sound_walk(false);
			state++;
		}
      break;
      case 1325: if(face(((PI/2)*3))) state++; break;
      case 1326:
        moveto(posX, posY+0.15);
        if(posY>=12.5)
		{
			sound_walk(false);
			state++;
		}
      break;
      case 1327: if(face(0)) state++; break;
      case 1328:
        moveto(posX-0.15, posY);
        if(posX<=9.5)
		{
			sound_walk(false);
			state++;
		}
      break;
      case 1329: if(face(PI/2)) state++; break;
      case 1330:
        moveto(posX, posY-0.15);
        if(posY<=10.5)
		{
			sound_walk(false);
			state++;
		}
      break;
      case 1331: if(face(0)) state++; break;
      case 1332:
        moveto(posX-0.15, posY);
        if(posX<=3.5)
		{
			sound_walk(false);
			state++;
		}
      break;
      case 1333://comes in with knife
        controlstate=1;
        settextbox("Oh it's...you...again.", PRISONER, 2, 180,1);break;
      case 1334:
        settextbox("I've made a decision.", JUDITH, 2, 180,1);break;
      case 1335:
        settextbox("I'm... going to help you.", JUDITH, 2, 180,1);break;
      case 1336:
        settextbox("Thank... you...", PRISONER, 2, 180,1);break;
      case 1337:
		deactivate_sound(0);
		play_effect(44);
        state++; statedelay=60;
        break;
      case 1338:
        controlstate=2;
        removesprites(51); addsprite(3.5,10.5,65); state++; break;
      case 1339:
        moveto(posX+0.1, posY);
        if(posX>=4.5)
		{
			sound_walk(false);
			state++;
		}
      break;
      case 1340:
        controlstate=1;
			addtrigger(13,25,1360);
			addtrigger(15,25,1360);
        addobject(11,2,1355,"Return Dagger");
        settextbox("I hope I've done", JUDITH, 2, 180,1);
			addline("the right thing.");break;
		case 1341: state=0; controlstate=0; play_effect(18);break;


      case 1350:
        controlstate=1; removetrigger(1350);
        settextbox("It looks like an armory.", JUDITH, 2, 180);
			break;
	case 1351:
			controlstate=1; removetrigger(1350);
			settextbox("But...these weapons...", JUDITH, 2, 180);
			break;
		case 1352:
			controlstate=1; removetrigger(1350);
			settextbox("These weapons are all drenched", JUDITH, 2, 180);
			addline("in blood.");
			break;
		case 1353:
			controlstate=1; removetrigger(1350);
			settextbox("What sort of man must my", JUDITH, 2, 180);
			addline("husband be?");
			break;
		case 1354: state=0; controlstate=0; play_effect(29); break;

      case 1355:
        controlstate=1; removeobject(1355);
        removetrigger(1360);
			addtrigger(13,25,1370);
			addtrigger(15,25,1370);
        addsprite(12,2,67);
        settextbox("There...", JUDITH, 2, 180);
      break;
      case 1356: state=0; controlstate=0; break;


      case 1360:
        controlstate=1; removetrigger(1360);
        settextbox("Wait - I can't just take this", JUDITH, 2, 180);
           addline("bloody knife back to bed.");
      break;
      case 1361:
        settextbox("I need to return it.", JUDITH, 2, 180);
      break;
		case 1362: state=0; controlstate=0; play_effect(22); break;

      case 1370:
        controlstate=1; removetrigger(1370);
			do_fadeout();
      break;
      case 1371:
        state=1500;
      break;




/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//

  1500 - 1700 : Jeff finds the dagger.

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/

		case 1500:
			savestate(state);
        numsprites=0; numobjects=0; numtriggers=0;
			judithpos();controlstate=1;
        settextbox("What did you do today, Judith?", BLUEBEARD, 2, 180,1);break;
      case 1501:
        settextbox("I... read in the drawing room.", JUDITH, 2, 180,1);break;//maybe reading
      case 1502:
        settextbox("The weather was clement.", BLUEBEARD, 2, 180,1);break;
      case 1503:
        settextbox("I hope it's good tomorrow as well.", JUDITH, 2, 180,1);break;
      case 1504:
        settextbox("Good night, my dear.", BLUEBEARD, 2, 180,1);break;
      case 1505:
        settextbox("Good night.", JUDITH, 2, 180,1);
			
			set_player(0);
			changedir(-realdir); changedir(PI); moveto(3.5,10.5);
			break;
      case 1506:
			do_fadein();
      break;
		case 1507: state=1510; controlstate=0; play_effect(26);break;

      case 1510:
        //Special: Restore the doors/triggers for this section!

        //Very short section with Jeff: Find the bloody knife.
        numobjects=0; numtriggers=0; numsprites=0;

        worldMap[13][18]=0;
        addobject(14,23,5,"Open Door"); worldMap[14][23]=8;
        addobject(19,23,6,"Open Door"); worldMap[19][23]=8;
        worldMap[11][32]=8;
        state=0;

        worldMap[13][6]=0;
        worldMap[11][12]=0;
        addobject(21,6,411,"Open Door");
        addobject(24,8,411,"Open Door");


        addsprite(12,2,67);
        addobject(11,2,1530,"Examine Bloody Dagger");

        addtrigger(13,6,1520);

        addsprite(12,2,67);
      break;


      case 1520:
        controlstate=1; removetrigger(1520);
        settextbox("Looks like this used to be", JEFF, 2, 180);
           addline("the armory.");
      break;
      case 1521:
        settextbox("Emily's not in here either.", JEFF, 2, 180);
      break;
      case 1522:
        settextbox("I'm sure... she must be around", JEFF, 2, 180);
           addline("here somewhere.");
      break;
      case 1523: state=0; controlstate=0; break;


      case 1530:
        controlstate=1; removeobject(1530);
        removesprites(67);
        settextbox("It's a dagger,", JEFF, 2, 180);
			addline("with blood on it.");
      break;
      case 1531:
        settextbox("Emily; what's become of you?", JEFF, 2, 180);
      break;
      case 1532:
        settextbox("I hope...I hope you're still", JEFF, 2, 180);
			addline("alive.");
      break;
      case 1533:
			  play_effect(29);
        settextbox("My dear, sweetest Emily.", JEFF, 2, 180);
        backdrop=bg_black;
      break;
      case 1534:
			  do_fadeout();
      break;
      case 1535:
        controlstate=1;
        state=1700;
      break;



/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//

  1700 - 2100 : Judith, burying the prisoner in the garden

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/

		case 1700://bluebeard talks first
			savestate(state);
        changedir(-realdir); moveto(23.5,19.5);
			set_player(1);
        settextbox("Did you dream again, my dear?", BLUEBEARD, 2, 180,1);break;
      case 1701:
        settextbox("Yes, but I can barely", JUDITH, 2, 180,1);
           addline("remember it...");
      break;
      case 1702:
        settextbox("What happened?", BLUEBEARD, 2, 180,1);break;
      case 1703:
        settextbox("I think... I think he was", JUDITH, 2, 180,1);
           addline("digging a grave for her.");
      break;
      case 1704:
        settextbox("You have such dark dreams.", BLUEBEARD, 2, 180,1);break;
      case 1705:
			do_fadein();
      break;
		case 1706: backdrop=bg_outside; state=1710; controlstate=0; play_effect(28); break;

      case 1710:
        //Special: Restore the doors/triggers for this section!

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//

  1700 - 2100 : Judith, burying the prisoner in the garden

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/

        //Long section: Judith discovers the garden, decides to bury the
        //Prisoner there. This section is the first real "lack of
			//interaction" section, should we go with that idea.
			savestate(state);
        backdrop=bg_outside2;
        numobjects=0; numtriggers=0; numsprites=0;
			set_player(1);
        worldMap[13][18]=40;
        addobject(14,23,5,"Open Door"); worldMap[14][23]=8;
        addobject(19,23,6,"Open Door"); worldMap[19][23]=8;
        worldMap[11][32]=8;
        state=0;

        worldMap[11][12]=0;
        worldMap[13][6]=0;
        worldMap[21][6]=0;
        addobject(24,8,211,"Open Door");

        addobject(13,18,1770,"Investigage next room");

        addroom("treasury");
        addroom("garden");
        addsprite(3.5,10.5,65); //Dead prisoner

        //Just add the knife for the armory
        addsprite(12,2,67);

        addobject(2,10,1715,"Examine Prisoner");
        addobject(29,5,1725,"Bury prisoner with shovel");
      break;


      case 1715:
        controlstate=1;
        settextbox("He's dead...", JUDITH, 2, 180);
      break;
      case 1716:
        settextbox("It doesn't feel right to just leave", JUDITH, 2, 180);
           addline("him lying on the floor like this...");
      break;
      case 1717: state=0; controlstate=0; break;


      case 1720:
        shadows=sh_none;
        state=0; controlstate=0;
      break;
      case 1721:
        shadows=sh_normal;
        state=0; controlstate=0;
      break;


            


      case 1725:
      removeobject(1725);
        controlstate=1;
        settextbox("I can't leave the prisoner's", JUDITH, 2, 180,1);
           addline("body lying on that floor.");
      break;
      case 1726:
        settextbox("It's not right. He... needs", JUDITH, 2, 180,1);
           addline("to be put to rest.");
      break;
      case 1727:
        removetrigger(1720); removetrigger(1721);
        removeobject(1715);
        controlstate=2;
        state++;
      break;
      case 1728: if(face(0)) state++; break;
      case 1729: if(movex(-0.2,21.5)) state++; break;
      case 1730: if(face(3*(PI/2))) state++; break;
      case 1731: shadows=sh_normal; if(movey(0.2,8.5)) state++; break;
      case 1732: if(face(0)) state++; break;
      case 1733: if(movex(-0.2,13.5)) state++; break;
      case 1734: if(face(3*(PI/2))) state++; break;
      case 1735: if(movey(0.2,12.5)) state++; break;
      case 1736: if(face(0)) state++; break;
      case 1737:
        moveto(posX-0.2, posY);
        if(posX<=9.5)
		{
			sound_walk(false);
			state++;
		}
      break;
      case 1738: if(face(PI/2)) state++; break;
      case 1739:
        moveto(posX, posY-0.2);
        if(posY<=10.5)
		{
			sound_walk(false);
			state++;
		}
      break;
      case 1740: if(face(0)) state++; break;
      case 1741:
        moveto(posX-0.2, posY);
        if(posX<=4.5)
		{
			sound_walk(false);
			state++;
		}
      break;
      case 1742:
        removesprites(65); //Pick up the prisoner
        state++; statedelay=10;
      break;
      case 1743: if(face(PI)) state++; break;
      case 1744:
        moveto(posX+0.2, posY);
        if(posX>=9.5)
		{
			sound_walk(false);
			state++;
		}
      break;
      case 1745: if(face(3*(PI/2))) state++; break;
      case 1746:
        moveto(posX, posY+0.2);
        if(posY>=12.5)
		{
			sound_walk(false);
			state++;
		}
      break;
      case 1747: if(face(PI)) state++; break;
      case 1748: if(movex(+0.2,13.5)) state++; break;
      case 1749: if(face(PI/2)) state++; break;
      case 1750:
        if(movey(-0.2,8.5)) state++;
      break;
      case 1751: if(face(PI)) state++; break;
      case 1752: if(movex(0.2,21.5)) state++; break;
      case 1753: if(face(PI/2)) state++; break;
      case 1754: if(movey(-0.2,5.5)) state++; break;
      case 1755:
        shadows=sh_none;
        addtrigger(21,6,1720); //Lights on!
        addtrigger(21,7,1721); //Lights off!
        addtrigger(13,17,238); //Close passage behind you
        state++; controlstate=1;
      break;


      case 1756: if(face(PI)) state++; break;
      case 1757: if(movex(0.2,24.5)) state++; break;
      case 1758: if(face(PI/2)) state++; break;

      case 1759:
        addsprite(25.5, 4, 65);
        state++; statedelay=10;
      break;
      case 1760: if(face(PI)) state++; break;
      case 1761: if(movex(0.2,28.5)) state++; break;
      case 1762:
        removesprites(62);
        state++; statedelay=10;
      break;
      case 1763: if(face(0)) state++; break;
      case 1764: if(movex(-0.2,25.5)) state++; break;
      case 1765: if(face(PI/2)) state++; break;
      case 1766:
        //The whole actual digging thing
        state=1800; statedelay=10; //Skip to 1800
      break;

      case 1800:
        //The whole actual digging thing continued!
        //add the clay sprites later
        floorMap[25][3]=25;
        state++; statedelay=20; //Skip to 1800
      break;
      case 1801:
        floorMap[25][3]=0;
        state++; statedelay=20;
      break;
      case 1802: if(face(0)) state++; break;
      case 1803: if(movex(-0.2,24.5)) state++; break;
      case 1804: if(face(PI/2)) state++; break;
      case 1805:
        removesprites(65);
        state++; statedelay=10;
      break;
      case 1806: if(face(PI)) state++; break;
      case 1807: if(movex(0.2,25.5)) state++; break;
      case 1808: if(face(PI/2)) state++; break;
      case 1809: state++; statedelay=20; break;
      case 1810:
        floorMap[25][3]=25;
        state++; statedelay=20;
			addtrigger(13,25,1850);
			addtrigger(15,25,1850);
      break;
      case 1811:
        controlstate=1;
        settextbox("Rest in peace.", JUDITH, 2, 180);
      break;
		case 1812: state=0; controlstate=0;  play_effect(33); break;


      case 1850:
        controlstate=1; removetrigger(1850);
        backdrop=bg_black;
        state++;
      break;
      case 1851:
			  do_fadeout();
      break;
      case 1852:
        state=2100;
      break;





      case 1770:
        removeobject(1770);
        posX=13.5;
        controlstate=2; state++;
      break;
      case 1771: if(face(PI/2)) state++; break;
      case 1772:
        controlstate=1;
        settextbox("I know I shouldn't be", JUDITH, 2, 180);
           addline("going back here...");
      break;
      case 1773:
        settextbox("But I need to know what", JUDITH, 2, 180);
           addline("kind of man I'm married to.");
      break;
      case 1774:
			worldMap[13][18]=0; state++; statedelay=10;
			place_sound(3, 13, 18); activate_sound(3);
      break;
      case 1775:
        settextbox("I need to know...", JUDITH, 2, 180);
      break;
      case 1776:
        controlstate=2;
        if(movey(-0.2,8.5)) state++;
      break;
      case 1777: if(face(PI)) state++; break;
      case 1778: if(movex(0.2,21.5)) state++; break;
      case 1779: if(face(PI/2)) state++; break;
      case 1780: if(movey(-0.2,5.5)) state++; break;
      case 1781:
        shadows=sh_none;
        addtrigger(21,6,1720); //Lights on!
        addtrigger(21,7,1721); //Lights off!
        addtrigger(13,17,238); //Close passage behind you
        state++; controlstate=1;
      break;
      case 1782:
        settextbox("Oh! It's a garden...", JUDITH, 2, 180);
      break;
      case 1783: state=0; controlstate=0; play_effect(34); break;




/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//

  2100 - 2400 : Jeff finds the key in the garden.

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/


		case 2100://goes to bed, having buried the body
			savestate(state);
        controlstate=1; 
        numobjects=0; numtriggers=0; numsprites=0;
			judithpos();
        settextbox("What did you do today, Judith?", BLUEBEARD, 2, 180,1);break;
      case 2101:
        settextbox("Oh, I picked some flowers in the", JUDITH, 2, 180,1);
           addline("front garden.");
      break;
      case 2102:
        settextbox("It's beautiful this time of year.", BLUEBEARD, 2, 180,1);break;
      case 2103:
        settextbox("I remember the first time I", BLUEBEARD, 2, 180,1);
           addline("brought you to the castle.");
      break;
      case 2104:
        settextbox("Yes, I remember too. The", JUDITH, 2, 180,1);
           addline("violets were in bloom.");
      break;
      case 2105:
        settextbox("Darling, I... was wondering...", JUDITH, 2, 180,1); break;
      case 2106:
        settextbox("What is it?", BLUEBEARD, 2, 180,1);break;
      case 2107:
        settextbox("...", JUDITH, 2, 180,1); break;
      case 2108:
        settextbox("Nevermind. It's nothing.", JUDITH, 2, 180,1); break;
      case 2109:
        settextbox("Good night, my dear.", BLUEBEARD, 2, 180,1);break;
      case 2110:
        settextbox("Good night.", JUDITH, 2, 180,1);
			changedir(-realdir); moveto(12.5,2.5);set_player(0);
			break;
      case 2111: do_fadein(); break;
      case 2112: backdrop=bg_outside; state=2115; controlstate=0; play_effect(41);break;


      case 2115:
        //Special: Restore the doors/triggers for this section!

        //Jeff's garden scene with get key puzzle
        backdrop=bg_outside;
        numobjects=0; numtriggers=0; numsprites=0;
        worldMap[13][18]=0;
        worldMap[14][23]=0;
        worldMap[19][23]=0;
        worldMap[11][32]=8;
        state=0;

        worldMap[11][12]=0;
        worldMap[13][6]=0;
        worldMap[21][6]=0;
        floorMap[25][3]=25;
        addobject(24,8,411,"Open Door");
        addobject(25,3,2120,"Examine Ground");
        addobject(29,5,2122,"Take Shovel");
        addtrigger(21,5,2118);

        addroom("garden");
      break;

      case 2118:
        controlstate=1; removetrigger(2118);
        settextbox("A garden. There's nobody here either.", JEFF, 2, 180);
      break;
      case 2119: state=0; controlstate=0; break;

      case 2120:
        settextbox("It looks like there might be", JEFF, 2, 180,1);
           addline("something buried here.");
      break;
      case 2121: state=0; controlstate=0; break;

      case 2122:
        removesprites(62); removeobject(2122);
        removeobject(2120);

        addobject(25,3,2130,"Dig");
        settextbox("This shovel.", JEFF, 2, 180,1);
      break;
      case 2123:
        settextbox("There's only one way to know", JEFF, 2, 180,1);
			addline("for sure if it's her.");
        //ok, maybe have a little menu here or something
      break;
      case 2124: state=0; controlstate=0; break;


      case 2130:
        controlstate=2;
        removeobject(2130);
        state++; statedelay=10; //Add proper diggy sprites and stuff here later
      break;
      case 2131:
        floorMap[25][3]=109;
        state++; statedelay=20;
      break;
      case 2132:
        controlstate=1;
        settextbox("There's a skeleton here.", JEFF, 2, 180);
           addline("He's long dead.");
      break;
      case 2133:
        settextbox("Wait...", JEFF, 2, 180,1);break;
      case 2134:
        addobject(25,3,2134,"Examine Skeleton");
        settextbox("I think I see something amongst", JEFF, 2, 180,1);
           addline("the bones.");
      break;
      case 2135:
        settextbox("Disturb the body?", JEFF, 4, 180,1);
           addline("YES");
           addline("NO"); nostate=0;
      break;
      case 2136:
        removeobject(2134);
        settextbox("It's a key!", JEFF, 2, 180,1);
        removeobject(411); addobject(24,8,2140,"Open Door");
      break;
      case 2137:
        settextbox("I wonder why this poor man was", JEFF, 2, 180,1);
			addline("buried here, in an unmarked grave?");
      break;
      case 2138: state=0; controlstate=0; play_effect(25);break;

      case 2140:
        controlstate=1; removeobject(2140);
        settextbox("The key fits.", JEFF, 2, 180);
		place_sound(2, 24, 8); activate_sound(2);
        backdrop=bg_black;
      break;
      case 2141: do_fadeout(); break;
      case 2142:
        controlstate=1;
        state=2400;
      break;




/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//

  2400 - : All Judith scripts, Kingdom onwards.

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/

      //Judith in bed
		case 2400:
			savestate(state);
        changedir(-realdir); moveto(23.5,19.5);
			set_player(1);
        settextbox("I dreamt of him again.", JUDITH, 2, 180,1);break;
      case 2401:
        settextbox("He took his own life.", JUDITH, 2, 180,1);break;
      case 2402:
		    settextbox("You dream of such a weak man,", BLUEBEARD, 2, 180,1);
		       addline("my wife.");break;
	    case 2403:
		    settextbox("Do you think me so weak?",BLUEBEARD,2,180,1);
		  break;
		  case 2404: play_effect(35); state++; break;
		  case 2405: do_fadein();  break;
      case 2406: state=2410; controlstate=0; break;

      case 2410:
        //Special: Restore the doors/triggers for this section!

        //Judith's Kingdom section
        backdrop=bg_black;
        numobjects=0; numtriggers=0; numsprites=0;
		    placeDoors("lccllllocll");
        worldMap[13][18]=40;
        addobject(14,23,5,"Open Door"); worldMap[14][23]=8;
        addobject(19,23,6,"Open Door"); worldMap[19][23]=8;
        //worldMap[11][32]=8;
        state=0;

        /*worldMap[15][15]=47; addobject(15,15,211,"Open Door");
        worldMap[11][12]=48; addobject(11,12,211,"Open Door");
        worldMap[13][6]=49; addobject(13,6,211,"Open Door");
        worldMap[21][6]=50; addobject(21,6,211,"Open Door");
        worldMap[24][8]=0;*/

        addroom("kingdom");

        addobject(13,18,245,"Open secret passage");
        addtrigger(13,17,238);

        for(int i=0; i<10; i++){
          addtrigger(32,8+i,2500);
          addtrigger(28+i,11,2500);
          addtrigger(28+i,15,2500);
        }
      break;


        //goes to balcony, finds husband
      case 2500:
        removetrigger(2500);
        addsprite(31.5,13.5,50);
        state++;
      break;
      case 2501:
        controlstate=2;

        if(posX<32)posX+=.5;
        if(posY<13)posY+=.5;
        if(posY>14)posY-=.5;
        if(posY>13 && posY<14) posY=13.5;
        if(face(0)) state++;
      break;
      case 2502:
        controlstate=1;
        settextbox("Judith.", BLUEBEARD, 2, 180,1);
      break;
      case 2503:
        settextbox("I know what you've been doing.", BLUEBEARD, 2, 180,1);
      break;
      case 2504: if(movex(0.15,33)) state++; break;
      case 2505:
        settextbox("I'm... I'm sorry...", JUDITH, 2, 180,1);break;
      case 2506:
        settextbox("You're my wife - I didn't expect", BLUEBEARD, 2, 180,1);
           addline("I could keep these secrets");
           addline("from you forever...");
      break;
      case 2507: if(movex(0.15,34)) state++; break;
      case 2508:
        settextbox("Please... don't hurt me...", JUDITH, 2, 180,1);
			play_effect(39);
      break;
      case 2509: if(movex(0.15,35)) state++; break;
      case 2510:
        controlstate=2;
        if(face(PI)) state=2511;
        if(face(PI)) state=2511; //Double speed! Hence the exact numbers
      break;
      case 2511:
        shakyrun--;
        if(shakyrun<=0){
          posY+=(int(rand()%10)-5)*0.01;
          shakyrun=4;
        }
        if(movex(0.4,85)) state++;
      break;
      case 2512: do_fadeout(); break;
      case 2513:
        controlstate=1;
        state=2600;
      break;
//she just runs off

      //waking up in bed

      case 2600:
        changedir(-realdir); moveto(23.5,19.5);
        controlstate=1;
        settextbox("Judith?", BLUEBEARD, 2, 180,1);break;
      case 2601:
        settextbox("Are you awake?", BLUEBEARD, 2, 180,1);break;
      case 2602:
        settextbox("I... what happened?", JUDITH, 2, 180,1);break;
      case 2603:
        settextbox("My dear, you almost died.", BLUEBEARD, 2, 180,1);break;
      case 2604:
        settextbox("I did?", JUDITH, 2, 180,1);break;
      case 2605:
        settextbox("You ran off, and were caught", BLUEBEARD, 2, 180,1);
           addline("up in a terrible storm.");
      break;
      case 2606:
        settextbox("...", JUDITH, 2, 180,1);break;
      case 2607:
        settextbox("Judith... I'm sorry that I've", BLUEBEARD, 2, 180,1);
           addline("kept so many secrets from you.");
      break;
      case 2608:
        settextbox("When you're ready, I want", BLUEBEARD, 2, 180,1);
           addline("to show you something.");
      break;
		case 2609: do_fadein(); play_effect(38);break;
      case 2610: state=2620; controlstate=0; break;

      case 2620:
        //Special: Restore the doors/triggers for this section!

        //Judith's Lake section
        backdrop=bg_black;
        numobjects=0; numtriggers=0; numsprites=0;

		    placeDoors("lccllllolcl");

        worldMap[13][18]=0;
        addobject(14,23,5,"Open Door"); worldMap[14][23]=8;
        addobject(19,23,6,"Open Door"); worldMap[19][23]=8;
        worldMap[11][32]=8;
        state=0;

        /*worldMap[15][15]=47; addobject(15,15,211,"Open Door");
        worldMap[11][12]=48; addobject(11,12,211,"Open Door");
        worldMap[13][6]=49; addobject(13,6,211,"Open Door");
        worldMap[21][6]=50; addobject(21,6,211,"Open Door");
        worldMap[28][13]=51; addobject(28,13,211,"Open Door");*/
        worldMap[24][8]=0;
        //worldMap[28][30]=0;

        addsprite(32.5,30.5,50);
        addtrigger(28,30,2700);
      break;

      //Pool of tears
      case 2700:
        removetrigger(2700);
        if(face(PI))state++;
      break;
      case 2701:
        settextbox("What is this? This pool...", JUDITH, 2, 180,1);break;
      case 2702:
        settextbox("Please understand - I've shed so", BLUEBEARD, 2, 180,1);
           addline("many tears over what I've had to do.");
      break;
      case 2703:
        settextbox("My love... you don't need to", JUDITH, 2, 180,1);
           addline("keep secrets from me.");
      break;
      case 2704:
        settextbox("There are some things", BLUEBEARD, 2, 180,1);
           addline("I can never tell you.");
      break;
      case 2705:
        settextbox("Please! Tell me everything!", JUDITH, 2, 180,1);
      break;
      case 2706:
        settextbox("You can trust me.", JUDITH, 2, 180,1);
           addline("I'll understand.");
      break;
      case 2707:
        settextbox("There is one more door", BLUEBEARD, 2, 180,1);
           addline("in this castle.");
      break;
      case 2708:
        settextbox("But please - don't ask me", BLUEBEARD, 2, 180,1);
           addline("to open it for you.");
      break;
      case 2709:
        settextbox("Ask to see the final room?", JUDITH, 5, 180,1);
           addline("YES");
      break;
      case 2710:
        settextbox("I... want to know everything", JUDITH, 2, 180,1);
           addline("about you, my love.");
      break;
      case 2711:
        settextbox("I was afraid you'd say that.", BLUEBEARD, 2, 180,1);
      break;
      case 2712:
        if(movex(0.1,29.5)) state++;
      break;
      case 2713:
        settextbox("...as you wish, my dear.", BLUEBEARD, 2, 180,1);
      break;
      case 2714:
        settextbox("This key unlocks the final door.", BLUEBEARD, 2, 180,1);
      break;
      case 2715: state++; controlstate=2; statedelay=20; break;
      case 2716: if(face(0)) state++; break;
      case 2717: if(movex(-0.1,26.5)) state++; break;
      case 2718: if(face(3*(PI/2))) state++; break;
      case 2719: if(movey(0.1,40.5)) state++; break;

      case 2720:
        removesprites(50);
        addsprite(27.5,38.5,50);
        worldMap[26][42]=53;
        state++; controlstate=1; statedelay=20;
      break;
      case 2721:
        settextbox("The key fits.", JUDITH, 2, 180,1);
      break;
      case 2722:
        controlstate=2;
        if(face(PI/2)) state++;
      break;
      case 2723:
        controlstate=1;
        settextbox("Open the door, now, and see.", BLUEBEARD, 2, 180,1);
           addline("My former loves await you.");
      break;
      case 2724:
        controlstate=2;
        state++; statedelay=25;
      break;
      case 2725:
        if(face(3*(PI/2))) state++;
      break;
      case 2726:
        state++; statedelay=25;
      break;
      case 2727:
        place_sound(2, 26, 42); activate_sound(2);
        worldMap[26][42]=0;
        state++;
      break;
      case 2728:
        state++; statedelay=75;
      break;
      case 2729:
        if(movey(0.1,45.5)) state++;
      break;
      case 2730:
        if(face(PI/2)) state++;
      break;
      case 2731:
        state++; statedelay=75;
      break;
      case 2732:
        place_sound(2, 26, 42); activate_sound(2);
        worldMap[26][42]=5;
        state++;
      break;
      case 2733:
        removesprites(50);
        rendermode=rm_black;
        state++;
      break;
      case 2734: state=2750; statedelay=75; break;


      case 2750:
        numsprites=0; numobjects=0; numtriggers=0;
			  set_player(0);
        controlstate=2; changedir(-realdir); changedir(PI); moveto(22.5,8.5);
        state++;
      break;
      case 2751:
        //Special: Jeff's last scene

		    placeDoors("loooooocllc");
		    state++;
      break;
      case 2752:
			  do_fadein();
      break;
		  case 2753:
		    addsprite(27.5, 43.5, 45);
		    backdrop=bg_outside;
		    state=0; controlstate=0;
      break;


      //Flashes back to Jeff, meets Emily
      case 2800:
        settextbox("Emily!", JEFF, 2, 180,1);break;
      case 2801:
        settextbox("Jeff! Is that you?", EMILY, 2, 180,1);break;
      case 2802:
        settextbox("I thought... something had", JEFF, 2, 180,1);
           addline("happened to you.");
      break;

      case 2803:
        settextbox("I just turned around and you", EMILY, 2, 180,1);
           addline("were gone... I got so lost...");
      break;
      case 2804:
        settextbox("I've never been so afraid.", EMILY, 2, 180,1);
      break;
      case 2805:
        settextbox("Jeff, please promise you'll", EMILY, 2, 180,1);
			addline("never leave me again.");
      break;
		case 2806:controlstate=3; state++;play_effect(43);savestate(0);
      case 2807: break;




      case 10000:
        //return to normal controlmode
        controlstate=0;
        state=0;
      break;

			/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//

			 5000 + : Misc. scenes added later.

			 //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/

		case 5000:
			//bookcase closing behind you (Judith, talked to prisoner)
			removetrigger(5000);
			controlstate=2;
			state++;
			break;
		case 5001: if(face(PI/2)) state++; break;
		case 5002:
			moveto(posX, posY+0.1);
			if(posY>20.5)
			{
				sound_walk(false);
				state++;
			}
			break;
		case 5003:
			worldMap[13][18]=40;
			state++; statedelay=10;
			break;
		case 5004:
			controlstate=1;
			settextbox("I... I don't believe it...", JUDITH, 2, 180);
			break;
		case 5005:
			settextbox("That poor man... his legs, broken...", JUDITH, 2, 180);
			addline("practically starved to death...");
			break;
		case 5006:
			settextbox("My husband's just barely", JUDITH, 2, 180);
			addline("keeping him alive...");
			break;
		case 5007:
			settextbox("He'll never be able to", JUDITH, 2, 180);
			addline("leave that dungeon.");
			break;
		case 5008:
			settextbox("My husband has so many", JUDITH, 2, 180);
			addline("terrible secrets.");
			break;
		case 5009:
			settextbox("I must know them all", JUDITH, 2, 180);
			addline("if I am to love him.");
			break;
		case 5010: state=0; controlstate=0; break;


		case 5020:
			//Closing the door in opening scene
			removetrigger(5020);
			controlstate=2;
			state++;
			break;
		case 5021:
			if(face(3*(PI/2))) state++;
			break;
		case 5022:
			state++; statedelay=10;
			break;
		case 5023:
			worldMap[11][32]=8;
			place_sound(2, 11, 32); activate_sound(2);
			state++; statedelay=10;
			break;
		case 5024:
			if(face(PI/2)) state++;
			break;
		case 5025:
			addtrigger(13,22,5030); addtrigger(14,22,5030);  addtrigger(15,22,5030);
			controlstate=0; state=0;
			break;


		case 5030:
			controlstate=1; removetrigger(5030);
			settextbox("Emily?", JEFF, 2, 180);
			break;
		case 5031:
			addtrigger(18,22,5040); addtrigger(19,22,5040);  addtrigger(20,22,5040);
			settextbox("Oh, she must be in", JEFF, 2, 180);
			addline("the other room.");
			break;
		case 5032: state=0; controlstate=0; break;


		case 5040:
			controlstate=1; removetrigger(5040);
			settextbox("Emily?", JEFF, 2, 180);
			break;
		case 5041:
			addobject(23,20,7,"Examine Bed");
			addobject(23,21,7,"Examine Bed");
			addobject(22,20,7,"Examine Bed");
			addobject(22,21,7,"Examine Bed");
			settextbox("That's strange... ", JEFF, 2, 180);
			break;
		case 5042:
			settextbox("She doesn't seem to be", JEFF,2,180);
			addline("in here either.");break;
		case 5043: state=0; controlstate=0; break;


            
            // Jason's slideshow hack states here
        case 6001:
            // post intro, moving down hall
            
            controlstate=2; play_effect(1);;
            addtrigger(12,52,100);
            addtrigger(10,52,100);
            addtrigger(11,52,100);

            addtrigger(10,34,140);
            addtrigger(11,34,140);
            addtrigger(12,34,140);

            // skip this, and just add the Emily sprite now
            //addtrigger(2,56,139);
            addsprite(12.5,50.5,45);
            //just adding the Emily sprite
            

            worldMap[10][65]=5;


            addroom("frontgarden");
            controlstate=0;
            state=0;

            controlstate=0;
            moveto( 4.972000, 65.500000 );
            forceDir( 0.000000 );
            state=0;
            break;
        case 6002:
            // post intro, moving down hall farther (assume 6001 happened to
            // set things up)
            controlstate=0;
            moveto( 9.631307, 55.648786 );
            forceDir( 2.876000 );
            backdrop=bg_outside;
            state=0;
            break;
        case 6003:
            // meeting emily

            // make sure she's in the right spot, right side
            removesprites(45);
            removesprites(46);
            addsprite(12.5,50.5,45);

            removetrigger( 100 );
            moveto( 11.451766, 52.972857 );
            forceDir( 1.570760 );
            state=102;
            break;
        case 6004:
            // outside, emily waiting at door, looking at flowers
            controlstate=2;
            removesprites(45);
            addsprite(12.5,50.5,46); state++;
            play_effect(2);
            
            controlstate=0;
            
            
            addsprite(12.5,33.6,45);
            controlstate=0; //cutscenemode=0;
            state=0;
            
            moveto( 11.053878, 37.348724 );
            forceDir( 2.194760 );
            
            backdrop=bg_outside;
            break;
        case 6005: {
            // talking to Emily at front door
            moveto( 11.425761, 34.724934 ); forceDir( 1.570760 );
            controlstate=2;
            removetrigger(140);
            state=142;
            }
            break;
        case 6006: {
            // looking for emily at bed
            moveto( 20.598554, 20.860332 ); forceDir( 3.279721 );
            state=7;
            }
            break;
        case 6007: {
            // Judith looking at neclace
            moveto( 17.208855, 15.794202 ); forceDir( 2.553519 );
            
            rendermode = rm_normal;
            backdrop=bg_outside; controlstate=0;

            //Special: Restore the doors/triggers for this section!
            numobjects=0; numtriggers=0;
            placeDoors("lccclllllll");
            addobject(13,18,215,"Examine Bookcase"); worldMap[13][18]=40;
            addobject(14,23,5,"Open Door");
            addobject(19,23,6,"Open Door");
            state=0;
            
            addobject(11,12,213,"Open Door");
            addobject(13,6,211,"Open Door");
            addobject(21,6,211,"Open Door");
            addobject(24,8,211,"Open Door");
            
            addobject(20,14,230,"Examine Necklace");
			addtrigger(17, 14, 226);
			addtrigger(17, 15, 226);

			addtrigger(17, 16, 226);

            addroom("treasury");

            state = 227;
            }
            break;
        case 6008: {
            // closing bookcase
            moveto( 13.143044, 20.585773 ); forceDir( 1.570760 );

            state=264;
            }
            break;
        case 6009: {
            // Take Neclace?

                        rendermode = rm_normal;
            backdrop=bg_outside; controlstate=0;

            //Special: Restore the doors/triggers for this section!
            numobjects=0; numtriggers=0;
            placeDoors("lccclllllll");
            addobject(13,18,215,"Examine Bookcase"); worldMap[13][18]=40;
            addobject(14,23,5,"Open Door");
            addobject(19,23,6,"Open Door");
            state=0;
            
            addobject(11,12,213,"Open Door");
            addobject(13,6,211,"Open Door");
            addobject(21,6,211,"Open Door");
            addobject(24,8,211,"Open Door");
            
            addobject(20,14,230,"Examine Necklace");
			addtrigger(17, 14, 226);
			addtrigger(17, 15, 226);

			addtrigger(17, 16, 226);

            addroom("treasury");

            moveto( 18.307481, 15.352753 ); forceDir( 2.759519 );
            state = 232;
            controlstate=1;
            }
            break;
        case 6010: {
            
            // Judith about to pick up the shovel
            backdrop=bg_outside; state=1710; controlstate=0; play_effect(28);
			
            backdrop=bg_outside2;
            numobjects=0; numtriggers=0; numsprites=0;
			set_player(1);
            worldMap[13][18]=40;
            addobject(14,23,5,"Open Door"); worldMap[14][23]=8;
            addobject(19,23,6,"Open Door"); worldMap[19][23]=8;
            worldMap[11][32]=8;
            state=0;

            worldMap[11][12]=0;
            worldMap[13][6]=0;
            worldMap[21][6]=0;
            addobject(24,8,211,"Open Door");

            addobject(13,18,1770,"Investigage next room");

            addroom("treasury");
            addroom("garden");
            addsprite(3.5,10.5,65); //Dead prisoner

            //Just add the knife for the armory
            addsprite(12,2,67);

            addobject(2,10,1715,"Examine Prisoner");
            addobject(29,5,1725,"Bury prisoner with shovel");


            // looking at shovel
            // Player pos(27.945263, 4.997760), rot(1.247481, 0.541405)
            // New realdir = 3.476000

            //moveto(29,5);

            moveto( 27.9, 4.99 );
            forceDir( 3.47 );

            //do_fadein();
            rendermode = rm_normal;
            backdrop=bg_outside2;
            }
            break;
        case 6011: {
            // Jeff about to dig up grave
            numobjects=0; numtriggers=0; numsprites=0;

            backdrop=bg_outside; controlstate=0; play_effect(41);
            
            //Special: Restore the doors/triggers for this section!

            //Jeff's garden scene with get key puzzle
            backdrop=bg_outside;
            numobjects=0; numtriggers=0; numsprites=0;
            worldMap[13][18]=0;
            worldMap[14][23]=0;
            worldMap[19][23]=0;
            worldMap[11][32]=8;
            state=0;
            
            worldMap[11][12]=0;
            worldMap[13][6]=0;
            worldMap[21][6]=0;
            floorMap[25][3]=25;
            addobject(24,8,411,"Open Door");
            addobject(25,3,2120,"Examine Ground");
            addobject(29,5,2122,"Take Shovel");
            addtrigger(21,5,2118);
            
            addroom("garden");
            rendermode = rm_normal;
            moveto( 21.671870, 5.794336 ); forceDir( 2.552000 );
            }
            break;

        case 6012: {
            // ask to see pool

            backdrop=bg_black;
            numobjects=0; numtriggers=0; numsprites=0;

		    placeDoors("lccllllolcl");

            worldMap[13][18]=0;
            addobject(14,23,5,"Open Door"); worldMap[14][23]=8;
            addobject(19,23,6,"Open Door"); worldMap[19][23]=8;
            worldMap[11][32]=8;
            state=0;

            /*worldMap[15][15]=47; addobject(15,15,211,"Open Door");
              worldMap[11][12]=48; addobject(11,12,211,"Open Door");
              worldMap[13][6]=49; addobject(13,6,211,"Open Door");
              worldMap[21][6]=50; addobject(21,6,211,"Open Door");
              worldMap[28][13]=51; addobject(28,13,211,"Open Door");*/
            worldMap[24][8]=0;
            //worldMap[28][30]=0;

            addsprite(32.5,30.5,50);

            moveto( 28.724872, 30.657436 ); forceDir( 3.141519 );

            state=2707;
            }
            break;
        
        case 6100: {
            // template
            }
            break;





      default:
      break;
    }
  }
}
