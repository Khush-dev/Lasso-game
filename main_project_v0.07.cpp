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
  // Moving parts
  Circle coin_circle;

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
    magnet = false;
    is_bomb = false;
 }
  bool getMag(){return magnet;}
  bool getBomb(){return is_bomb;}
  void initCoin();
  void resetCoin();
  void hide();  //HIDES THE COIN
  void unhide();    //RESETS THE COIN
  void unhide_s(int, int, int);     //UNHIDE IN SPACE MODE
  void magnetize();      //COLLECTING MAGNETIZED COINS INCREASES LASSO RADIUS FOR SOME TIME
  void demagnetize();      //DEMAGNETIZES THE
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

#define n 5         //NUMBER OF COINS
#define INIT_LIVES 5        //INITIAL NUMBER OF LIVES
#define MAG_STR 2       //FACTOR BY WHICH THE RADIUS INCREASES
#define MAG_TIME 5      //TIME DURATION FOR MAGNETIC LASSO TO LAST
//#define NEXT_LVL_COINS 2        // NUMBER OF COINS REQUIRED TO GO THE NEXT LEVEL

//OTHER GLOBAL VARIABLES
// WITH INITIAL VALUES

bool updaterq = false;      //NOTIFICATION UPDATE REQUIRED OR NOT
bool is_spikes = false;     //SPIKES( I.E. FIRE/WATER) ON THE GROUND
int mode = 1;         //GAME MODE ID: CLASSIC --> 1; SPACE --> 2;
bool dark_theme = 1;            // DARK/LIGHT THEMES, TRUE REFERS TO DARK THEME
int next_lvl_coins = 2;         //NUMBER OF COINS REQUIRED TO GO THE NEXT LEVEL
int difficulty = 1;         //DIFFICULTY LEVEL, EASY --> 1; MEDIUM --> 2; HARD -->3;

vector <string> Message;        //VECTOR FOR NOTIFICATION IN RIGHT BOTTOM

string getTip(){        //RETURNS A RANDOM TIP FOR THE USER
  string tips[] = {"Stay away from bombs!", "Time your throw well", "Don't forget to yank ;) ",
  "A Magnet lasts for 5 seconds |=_=|  ", "Beware of Bombs! |=_*| ", "Red coins are Magnetic '-' " };
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
  void yank(Line*);      //PULLS THE LASSO BACK AND RESETS THE COINS COLLECTED/ATTACHED, IF ANY. ADDS LIFE FOR MORE THAN ONE COIN, COSTS LIVES FOR COLLECTING BOMBS
  void loopit();
  void addAngle(double angle_deg);
  void addSpeed(double speed);
  void set_ax_ay(double arg_ax,double arg_ay );

  void nextStep(double t);
  void check_for_coin(Coin *coin, int  coin_ID = -1);

  int getNumCoins() {
    return num_coins;      //SCORE
  }

  int getNumLives() {
    return no_of_lives;     //LIVES
  }

  bool get_is_magnetic(){
    return is_magnetic;
  }

  int getLvl() {
    return lvl;     //GAME LEVEL
  }

  void loselife();      //REDUCES LIFE AND UPDATES DATA
  void make_mag();      //ATTACHES MAGNET TO THE LASSO
  void de_mag();       //REMOVES THE ATTACHED MAGNET
  float getMagTimer(){ return mag_timer; }
  void nextLvl();

}; // End class Lasso

#endif

//coin.h

void Coin::initCoin() {
  coin_start_x = (PLAY_X_START+WINDOW_X)/2 + rand()%100;     //SPAWNING RANDOMLY NEAR THE CENTRE OF X AXES
  coin_start_y = PLAY_Y_HEIGHT;
  coin_circle.reset(coin_start_x, coin_start_y, COIN_SIZE);
  coin_circle.setColor(COLOR("yellow"));     //COIN COLOUR
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
  reset_all(0,0, 0,0,0,0, true, true);      //PAUSED COINS CANNOT BE COLLECTED, NOR CAN THE LASSO REACH THERE
  if(dark_theme)
    coin_circle.setColor(COLOR("black"));
  else
    coin_circle.setColor(COLOR("white"));
  coin_circle.setFill(false);
  if(magnet){
    demagnetize();
  }
  if(is_bomb){
    deignite();
  }
}

