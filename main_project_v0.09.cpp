#include <simplecpp>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
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
  bool is_attached;
  // Moving parts
  Circle coin_circle;
  Line line1, line2;

 public:


 Coin(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta, bool mag=false, bool bomb= false) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    coin_ax = argax;
    coin_ay = argay;
    magnet = mag;
    is_bomb = bomb;
    initCoin();
  }
 Coin(): MovingObject(0,0,0,0,1,0){     //DEFAULT CONSTRUCTOR FOR MAKING ARRAYS
    initCoin();
    hide();
    magnet = is_bomb = false;
    is_attached = false;
 }
  bool getMag(){return magnet;}
  bool getBomb(){return is_bomb;}
  void initCoin();
  void resetCoin();
  void hide();      //HIDES THE COIN
  void hide_s();
  void unhide();    //RESETS THE COIN
  void unhide_s();     //UNHIDE IN SPACE MODE
  void magnetize();      //COLLECTING MAGNETIZED COINS INCREASES LASSO RADIUS FOR SOME TIME
  void demagnetize();      //DEMAGNETIZES THE COIN
  void ignite();    // TURNS THE COIN INTO A BOMB
  void deignite();      //TURNS IT BACK TO A NORMAL COIN
  int getY2(){return line2.getY();};    //RETURNS THE EXIT PORTAL Y COORDINATE
  bool getAttached(){return is_attached;}
  void getAttachedTo(MovingObject*);


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

#define COIN_SPEED 100
#define COIN_ANGLE_DEG 85

#define LASSO_G 30
#define COIN_G 30

#define LASSO_SIZE 10
#define LASSO_RADIUS 50
#define COIN_SIZE 5

#define n 5         //NUMBER OF COINS
//#define INIT_LIVES 5        //INITIAL NUMBER OF LIVES
//#define MAG_STR 2       //FACTOR BY WHICH THE RADIUS INCREASES
#define MAG_TIME 5      //TIME DURATION FOR MAGNETIC LASSO TO LAST
//#define NEXT_LVL_COINS 2        // NUMBER OF COINS REQUIRED TO GO THE NEXT LEVEL

//OTHER GLOBAL VARIABLES
// WITH INITIAL VALUES

bool updaterq = false;      //NOTIFICATION UPDATE REQUIRED OR NOT
bool is_spikes = false;     //SPIKES( I.E. FIRE/WATER) ON THE GROUND
int mode;         //GAME MODE ID: CLASSIC --> 1; SPACE --> 2;
bool dark_theme = 1;            // DARK/LIGHT THEMES, TRUE REFERS TO DARK THEME
int next_lvl_coins;         //NUMBER OF COINS REQUIRED TO GO THE NEXT LEVEL
int difficulty;         //DIFFICULTY LEVEL, EASY --> 1; MEDIUM --> 2; HARD -->3;
double mag_str;
int init_lives;
int bonus;
int coin_speed = COIN_SPEED;

vector <string> Message;        //VECTOR FOR NOTIFICATION IN RIGHT BOTTOM

void exit_game(int lvl, int score, int life);       //EXIT GAME AND SHOW GAME STATS
void show_help();       //SHOWS HELP DIALOGUE BOX

string getTip(){        //RETURNS A RANDOM TIP FOR THE USER
  string tips[] = {"Stay away from bombs!", "Time your throw well", "Don't forget to yank ;) ",
  "A Magnet lasts for 5 seconds |=_=|  ", "Beware of Bombs! |=_*| ", "Red coins are Magnetic '-' ",
   "Longer the portal, faster the coin" ,"Two coins in one throw earns a life!", "Space looks pretty in the dark", "Best of Luck ;) " };
  int randm = rand()%(sizeof(tips)/sizeof(tips[0])) ;
  return tips[randm];
}

void fire(){
  for(int i = 0; i<10; i++){  //FIRE ANIMATION
    Rectangle fire(WINDOW_X/4+randuv(0, WINDOW_X/2),PLAY_Y_HEIGHT+5,10,10);
    fire.setFill(true);
    fire.setColor(COLOR("red"));
  }
  is_spikes = true;
}

