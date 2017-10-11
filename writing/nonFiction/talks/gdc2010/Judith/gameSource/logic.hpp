void gamelogic(){
  if(game.controlstate>0){ cutscenemode=1; }else cutscenemode=0;
  if(cutscenemode==0){
    if(csbanners>0) csbanners-=4;
    if(csbanners<0) csbanners=0;
  }else if(cutscenemode==1){
    if(csbanners<10) csbanners+=4;
    if(csbanners>10) csbanners=10;
  }

  process_glow();
  game.updatestate();
  game.checktrigger(int(posX), int(posY));

}
