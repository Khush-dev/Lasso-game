#include <simplecpp>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include<time.h>
using namespace simplecpp;

//movingObject.h
#ifndef _MOVINGOBJECT_INCLUDED_
#define _MOVINGOBJECT_INCLUDED_

#include <simplecpp>
#include <vector>
#include <composite.h>
#include <sprite.h>

using namespace simplecpp;

class MovingObject : public Sprite {
  vector<Sprite*> parts;
  double vx, vy;
  double ax, ay;
  bool paused;
  void initMO(double argvx, double argvy, double argax, double argay, bool argpaused=true) {
    vx=argvx; vy=argvy; ax=argax; ay=argay; paused=argpaused;
  }
 public:
 MovingObject(double argvx, double argvy, double argax, double argay, bool argpaused=true)
    : Sprite() {
    initMO(argvx, argvy, argax, argay, argpaused);
  }
 MovingObject(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : Sprite() {
   double angle_rad = angle_deg*PI/180.0;
   double argvx = speed*cos(angle_rad);
   double argvy = -speed*sin(angle_rad);
   initMO(argvx, argvy, argax, argay, argpaused);
  }
  void set_vx(double argvx) { vx = argvx; }
  void set_vy(double argvy) { vy = argvy; }
  void set_ax(double argax) { ax = argax; }
  void set_ay(double argay) { ay = argay; }
  double getXPos();
  double getYPos();
  void reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta);

  void pause() { paused = true; }
  void unpause() { paused = false; }
  bool isPaused() { return paused; }

  void addPart(Sprite* p) {
    parts.push_back(p);
  }
  void nextStep(double t);
  void getAttachedTo(MovingObject *m);
};

#endif

//MovingObject.cpp

void MovingObject::nextStep(double t) {
  if(paused) { return; }
  //cerr << "x=" << getXPos() << ",y=" << getYPos() << endl;
  //cerr << "vx=" << vx << ",vy=" << vy << endl;
  //cerr << "ax=" << ax << ",ay=" << ay << endl;

  for(size_t i=0; i<parts.size(); i++){
    parts[i]->move(vx*t, vy*t);
  }
  vx += ax*t;
  vy += ay*t;
} // End MovingObject::nextStep()

double MovingObject::getXPos() {
  return (parts.size() > 0) ? parts[0]->getX() : -1;
}

double MovingObject::getYPos() {
  return (parts.size() > 0) ? parts[0]->getY() : -1;
}

void MovingObject::reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) {
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(argx, argy);
  }
  double angle_rad = angle_deg*PI/180.0;
  double argvx = speed*cos(angle_rad);
  double argvy = -speed*sin(angle_rad);
  vx = argvx; vy = argvy; ax = argax; ay = argay; paused = argpaused;
} // End MovingObject::reset_all()

void MovingObject::getAttachedTo(MovingObject *m) {
  double xpos = m->getXPos();
  double ypos = m->getYPos();
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(xpos, ypos);
  }
  initMO(m->vx, m->vy, m->ax, m->ay, m->paused);
}

//coin.h
#ifndef __COIN_H__
#define __COIN_H__



class Coin : public MovingObject {
  double coin_start_x;
  double coin_start_y;
  double release_speed;
  double release_angle_deg;
  double coin_ax;
  double coin_ay;
  bool magnet, is_bomb;
  int Id;
  // Moving parts
  Circle coin_circle;

 public:


 Coin(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta, int id, bool mag=false, bool bomb= false) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    coin_ax = argax;
    coin_ay = argay;
    Id = id;
    magnet = mag;
    is_bomb = bomb;
    initCoin();
  }
  int getID(){return Id;}
  bool getMag(){return magnet;}
  bool getBomb(){return is_bomb;}
  void initCoin();
  void resetCoin();
  void hide();
  void unhide();
  void magnetize();
  void demagnetize();
  void ignite(); // TURNS THE COIN INTO A BOMB
  void deignite(); //TURNS IT BACK TO A NORMAL COIN


}; // End class Coin

#endif

//lasso.h
#ifndef __LASSO_H__
#define __LASSO_H__