void rain(){
  for(int i = 0; i<10; i++){  //RAIN ANIMATION
    int x = WINDOW_X/4+randuv(0, WINDOW_X/2) ,y = randuv(20, PLAY_Y_HEIGHT-20);
    Line water(x,y,x,y+randuv(0,10));
    water.setColor(COLOR("blue"));
    wait(0.05);
  }
  is_spikes = true;
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
  bool is_magnetic;         //MAGNETIC STATUS
  float mag_timer;      //TIMER FOR MAGNET
  int lasso_r;      //LASSO RADIUS, AFTER CONSIDERING MAGNETIC PROPERTIES

  Coin *the_coin[n];        //ARRAY OF COIN POINTERS
  int num_coins;        //SCORE- COINS COLLECTED
  int no_of_lives;      //NUMBER OF LIVES
  int lvl = 1;        //LEVEL NUMBER

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
  void yank();      //PULLS THE LASSO BACK AND RESETS THE COINS COLLECTED/ATTACHED, IF ANY. ADDS LIFE FOR MORE THAN ONE COIN, COSTS LIVES FOR COLLECTING BOMBS
  void loopit();
  void addAngle(double angle_deg);
  void addSpeed(double speed);
  void set_ax_ay(double arg_ax,double arg_ay );

  void nextStep(double t);
  void check_for_coin(Coin *coin, int  coin_ID = -1);

  int getNumCoins() {return num_coins;}      //SCORE
  int getNumLives() {return no_of_lives;}    //LIVES

  bool get_is_magnetic(){return is_magnetic;}

  int getLvl() {return lvl;}     //GAME LEVEL

  void loselife();      //REDUCES LIFE AND UPDATES DATA
  void make_mag();      //ATTACHES MAGNET TO THE LASSO
  void de_mag();       //REMOVES THE ATTACHED MAGNET
  float getMagTimer(){ return mag_timer; }      //RETURNS TIME ELAPSED, SINCE LAST MAGNETIC COIN COLLECTED
  void nextLvl();

}; // End class Lasso

#endif

//coin.h

void Coin::initCoin() {
  if(mode == 1){
    coin_start_x = (PLAY_X_START+WINDOW_X)/2 + rand()%100;     //SPAWNING RANDOMLY NEAR THE CENTRE OF X AXES
    coin_start_y = PLAY_Y_HEIGHT;
  }
  else
    coin_start_x = coin_start_y = 0;
  coin_circle.reset(coin_start_x, coin_start_y, COIN_SIZE);
  addPart(&coin_circle);
  if(dark_theme){
    line1.setColor(COLOR("white"));
    line2.setColor(COLOR("white"));
  }
  else{
    line1.setColor(COLOR("black"));
    line2.setColor(COLOR("black"));
  }
}

