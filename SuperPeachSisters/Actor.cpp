#include "Actor.h"
#include "StudentWorld.h"
//class StudentWorld;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(StudentWorld* world, int id, int start_x, int start_y, int start_dir, int depth) : GraphObject(id, start_x, start_y, start_dir, depth, 1.0), m_isAlive(true), m_world(world) {}


Peach::Peach(StudentWorld* world, int start_x, int start_y) : Actor(world, IID_PEACH, start_x, start_y, 0, 0), m_health(1), m_invincible_time(0), m_temp_invincible_time(0), m_recharge_time(0), m_jump_distance(0), m_invincible(false), m_temp_invincible(false), m_star(false), m_shoot(false), m_jump(false), m_jumping(false) {}


void Peach::doSomething() {
	if (!isAlive()) {
		return;
	}
	if (m_invincible) {
		m_invincible_time--;
		if (m_invincible_time == 0) {
			m_invincible = false;
			if (m_star) {
				m_star = false;
			}
		}
	}
	if (m_temp_invincible) {
		m_temp_invincible_time--;
		if (m_temp_invincible_time == 0) {
			m_temp_invincible = false;
		}
	}
	if (m_recharge_time > 0) {
		m_recharge_time--;
	}
	double x_pos = getX();
	double y_pos = getY();
	if (World()->isBlockingObjectAt(x_pos, y_pos) || World()->isDamageableObjectAt(x_pos, y_pos)) {
		World()->bonkObject(x_pos, y_pos);
	}

	if (m_jumping) {
		if (m_jump_distance > 0) {
			double x_pos = getX();
			double y_pos = getY() + 4;
			
			if (!World()->moveOrBonk(this, x_pos, y_pos)) {
				m_jump_distance = 0;
				m_jumping = false;
			}
			else {
				m_jump_distance--;
			}
			
			if (m_jump_distance <= 0) {
				m_jumping = false;
			}
		}
	}

	if (!m_jumping) {
		double x_pos = getX();
		double y_pos = getY() - 4;
		World()->moveIfPossible(this, x_pos, y_pos);
	}

	int key = 0;
	bool keyPressed = World()->getKey(key);
	if (keyPressed) {
		double y_pos = getY();
		if (key == KEY_PRESS_LEFT) {
			setDirection(180);
			double x_pos = getX() - 4;
			World()->moveOrBonk(this, x_pos, y_pos);
			
		}

		if (key == KEY_PRESS_RIGHT) {
			double x_pos = getX() + 4;
			setDirection(0);
			World()->moveOrBonk(this, x_pos, y_pos);
		}

		if (key == KEY_PRESS_UP) {
			double x_pos = getX();
			double y_pos = getY() - 1;
			if (World()->isBlockingObjectAt(x_pos, y_pos)) {
				if (!m_jump) {
					m_jump_distance = 8;
				}
				else if (m_jump) {
					m_jump_distance = 12;
				}
				World()->playSound(SOUND_PLAYER_JUMP);
				m_jumping = true;
			}
		}

		if (key == KEY_PRESS_SPACE) {
			if (m_shoot) {
				if (m_recharge_time <= 0) {
					World()->playSound(SOUND_PLAYER_FIRE);
					m_recharge_time = 8;
					double x_pos = getX();
					double y_pos = getY();
					if (getDirection() == 0) {
						x_pos += 4;
						PeachFireball* f = new PeachFireball(World(), x_pos, y_pos, 0);
						World()->addActor(f);
					}
					else if (getDirection() == 180) {
						x_pos -= 4;
						PeachFireball* f = new PeachFireball(World(), x_pos, y_pos, 180);
						World()->addActor(f);
					}
					
				}
			}
		}
		
	}
}

void Peach::bonk() {
	if (!m_star && !m_temp_invincible) {
		m_health--;
		m_temp_invincible_time = 10;
		if (m_shoot) {
			m_shoot = false;
		}
		if (m_jump) {
			m_jump = false;
		}
		if (m_health >= 1) {
			World()->playSound(SOUND_PLAYER_HURT);
		}
		if (m_health <= 0) {
			setDead();
		}
	}
}

Ground::Ground(StudentWorld* world, int start_x, int start_y, int id) : Actor(world, id, start_x, start_y, 0, 2) {}

Block::Block(StudentWorld* world, int start_x, int start_y, char goodie) : Ground(world, start_x, start_y, IID_BLOCK), m_goodie(goodie), m_released(false) {}