//#define WINDOW_X 1370
//#define WINDOW_Y 750
#define WINDOW_X 800
#define WINDOW_Y 600

#define STEP_TIME 0.05

#define PLAY_X_START 100
#define PLAY_Y_START 0
#define PLAY_X_WIDTH (WINDOW_X-PLAY_X_START)
#define PLAY_Y_HEIGHT (WINDOW_Y-100)

#define LASSO_X_OFFSET 100
#define LASSO_Y_HEIGHT 100
#define LASSO_BAND_LENGTH LASSO_X_OFFSET
#define LASSO_THICKNESS 5

#define COIN_GAP 1

#define RELEASE_ANGLE_STEP_DEG 5
#define MIN_RELEASE_ANGLE_DEG 0
#define MAX_RELEASE_ANGLE_DEG (360-RELEASE_ANGLE_STEP_DEG)
#define INIT_RELEASE_ANGLE_DEG 45

#define RELEASE_SPEED_STEP 20
#define MIN_RELEASE_SPEED 0
#define MAX_RELEASE_SPEED 200
#define INIT_RELEASE_SPEED 100

#define COIN_SPEED 120
#define COIN_ANGLE_DEG 85

#define LASSO_G 30
#define COIN_G 30

#define LASSO_SIZE 10
#define LASSO_RADIUS 50
#define COIN_SIZE 5

#define n 3 //number of coins
#define INIT_LIVES 5        //INITIAL NUMBER OF LIVES
#define MAG_STR 2
#define MAG_TIME 5

bool i_coin_vis[3] = {true,false,false};
bool updaterq = false;
bool is_spikes = false;
int lvl = 1;
vector <string> Message;

string getTip(){
  string tips[] = {"Stay away from bombs!", "Time your throw well", "Don't forget to yank ;) ",
  "A Magnet lasts for 5 seconds |=_=|  ", "Beware of spikes! |=_*| ", "Red coins are Magnetic '-' ", "Can't really help with the lag :[ " };
  int randm = rand()%(sizeof(tips)/sizeof(tips[0])) ;
  return tips[randm];
}

void fire(){
  for(int i = 0; i<10; i++){  //ANIMATION
    Rectangle fire(WINDOW_X/4+randuv(20, 200),PLAY_Y_HEIGHT+5,10,10);
    fire.setFill(true);
    fire.setColor(COLOR("red"));
  }
  is_spikes = true;
}

void nextLvl(){
  lvl++;
  Rectangle new_lvl(WINDOW_X/2, WINDOW_Y/2,WINDOW_X/4, WINDOW_Y/4 );
  new_lvl.setFill(true);
  new_lvl.setColor(COLOR("white"));
  Text lvltxt(WINDOW_X/2, WINDOW_Y/2 - 20, "New Level!"),
  lvltxt1(WINDOW_X/2, WINDOW_Y/2, "GG.. ^-^ "),
  lvltxt2(WINDOW_X/2, WINDOW_Y/2 + 60, "Click to exit."),
  lvltxt3(WINDOW_X/2, WINDOW_Y/2 + 20, "");
  if(lvl==2){
    fire();
    lvltxt3.setMessage("The Fire is your enemy");
    fire();
  }
  getClick();

}



class Lasso : public MovingObject {
  double lasso_start_x;
  double lasso_start_y;
  double release_speed;
  double release_angle_deg;
  double lasso_ax;
  double lasso_ay;

  // Moving parts
  Circle lasso_circle;
  Circle lasso_loop;

  // Non-moving parts
  Line lasso_line;
  Line lasso_band;

  // State info
  bool lasso_looped;
  bool is_magnetic;
  float mag_timer;
  int lasso_r;

  Coin *the_coin[n];
  int num_coins;
  int no_of_lives;

  void initLasso();
 public:
 Lasso(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    lasso_ax = argax;
    lasso_ay = argay;
    initLasso();
  }

  void draw_lasso_band();
  void yank();
  void loopit();
  void addAngle(double angle_deg);
  void addSpeed(double speed);

