#include "cbase.h"
#include "hud.h"
#include "hud_macros.h"
#include "hudelement.h"
#include "hud_numericdisplay.h"
#include <vgui/ISurface.h>
#include "c_team.h"
#include "hl2mp_gamerules.h"
#include "iclientmode.h"
#include <vgui_controls/AnimationController.h>

#include "tier0/memdbgon.h"

#define DISPLAY_TIME 0.8f

using namespace vgui;

class CHudDamageNumber : public CHudNumericDisplay, public CHudElement
{
	DECLARE_CLASS_SIMPLE(CHudDamageNumber, CHudNumericDisplay);

public:
	CHudDamageNumber(const char *pElementName);
	void Init(void);
	void Update(void);
	void FireGameEvent(IGameEvent *event);
	void DisplayDamage(C_BasePlayer *victim, int damage);

protected:
	virtual void OnThink();
	int lastDamage;
	float lastTime;
};

DECLARE_HUDELEMENT(CHudDamageNumber);

CHudDamageNumber::CHudDamageNumber(const char *pElementName) : BaseClass(NULL, "HudDamageNumber"), CHudElement(pElementName) {
	ListenForGameEvent("player_hurt");
	SetHiddenBits(HIDEHUD_PLAYERDEAD);	
	SetPaintEnabled(true);
	SetPaintBackgroundEnabled(false);
}

void CHudDamageNumber::DisplayDamage(C_BasePlayer *victim, int damage) {
	lastDamage = damage;

	lastTime = gpGlobals->curtime;

	g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("DamageNumberDisplay");
}

void CHudDamageNumber::FireGameEvent(IGameEvent *event)
{
	if (FStrEq(event->GetName(), "player_hurt")) {
		const int iDamage = event->GetInt("damage");
		//const int iHealth = event->GetInt("health");
		const int iAttacker = engine->GetPlayerForUserID(event->GetInt("attacker"));
		const int iVictim = engine->GetPlayerForUserID(event->GetInt("userid"));

		C_BasePlayer *attacker = UTIL_PlayerByIndex(iAttacker);
		C_BasePlayer *player = C_BasePlayer::GetLocalPlayer();
		C_BasePlayer *victim = UTIL_PlayerByIndex(iVictim);

		if (attacker != player) return;
		if (!player || !victim) return;

		DisplayDamage(victim, iDamage);
	}
}

void CHudDamageNumber::Init(void) {
	lastDamage = -1;
}

void CHudDamageNumber::Update(void) {
	SetDisplayValue(lastDamage);
	if (gpGlobals->curtime < lastTime + DISPLAY_TIME)
		SetVisible(true);
	else
		SetVisible(false);
}

//-----------------------------------------------------------------------------
// Purpose: called every frame to get capture info from the server
//-----------------------------------------------------------------------------
void CHudDamageNumber::OnThink()
{
	Update();
}