Pipe::Pipe(StudentWorld* world, int start_x, int start_y) : Ground(world, start_x, start_y, IID_PIPE) {}

void Block::bonk() {
	if (m_goodie == 'n' || m_released) {
		World()->playSound(SOUND_PLAYER_BONK);
	}
	else if (m_goodie != 'n' && !m_released) {
		World()->playSound(SOUND_POWERUP_APPEARS);
		if (m_goodie == 'f') {
			Flower* f = new Flower(World(), getX(), getY() + 8.0);
			World()->addActor(f);
		}
		else if (m_goodie == 'm') {
			Mushroom* m = new Mushroom(World(), getX(), getY() + 8.0);
			World()->addActor(m);
		}
		else if (m_goodie == 's') {
			Star* st = new Star(World(), getX(), getY() + 8.0);
			World()->addActor(st);
		}
		m_released = true;
	}
}

Goal::Goal(StudentWorld* world, int start_x, int start_y, int id) : Actor(world, id, start_x, start_y, 0, 1) {}

Flag::Flag(StudentWorld* world, int start_x, int start_y) : Goal(world, start_x, start_y, IID_FLAG) {}

Mario::Mario(StudentWorld* world, int start_x, int start_y) : Goal(world, start_x, start_y, IID_MARIO) {}

void Goal::doSomething() {
	if (!isAlive()) {
		return;
	}
	double x_pos = getX();
	double y_pos = getY();
	if (World()->isPeachAt(x_pos, y_pos)) {
		World()->increaseScore(1000);
		setDead();
		informWorld();
	}
}

void Flag::informWorld() const {
	World()->endLevel(false);
}

void Mario::informWorld() const {
	World()->endLevel(true);
}

Goodie::Goodie(StudentWorld* world, int start_x, int start_y, int id, int pts) : Actor(world, id, start_x, start_y, 0, 1), m_pts(pts) {}

Flower::Flower(StudentWorld* world, int start_x, int start_y) : Goodie(world, start_x, start_y, IID_FLOWER, 50) {}

Mushroom::Mushroom(StudentWorld* world, int start_x, int start_y) : Goodie(world, start_x, start_y, IID_MUSHROOM, 75) {}

Star::Star(StudentWorld* world, int start_x, int start_y) : Goodie(world, start_x, start_y, IID_STAR, 100) {}

void Goodie::incHit() const { World()->setPeachHit(2); }

void Goodie::incPoints() const { World()->increaseScore(m_pts); }

void Flower::informPeach() const { World()->givePower(true, false, false, 0); }

void Mushroom::informPeach() const { World()->givePower(false, true, false, 0); }

void Star::informPeach() const { World()->givePower(false, false, true, 150); }

void Goodie::doSomething() {
	double x_pos = getX();
	double y_pos = getY();
	if (World()->isPeachAt(x_pos, y_pos)) {
		incPoints();
		informPeach();
		incHit();
		setDead();
		World()->playSound(SOUND_PLAYER_POWERUP);
		return;
	}
	else {
		double x_pos = getX();
		double y_pos = getY() - 2.0;
		World()->moveIfPossible(this, x_pos, y_pos);
	}
	int dir = getDirection();
	double x_pos2 = getX();
	double y_pos2 = getY();
	if (dir == 0) {
		x_pos2 += 2.0;
	}
	else if (dir == 180) {
		x_pos2 -= 2.0;
	}
	if (!World()->moveOrSwitch(this, x_pos2, y_pos2)) {
		return;
	}
}

Enemy::Enemy(StudentWorld* world, int start_x, int start_y, int id) : Actor(world, id, start_x, start_y, randInt(0, 1)*180, 0) {}

Goomba::Goomba(StudentWorld* world, int start_x, int start_y) : Enemy(world, start_x, start_y, IID_GOOMBA) {}

Koopa::Koopa(StudentWorld* world, int start_x, int start_y) : Enemy(world, start_x, start_y, IID_KOOPA) {}

Piranha::Piranha(StudentWorld* world, int start_x, int start_y) : Enemy(world, start_x, start_y, IID_PIRANHA), m_firing_delay(0) {}