  void nextStep(double t);
  void check_for_coin(Coin *coin);
  int getNumCoins() { return num_coins; }       //SCORE
  int getNumLives() {
    return no_of_lives;
  }
  bool get_is_magnetic(){ return is_magnetic; }
  void make_mag();
  void de_mag();
  float getMagTimer(){ return mag_timer; }

}; // End class Lasso

#endif

//coin.h

void Coin::initCoin() {
  coin_start_x = (PLAY_X_START+WINDOW_X)/2 + rand()%50;     //SPAWNING RANDOMLY NEAR THE CENTRE OF X AXES
  coin_start_y = PLAY_Y_HEIGHT;
  coin_circle.reset(coin_start_x, coin_start_y, COIN_SIZE);
  coin_circle.setColor(COLOR("gold"));     //COIN COLOUR
  coin_circle.setFill(true);
  addPart(&coin_circle);
}

void Coin::resetCoin() {
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = COIN_ANGLE_DEG;
  coin_ax = 0;
  coin_ay = COIN_G;
  bool paused = true, rtheta = true;
  reset_all(coin_start_x, coin_start_y, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);

}

void Coin::hide(){
  reset_all(PLAY_X_START+LASSO_X_OFFSET, PLAY_Y_HEIGHT-WINDOW_Y, 0,0,0,0, true, true);
  demagnetize();
  coin_circle.setColor(COLOR("white"));
  coin_circle.setFill(false);
}

void Coin::unhide(){
  coin_circle.setColor(COLOR("gold"));
  coin_circle.setFill(true);
  resetCoin();
  unpause();
}

void Coin::magnetize(){         //COLLECTING MAGNETIZED COINS INCREASES LASSO RADIUS FOR SOME TIME
  magnet = true;
  coin_circle.setColor(COLOR("red"));
}

void Coin::demagnetize(){
  magnet = false;
  coin_circle.setColor(COLOR("gold"));
}

void Coin::ignite(){
  is_bomb = true;
  coin_circle.setColor(COLOR("blue"));
}

void Coin::deignite(){
  is_bomb = false;
  coin_circle.setColor(COLOR("gold"));
}

//lasso.cpp

void Lasso::draw_lasso_band() {
  double len = (release_speed/MAX_RELEASE_SPEED)*LASSO_BAND_LENGTH;         //LENGTH PROPORTIONAL TO SPEED
  double arad = release_angle_deg*PI/180.0;
  double xlen = len*cos(arad);
  double ylen = len*sin(arad);
  lasso_band.reset(lasso_start_x, lasso_start_y, (lasso_start_x-xlen), (lasso_start_y+ylen));
  lasso_band.setThickness(LASSO_THICKNESS);
} // End Lasso::draw_lasso_band()

void Lasso::initLasso() {
  lasso_start_x = (PLAY_X_START+LASSO_X_OFFSET);
  lasso_start_y = (PLAY_Y_HEIGHT-LASSO_Y_HEIGHT);
  lasso_circle.reset(lasso_start_x, lasso_start_y, LASSO_SIZE);
  lasso_circle.setColor(COLOR("red"));
  lasso_circle.setFill(true);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setColor(COLOR("yellow"));
  lasso_loop.setFill(true);
  addPart(&lasso_circle);
  addPart(&lasso_loop);
  lasso_looped = false;
  is_magnetic = false;
  mag_timer = MAG_TIME;
  lasso_r = LASSO_RADIUS;
  for(int i = 0; i<n;i++)
    the_coin[i] = NULL;
  num_coins = 0;
  no_of_lives = INIT_LIVES;

  lasso_line.reset(lasso_start_x, lasso_start_y, lasso_start_x, lasso_start_y);
  lasso_line.setColor(COLOR("yellow"));

  lasso_band.setColor(COLOR("blue"));
  draw_lasso_band();

} // End Lasso::initLasso()

