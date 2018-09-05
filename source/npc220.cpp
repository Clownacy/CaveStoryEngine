#include "npc220.h"

#include <vector>
#include "player.h"
#include "sound.h"
#include "mathUtils.h"
#include "caret.h"

using std::vector;

void npcAct220(npc *NPC) // Shovel Brigade, standing
{
	vector<RECT> rcLeft = { {0, 64, 16, 80}, {16, 64, 32, 80} };
	vector<RECT> rcRight = { {0, 80, 16, 96}, {16, 80, 32, 96} };

	enum
	{
		aniStanding = 0,
		aniBlinking = 1,
	};

	enum
	{
		init = 0,
		standing = 1,
		blinking = 2,
	};

	switch (NPC->act_no)
	{
	case init:
		NPC->act_no = standing;
		NPC->ani_no = aniStanding;
		NPC->ani_wait = 0;
		// Fallthrough
	case standing:
		if (!random(0, 120))
		{
			NPC->act_no = blinking;
			NPC->act_wait = 0;
			NPC->ani_no = aniBlinking;
		}
		break;

	case blinking:
		if (++NPC->act_wait > 8)
		{
			NPC->act_no = standing;
			NPC->ani_no = aniStanding;
		}
	}

	NPC->doRects(rcLeft, rcRight);
}

void npcAct222(npc *NPC) // Prison bars
{
	if (!NPC->act_no)
	{
		++NPC->act_no;
		NPC->y -= 0x1000;
	}

	NPC->rect = { 96, 168, 112, 200 };
}

void npcAct223(npc *NPC) // Momorin
{
    vector<RECT> rcLeft = {{80, 192, 96, 216}, {96, 192, 112, 216}, {112, 192, 128, 216}};
    vector<RECT> rcRight = {{80, 216, 96, 240}, {96, 216, 112, 240}, {112, 216, 128, 240}};

    switch (NPC->act_no)
    {
    case 0:
        NPC->act_no = 1;
        NPC->ani_no = 0;
        NPC->ani_wait = 0;
        // Fallthrough
    case 1:
        if (!random(0, 160))
        {
            NPC->act_no = 2;
            NPC->act_wait = 0;
            NPC->ani_no = 1;
        }
        break;

    case 2:
        if (++NPC->act_wait > 12)
        {
            NPC->act_no = 1;
            NPC->ani_no = 0;
        }
        break;

    case 3:
        NPC->ani_no = 2;
        break;

    default:
        break;
    }

    if (NPC->act_no < 2 && currentPlayer.y < NPC->y + 0x2000 && currentPlayer.y > NPC->y - 0x2000)
        NPC->facePlayer();

    NPC->doRects(rcLeft, rcRight);
}

void npcAct227(npc *NPC) // Bucket
{
	NPC->rect = { 208, 32, 224, 48 };
}

void npcAct229(npc *NPC) // Red Flowers, small
{
	if (!NPC->act_no)
	{
		NPC->act_no = 1;
		NPC->y -= 0x2000;
	}
	if (NPC->direct)
		NPC->rect = { 0, 112, 59, 129 };
	else
		NPC->rect = { 0, 96, 48, 112 };
}

void npcAct231(npc *NPC) //Momorin's rocket
{
	RECT rcNPC[2];

	rcNPC[0] = { 176, 32, 208, 48 };
	rcNPC[1] = { 176, 48, 208, 64 };

	switch (NPC->act_no)
	{
	case 0:
		NPC->act_no = 1;
		NPC->ani_no = 0;
		break;

	case 1:
		NPC->ani_no = 0;
		break;

	case 10:
		NPC->act_no = 11;
		NPC->act_wait = 0;
	// fallthrough
	case 11:
		++NPC->act_wait;
		NPC->ym += 8;
		if (NPC->flag & ground)
		{
			if (NPC->act_wait > 9)
				NPC->act_no = 1;
			else
				NPC->act_no = 12;
		}
		break;

	case 12:
		NPC->bits &= ~npc_interact;

		NPC->act_no = 13;
		NPC->act_wait = 0;
		NPC->ani_no = 1;

		for (int i = 0; i < 10; ++i)
		{
			createNpc(NPC_Smoke, NPC->x + (random(-16, 16) << 9), NPC->y + (random(-8, 8) << 9));
			playSound(SFX_DestroyBreakableBlock);
		}
	// fallthrough
	case 13:
		NPC->ym -= 8;

		if (!(++NPC->act_wait & 1))
			createCaret(NPC->x - 0x1400, NPC->y + 0x1000, effect_BoosterSmoke, dirDown);

		if (NPC->act_wait % 2 == 1)
			createCaret(NPC->x + 0x1400, NPC->y + 0x1000, effect_BoosterSmoke, dirDown);

		if (NPC->act_wait % 4 == 1)
			playSound(SFX_FireballBounce);

		if (NPC->flag & ceiling || currentPlayer.flag & ceiling || NPC->act_wait > 450)
		{
			if (NPC->flag & ceiling || currentPlayer.flag & ceiling)
				NPC->ym = 0;

			NPC->act_no = 15;

			for (int i = 0; i < 6; ++i)
			{
				createNpc(NPC_Smoke, NPC->x + (random(-16, 16) << 9), NPC->y + (random(-8, 8) << 9));
				playSound(SFX_DestroyBreakableBlock);
			}
		}
		break;

	case 0xF:
		NPC->ym += 8;
		++NPC->act_wait;

		if (NPC->ym < 0)
		{
			if (!(NPC->act_wait & 7))
				createCaret(NPC->x - 0x1400, NPC->y + 0x1000, effect_BoosterSmoke, dirDown);

			if (NPC->act_wait % 8 == 4)
				createCaret(NPC->x + 0x1400, NPC->y + 0x1000, effect_BoosterSmoke, dirDown);

			if (NPC->act_wait % 16 == 1)
				playSound(SFX_FireballBounce);
		}

		if (NPC->flag & ground)
		{
			NPC->bits |= npc_interact;

			NPC->act_no = 1;
			NPC->ani_no = 0;
		}
		break;
	default:
		break;
	}

	if (NPC->ym < -0x5FF)
		NPC->ym = -0x5FF;
	if (NPC->ym > 0x5FF)
		NPC->ym = 0x5FF;

	NPC->y += NPC->ym;

	NPC->rect = rcNPC[NPC->ani_no];
}