void Enemy::doSomething() {
	if (!isAlive()) {
		return;
	}
	double x_pos = getX();
	double y_pos = getY();
	if (World()->isPeachAt(x_pos, y_pos)) {
		World()->bonkObject(x_pos, y_pos);
		return;
	}
	double x_pos2 = getX();
	double y_pos2 = getY();
	double down = y_pos2 - 1;
	double extending = x_pos2;
	int newdir = getDirection();
	if (getDirection() == 0) {
		x_pos2++;
		extending += SPRITE_WIDTH;
		newdir = 180;
	}
	else if (getDirection() == 180) {
		x_pos2--;
		extending -= SPRITE_WIDTH;
		newdir = 0;
	}
	if (World()->isBlockingObjectAt(x_pos2, y_pos2)) {
		setDirection(newdir);
	}
	else if (!World()->isBlockingObjectAt(extending, down)) {
		setDirection(newdir);
	}
	
	double x_pos3 = getX();
	double y_pos3 = getY();
	if (getDirection() == 0) {
		x_pos3 += 1;
	}
	else if (getDirection() == 180) {
		x_pos3 -= 1;
	}
	if (!World()->moveIfPossible(this, x_pos3, y_pos3)) {
		return;
	}
}

void Enemy::bonk() {
	double x_pos = getX();
	double y_pos = getY();
	if (World()->isPeachAt(x_pos, y_pos)) {
		if (World()->kickEnemy(this)) {
			if (introducesShell() && !isAlive()) {
				Shell* shell = new Shell(World(), x_pos, y_pos, getDirection());
				World()->addActor(shell);
				return;
			}
		}
	}
}

bool Enemy::isDamageable() const {
	if (isAlive()) { 
		return true; 
	}
	else {
		return false;
	}
}

void Enemy::damage() {
	World()->increaseScore(100);
	setDead();
	if (introducesShell() && !isAlive()) {
		Shell* shell = new Shell(World(), getX(), getY(), getDirection());
		World()->addActor(shell);
		return;
	}
}

void Piranha::doSomething() {
	if (!isAlive()) {
		return;
	}
	increaseAnimationNumber();
	double x_pos = getX();
	double y_pos = getY();
	if (World()->isPeachAt(x_pos, y_pos)) {
		World()->bonkObject(x_pos, y_pos);
		return;
	}
	if (!World()->checkPeachLevel(getY())) {
		return;
	}
	else {
		setDirection(World()->checkPeachDirection(getX()));
	}
	if (m_firing_delay > 0) {
		m_firing_delay--;
		return;
	}
	else if (m_firing_delay <= 0) {
		if (getDirection() == 0) {
			if (World()->distanceBetween(getX(), 8, 'r')) {
				PiranhaFireball* f = new PiranhaFireball(World(), getX(), getY(), getDirection());
				World()->addActor(f);
				World()->playSound(SOUND_PIRANHA_FIRE);
				m_firing_delay = 40;
			}
		}
		else {
			if (World()->distanceBetween(getX(), 8, 'l')) {
				PiranhaFireball* f = new PiranhaFireball(World(), getX(), getY(), getDirection());
				World()->addActor(f);
				World()->playSound(SOUND_PIRANHA_FIRE); 
				m_firing_delay = 40;
			}
		}	
	}
}

Projectile::Projectile(StudentWorld* world, int start_x, int start_y, int dir, int id) : Actor(world, id, start_x, start_y, dir, 1) {}

PiranhaFireball::PiranhaFireball(StudentWorld* world, int start_x, int start_y, int dir) : Projectile(world, start_x, start_y, dir, IID_PIRANHA_FIRE) {}

PeachFireball::PeachFireball(StudentWorld* world, int start_x, int start_y, int dir) : Projectile(world, start_x, start_y, dir, IID_PEACH_FIRE) {}

Shell::Shell(StudentWorld* world, int start_x, int start_y, int dir) : Projectile(world, start_x, start_y, dir, IID_SHELL) {}

bool Projectile::overlapsWithDamageable(double& x, double& y) const {
	return World()->isDamageableObjectAt(x, y);
}

bool PiranhaFireball::overlapsWithDamageable(double& x, double& y) const {
	return World()->isPeachAt(x, y);
}

void Projectile::doSomething() {
	double x_pos = getX();
	double y_pos = getY();

	if (overlapsWithDamageable(x_pos, y_pos)) {
		World()->damageObject(x_pos, y_pos);
		setDead();
		return;
	}

	double x_pos2 = getX();
	double y_pos2 = getY() - 2;
	World()->moveIfPossible(this, x_pos2, y_pos2);

	double x_pos3 = getX();
	double y_pos3 = getY();
	if (getDirection() == 0) {
		x_pos3 += 2;
	}
	else if (getDirection() == 180) {
		x_pos3 -= 2;
	}
	if (!World()->moveIfPossible(this, x_pos3, y_pos3)) {
		setDead();
		return;
	}
}