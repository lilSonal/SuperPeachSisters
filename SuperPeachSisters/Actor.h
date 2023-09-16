#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"


class StudentWorld;

class Actor : public GraphObject {
	public:
		Actor(StudentWorld* world, int id, int start_x, int start_y, int start_dir, int depth);
		virtual void doSomething() {};
		bool isAlive() const { return m_isAlive; }
		void setDead() { m_isAlive = false; }
		StudentWorld* World() const { return m_world;  }
		virtual void bonk() {};
		virtual void damage() {};
		virtual bool isDamageable() const { return false; }
		virtual bool blocks() const { return false; }

	private:
		bool m_isAlive;
		StudentWorld* m_world;
};

class Peach : public Actor {
	public: 
		Peach(StudentWorld* world, int start_x, int start_y);
		void doSomething();
		void bonk();
		bool hasStar() const { return m_star; }
		bool hasShoot() const { return m_shoot; }
		bool hasJump() const { return m_jump; }
		bool isDamageable() const { return true; }
		void damage() { bonk(); }
		void giveStar(int invincible_time) { m_star = true; m_invincible = true; m_invincible_time = invincible_time; }
		void giveJump() { m_jump = true; }
		void giveShoot() { m_shoot = true; }
		void setHealth(int pts) { m_health = pts; }

	private:
		int m_health;
		int m_invincible_time;
		int m_temp_invincible_time;
		int m_recharge_time;
		int m_jump_distance;
		bool m_invincible;
		bool m_temp_invincible;
		bool m_star;
		bool m_shoot;
		bool m_jump;
		bool m_jumping;

};

class Ground : public Actor {
	public: 
		Ground(StudentWorld* world, int start_x, int start_y, int id);
		bool blocks() const { return true; }
};

class Block : public Ground {
	public:
		Block(StudentWorld* world, int start_x, int start_y, char goodie);
		void bonk();
	
	private:
		char m_goodie;
		bool m_released;
};

class Pipe : public Ground {
	public: 
		Pipe(StudentWorld* world, int start_x, int start_y);
};

class Goal : public Actor {
	public:
		Goal(StudentWorld* world, int start_x, int start_y, int id);
		
	private:
		void doSomething();
		virtual void informWorld() const = 0;
};

class Flag : public Goal {
	public:
		Flag(StudentWorld* world, int start_x, int start_y);

	private:
		void informWorld() const;

};

class Mario : public Goal {
	public:
		Mario(StudentWorld* world, int start_x, int start_y);

	private:
		void informWorld() const;
};

class Goodie : public Actor {
	public:
		Goodie(StudentWorld* world, int start_x, int start_y, int id, int pts);
		
	private:
		void doSomething();
		virtual void informPeach() const = 0;
		virtual void incHit() const;
		void incPoints() const;
		int m_pts;	
};

class Flower : public Goodie {
	public:
		Flower(StudentWorld* world, int start_x, int start_y);

	public:
		void informPeach() const;
};

class Mushroom : public Goodie {
	public:
		Mushroom(StudentWorld* world, int start_x, int start_y);

	private:
		void informPeach() const;
};

class Star : public Goodie {
	public:
		Star(StudentWorld* world, int start_x, int start_y);

	private:
		void informPeach() const;
		void incHit() const {};
};

class Enemy : public Actor {
	public:
		Enemy(StudentWorld* world, int start_x, int start_y, int id);
		void bonk();
		bool isDamageable() const;
		void damage();

	private:
		virtual void doSomething();
		virtual bool introducesShell() const { return false; }
};

class Goomba : public Enemy {
	public:
		Goomba(StudentWorld* world, int start_x, int start_y);
};

class Koopa : public Enemy {
	public:
		Koopa(StudentWorld* world, int start_x, int start_y);

	private:
		bool introducesShell() const { return true; }
		
};

class Piranha : public Enemy {
	public:
		Piranha(StudentWorld* world, int start_x, int start_y);
		
	private:
		void doSomething();
		int m_firing_delay;
};

class Projectile : public Actor {
	public:
		Projectile(StudentWorld* world, int start_x, int start_y, int dir, int id);

	private:
		void doSomething();
		virtual bool overlapsWithDamageable(double& x, double& y) const;

};

class PiranhaFireball : public Projectile {
	public:
		PiranhaFireball(StudentWorld* world, int start_x, int start_y, int dir);

	private:
		bool overlapsWithDamageable(double& x, double& y) const;
};

class PeachFireball : public Projectile {
	public:
		PeachFireball(StudentWorld* world, int start_x, int start_y, int dir);
};

class Shell : public Projectile {
	public:
		Shell(StudentWorld* world, int start_x, int start_y, int dir);
};








// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_