void Coin::resetCoin() {
  double coin_speed_local = coin_speed + randuv(-30,30);
  double coin_angle_deg = COIN_ANGLE_DEG + randuv(-10,10);
  coin_ax = 0;
  coin_ay = COIN_G;
  bool paused = true, rtheta = true;
  reset_all(coin_start_x, coin_start_y, coin_speed_local, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
}

void Coin::hide(){
  reset_all(0,0, 0,0,0,0, true, true);      //PAUSED COINS CANNOT BE COLLECTED, NOR CAN THE LASSO REACH THERE
  if(dark_theme)
    coin_circle.setColor(COLOR("black"));       //BLENDS WITH BACKGROUND COLOR
  else
    coin_circle.setColor(COLOR("white"));
  if(magnet){
    demagnetize();
  }
  if(is_bomb){
    deignite();
  }
  is_attached = false;
}

void Coin::hide_s(){      //SPACE MODE
  reset_all(0,0, 0,0,0,0, true, true);
  if(dark_theme)
    coin_circle.setColor(COLOR("black"));       //BLENDS WITH BACKGROUND COLOR
  else
    coin_circle.setColor(COLOR("white"));
  line1.hide();
  line2.hide();
  if(magnet){
    demagnetize();
  }
  if(is_bomb){
    deignite();
  }
  is_attached = false;
}

void Coin::unhide(){
  coin_circle.setColor(COLOR(255,215,0));   //GOLDEN YELLOW
  coin_circle.setFill(true);
  resetCoin();
  unpause();
}

void Coin::unhide_s(){      //SPACE MODE, RESETS THE COIN
  int x, y1, y2, b, v;
  x = randuv(PLAY_X_START + 50, WINDOW_X-50);
  y1 = randuv(PLAY_Y_START, PLAY_Y_START + PLAY_Y_HEIGHT );
  y2 = randuv(PLAY_Y_START, PLAY_Y_START + PLAY_Y_HEIGHT-100 );
  if(abs(y1 - y2) < 20){ y2 += 100;}
  if(y2<y1)
      v = randuv(20, coin_speed);
  else
      v = -1*randuv(20, coin_speed);
  b = 2*abs(v)/3 + 25 ;        //LENGTH INCREASES WITH SPEED
  line1.reset(x-b/4, y1, x+b/4, y1);       //LINES REPRESENT THE PORTALS
  line2.reset(x-b/4, y2, x+b/4, y2);
  line1.show();
  line2.show();
  wait(0.03);       //FOR PORTAL OPENING ANIMATION
  line1.reset(x-b/4, y1, x+3*b/8, y1);
  line2.reset(x-b/4, y2, x+3*b/8, y2);
  wait(0.02);
  line1.reset(x-b/2, y1, x+b/2, y1);
  line2.reset(x-b/2, y2, x+b/2, y2);
  coin_circle.setColor(COLOR(255,215,0));   //GOLDEN YELLOW
  coin_circle.setFill(true);
  reset_all(x, y1, v, 90, 0, 0, 0, 1);
}

void Coin::getAttachedTo(MovingObject* m){
  MovingObject::getAttachedTo(m);
  is_attached = true;
}

void Coin::magnetize(){
  magnet = true;
  coin_circle.setColor(COLOR("red"));
}

void Coin::demagnetize(){
  magnet = false;
  coin_circle.setColor(COLOR(255,215,0));   //GOLDEN YELLOW
}

void Coin::ignite(){
  is_bomb = true;
  coin_circle.setColor(COLOR("blue"));
}

void Coin::deignite(){
  is_bomb = false;
  coin_circle.setColor(COLOR(255,215,0));   //GOLDEN YELLOW
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
  lasso_circle.setFill(true);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
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
  no_of_lives = init_lives;

  lasso_line.reset(lasso_start_x, lasso_start_y, lasso_start_x, lasso_start_y);
  if(dark_theme){
    lasso_line.setColor(COLOR(0, 150, 150));
    lasso_band.setColor(COLOR(0, 223, 0));
    lasso_circle.setColor(COLOR(0,200,200));
    lasso_loop.setColor(COLOR(192,192,192));
  }
  else{
    lasso_line.setColor(COLOR(153, 76, 0));
    lasso_band.setColor(COLOR(255, 203, 100));
    lasso_circle.setColor(COLOR(200,100,0));
    lasso_loop.setColor(COLOR(192,192,192));
  }
  draw_lasso_band();

} // End Lasso::initLasso()

void Lasso::yank() {
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setFill(true);
  lasso_looped = false;
  int temp = num_coins;
  for(int i = 0; i<n;i++)
    if(the_coin[i] != NULL) {
        if(the_coin[i]->getBomb()){
          loselife();
        }
        else{
         num_coins++;
         if(num_coins%next_lvl_coins==0)
           nextLvl();
        }
        if(mode == 1)
          the_coin[i]->hide();
        if(mode == 2)
          the_coin[i]->hide_s();

        updaterq = true;
        the_coin[i] = NULL;     //RESET THE POINTER TO NULL
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

void Lasso::set_ax_ay(double arg_ax,double arg_ay ){
  lasso_ax = arg_ax;
  lasso_ay = arg_ay;
}

void Lasso::nextStep(double stepTime) {
  if(getYPos() > PLAY_Y_HEIGHT) {
    if(is_spikes){
      loselife();
    }
    yank();
  }
  if(getYPos() + 5*WINDOW_Y/4 < 0 || getXPos() > 5*WINDOW_X/4 || getXPos() + WINDOW_X/4<0){
    loselife();
    yank();
  }
  if(isPaused())
    draw_lasso_band();
  MovingObject::nextStep(stepTime);
  lasso_line.reset(lasso_start_x, lasso_start_y, getXPos(), getYPos());
  if(is_magnetic){
    mag_timer += stepTime;
    if(mag_timer>MAG_TIME){de_mag();}
  }
} // End Lasso::nextStep()

void Lasso::check_for_coin(Coin *coinPtr, int coin_ID) {//CHECKS FOR NEARBY COINS AND APPLIES SPECIAL PROPERTIES, IF ANY
  if(coinPtr->isPaused()) return;
  if(coinPtr->getAttached()) return;
  if(isPaused()) return;
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double coin_x = coinPtr->getXPos();
  double coin_y = coinPtr->getYPos();
  double xdiff = (lasso_x - coin_x);
  double ydiff = (lasso_y - coin_y);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));

  if( distance <= lasso_r )  {
    the_coin[coin_ID] = coinPtr;
    the_coin[coin_ID]->getAttachedTo(this);
    if(coinPtr->getMag()) make_mag();
  }
} // End Lasso::check_for_coin()

void Lasso::make_mag(){//ATTACHES MAGNET TO THE LASSO
  is_magnetic = true;
  mag_timer = 0;
  lasso_r = mag_str*LASSO_RADIUS;
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

void Lasso::loselife(){
  Rectangle flash_red(WINDOW_X/2, WINDOW_Y/2,WINDOW_X, WINDOW_Y);
  flash_red.setColor(COLOR("red"));
  flash_red.setFill(true);
  wait(0.02);
  if(no_of_lives==1){
  Text quit(WINDOW_X/2, WINDOW_Y/2, "GAME OVER.");
  getClick();
  exit_game(lvl, num_coins, no_of_lives = 0);
  }
  no_of_lives--;
  Message.insert(Message.begin(), "You lost a life! :( ");
  updaterq = true;
}

void Lasso::nextLvl(){         //UPDATES THE LEVEL AND NOTIFIES THE USER
  lvl++;
  Rectangle new_lvl(WINDOW_X/2, WINDOW_Y/2,WINDOW_X/2, WINDOW_Y/4 );
  new_lvl.setFill(true);
  new_lvl.setColor(COLOR("white"));
  char bonus_str[256];
  sprintf(bonus_str, "Coin Bonus: %d", bonus);
  Text lvltxt(WINDOW_X/2, WINDOW_Y/2 - 60, "New Level!"),
  lvltxt1(WINDOW_X/2, WINDOW_Y/2-40, ("GG .. ^-^ ")),
  lvltxt2(WINDOW_X/2, WINDOW_Y/2 + 60, "Click or Press any key to exit."),
  lvltxt3(WINDOW_X/2, WINDOW_Y/2 , ""),
  lvltxt4(WINDOW_X/2, WINDOW_Y/2 + 20, ""),
  lvltxt5(WINDOW_X/2, WINDOW_Y/2 + 40, bonus_str);
  num_coins += bonus;
  coin_speed += 10;

  if(mode == 1){
   if(lvl%4==3){
    rain();
    lvltxt3.setMessage("Caution! The Lasso is not immune to water");
    lvltxt4.setMessage("(The ground is wet due to rains)");
    rain();
   }
   if(lvl%4==2){
    fire();
    lvltxt3.setMessage("The Fire is your enemy");
    lvltxt4.setMessage("(Don't touch the ground)");
    fire();
   }
   if(lvl%4==0){
    set_ax_ay(lvl*2, LASSO_G);
    lvltxt3.setMessage("It is windy...swoosh");
    lvltxt4.setMessage("(A coin is heavier than a lasso ;) )");
   }
  }

  XEvent event;
  nextEvent(event);     //WAIT FOR EVENT
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
  h5(WINDOW_X/2, WINDOW_Y/4 + 180, "'q' : Quits the Game.   'h' : Opens this Help Menu."),
  h6(WINDOW_X/2, 3*WINDOW_Y/4 - 40, "Press any key to exit Help Menu.");

  while(true){
    XEvent event;
    nextEvent(event);     //WAIT FOR EVENT
    if(keyPressEvent(event))
      return;
  }
}

void exit_game(int lvl, int score, int life){
  Rectangle exit_background(WINDOW_X/2, WINDOW_Y/2,WINDOW_X/2, WINDOW_Y/2 );
  exit_background.setFill(true);
  exit_background.setColor(COLOR("white"));

  char coinScoreStr[256], t_life[256], t_lvl[256];
  sprintf(coinScoreStr, "You collected %d Coins", score);
  sprintf(t_life, "Number of Lives: %d", life);
  sprintf(t_lvl, "You completed %d Levels", lvl-1);
  Text t1(WINDOW_X/2, WINDOW_Y/4 + 20, "...  Hope you liked the game!! ..."),
  t2(WINDOW_X/2, WINDOW_Y/4 + 50, ". |^_^| ."),
  t3(WINDOW_X/2, WINDOW_Y/4 + 80, coinScoreStr),
  t4(WINDOW_X/2, WINDOW_Y/4 + 100, t_lvl),
  t5(WINDOW_X/2, WINDOW_Y/4 + 120, t_life),
  t6(WINDOW_X/2, 3*WINDOW_Y/4 - 40, "Click or Press any key to exit.");

  XEvent event;
  nextEvent(event);     //WAIT FOR EVENT
  exit(0);
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
  simplecpp::srand(time(0) %1000);

  {//WELCOME DIALOGUE BOX
    Text welcome(WINDOW_X/2, WINDOW_Y/2, ".. WELCOME ..");
    XEvent event;
    nextEvent(event);
  }
  // MODE DIALOGUE BOX
  {
    Rectangle ask_mode(WINDOW_X/2, WINDOW_Y/2,WINDOW_X/2, WINDOW_Y/2 );
    ask_mode.setFill(true);
    ask_mode.setColor(COLOR(0,255,255));
    Text txt1(WINDOW_X/2, WINDOW_Y/2-40, "(1). Classic"), txt2(WINDOW_X/2, WINDOW_Y/2-20, "(2). Space"),
    txt3(WINDOW_X/2, WINDOW_Y/2+40, "Click on one of the modes to select, or type (1/2).");
    Rectangle r1(WINDOW_X/2, WINDOW_Y/2-40, textWidth("(1). Classic"), textHeight()),
    r2(WINDOW_X/2, WINDOW_Y/2-20, textWidth("(2). Space"), textHeight());
    while(true){
      XEvent event;
      nextEvent(event);
      if(mouseButtonPressEvent(event)){
        int x,y;
        y = event.xbutton.y;
        x = event.xbutton.x;
        if(abs(x-WINDOW_X/2)<textWidth("(1). Classic")/2 && abs(y-WINDOW_Y/2+40)<textHeight()/2 ){
          mode = 1;
          break;
        }
        else if(abs(x-WINDOW_X/2)<textWidth("(2). Space")/2 && abs(y-WINDOW_Y/2+20)<textHeight()/2 ){
          mode = 2;
          break;
        }
      }
      else if(keyPressEvent(event)){
        if(charFromEvent(event) == '1'){
            mode = 1;
            break;
        }
        else if(charFromEvent(event) == '2'){
            mode = 2;
            break;
        }
      }
    }
  }//AUTO DELETE VARIABLES USED

  {
    Rectangle ask_diff(WINDOW_X/2, WINDOW_Y/2,WINDOW_X/2, WINDOW_Y/2 );
    ask_diff.setFill(true);
    ask_diff.setColor(COLOR(0,255,255));
    Text txt1(WINDOW_X/2-80, WINDOW_Y/2-20, "(1). Easy"), txt2(WINDOW_X/2, WINDOW_Y/2-20, "(2). Medium"),
    txt3(WINDOW_X/2+80, WINDOW_Y/2-20, "(3). Hard"),
    txt4(WINDOW_X/2, WINDOW_Y/2+40, "Click on one of the modes to select, or type (1/2/3).");
    Rectangle r1(WINDOW_X/2-80, WINDOW_Y/2-20, textWidth("(1). Easy"), textHeight()),
    r2(WINDOW_X/2, WINDOW_Y/2-20, textWidth("(2). Medium"), textHeight()),
    r3(WINDOW_X/2+80, WINDOW_Y/2-20, textWidth("(3). Hard"), textHeight());
    while(true){
      XEvent event;
      nextEvent(event);
      if(mouseButtonPressEvent(event)){
        int x,y;
        y = event.xbutton.y;
        x = event.xbutton.x;
        if(abs(x-WINDOW_X/2+80)<textWidth("(1). Easy")/2 && abs(y-WINDOW_Y/2+20)<textHeight()/2 ){
          difficulty = 1;
          break;
        }
        else if(abs(x-WINDOW_X/2)<textWidth("(2). Medium")/2 && abs(y-WINDOW_Y/2+20)<textHeight()/2 ){
          difficulty = 2;
          break;
        }
        else if(abs(x-WINDOW_X/2-80)<textWidth("(3). Hard")/2 && abs(y-WINDOW_Y/2+20)<textHeight()/2 ){
          difficulty = 2;
          break;
        }
      }
      else if(keyPressEvent(event)){
        if(charFromEvent(event) == '1'){
            difficulty = 1;
            break;
        }
        else if(charFromEvent(event) == '2'){
            difficulty = 2;
            break;
        }
        else if(charFromEvent(event) == '3'){
            difficulty = 3;
            break;
        }
      }
    }
  }//AUTO DELETE VARIABLES USED
  switch(difficulty){       //IMPLEMENTING DIFFICULTY MODE
  case(1):
    mag_str = 2;
    init_lives = 5;
    next_lvl_coins = 4;
    bonus = 2;
    break;
  case(2):
    mag_str = 1.5;
    init_lives = 3;
    next_lvl_coins = 4;
    bonus = 1;
    coin_speed *= 1.2;
    break;
  case(3):
    mag_str = 1.25;
    init_lives = 1;
    next_lvl_coins = 5;
    bonus = 1;
    if(mode == 1)
      coin_speed *= 1.3;
    if(mode == 2)       //FOR SPACE MODE
      coin_speed *=1.5;
    break;
  }

    //THEME DIALOGUE BOX
  {
    Rectangle ask_theme(WINDOW_X/2, WINDOW_Y/2,WINDOW_X/2, WINDOW_Y/2 );
    ask_theme.setFill(true);
    ask_theme.setColor(COLOR(0,255,255));
    Text txt1(WINDOW_X/2, WINDOW_Y/2-40, "(1). Dark"), txt2(WINDOW_X/2, WINDOW_Y/2-20, "(2). Light"),
    txt3(WINDOW_X/2, WINDOW_Y/2+40, "Click on one of the themes to select, or type (1/2).");
    Rectangle r1(WINDOW_X/2, WINDOW_Y/2-40, textWidth("(1). Dark"), textHeight()),
    r2(WINDOW_X/2, WINDOW_Y/2-20, textWidth("(2). Light"), textHeight());
    while(true){
      XEvent event;
      nextEvent(event);
      if(mouseButtonPressEvent(event)){
        int x,y;
        y = event.xbutton.y;
        x = event.xbutton.x;
        if(abs(x-WINDOW_X/2)<textWidth("(1). Dark")/2 && abs(y-WINDOW_Y/2+40)<textHeight()/2 ){
        dark_theme = 1;
          break;
        }
        else if(abs(x-WINDOW_X/2)<textWidth("(2). Light")/2 && abs(y-WINDOW_Y/2+20)<textHeight()/2 ){
          dark_theme = 0;
          break;
        }
      }
      else if(keyPressEvent(event)){
        if(charFromEvent(event) == '1'){
            dark_theme = 1;
            break;
        }
        else if(charFromEvent(event) == '2'){
            dark_theme = 0;
            break;
        }
      }
    }
  }//AUTO DELETE VARIABLES USED


  // Draw lasso at start position
  double release_speed = INIT_RELEASE_SPEED; // m/s
  double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // degrees
  double lasso_ax = 0;
  double lasso_ay;
  if(mode == 1)
    lasso_ay = LASSO_G;
  if(mode == 2)             //FOR SPACE MODE
    lasso_ay = 0;
  bool paused = true;
  bool rtheta = true;
  Lasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);

  Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
  Line b2(PLAY_X_START, 0, PLAY_X_START, WINDOW_Y);
  if(dark_theme){
    b1.setColor(COLOR("white"));
    b2.setColor(COLOR("white"));
  }
  else{
    background.setFill(false);
    b1.setColor(COLOR("black"));
    b2.setColor(COLOR("black"));
  }

  string msg("Cmd: _");
  Text charPressed(PLAY_X_START+50, PLAY_Y_HEIGHT+20, msg);
  char coinScoreStr[256], nlife[256], mag_time_str[256]={}, nlvl[256];
  sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
  sprintf(nlife, "Lives: %d", lasso.getNumLives());
  sprintf(nlvl, "Level: %d", lasso.getLvl());
  Text coinScore(PLAY_X_START+50, PLAY_Y_HEIGHT+50, coinScoreStr);
  Text lives(WINDOW_X-100, 20, nlife), mag_Time(WINDOW_X-100, 60, mag_time_str);
  Text level(WINDOW_X-100, 40, nlvl);

  Message.insert(Message.begin(), "WELCOME!");      //INITIAL NOTIFICATIONS
  Message.insert(Message.begin(), "Best of Luck!! ");
  Message.insert(Message.begin(), "TIP: " + getTip());
  Text flash_msg1(WINDOW_X-120, PLAY_Y_HEIGHT+20, Message.at(0));
  Text flash_msg2(WINDOW_X-120, PLAY_Y_HEIGHT+40, Message.at(1));
  Text flash_msg3(WINDOW_X-120, PLAY_Y_HEIGHT+60, Message.at(2));

  show_help();

  Coin coins[n];

  // After every COIN_GAP sec, make the first coin jump

  double last_coin_jump_end[n] = {};  //INITIALISING           ( FOR CLASSIC MODE )

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
    for(int i = 0; i<n; i++){
      lasso.check_for_coin(&coins[i],i);
    }
	wait(STEP_TIME*3);
	break;
	  case 'h':
    show_help();
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
	exit_game(lasso.getLvl(), lasso.getNumCoins(), lasso.getNumLives());
      default:
	break;
      }
    }

    lasso.nextStep(stepTime);
