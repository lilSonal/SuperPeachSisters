# Overview
This is intended to be a clone of the Super Mario Bros video game. Features include:
- 3 enemies (koopas, goombas, and piranhas that shoot fireballs)
- 3 lives for Peach -- she loses a life each time she contacts an enemy or fireball
- 3 levels of gameplay that must each be cleared to reach Mario
- 3 powerups (Star, Flower, Mushroom) which grant different superpowers. Star grants temporary invincibility from enemy attacks, Flower grants increased jumping height, and Mushroom grants the ability to shoot fireballs at enemies. If Peach shoots a fireball with the mushroom powerup that hits a koopa, the koopa will release a shell that destroys all enemies it contacts.

To code this video game, I used an object-oriented design with C++. I used three main classes: Enemy, PowerUp, and Projectile. Koopa, Goomba, and Piranha were sub-classes of Enemy. Star, Flower, and Mushroom were sub-classes of PowerUp. Shell, PiranhaFireball, and PeachFireball were sub-classes of Projectile.
