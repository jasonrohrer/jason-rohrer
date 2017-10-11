int worldMap[200][200];
int floorMap[200][200];
int ceillingMap[200][200];

void loadmap(string filename){
  int xp, yp, t, n;
  string lne;
  string s = "data/maps/"+ filename+".map";
  char line[1000];

  ifstream file;
  file.open(s.c_str());
  if(!file){
    std::cout << "error loading images" << std::endl;
  }

  file >> mapWidth >> mapHeight;
  for(int j=0; j<mapHeight; j++){
    for(int i=0; i<mapWidth; i++){
      file >> worldMap[mapWidth-1-i][j];
    }
  }

  file >> t;
  for(int j=0; j<mapHeight; j++){
    for(int i=0; i<mapWidth; i++){
      file >> floorMap[mapWidth-1-i][j];
    }
  }

  file >> t;
  for(int j=0; j<mapHeight; j++){
    for(int i=0; i<mapWidth; i++){
      file >> ceillingMap[mapWidth-1-i][j];
    }
  }

  file.close();
}

