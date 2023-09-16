#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>


class Peach;
class Actor;


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  virtual ~StudentWorld() { cleanUp(); }

  virtual int init();
  virtual int move();
  virtual void cleanUp();

  void addActor(Actor* actor);
  bool isBlockingObjectAt(double& x2, double& y2) const;
  bool isDamageableObjectAt(double& x2, double& y2) const;
  bool isPeachAt(double& x1, double& y1) const;
  void bonkObject(double x, double y) const;
  void damageObject(double x, double y) const;
  void setPeachHit(int points) const;
  void givePower(bool shoot, bool jump, bool star, int ticks) const;
  bool kickEnemy(Actor* enemy);
  bool checkPeachLevel(double y) const;
  int checkPeachDirection(double x) const;
  bool distanceBetween(double x, int num, char dir) const;
  void endLevel(bool isGameWon);
  bool moveOrBonk(Actor* actor, double destx, double desty) const;
  bool moveIfPossible(Actor* actor, double destx, double desty) const;
  bool moveOrSwitch(Actor* actor, double destx, double desty) const;

private:
	std::vector<Actor*> m_actors;
	Peach* m_peach;
	bool m_level_finished;
	bool m_game_finished;
};

#endif // STUDENTWORLD_H_
