#include "rendermodes.hpp"


int calcHeight(int x, double perpWallDist)
{
    switch (rendermode)
    {
           case rm_normal:
           case rm_fuzzy:
           case rm_fade:
		   case rm_black:
                return abs(int(h / perpWallDist))-heights[x];
                break;
           case rm_delay:
                heights[x]=heights[x]+(abs(int(h / perpWallDist))-heights[x])/5;
                return heights[x];
                break;
    }
}



void do_fadeout()
{
	if (rendermode==rm_fade)
		return;
    rendermode=rm_fade;
    fadein=false;
    fadeblackness=0.01f;
    backdrop=bg_black;
}

void do_fadein()
{
	if (rendermode==rm_fade)
		return;
    rendermode=rm_fade;
    fadein=true;
    fadeblackness=1.0f;
    backdrop=bg_black;
}

void gamerender(){
  if (rendermode==rm_fade){
    if (fadein){
      fadeblackness-=0.5f*(ftime - foldTime) /1000.0f;
      if (fadeblackness<0){
        game.state++;
        game.statedelay=0;
        rendermode=rm_normal;
      }
    }else{
      fadeblackness+=0.5f*(ftime - foldTime) /1000.0f;
      if (fadeblackness>1){
        game.state++;
//        game.statedelay=0;
        rendermode=rm_black;
		game.statedelay=30;
      }
    }
  }

  for (int x = 0; x < w; x++){
        //calculate ray position and direction
        double cameraX = 2 * x / double(w) - 1; //x-coordinate in camera space
        double rayPosX = posX;
        double rayPosY = posY;
        double rayDirX = dirX + planeX * cameraX;
        double rayDirY = dirY + planeY * cameraX;

        //which box of the map we're in
        int mapX = int(rayPosX);
        int mapY = int(rayPosY);

        //length of ray from current position to next x or y-side
        double sideDistX;
        double sideDistY;

        //length of ray from one x or y-side to next x or y-side
        double deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
        double deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));
        double perpWallDist;

        //what direction to step in x or y-direction (either +1 or -1)
        int stepX;
        int stepY;

        int hit = 0; //was there a wall hit?
        int side; //was a NS or a EW wall hit?

        //calculate step and initial sideDist
        if (rayDirX < 0)
        {
            stepX = -1;
            sideDistX = (rayPosX - mapX) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX;
        }

        if (rayDirY < 0)
        {
            stepY = -1;
            sideDistY = (rayPosY - mapY) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
        }

        //perform DDA
        while (hit == 0)
        {
            //jump to next map square, OR in x-direction, OR in y-direction
            if (sideDistX < sideDistY)
            {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
            else
            {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            //Check if ray has hit a wall
            if (worldMap[mapX][mapY] > 0) hit = 1;
        }

        //Calculate distance of perpendicular ray (oblique distance will give fisheye effect!)
        if (side == 0) perpWallDist = fabs((mapX - rayPosX + (1 - stepX) / 2) / rayDirX);
        else       perpWallDist = fabs((mapY - rayPosY + (1 - stepY) / 2) / rayDirY);

        //Calculate height of line to draw on screen
        int height=calcHeight(x,perpWallDist);

        //calculate lowest and highest pixel to fill in current stripe
        int drawStart = -height / 2 + h / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = height / 2 + h / 2;
        if (drawEnd >= h+2) drawEnd = h+1;
        //texturing calculations
        int texNum = worldMap[mapX][mapY]-1; //1 subtracted from it so that texture 0 can be used!

        //calculate value of wallX
        double wallX; //where exactly the wall was hit
        if (side == 1) wallX = rayPosX + ((mapY - rayPosY + (1 - stepY) / 2) / rayDirY) * rayDirX;
        else       wallX = rayPosY + ((mapX - rayPosX + (1 - stepX) / 2) / rayDirX) * rayDirY;
        wallX -= floor((wallX));

        //x coordinate on the texture
        int texX = int(wallX * double(texWidth));
        if (side == 0 && rayDirX > 0) texX = texWidth - texX - 1;
        if (side == 1 && rayDirY < 0) texX = texWidth - texX - 1;
        for (int y = drawStart; y < drawEnd; y++)
        {
            int d = y * 256 - h * 128 + height * 128; //256 and 128 factors to avoid floats
            int texY = ((d * texHeight) / height) / 256;
            int color = texture[wall[texNum].s(side)][texWidth * texY + texX];
            //make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
            if (side == 1 && shadows!=sh_none)
            {
                color = (color >> 1) & 8355711;
                if (shadows==sh_enhanced)
                {
                    color = (color >> 1) & 8355711;
                    color = (color >> 1) & 8355711;
                }
            }
            color = (color >> 1) & 8355711;
            //float tst=(2.5 + (rand()%250)/100)/perpWallDist;
            float tst=(lighting*3)/perpWallDist;
            if (tst<0)tst=0;
            if (tst>1||shadows==sh_none) tst=1;
            if (rendermode==rm_fuzzy)
                buffer[x+(rand()%5)][y+(rand()%5)] = dim(color, tst*2);
            else if (rendermode==rm_fade)
            {
                if (rand()%(100)>=(fadeblackness*100))
                  buffer[x][y] = dim(color, tst*2);
            }
            else
                buffer[x][y] = dim(color, tst*2);
            //buffer[x][y] = color;
        }

        //SET THE ZBUFFER FOR THE SPRITE CASTING
        ZBuffer[x] = perpWallDist; //perpendicular distance is used

        //FLOOR CASTING
        double floorXWall, floorYWall; //x, y position of the floor texel at the bottom of the wall

        //4 different wall directions possible
        if (side == 0 && rayDirX > 0)
        {
            floorXWall = mapX;
            floorYWall = mapY + wallX;
        }
        else if (side == 0 && rayDirX < 0)
        {
            floorXWall = mapX + 1.0;
            floorYWall = mapY + wallX;
        }
        else if (side == 1 && rayDirY > 0)
        {
            floorXWall = mapX + wallX;
            floorYWall = mapY;
        }
        else
        {
            floorXWall = mapX + wallX;
            floorYWall = mapY + 1.0;
        }

        double distWall, distPlayer, currentDist;

        distWall = perpWallDist;
        distPlayer = 0.0;
        int color=0;
        float tst;
        if (drawEnd < 0) drawEnd = h; //becomes < 0 when the integer overflows
        //draw the floor from drawEnd to the bottom of the screen
        for (int y = drawEnd+1; y <= h; y++)
        {
            currentDist = h / (2.0 * y - h); //you could make a small lookup table for this instead

            double weight = (currentDist - distPlayer) / (distWall - distPlayer);

            double currentFloorX = weight * floorXWall + (1.0 - weight) * posX;
            double currentFloorY = weight * floorYWall + (1.0 - weight) * posY;

            int floorTexX, floorTexY;
            floorTexX = int(currentFloorX * texWidth) % texWidth;
            floorTexY = int(currentFloorY * texHeight) % texHeight;

            int floorTexture, ceilingTexture;
            floorTexture=floorMap[int(currentFloorX)][int(currentFloorY)];
            ceilingTexture=ceillingMap[int(currentFloorX)][int(currentFloorY)];


            /*
                  //floor
                  //buffer[x][y - 1] = (texture[floorTexture][texWidth * floorTexY + floorTexX] >> 1) & 8355711;
                  buffer[x][y - 1] = texture[floorTexture][texWidth * floorTexY + floorTexX];
                  //ceiling (symmetrical!)
                  buffer[x][h - y] = texture[ciellingTexture][texWidth * floorTexY + floorTexX];
              */
            if (floorTexture>=1)
            {
                color = texture[floorTexture-1][texWidth * floorTexY + floorTexX];
                color = (color >> 1) & 8355711;
                tst=lighting/currentDist;

                if (tst<0)tst=0;
                if (tst>1 || shadows==sh_none) tst=1;
                if (rendermode==rm_fuzzy)
                    buffer[x+(rand()%5)][y+(rand()%5) - 1] = dim(color, tst*2);
                else if (rendermode==rm_fade)
                {
                    if (rand()%(100)>=(fadeblackness*100))
                        buffer[x][y-1] = dim(color, tst*2);
                }
                else
                    buffer[x][y - 1] = dim(color, tst*2);

            }

            if (ceilingTexture>=1)
            {
                color = texture[ceilingTexture-1][texWidth * floorTexY + floorTexX];
                color = (color >> 1) & 8355711;
                tst=lighting/currentDist;

                if (tst<0)tst=0;
                if (tst>1 || shadows==sh_none) tst=1;

                if (rendermode==rm_fuzzy)
                    buffer[x+(rand()%5)][h +(rand()%5)- y] = dim(color, tst*2);
                else if (rendermode==rm_fade)
                {
                    if (rand()%(100)>=(fadeblackness*100))
                        buffer[x][h-y] = dim(color, tst*2);
                }
                else
                    buffer[x][h - y] = dim(color, tst*2);
            }
        }
    }

    //SPRITE CASTING
    //sort sprites from far to close
    for (int i = 0; i < numsprites; i++)
    {
        spriteOrder[i] = i;
        spriteDistance[i] = ((posX - sprite[i].x) * (posX - sprite[i].x) + (posY - sprite[i].y) * (posY - sprite[i].y)); //sqrt not taken, unneeded
    }
    combSort(spriteOrder, spriteDistance, numsprites);

    //after sorting the sprites, do the projection and draw them
    for (int i = 0; i < numsprites; i++)
    {
        //translate sprite position to relative to camera
        double spriteX = sprite[spriteOrder[i]].x - posX;
        double spriteY = sprite[spriteOrder[i]].y - posY;

        //transform sprite with the inverse camera matrix
        // [ planeX   dirX ] -1                                       [ dirY      -dirX ]
        // [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
        // [ planeY   dirY ]                                          [ -planeY  planeX ]

        double invDet = 1.0 / (planeX * dirY - dirX * planeY); //required for correct matrix multiplication

        double transformX = invDet * (dirY * spriteX - dirX * spriteY);
        double transformY = invDet * (-planeY * spriteX + planeX * spriteY); //this is actually the depth inside the screen, that what Z is in 3D

        int spriteScreenX = int((w / 2) * (1 + transformX / transformY));

        //parameters for scaling and moving the sprites
#define uDiv 1
#define vDiv 1
#define vMove 0.0
        int vMoveScreen = int(vMove / transformY);

        //calculate height of the sprite on screen
        int spriteHeight = abs(int(h / (transformY))) / vDiv; //using "transformY" instead of the real distance prevents fisheye
        //calculate lowest and highest pixel to fill in current stripe
        int drawStartY = -spriteHeight / 2 + h / 2 + vMoveScreen;
        if (drawStartY < 0) drawStartY = 0;
        int drawEndY = spriteHeight / 2 + h / 2 + vMoveScreen;
        if (drawEndY >= h) drawEndY = h - 1;

        //calculate width of the sprite
        int spriteWidth = abs( int (h / (transformY))) / uDiv;
        int drawStartX = -spriteWidth / 2 + spriteScreenX;
        if (drawStartX < 0) drawStartX = 0;
        int drawEndX = spriteWidth / 2 + spriteScreenX;
        if (drawEndX >= w) drawEndX = w - 1;

        //loop through every vertical stripe of the sprite on screen
        for (int stripe = drawStartX; stripe < drawEndX; stripe++)
        {
            int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * texWidth / spriteWidth) / 256;
            //the conditions in the if are:
            //1) it's in front of camera plane so you don't see things behind you
            //2) it's on the screen (left)
            //3) it's on the screen (right)
            //4) ZBuffer, with perpendicular distance
            if (transformY > 0 && stripe > 0 && stripe < w && transformY < ZBuffer[stripe])
                for (int y = drawStartY; y < drawEndY; y++) //for every pixel of the current stripe
                {
                    int d = (y-vMoveScreen) * 256 - h * 128 + spriteHeight * 128; //256 and 128 factors to avoid floats
                    int texY = ((d * texHeight) / spriteHeight) / 256;
                    Uint32 color = texture[sprite[spriteOrder[i]].texture][texWidth * texY + texX]; //get current color from the texture
                    if ((color & 0x00FFFFFF) != 0)
                    {
                        buffer[stripe][y] = color; //paint pixel if it isn't black, black is the invisible color

                    }
                }
        }
    }


    drawBuffer(buffer[0]);

    switch (backdrop)
    {
    case bg_outside:
    {

        for (int y = 0; y < 120; y++)
        {
            for (int x = 0; x < w; x++)
            {
                buffer[x][y] = dim(RGBtoINT(ColorRGB(133, 192, 197)),1.0);
            }
        }
        for (int y = 120; y < 240; y++)
        {
            for (int x = 0; x < w; x++)
            {
                buffer[x][y] = dim(RGBtoINT(ColorRGB(0,0,0)),1.0);
            }
        }


    }
    break;
    case bg_outside2:
    {

        for (int y = 0; y < 120; y++)
        {
            for (int x = 0; x < w; x++)
            {
                buffer[x][y] = dim(RGBtoINT(ColorRGB(245, 207, 228)),1.0);
            }
        }
        for (int y = 120; y < 240; y++)
        {
            for (int x = 0; x < w; x++)
            {
                buffer[x][y] = dim(RGBtoINT(ColorRGB(0,0,0)),1.0);
            }
        }


    }
    break;
    case bg_black:
    {

        for (int y = 0; y < 240; y++)
        {
            for (int x = 0; x < w; x++)
            {
                buffer[x][y] = 0;
            }
        }
    }
    break;
    }

    //timing for input and FPS counter
    foldTime = ftime;
    ftime = getTicks();

    if (rendermode==rm_black){
      cls();
    }