void Coin::unhide(){
  coin_circle.setColor(COLOR("yellow"));
  coin_circle.setFill(true);
  resetCoin();
  unpause();
}

void Coin::unhide_s(int x, int y, int v){      //SPACE MODE
  coin_circle.setColor(COLOR("yellow"));
  coin_circle.setFill(true);
  reset_all(x, y, v, 90, 0, 0, 0, 1);
}

void Coin::magnetize(){
  magnet = true;
  coin_circle.setColor(COLOR("red"));
}

void Coin::demagnetize(){
  magnet = false;
  coin_circle.setColor(COLOR("yellow"));
}

void Coin::ignite(){
  is_bomb = true;
  coin_circle.setColor(COLOR("blue"));
}

void Coin::deignite(){
  is_bomb = false;
  coin_circle.setColor(COLOR("yellow"));
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

void Lasso::yank( Line* lines = NULL) {
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
        else{ num_coins++;
        if(num_coins%next_lvl_coins==0)nextLvl();
        }
        updaterq = true;

        the_coin[i]->hide();
        if(mode == 2){
        lines[2*i].hide();
        lines[2*i+1].hide();
        }
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

void Lasso::set_ax_ay(double arg_ax,double arg_ay ){
  lasso_ax = arg_ax;
  lasso_ay = arg_ay;
}

void Lasso::nextStep(double stepTime) {
  draw_lasso_band();
  MovingObject::nextStep(stepTime);
  if(getYPos() > PLAY_Y_HEIGHT) {
    yank();
    if(is_spikes){
      loselife();
    }
  }
  lasso_line.reset(lasso_start_x, lasso_start_y, getXPos(), getYPos());
  if(is_magnetic){
    mag_timer += stepTime;
    if(mag_timer>MAG_TIME){de_mag();}
  }
} // End Lasso::nextStep()

void Lasso::check_for_coin(Coin *coinPtr, int coin_ID) {//CHECKS FOR NEARBY COINS AND APPLIES SPECIAL PROPERTIES, IF ANY
  if(coinPtr->isPaused()) return;
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

void Lasso::loselife(){
  if(no_of_lives==1){
  Text quit(WINDOW_X/2, WINDOW_Y/2, "GAME OVER.");
  getClick();
  exit(0);
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
  Text lvltxt(WINDOW_X/2, WINDOW_Y/2 - 60, "New Level!"),
  lvltxt1(WINDOW_X/2, WINDOW_Y/2-40, "GG.. ^-^ "),
  lvltxt2(WINDOW_X/2, WINDOW_Y/2 + 60, "Click or Press any key to exit."),
  lvltxt3(WINDOW_X/2, WINDOW_Y/2 , ""),
  lvltxt4(WINDOW_X/2, WINDOW_Y/2 + 20, "");
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
  h5(WINDOW_X/2, WINDOW_Y/4 + 180, "'q' : Quits the Game."),
  h6(WINDOW_X/2, 3*WINDOW_Y/4 - 40, "Click or Press any key to exit Help Menu.");

  XEvent event;
  nextEvent(event);
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

  //THEME
  {
    Rectangle ask_theme(WINDOW_X/2, WINDOW_Y/2,WINDOW_X/2, WINDOW_Y/2 );
    ask_theme.setFill(true);
    ask_theme.setColor(COLOR("neon"));
    Text modetxt1(WINDOW_X/2, WINDOW_Y/2-40, "(1). Dark"), modetxt2(WINDOW_X/2, WINDOW_Y/2-20, "(2). Light"),
    modetxt3(WINDOW_X/2, WINDOW_Y/2+40, "Click on one of the themes to select, or type (d/l).");
    Rectangle m1(WINDOW_X/2, WINDOW_Y/2-40, textWidth("(1). Dark"), textHeight()),
    m2(WINDOW_X/2, WINDOW_Y/2-20, textWidth("(2). Light"), textHeight());
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
        if(charFromEvent(event) == 'd'){
            dark_theme = 1;
            break;
        }
        else if(charFromEvent(event) == 'l'){
            dark_theme = 0;
            break;
        }
    }
    }
  }

  // HELP DIALOGUE BOX
  {
    Rectangle ask_mode(WINDOW_X/2, WINDOW_Y/2,WINDOW_X/2, WINDOW_Y/2 );
    ask_mode.setFill(true);
    ask_mode.setColor(COLOR("neon"));
    Text modetxt1(WINDOW_X/2, WINDOW_Y/2-40, "(1). Classic"), modetxt2(WINDOW_X/2, WINDOW_Y/2-20, "(2). Space"),
    modetxt3(WINDOW_X/2, WINDOW_Y/2+40, "Click on one of the mode to select, or type (c/s).");
    Rectangle m1(WINDOW_X/2, WINDOW_Y/2-40, textWidth("(1). Classic"), textHeight()),
    m2(WINDOW_X/2, WINDOW_Y/2-20, textWidth("(2). Space"), textHeight());
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
        if(charFromEvent(event) == 'c'){
            mode = 1;
            break;
        }
        else if(charFromEvent(event) == 's'){
            mode = 2;
            break;
        }
    }
    }

  }//AUTO DELETE VARIABLES USED


  // Draw lasso at start position
  double release_speed = INIT_RELEASE_SPEED; // m/s
  double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // degrees
  double lasso_ax = 0;
  double lasso_ay = LASSO_G;
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

  Message.insert(Message.begin(), "WELCOME!");
  Message.insert(Message.begin(), "Best of Luck!! ");
  Message.insert(Message.begin(), "TIP: " + getTip());
  Text flash_msg1(WINDOW_X-120, PLAY_Y_HEIGHT+20, Message.at(0));
  Text flash_msg2(WINDOW_X-120, PLAY_Y_HEIGHT+40, Message.at(1));
  Text flash_msg3(WINDOW_X-120, PLAY_Y_HEIGHT+60, Message.at(2));

  show_help();

  Coin coins[n];

  Line lines[2*n];
  if(dark_theme)
    for(int i = 0; i<2*n; i++){
      lines[i].setColor(COLOR("white"));
    }
  else
    for(int i = 0; i<2*n; i++){
      lines[i].setColor(COLOR("black"));
    }
  if(mode == 2){             //FOR SPACE MODE
    lasso.set_ax_ay(0,0);
  }

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
      int x, y1, y2, b, v;
      x = randuv(PLAY_X_START + 50, WINDOW_X-50);
      y1 = randuv(PLAY_Y_START, PLAY_Y_START + PLAY_Y_HEIGHT );
      y2 = randuv(PLAY_Y_START, PLAY_Y_START + PLAY_Y_HEIGHT );
      if(abs(y1 - y2) < 20){ y2 += 100;}
      if(y2<y1)
      v = randuv(20, COIN_SPEED);
      else
      v = -1*randuv(20, COIN_SPEED);
      b = abs(v)/3 + 25;        //LENGTH INCREASES WITH SPEED
      coins[i].unhide_s(x, y1, v);
      lines[2*i].reset(x-b/4, y1, x+b/4, y1);       //LINES REPRESENT THE PORTALS
      lines[2*i+1].reset(x-b/4, y2, x+b/4, y2);
      lines[2*i].show();
      lines[2*i+1].show();
      wait(0.02);       //FOR PORTAL OPENING ANIMATION
      lines[2*i].reset(x-b/4, y1, x+3*b/8, y1);
      lines[2*i+1].reset(x-b/4, y2, x+3*b/8, y2);
      wait(0.01);
      lines[2*i].reset(x-b/2, y1, x+b/2, y1);
      lines[2*i+1].reset(x-b/2, y2, x+b/2, y2);
      }}
      else{
        coins[i].nextStep(stepTime);
        if(abs(coins[i].getYPos() - lines[2*i+1].getY()) < COIN_SIZE) {
          coins[i].hide();
          lines[2*i].hide();
          lines[2*i+1].hide();
        }
      }


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
