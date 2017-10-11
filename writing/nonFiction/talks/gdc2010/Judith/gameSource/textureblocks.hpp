#include <string>

struct TextureBlock {
         int a;
         int b;
         string description;
         };

const int blockcount=69;

const TextureBlock blocks[blockcount] =
{
{1,1,"castle walls"},                  // 0
{2,2,"castle walls dec1"},             // 1
{3,3,"castle walls dec2"},             // 2
{4,4,"castle walls dec3"},             // 3
{6,6,"black"},                         // 4
{7,7,"banister 1"},                    // 5
{8,8,"banister 2"},                    // 6
{9,9,"door"},                          // 7
{10,10,"locked door"},                 // 8
{11,11,"torture walls 1"},             // 9
{12,12,"torture walls 2"},             // 10
{13,13,"torture walls 3"},             // 11
{14,14,"torture walls 4"},             // 12
{15,15,"torture walls 5"},             // 13
{16,16,"torture walls 6"},             // 14
{17,17,"armory wall"},                 // 15
{19,19,"armory wall with weapons 1"},  // 16
{20,20,"treasure wall1"},              // 17
{21,21,"treasure wall2"},              // 18
{22,22,"treasure wall3"},              // 19
{25,25,"hedge"},                       // 20
{26,26,"garden wall 1"},               // 21
{27,27,"garden wall 1 nograss"},       // 22
{28,28,"garden wall 2"},               // 23
{29,29,"garden wall 2 nograss"},       // 24
{30,30,"garden transition 1"},         // 25
{30,30,"garden transition 2"},         // 26
{43,43,"garden transition 3"},         // 27
{44,44,"garden transition 4"},         // 28
{33,33,"garden hedge"},                // 29
{36,36,"lake wall 1"},                 // 30
{76,76,"bedroom wall"},                // 31
{9,76,"open door vert"},               // 32
{76,9,"open door horizontal"},         // 33
{9,17,"armory open door vert"},        // 34
{17,9,"armory open door horizontal"},  // 35
{26,27,"garden wall 1 nograss vert"},  // 36
{27,26,"garden wall 1 nograss hor"},   // 37
{80,80,"bookcase 1"},			           	// 38
{81,81,"bookcase 2"},				          // 39
{86,86,"armory wall with weapons 2"},  // 40
{87,87,"hedge path"},				          // 41
{88,88,"lake wall 2"},   			        // 42
{89,91,"bed 1"},						            // 43
{90,92,"bed 2"},				            		// 44
{93,93,"fireplace"},					          // 45
{68,68,"treasury door"},   	          // 46
{69,69,"torture door"},   	            // 47
{70,70,"armory door"},   	            // 48
{71,71,"garden door"},      	          // 49
{72,72,"kingdom door"},    	          // 50
{73,73,"lake door"},       	          // 51
{74,74,"final door"},      	          // 52
{106,106,"half-way gate"},      	          // 53
{99,68,"treasury door locked"},   	          // 54
{100,100,"torture door locked"},   	            // 55
{101,101,"armory door locked"},   	            // 56
{102,102,"garden door locked"},      	          // 57
{103,103,"kingdom door locked"},    	          // 58
{104,104,"lake door locked"},       	          // 59
{105,105,"final door locked"},      	          // 60
{68,68,"treasury door open"},   	          // 61
{69,69,"torture door open"},   	            // 62
{70,17,"armory door open"},   	            // 63
{71,1,"garden door open"},      	          // 64
{26,72,"kingdom door open"},    	          // 65
{88,73,"lake door open"},       	          // 66
{74,1,"final door open"},      	          // 67
{107,76,"bedroom clock"}				//68
};

struct tiledata {
                int i;
                string s;
       };

const int floorindexcount=21;

tiledata floorindices[floorindexcount]={
        {-1,"none"},                       // 0
        {0,"castle floor"},                // 1
        {6,"black walkable"},              // 2
        {18,"armoury floor"},              // 3
        {23,"grass"},                      // 4
        {24,"flower bed"},                 // 5
        {32,"back garden floor"},          // 6
        {34,"back garden grass"},          // 7
        {35,"balcony floor"},              // 8
        {37,"lake water"},                 // 9
        {77,"bedroom floor"},              // 10
        {77,"treasury floor 1"},           // 11
        {82,"treasury floor 2"},           // 12
        {83,"torture floor 1"},            // 13
		{84,"torture floor 2"},		       // 14
		{85,"torture floor 3"},            // 15
		{94,"floor by fireplace1"},        // 16
		{95,"floor by fireplace2"},        // 17
		{96,"floor by fireplace3"},        // 18
		{97,"floor by fireplace4"},        // 19
		{108,"black impassible"},            // 20

};

const int ceilingindexcount=5;

tiledata ceilingindices[ceilingindexcount]={
    {-1,"none"},                         // 0
    {5,"castle ceiling"},                // 1
    {6,"black"},                         // 2
    {78,"bedroom ceiling"},              // 3
    {75,"stormcloud"},                   // 4
};

