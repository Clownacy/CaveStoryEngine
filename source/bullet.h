#pragma once
#include "common.h"
#include "weapons.h"

struct BULLETSTATS
{
	int8_t damage;
	int8_t life;
	int life_count;
	int bbits;

	int enemyXL;
	int enemyYL;
	int blockXL;
	int blockYL;

	RECT view;
};

class bullet
{
public:
	int flag;
	int code_bullet;
	int bbits;
	int cond;

	int x;
	int y;

	int xm;
	int ym;

	int tgt_x;
	int tgt_y;

	int act_no;
	int act_wait;

	int ani_wait;
	int ani_no;
	uint8_t direct;

	RECT rect;

	int count1;
	int count2;

	int life_count;
	int damage;
	int life;

	int enemyXL;
	int enemyYL;
	int blockXL;
	int blockYL;

	RECT view;

public:
	void init(int setCode, int setX, int setY, uint8_t setDir);

	void update();
};

typedef void(*bulletAct)(bullet *);

extern std::vector<bullet> bullets;
