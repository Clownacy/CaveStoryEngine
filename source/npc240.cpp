#include "npc240.h"

#include <vector>
#include "render.h"
#include "player.h"
#include "level.h"
#include "caret.h"
#include "sound.h"

using std::vector;

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

void npcAct259(npc *NPC) // Sleeping mimiga
{
	NPC->rect = { 48, 32, 64, 48 };
}
