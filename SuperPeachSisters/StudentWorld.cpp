#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), m_level_finished(false), m_game_finished(false)
{
}

int StudentWorld::init()
{
    m_level_finished = false;
    m_game_finished = false;
    int lvl = getLevel();
    Level lev(assetPath());
    stringstream level;
    level.fill('0');
    level << "level" << setw(2) << lvl << ".txt";
    //level << "level03.txt";
    string level_file = level.str();
    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_fail_file_not_found || result == Level::load_fail_bad_format) {
        return GWSTATUS_LEVEL_ERROR;
    }
    else if (result == Level::load_success) {
        Level::GridEntry ge;
        for (int lx = 0; lx < GRID_WIDTH; lx++) {
            for (int ly = 0; ly < GRID_HEIGHT; ly++) {

                ge = lev.getContentsOf(lx, ly); // x=5, y=10
                int actual_x = lx * SPRITE_WIDTH;
                int actual_y = ly * SPRITE_HEIGHT;

                if (ge == Level::peach) {
                    m_peach = new Peach(this, actual_x, actual_y);
                }
                else if (ge == Level::block) {
                    Block* bl = new Block(this, actual_x, actual_y, 'n');
                    m_actors.push_back(bl);
                }
                else if (ge == Level::pipe) {
                    Pipe* p = new Pipe(this, actual_x, actual_y);
                    m_actors.push_back(p);
                }
                else if (ge == Level::flag) {
                    Flag* f = new Flag(this, actual_x, actual_y);
                    m_actors.push_back(f);
                }
                else if (ge == Level::mario) {
                    Mario* m = new Mario(this, actual_x, actual_y);
                    m_actors.push_back(m);
                }
                else if (ge == Level::flower_goodie_block) {
                    Block* bl = new Block(this, actual_x, actual_y, 'f');
                    m_actors.push_back(bl);
                }
                else if (ge == Level::mushroom_goodie_block) {
                    Block* bl = new Block(this, actual_x, actual_y, 'm');
                    m_actors.push_back(bl);
                }
                else if (ge == Level::star_goodie_block) {
                    Block* bl = new Block(this, actual_x, actual_y, 's');
                    m_actors.push_back(bl);
                }
                else if (ge == Level::goomba) {
                    Goomba* gb = new Goomba(this, actual_x, actual_y);
                    m_actors.push_back(gb);
                }
                else if (ge == Level::koopa) {
                    Koopa* kp = new Koopa(this, actual_x, actual_y);
                    m_actors.push_back(kp);
                }
                else if (ge == Level::piranha) {
                    Piranha* pi = new Piranha(this, actual_x, actual_y);
                    m_actors.push_back(pi);
                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() { 
    m_peach->doSomething();
    for (int i = 0; i != m_actors.size(); i++) {
        if (m_actors[i]->isAlive()) {
            m_actors[i]->doSomething();
            if (!m_peach->isAlive()) {
                playSound(SOUND_PLAYER_DIE);
                decLives();
                return  GWSTATUS_PLAYER_DIED;
            }
            if (m_level_finished && !m_game_finished) {
                playSound(SOUND_FINISHED_LEVEL);
                return GWSTATUS_FINISHED_LEVEL;
            }
            if (m_game_finished) {
                playSound(SOUND_GAME_OVER);
                return GWSTATUS_PLAYER_WON;
            }
        }
    }
    
    vector<Actor*>::iterator e;
    for (e = m_actors.begin(); e != m_actors.end(); ) {
        int index = e - m_actors.begin();
        if (!m_actors[index]->isAlive()) {
            delete m_actors[index];
            e = m_actors.erase(e);
        }
        else {
            e++;
        }
    }

    ostringstream oss;
    oss.fill('0');
    oss << "Lives: " << getLives() << "  " << " Level: " << getLevel() << "  " << " Points: " << getScore() << " ";
    if (m_peach->hasStar()) {
        oss << "StarPower! ";
    }
    if (m_peach->hasJump()) {
        oss << "JumpPower! ";
    }
    if (m_peach->hasShoot()) {
        oss << "ShootPower! ";
    }
    
    string text = oss.str();
    setGameStatText(text);
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (int i = 0; i < m_actors.size(); i++) {
        delete m_actors[i];
    }
    m_actors.clear();
    delete m_peach;
}



bool StudentWorld::isBlockingObjectAt(double& x2, double& y2) const {
    for (int i = 0; i < m_actors.size(); i++) {
        double x1 = m_actors[i]->getX();
        double y1 = m_actors[i]->getY();

        if (x2 >= x1 && x2 <= x1 + SPRITE_WIDTH - 1 && y2 >= y1 && y2 <= y1 + SPRITE_HEIGHT - 1) {
            if (m_actors[i]->blocks()) {
                x2 = x1;
                y2 = y1;
                return true;
            }
            
        }

        else if (x2 >= x1 && x2 <= x1 + SPRITE_WIDTH - 1 && y2 <= y1 && y2 >= y1 - SPRITE_HEIGHT + 1) {
            if (m_actors[i]->blocks()) {
                x2 = x1;
                y2 = y1;
                return true;
            }
        }

        else if (x2 <= x1 && x2 >= x1 - SPRITE_WIDTH + 1 && y2 >= y1 && y2 <= y1 + SPRITE_HEIGHT - 1) {
            if (m_actors[i]->blocks()) {
                x2 = x1;
                y2 = y1;
                return true;
            }
        }

        else if (x2 <= x1 && x2 >= x1 - SPRITE_WIDTH + 1 && y2 <= y1 && y2 >= y1 - SPRITE_HEIGHT + 1) {
            if (m_actors[i]->blocks()) {
                x2 = x1;
                y2 = y1;
                return true;
            }
        }
    }
    return false;
}

bool StudentWorld::isDamageableObjectAt(double& x2, double& y2) const {
    for (int i = 0; i < m_actors.size(); i++) {
        double x1 = m_actors[i]->getX();
        double y1 = m_actors[i]->getY();

        if (x2 >= x1 && x2 <= x1 + SPRITE_WIDTH - 1 && y2 >= y1 && y2 <= y1 + SPRITE_HEIGHT - 1) {
            if (m_actors[i]->isDamageable()) {
                x2 = x1;
                y2 = y1;
                return true;
            }

        }

        else if (x2 >= x1 && x2 <= x1 + SPRITE_WIDTH - 1 && y2 <= y1 && y2 >= y1 - SPRITE_HEIGHT + 1) {
            if (m_actors[i]->isDamageable()) {
                x2 = x1;
                y2 = y1;
                return true;
            }
        }

        else if (x2 <= x1 && x2 >= x1 - SPRITE_WIDTH + 1 && y2 >= y1 && y2 <= y1 + SPRITE_HEIGHT - 1) {
            if (m_actors[i]->isDamageable()) {
                x2 = x1;
                y2 = y1;
                return true;
            }
        }

        else if (x2 <= x1 && x2 >= x1 - SPRITE_WIDTH + 1 && y2 <= y1 && y2 >= y1 - SPRITE_HEIGHT + 1) {
            if (m_actors[i]->isDamageable()) {
                x2 = x1;
                y2 = y1;
                return true;
            }
        }
    }
    return false;
}

bool StudentWorld::isPeachAt(double& x1, double& y1) const {
    double x2 = m_peach->getX();
    double y2 = m_peach->getY();

    if (x1 >= x2 && x1 <= x2 + SPRITE_WIDTH - 1 && y1 >= y2 && y1 <= y2 + SPRITE_HEIGHT - 1) {
        x1 = x2;
        y1 = y2;
        return true;
    }

    else if (x1 >= x2 && x1 <= x2 + SPRITE_WIDTH - 1 && y1 <= y2 && y1 >= y2 - SPRITE_HEIGHT + 1) {
        x1 = x2;
        y1 = y2;
        return true;
    }

    else if (x1 <= x2 && x1 >= x2 - SPRITE_WIDTH + 1 && y1 >= y2 && y1 <= y2 + SPRITE_HEIGHT - 1) {
        x1 = x2;
        y1 = y2;
        return true;
    }

    else if (x1 <= x2 && x1 >= x2 - SPRITE_WIDTH + 1 && y1 <= y2 && y1 >= y2 - SPRITE_HEIGHT + 1) {
        x1 = x2;
        y1 = y2;
        return true;
    }

    return false;
}

void StudentWorld::endLevel(bool isGameWon) {
    m_level_finished = true;
    m_game_finished = isGameWon;
}

void StudentWorld::setPeachHit(int points) const {
    m_peach->setHealth(points);
}

void StudentWorld::givePower(bool shoot, bool jump, bool star, int ticks) const {
    if (shoot) {
        m_peach->giveShoot();
    }
    if (jump) {
        m_peach->giveJump();
    }
    if (star) {
        m_peach->giveStar(ticks);
    }
}


void StudentWorld::addActor(Actor* actor) {
    m_actors.push_back(actor);
}

bool StudentWorld::kickEnemy(Actor* enemy) {
    if (m_peach->hasStar()) {
        playSound(SOUND_PLAYER_KICK);
        increaseScore(100);
        enemy->setDead();
        return true;
    }
    return false;
}

void StudentWorld::bonkObject(double x, double y) const {
    if (m_peach->getX() == x && m_peach->getY() == y) {
        m_peach->bonk();
    }
    for (int i = 0; i < m_actors.size(); i++) {
        if (m_actors[i]->getX() == x && m_actors[i]->getY() == y) {
            m_actors[i]->bonk();
        }
    }
}

void StudentWorld::damageObject(double x, double y) const {
    if (m_peach->getX() == x && m_peach->getY() == y) {
        m_peach->damage();
    }
    for (int i = 0; i < m_actors.size(); i++) {
        if (m_actors[i]->getX() == x && m_actors[i]->getY() == y) {
            m_actors[i]->damage();
        }
    }
}


bool StudentWorld::checkPeachLevel(double y) const {
    if (m_peach->getY() >= y && m_peach->getY() <= y + (1.5 * SPRITE_HEIGHT)) {
        return true;
    }
    else if (m_peach->getY() <= y && m_peach->getY() >= y - (1.5 * SPRITE_HEIGHT)) {
        return true;
    }
    return false;
}

int StudentWorld::checkPeachDirection(double x) const {
    if (m_peach->getX() >= x) {
        //Peach is to the right
        return 0;
    }
    else {
        //Peach is to the left
        return 180;
    }
}

bool StudentWorld::distanceBetween(double x, int num, char dir) const {
    if (dir == 'r') {
        if (m_peach->getX() < x + (num * SPRITE_WIDTH)) {
            return true;
        }
    }
    if (dir == 'l') {
        if (m_peach->getX() > x - (num * SPRITE_WIDTH)) {
            return true;
        }
    }
    
    return false;
}

bool StudentWorld::moveOrBonk(Actor* actor, double x2, double y2) const {
    if (isBlockingObjectAt(x2, y2)) {
        bonkObject(x2, y2);
        return false;
    }
    else {
        actor->moveTo(x2, y2);
        return true;
    }
}


bool StudentWorld::moveIfPossible(Actor* actor, double destx, double desty) const {
    if (!isBlockingObjectAt(destx, desty)) {
        actor->moveTo(destx, desty);
        return true;
    }
    return false;
}

bool StudentWorld::moveOrSwitch(Actor* actor, double destx, double desty) const {
    if (!isBlockingObjectAt(destx, desty)) {
        actor->moveTo(destx, desty);
        return true;
    }
    else {
        if (actor->getDirection() == 180) {
            actor->setDirection(0);
        }
        else if (actor->getDirection() == 0) {
            actor->setDirection(180);
        }
        return false;
    }
    
}