//UPDATING COINS
   if(mode == 1){
    for(int i = 0; i<n; i++){
    coins[i].nextStep(stepTime);
    if(coins[i].isPaused()) {
      if((currTime-last_coin_jump_end[1]) >= COIN_GAP) {
        if(rand()%100==5){      //RANDOMLY POPPING COIN...
          coins[i].unhide();
          if(rand()%4==1){
            coins[i].magnetize();
          }
          else if(rand()%4==1){
            coins[i].ignite();
          }
        }
      }
    }

    if(coins[i].getYPos() > PLAY_Y_HEIGHT) {
      coins[i].hide();
      last_coin_jump_end[1] = currTime;
    }
    }}      // MODE 1 CODE ENDS HERE

    if(mode == 2){
      for(int i = 0; i<n; i++){
        if(coins[i].isPaused() ){
          if(rand()%100 == 5){
            coins[i].unhide_s();
            if(rand()%4==1){
              coins[i].magnetize();
            }
            else if(rand()%4==1){
              coins[i].ignite();
            }
          }
        }
        else{
          coins[i].nextStep(stepTime);
          if(abs(coins[i].getYPos() - coins[i].getY2()) < COIN_SIZE) {
            coins[i].hide_s();
          }
        }
      }//EXIT FOR LOOP
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

      sprintf(nlvl, "Level: %d", lasso.getLvl());
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
