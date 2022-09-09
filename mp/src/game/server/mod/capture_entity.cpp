#include "cbase.h"
#include "team.h"
#include "usermessages.h"
#include "hl2mp_gamerules.h"

class CCaptureEntity : public CBaseAnimating
{
public:
	DECLARE_CLASS(CCaptureEntity, CBaseAnimating);
	DECLARE_DATADESC();

	CCaptureEntity()
	{
		percentCapped = 0;
		percentCapped2 = 0;

		team1Capping = false;
		team2Capping = false;
	}

	void Spawn(void);
	void Precache(void);
	void Touch(CBaseEntity *pOther);
	void Think();
private:

	int percentCapped; // combine
	int percentCapped2; // rebels

	bool team1Capping;
	bool team2Capping;
};

LINK_ENTITY_TO_CLASS(capture_entity, CCaptureEntity);

// Start of our data description for the class
BEGIN_DATADESC(CCaptureEntity)
DEFINE_FIELD(percentCapped, FIELD_INTEGER),
END_DATADESC()

void CCaptureEntity::Spawn(void)
{
	Precache();
	SetModel("models/items/healthkit.mdl");
	SetSolid(SOLID_BBOX);
	AddSolidFlags(FSOLID_NOT_SOLID | FSOLID_TRIGGER);
	UTIL_SetSize(this, -Vector(50, 50, 20), Vector(50, 50, 50));

	//BaseClass::Spawn();

	SetTouch(&CCaptureEntity::Touch);
	SetNextThink(gpGlobals->curtime + 0.15f);
}

void CCaptureEntity::Think() {
	if (team1Capping)
		percentCapped+=2;
	else
		percentCapped--;

	if (team2Capping)
		percentCapped2+=2;
	else
		percentCapped2--;

	if (percentCapped >= 100) {
		GetGlobalTeam(TEAM_COMBINE)->AddScore(1);
		percentCapped = 0;
	}

	if (percentCapped2 >= 100) {
		GetGlobalTeam(TEAM_REBELS)->AddScore(1);
		percentCapped2 = 0;
	}

	if (percentCapped < 0)
		percentCapped = 0;

	if (percentCapped2 < 0)
		percentCapped2 = 0;

	// update HUD capture percents
	CReliableBroadcastRecipientFilter filter;
	UserMessageBegin(filter, "CapturePercent");
	WRITE_SHORT(percentCapped);
	WRITE_SHORT(percentCapped2);
	MessageEnd();

	team1Capping = false;
	team2Capping = false;
	SetNextThink(gpGlobals->curtime + 0.15f);
}

void CCaptureEntity::Touch(CBaseEntity *pOther) {
	// get player
	if (!pOther->IsPlayer()) return;

	CBasePlayer *player = dynamic_cast< CBasePlayer * >(pOther);
	if (player->GetTeamNumber() == TEAM_COMBINE)
		team1Capping = true;
	if (player->GetTeamNumber() == TEAM_REBELS)
		team2Capping = true;
}

void CCaptureEntity::Precache(void)
{
	PrecacheModel("models/items/healthkit.mdl");

	BaseClass::Precache();
}