void npcAct234(npc * NPC) // Red Flowers, picked
{
	if (!NPC->act_no)
	{
		NPC->act_no = 1;
		NPC->y += 0x2000;
	}

	if (NPC->direct != dirLeft)
		NPC->rect = { 144, 112, 192, 128 };
	else
		NPC->rect = { 144, 96, 192, 112 };
}

void npcAct238(npc *NPC) //Killer press
{
	RECT rcNPC[3];

	rcNPC[0] = { 184, 200, 208, 216 };
	rcNPC[1] = { 208, 200, 232, 216 };
	rcNPC[2] = { 232, 200, 256, 216 };

	if (!NPC->act_no)
	{
		NPC->act_no = 1;
		NPC->tgt_x = NPC->x;
		NPC->tgt_y = NPC->y;
		NPC->view.left = 0x2000;
		NPC->view.right = 0x1000;
	}

	switch (NPC->act_no)
	{
	case 1:
		if (NPC->direct == dirLeft && currentPlayer.x < NPC->x && currentPlayer.x > NPC->x - 0x18000 && currentPlayer.y > NPC->y - 0x800 && currentPlayer.y < NPC->y + 0x1000)
		{
			NPC->act_no = 10;
			NPC->act_wait = 0;
			NPC->ani_no = 2;
		}

		if (NPC->direct == dirRight && currentPlayer.x > NPC->x && currentPlayer.x < NPC->x + 0x18000 && currentPlayer.y > NPC->y - 0x800 && currentPlayer.y < NPC->y + 0x1000)
		{
			NPC->act_no = 10;
			NPC->act_wait = 0;
			NPC->ani_no = 2;
		}

		break;

	case 10:
		NPC->damage = 127;

		if (NPC->direct != dirLeft)
			NPC->x += 0xC00;
		else
			NPC->x -= 0xC00;

		if (++NPC->act_wait == 8)
		{
			NPC->act_no = 20;
			NPC->act_wait = 0;

			for (int i = 0; i < 4; ++i)
			{
				createNpc(NPC_Smoke, NPC->x + (random(-16, 16) << 9), NPC->y + (random(-8, 8) << 9));
				playSound(SFX_DestroyBreakableBlock);
			}
		}
		break;

	case 20:
		NPC->damage = 0;

		if (++NPC->act_wait > 50)
		{
			NPC->act_wait = 0;
			NPC->act_no = 30;
		}

		break;

	case 30:
		NPC->damage = 0;
		NPC->ani_no = 1;

		if (++NPC->act_wait == 12)
		{
			NPC->act_no = 1;
			NPC->act_wait = 0;
			NPC->ani_no = 0;
		}

		if (NPC->direct != dirLeft)
			NPC->x -= 0x800;
		else
			NPC->x += 0x800;

		break;

	default:
		break;
	}

	if (NPC->direct != dirLeft || currentPlayer.x >= NPC->x)
	{
		if (NPC->direct != dirRight || currentPlayer.x <= NPC->x)
			NPC->hit.right = 0x1000;
		else
			NPC->hit.right = 0x2000;
	}
	else
		NPC->hit.right = 0x2000;

	NPC->rect = rcNPC[NPC->ani_no];
}

void npcAct239(npc *NPC) // Cage bars
{
    if (!NPC->act_no)
    {
        NPC->act_no = 1;
        if (NPC->direct != dirLeft)
        {
            NPC->view.left = 0x3000;
            NPC->view.right = 0x3000;
            NPC->view.top = 0x1000;
        }
        else
        {
            NPC->x += 0x1000;
            NPC->y += 0x2000;
        }
    }

    NPC->doRects({{192, 48, 256, 80}}, {{96, 112, 144, 144}});
}