//    tempstring=its(int(posX))+"," + its(int(posY)) + ", " + its(int(realdir*100));
//    print(tempstring);

    //Cutscene banners
    if(csbanners>0){
      drawRect(0, 0, 319, csbanners, RGB_Black);
      drawRect(0, 239-csbanners, 319, 239, RGB_Black);
    }

	if (game.textbox){
		drawRect(10, 230-(game.numlines*9)-3, 310, 230, RGB_Textbox);
		game.textincomplete=false;
		if (game.textmode==1 || game.textmode==2){
			textdisplaypara=0;
			for (int i=0; i<game.numlines; i++){
				for (int j=0; j<game.line[i].length(); j++){
					textdisplaypara++;
					if (textdisplaypara<=game.textpara){
						tempstring=game.line[i];
						tempstring2=tempstring[j];
						printcenter(tempstring2, 230-((game.numlines-i)*9), game.getcol(qglow/4), j, tempstring);
					}else{
						game.textincomplete=true;
					}
				}
			}
		}else if(game.textmode==4){
			//Option box
			drawRect(10, 220-((game.numlines)*9)-3, 310, 220, RGB_Textbox);
			printcenter(game.line[0], 220-((game.numlines-0)*9), game.gettextcol(qglow/4));
			if(game.currentoption==0){
				printcenter("[ " + game.line[1] + " ]", 220-((game.numlines-2)*9), game.getyescol(qglow/4));
				printcenter(game.line[2], 220-((game.numlines-3)*9), game.getnocol(qglow/4));
			}else{
				printcenter(game.line[1], 220-((game.numlines-2)*9), game.getnocol(qglow/4));
				printcenter("[ " + game.line[2] + " ]", 220-((game.numlines-3)*9), game.getyescol(qglow/4));
			}
		}else if(game.textmode==5){
			//Option box for single option thingy
			drawRect(10, 220-((game.numlines)*9)-3, 310, 220, RGB_Textbox);
			printcenter(game.line[0], 220-((game.numlines-0)*9), game.gettextcol(qglow/4));
			printcenter("[ " + game.line[1] + " ]", 220-((game.numlines-2)*9), game.getyescol(qglow/4));
		}else{
			for (int i=0; i<game.numlines; i++){
				printcenter(game.line[i], 230-((game.numlines-i)*9), game.getcol(qglow/4));
			}
		}
    }

    if(game.objecton>0 && game.controlstate==0){
      drawRect(10, 10, 310, 22, RGB_Textbox);
      printcenter(game.objectdes, 13, game.gettextcol(qglow/4), 0, game.objectdes);
    }

    redraw();
}