void Lasso::yank() {        //PULLS THE LASSO BACK AND RESETS THE COINS COLLECTED/ATTACHED, IF ANY. ADDS LIFE FOR MORE THAN ONE COIN
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setFill(true);
  lasso_looped = false;
  int temp = num_coins;
  for(int i = 0; i<n;i++)
    if(the_coin[i] != NULL) {

        if(the_coin[i]->getBomb()){
          if(no_of_lives==1){
          Message.insert(Message.begin(), "Game Over.");
          getClick();
          exit(0);
          }
          no_of_lives--;
          Message.insert(Message.begin(), "You lost a life! :( ");
        }
        else{ num_coins++;
        if(num_coins%5==0)nextLvl();
        }
        updaterq = true;

        if(!i_coin_vis[i])
          the_coin[i]->hide();
        else
          the_coin[i]->resetCoin();
        the_coin[i] = NULL;
      }
  no_of_lives += num_coins-temp ? num_coins-temp-1 : 0;//ADDS LIFE IF WE COLLECT MORE THAN ONE COINS
} // End Lasso::yank()

void Lasso::loopit() {
  if(lasso_looped) { return; } // Already looped

  lasso_loop.reset(getXPos(), getYPos(), lasso_r);
  lasso_loop.setFill(false);
  lasso_looped = true;
} // End Lasso::loopit()

