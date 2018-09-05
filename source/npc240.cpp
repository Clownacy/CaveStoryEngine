#include "npc240.h"

#include <vector>
#include "render.h"
#include "player.h"
#include "level.h"
#include "caret.h"
#include "sound.h"
#include "mathUtils.h"
#include "render.h"
#include "sound.h"

using std::vector;

void npcAct243(npc *NPC) // Generator - Red Bat
{
	if (NPC->act_no)
	{
		if (NPC->act_no != 1)
			return;
	}
	else
	{
		NPC->act_no = 1;
		NPC->act_wait = random(0, 500);
	}
	if (NPC->act_wait)
		--NPC->act_wait;
	else
	{
		NPC->act_no = 0;
		createNpc(NPC_EnemyBatRed, NPC->x, NPC->y + pixelsToUnits(random(-0x20, 0x20)), 0, 0, NPC->direct);
	}
}

void npcAct244(npc *NPC)
{
	NPC->ym += 0x40;
	bool die = false;
	if (NPC->flag & 0xFF || (NPC->act_wait > 10 && NPC->flag & 0x100))
		die = true;

	if (die)
	{
		for (int i = 0; i < 3; ++i)
			createCaret(NPC->x, NPC->y + pixelsToUnits(4), effect_fountainDisk, dirRight);
		if (NPC->x > currentPlayer.x - tilesToUnits(16)
		        && NPC->x < currentPlayer.x + tilesToUnits(16)
		        && NPC->y > currentPlayer.y - tilesToUnits(10)
		        && NPC->y < currentPlayer.y + tilesToUnits(10))
			playSound(SFX_Bubble);
        NPC->cond = 0;
	}
	else
    {
        if (NPC->ym > 0x5FF)
            NPC->ym = 0x5FF;
        NPC->y += NPC->ym;

        NPC->rect = {96, 0, 104, 16};
    }
}

void npcAct245(npc *NPC) // Generator - Lava Drop
{
	vector<RECT> rcNPC = {{0, 0, 0, 0}, {104, 0, 112, 16}, {112, 0, 120, 16}, {120, 0, 128, 16}};

	if (NPC->act_no)
	{
		if (NPC->act_no != 1)
		{
			if (NPC->act_no == 10)
			{
				if (++NPC->ani_wait > 10)
				{
					NPC->ani_wait = 0;
                    ++NPC->ani_no;
				}
				if (NPC->ani_no > 3)
				{
					NPC->ani_no = 0;
					NPC->act_no = 1;
                    NPC->act_wait = NPC->code_flag;
                    createNpc(NPC_AcidDrop, NPC->x, NPC->y);
				}
			}
			goto endOfAI;
		}
	}
	else
    {
        NPC->act_no = 1;
        NPC->tgt_x = NPC->x;
        NPC->act_wait = NPC->code_event;
    }

    NPC->ani_no = 0;
    if (NPC->act_wait)
    {
        --NPC->act_wait;
        return;
    }
    NPC->act_no = 10;
    NPC->act_wait = 0;

endOfAI:
    if (NPC->act_wait / 2 % 2)
        NPC->x = NPC->tgt_x;
    else
        NPC->x = NPC->tgt_x + 0x200;

    NPC->doRects(rcNPC);
}

void npcAct253(npc *NPC) // Energy Capsule
{
	if (!NPC->act_no)
		NPC->act_no = 1;
	if (NPC->act_no == 1)
		NPC->animate(4, 0, 1);

	if (NPC->life <= 100)
	{
		dropExperience(NPC->x, NPC->y, NPC->code_flag);
		createSmokeLeft(NPC->x, NPC->y, NPC->view.right, 8);
		playSound(SFX_SillyExplosion);
		NPC->cond = 0;
	}

	NPC->doRects({ {0, 64, 16, 80}, {16, 64, 32, 80} });
}

void npcAct254(npc *NPC) // Helicopter
{
    enum
    {
        stopped = 0,
        bladesRunning = 20,
        momorinBladesRunning = 30,
        creditsBladesRunning = 40
    };

    switch (NPC->act_no)
    {
    case stopped:
        NPC->act_no = stopped + 1;
        createNpc(NPC_HelicopterBlades, NPC->x + pixelsToUnits(18), NPC->y - pixelsToUnits(57), 0, 0, dirLeft, NPC);
        createNpc(NPC_HelicopterBlades, NPC->x - pixelsToUnits(32), NPC->y - pixelsToUnits(52), 0, 0, dirRight, NPC);
        break;

    case bladesRunning:
        NPC->act_wait = 0;
        NPC->count1 = 60;
        NPC->act_no = bladesRunning + 1;
        break;

    case momorinBladesRunning:
        NPC->act_no = bladesRunning + 1;
        createNpc(NPC_Momorin, NPC->x - pixelsToUnits(11), NPC->y - pixelsToUnits(14));
        break;

    case creditsBladesRunning:
        NPC->act_no = bladesRunning + 1;
        createNpc(NPC_Momorin, NPC->x - pixelsToUnits(9), NPC->y - pixelsToUnits(14));
        createNpc(NPC_Santa, NPC->x - pixelsToUnits(22), NPC->y - pixelsToUnits(14));
        createNpc(NPC_Chaco, NPC->x - pixelsToUnits(35), NPC->y - pixelsToUnits(14));
        break;

    default:
        break;
    }

    vector<RECT> rcLeft = {{0, 0, 128, 64}};
    vector<RECT> rcRight = {{0, 64, 128, 128}};
    NPC->doRects(rcLeft, rcRight);
}

void npcAct255(npc *NPC) // Helicopter Blades
{
    vector<RECT> rcLeft = {{128, 0, 240, 16}, {128, 16, 240, 32}, {128, 32, 240, 48}, {128, 16, 240, 32}};
    vector<RECT> rcRight = {{240, 0, 320, 16}, {240, 16, 320, 32}, {240, 32, 320, 48}, {240, 16, 320, 32}};

    switch (NPC->act_no)
    {
    case 0:
        NPC->act_no = 1;
        if (NPC->direct != dirLeft)
        {
            NPC->view.left = 0x5000;
            NPC->view.right = 0x5000;
        }
        else
        {
            NPC->view.left = 0x7000;
            NPC->view.right = 0x7000;
        }
        // Fallthrough
    case 1:
        if (NPC->pNpc && NPC->pNpc->act_no >= 20)
            NPC->act_no = 10;
        break;

    case 10:
        NPC->act_no = 11;
        // Fallthrough
    case 11:
        NPC->animate(0, 0, 3);
        break;

    default:
        break;
    }

    if (NPC->pNpc)
    {
        if (NPC->direct != dirLeft)
        {
            NPC->x = NPC->pNpc->x - tilesToUnits(2);
            NPC->y = NPC->pNpc->y - pixelsToUnits(52);
        }
        else
        {
            NPC->x = NPC->pNpc->x + pixelsToUnits(18);
            NPC->y = NPC->pNpc->y - pixelsToUnits(59);
        }
    }

    NPC->doRects(rcLeft, rcRight);
}

void npcAct259(npc *NPC) // Sleeping mimiga
{
	NPC->rect = { 48, 32, 64, 48 };
}