const int texturecount=109;
  char* texturenames[texturecount]= {
  //main castle
  "data/graphics/castle_floor.png",              // 0
  "data/graphics/castle_walls.png",              // 1
  "data/graphics/castle_dec1.png",               // 2
  "data/graphics/castle_dec2.png",               // 3
  "data/graphics/castle_dec3.png",               // 4
  "data/graphics/castle_ceiling.png",            // 5
  "data/graphics/black.png",                     // 6
  "data/graphics/banister1.png",                 // 7
  "data/graphics/banister2.png",                 // 8
  "data/graphics/door.png",                      // 9
  "data/graphics/door_locked.png",                // 10
  //room 1
  "data/graphics/torture_walls1.png",            // 11
  "data/graphics/torture_walls2.png",            // 12
  "data/graphics/torture_walls3.png",            // 13
  "data/graphics/torture_walls4.png",            // 14
  "data/graphics/torture_devicewall1.png",       // 15
  "data/graphics/torture_devicewall2.png",       // 16
  //room 2
  "data/graphics/armory_wall.png",               // 17
  "data/graphics/armory_floor.png",              // 18
  "data/graphics/armory_wall_with_weapons.png",  // 19
  //room 3
  "data/graphics/treasury_wall.png",             // 20
  "data/graphics/treasury_dec1.png",             // 21
  "data/graphics/treasury_dec2.png",             // 22
  //room 4
  "data/graphics/grass_floor.png",               // 23
  "data/graphics/flowerbed.png",                 // 24
  "data/graphics/hedge.png",                     // 25
  "data/graphics/garden_wall.png",               // 26
  "data/graphics/garden_wall_nograss.png",       // 27
  "data/graphics/garden_wall2.png",              // 28
  "data/graphics/garden_wall2_nograss.png",      // 29
  //room 4 back
  "data/graphics/garden_back_transition1.png",   // 30
  "data/graphics/garden_back_transition2.png",   // 31
  "data/graphics/floor.png",                     // 32
  "data/graphics/garden_back1.png",              // 33
  "data/graphics/garden_back_grass.png",         // 34
  //room 5
  "data/graphics/balcony_floor.png",             // 35
  //room 6
  "data/graphics/lake_wall.png",                 // 36
  "data/graphics/lake_water1.png",               // 37
  "data/graphics/lake_water2.png",               // 38
  "data/graphics/lake_water3.png",               // 39
  //New
  "data/graphics/castle_ceiling_light.png",      // 40
  "data/graphics/castle_floor_light.png",        // 41
  "data/graphics/castle_walls_light.png",        // 42
  "data/graphics/garden_back_transition1a.png",  // 43
  "data/graphics/garden_back_transition2a.png",  // 44
  //Sprites
  "data/graphics/emily1.png",					           // 45
  "data/graphics/emily2.png",					           // 46
  "data/graphics/flower4.png",                   // 47
  "data/graphics/fountain.png",                  // 48
  "data/graphics/flower6.png",                   // 49
  "data/graphics/bluebeard.png",                 // 50
  "data/graphics/hangingman.png",                // 51
  "data/graphics/treasury_gold1.png",            // 52
  "data/graphics/treasury_gold2.png",            // 53
  "data/graphics/treasury_gold3.png",            // 54
  "data/graphics/treasury_gold4.png",            // 55
  "data/graphics/treasury_gold5.png",            // 56
  "data/graphics/treasury_gold6.png",            // 57
  "data/graphics/treasury_gold7.png",            // 58
  "data/graphics/treasury_gold8.png",            // 59
  "data/graphics/desk.png",						           // 60
  "data/graphics/necklace.png",                  // 61
  "data/graphics/shovel.png",                    // 62
  "data/graphics/grave1.png",                    // 63
  "data/graphics/grave2.png",                    // 64
  "data/graphics/hangingmandead.png",            // 65
  "data/graphics/dagger.png",                    // 66
  "data/graphics/dagger2.png",                   // 67
  "data/graphics/door_1.png",                    // 68
  "data/graphics/door_2.png",                    // 69
  "data/graphics/door_3.png",                    // 70
  "data/graphics/door_4.png",                    // 71
  "data/graphics/door_5.png",                    // 72
  "data/graphics/door_6.png",                    // 73
  "data/graphics/door_7.png",                    // 74
  "data/graphics/stormcloud.png",                // 75
  //Bedroom stuff && misc textures
  "data/graphics/bedroompaper.png",              // 76
  "data/graphics/floor.png",                     // 77
  "data/graphics/ceiling.png",                   // 78
  "data/graphics/treasure_floor.png",            // 79
  "data/graphics/bookcase.png",					         // 80
  "data/graphics/bookcase2.png",					       // 81
  "data/graphics/treasury_floor.png",            // 82
  "data/graphics/torture_floor1.png",            // 83
  "data/graphics/torture_floor2.png",            // 84
  "data/graphics/torture_floor3.png",            // 85
  "data/graphics/armory_wall_with_weapons2.png", // 86
  "data/graphics/hedgepath.png",				 	       // 87
  "data/graphics/lake_wall2.png",					       // 88
  "data/graphics/bedend.png",				          	 // 89
  "data/graphics/bedend2.png",						       // 90
  "data/graphics/bedside1.png",						       // 91
  "data/graphics/bedside2.png",						       // 92
  "data/graphics/fireplace.png",					       // 93
  "data/graphics/floor_by_fireplace.png",			   // 94
  "data/graphics/floor_by_fireplace2.png",			 // 95
  "data/graphics/floor_by_fireplace3.png",			 // 96
  "data/graphics/floor_by_fireplace4.png",			 // 97
  "data/graphics/tree.png",			                 // 98
//More doors
"data/graphics/door_1_locked.png",//99
"data/graphics/door_2_locked.png",//100
"data/graphics/door_3_locked.png",//101
"data/graphics/door_4_locked.png",//102
"data/graphics/door_5_locked.png",//103
"data/graphics/door_6_locked.png",//104
"data/graphics/door_7_locked.png",//105
"data/graphics/gate.png",//106
"data/graphics/clock.png",//107
"data/graphics/black.png",//108
};