void Lasso::addAngle(double angle_deg) {
  release_angle_deg += angle_deg;
  if(release_angle_deg < MIN_RELEASE_ANGLE_DEG) { release_angle_deg = MIN_RELEASE_ANGLE_DEG; }
  if(release_angle_deg > MAX_RELEASE_ANGLE_DEG) { release_angle_deg = MAX_RELEASE_ANGLE_DEG; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addAngle()

void Lasso::addSpeed(double speed) {
  release_speed += speed;
  if(release_speed < MIN_RELEASE_SPEED) { release_speed = MIN_RELEASE_SPEED; }
  if(release_speed > MAX_RELEASE_SPEED) { release_speed = MAX_RELEASE_SPEED; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addSpeed()

void Lasso::nextStep(double stepTime) {
  draw_lasso_band();
  MovingObject::nextStep(stepTime);
  if(getYPos() > PLAY_Y_HEIGHT) { yank(); }
  lasso_line.reset(lasso_start_x, lasso_start_y, getXPos(), getYPos());
  if(is_magnetic){
    mag_timer += stepTime;
    if(mag_timer>MAG_TIME){de_mag();}
  }
} // End Lasso::nextStep()

void Lasso::check_for_coin(Coin *coinPtr) {//CHECKS FOR NEARBY COINS AND APPLIES SPECIAL PROPERTIES, IF ANY
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double coin_x = coinPtr->getXPos();
  double coin_y = coinPtr->getYPos();
  double xdiff = (lasso_x - coin_x);
  double ydiff = (lasso_y - coin_y);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
  int coin_ID = coinPtr->getID();

  if( distance <= lasso_r && !(coinPtr->isPaused()) )  {
    the_coin[coin_ID-1] = coinPtr;
    the_coin[coin_ID-1]->getAttachedTo(this);
    if(coinPtr->getMag()) make_mag();
  }
  Circle flash_radius(lasso_x, lasso_y, lasso_r);
  wait(0.2);
} // End Lasso::check_for_coin()

void Lasso::make_mag(){//ATTACHES MAGNET TO THE LASSO
  is_magnetic = true;
  mag_timer = 0;
  lasso_r = MAG_STR*LASSO_RADIUS;
  if(lasso_looped){
   lasso_loop.reset(getXPos(), getYPos(), lasso_r);
  }
  Message.insert(Message.begin(), "Magnet: ON");
  updaterq = true;
}

void Lasso::de_mag(){//REMOVES THE ATTACHED MAGNET
  is_magnetic = false;
  lasso_r = LASSO_RADIUS;
  if(lasso_looped){
   lasso_loop.reset(getXPos(), getYPos(), lasso_r);
  }
  Message.insert(Message.begin(), "Magnet: OFF");
  updaterq = true;
}
//End lasso.cpp

void show_help(){
  Rectangle help_background(WINDOW_X/2, WINDOW_Y/2,WINDOW_X/2, WINDOW_Y/2 );
  help_background.setFill(true);
  help_background.setColor(COLOR("white"));
  Text help(WINDOW_X/2, WINDOW_Y/4 + 20, "KEY:"),
  h1(WINDOW_X/2, WINDOW_Y/4 + 60, "'t' : Throws the lasso.   'y' : Yanks the lasso."),
  h2(WINDOW_X/2, WINDOW_Y/4 + 90, "'l' : Loops the lasso.   (Checks for Coins)"),
  h3(WINDOW_X/2, WINDOW_Y/4 + 120, "'=' : Increases throw speed.   '-' : Decreases throw speed."),
  h4(WINDOW_X/2, WINDOW_Y/4 + 150, "']' : Increases throw angle.   '[' : Decreases throw angle."),
  h5(WINDOW_X/2, WINDOW_Y/4 + 180, "'q' : Quits the Game."),
  h6(WINDOW_X/2, 3*WINDOW_Y/4 - 40, "Click to exit Help Menu.");

  getClick();
}

main_program {

  initCanvas("Lasso", WINDOW_X, WINDOW_Y);
  Rectangle background(WINDOW_X/2, WINDOW_Y/2,WINDOW_X, WINDOW_Y );
  background.setFill(true);
  background.setColor(COLOR("black"));


  int stepCount = 0;
  float stepTime = STEP_TIME;
  float runTime = -1; // sec; -ve means infinite
  float currTime = 0;
  simplecpp::srand(time(0)%1000);

  // Draw lasso at start position
  double release_speed = INIT_RELEASE_SPEED; // m/s
  double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // degrees
  double lasso_ax = 0;
  double lasso_ay = LASSO_G;
  bool paused = true;
  bool rtheta = true;
  Lasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);

  Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
  b1.setColor(COLOR("white"));
  Line b2(PLAY_X_START, 0, PLAY_X_START, WINDOW_Y);
  b2.setColor(COLOR("white"));

  string msg("Cmd: _");
  Text charPressed(PLAY_X_START+50, PLAY_Y_HEIGHT+20, msg);
  char coinScoreStr[256], nlife[256], mag_time_str[256]={}, nlvl[256];
  sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
  sprintf(nlife, "Lives: %d", lasso.getNumLives());
  sprintf(nlvl, "Level: %d", lvl);
  //sprintf(mag_time_str, "", MAG_TIME - int(lasso.getMagTimer())  );
  Text coinScore(PLAY_X_START+50, PLAY_Y_HEIGHT+50, coinScoreStr);
  Text lives(WINDOW_X-100, 20, nlife), mag_Time(WINDOW_X-100, 60, mag_time_str);
  Text level(WINDOW_X-100, 40, nlvl);

  Message.insert(Message.begin(), "WELCOME!");
  Message.insert(Message.begin(), "Best of Luck!! ");
  Message.insert(Message.begin(), "TIP: " + getTip());
  Text flash_msg1(WINDOW_X-120, PLAY_Y_HEIGHT+20, Message.at(0));
  Text flash_msg2(WINDOW_X-120, PLAY_Y_HEIGHT+40, Message.at(1));
  Text flash_msg3(WINDOW_X-120, PLAY_Y_HEIGHT+60, Message.at(2));

  show_help();

  paused = true; rtheta = true;
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = COIN_ANGLE_DEG;
  double coin_ax = 0;
  double coin_ay = COIN_G;

  Coin coin(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta,1);
  Coin coin2(coin_speed, coin_angle_deg+10, coin_ax, coin_ay, paused, rtheta,2);
  coin2.hide();
  Coin coin3(coin_speed, coin_angle_deg+10, coin_ax, coin_ay, paused, rtheta,3);
  coin3.hide();

  // After every COIN_GAP sec, make the first coin jump
  double last_coin_jump_end[n] = {};  //INITIALISING


  // When t is pressed, throw lasso
  // If lasso within range, make coin stick
  // When y is pressed, yank lasso
  // When l is pressed, loop lasso
  // When q is pressed, quit

  for(;;) {
    if((runTime > 0) && (currTime > runTime)) { break; }

    XEvent e;
    bool pendingEv = checkEvent(e);
    if(pendingEv) {
      char c = charFromEvent(e);
      msg[msg.length()-1] = c;
      charPressed.setMessage(msg);
      switch(c) {
      case 't':
	lasso.unpause();
	break;
      case 'y':
	lasso.yank();
	break;
      case 'l':
	lasso.loopit();
    lasso.check_for_coin(&coin);
    lasso.check_for_coin(&coin2);
    lasso.check_for_coin(&coin3);
	wait(STEP_TIME*5);
	break;
      case '[':
	if(lasso.isPaused()) { lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);	}
	break;
      case ']':
	if(lasso.isPaused()) { lasso.addAngle(+RELEASE_ANGLE_STEP_DEG); }
	break;
      case '-':
	if(lasso.isPaused()) { lasso.addSpeed(-RELEASE_SPEED_STEP); }
	break;
      case '=':
	if(lasso.isPaused()) { lasso.addSpeed(+RELEASE_SPEED_STEP); }
	break;
      case 'q':
	exit(0);
      default:
	break;
      }
    }

    lasso.nextStep(stepTime);
//UPDATING COINS
    coin.nextStep(stepTime);
    if(coin.isPaused()) {
      if((currTime-last_coin_jump_end[0]) >= COIN_GAP) {
	coin.unpause();
      }
    }

    if(coin.getYPos() > PLAY_Y_HEIGHT) {
      coin.resetCoin();
      last_coin_jump_end[0] = currTime;
    }

    coin2.nextStep(stepTime);
    if(coin2.isPaused()) {
      if((currTime-last_coin_jump_end[1]) >= COIN_GAP) {
        if(rand()%100==5){      //RANDOMLY POPPING COIN...
        coin2.unhide();
        if(rand()%4==1){
          coin2.magnetize();
        }
        else if(rand()%4==1){
          coin2.ignite();
        }
        }
      }
    }

    if(coin2.getYPos() > PLAY_Y_HEIGHT) {
      coin2.hide();
      last_coin_jump_end[1] = currTime;
      if(coin2.getMag()){
        coin2.demagnetize();
      }
      if(coin2.getBomb()){
        coin2.deignite();
      }
    }

    coin3.nextStep(stepTime);
    if(coin3.isPaused()) {
      if((currTime-last_coin_jump_end[1]) >= COIN_GAP) {
        if(rand()%100==5){      //RANDOMLY POPPING COIN
        coin3.unhide();
        if(rand()%4==1){
          coin3.magnetize();
        }
        else if(rand()%4==1){
          coin3.ignite();
        }
        }
      }
    }

    if(coin3.getYPos() > PLAY_Y_HEIGHT) {
      coin3.hide();
      last_coin_jump_end[2] = currTime;
      if(coin3.getMag()){
        coin3.demagnetize();
      }
      if(coin3.getBomb()){
        coin3.deignite();
      }
    }


    if(lasso.get_is_magnetic()){
      sprintf(mag_time_str, "Magnet: %d seconds left.", MAG_TIME - int(lasso.getMagTimer()) );
      mag_Time.setMessage(mag_time_str);
    }
    else{
      mag_Time.setMessage("");
    }

    if(updaterq){   //ATTEMPT TO REDUCE LAG, INCREASE EFFICIENCY
    sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
    coinScore.setMessage(coinScoreStr);

    sprintf(nlife, "Lives: %d", lasso.getNumLives());
    lives.setMessage(nlife);

    sprintf(nlvl, "Level: %d", lvl);
    level.setMessage(nlvl);

    Message.resize(3);
    flash_msg1.setMessage(Message.at(0));
    flash_msg2.setMessage(Message.at(1));
    flash_msg3.setMessage(Message.at(2));

    updaterq = false;
    }

    stepCount++;
    currTime += stepTime;
    //wait(stepTime); //COMPUTATION FOR SCREEN REFRESHING CAUSES ENOUGH TIME LAG (BASICALLY, COMMENTING THIS OUT REDUCES THE UNNECCESSARY MANUAL LAG)
  } // End for(;;)

  wait(3);
} // End main_program
