//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef WEAPON_ROCKET_H
#define WEAPON_ROCKET_H

#ifdef _WIN32
#pragma once
#endif

#include "weapon_hl2mpbasehlmpcombatweapon.h"

#ifdef CLIENT_DLL

	#include "iviewrender_beams.h"

#endif

#ifndef CLIENT_DLL
#include "Sprite.h"

class CWeaponRocket;
class RocketTrail;
 
//###########################################################################
//	>> CRocketMissile		(missile launcher class is below this one!)
//###########################################################################
class CRocketMissile : public CBaseCombatCharacter
{
	DECLARE_CLASS( CRocketMissile, CBaseCombatCharacter );

public:
	CRocketMissile();
	~CRocketMissile();

#ifdef HL1_DLL
	Class_T Classify( void ) { return CLASS_NONE; }
#else
	Class_T Classify( void ) { return CLASS_MISSILE; }
#endif
	
	void	Spawn( void );
	void	Precache( void );
	void	MissileTouch( CBaseEntity *pOther );
	void	Explode( void );
	void	IgniteThink( void );
	void	FlyThink( void );
	void	DumbFire( void );
	void	SetGracePeriod( float flGracePeriod );
	
	virtual float	GetDamage() { return m_flDamage; }
	virtual void	SetDamage(float flDamage) { m_flDamage = flDamage; }

	unsigned int PhysicsSolidMaskForEntity( void ) const;

	CHandle<CWeaponRocket>		m_hOwner;

	static CRocketMissile *Create( const Vector &vecOrigin, const QAngle &vecAngles, edict_t *pentOwner );

protected:
	virtual void DoExplosion();	

	// Creates the smoke trail
	void CreateSmokeTrail( void );

	CHandle<RocketTrail>	m_hRocketTrail;
	float					m_flMarkDeadTime;
	float					m_flDamage;

private:
	float					m_flGracePeriodEndsAt;

	DECLARE_DATADESC();
};


#endif

//-----------------------------------------------------------------------------
// RPG
//-----------------------------------------------------------------------------

#ifdef CLIENT_DLL
#define CWeaponRocket C_WeaponRocket
#endif

class CWeaponRocket : public CBaseHL2MPCombatWeapon
{
	DECLARE_CLASS( CWeaponRocket, CBaseHL2MPCombatWeapon );
public:

	CWeaponRocket();
	~CWeaponRocket();

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

	void	Precache( void );

	void	PrimaryAttack( void );
	virtual float GetFireRate( void ) { return 1; };
	void	ItemPostFrame( void );

	void	DecrementAmmo( CBaseCombatCharacter *pOwner );

	bool	Deploy( void );
	bool	Holster( CBaseCombatWeapon *pSwitchingTo = NULL );
	bool	Reload( void );
	bool	WeaponShouldBeLowered( void );
	bool	Lower( void );

	bool	CanHolster( void );

	virtual void Drop( const Vector &vecVelocity );

	int		GetMinBurst() { return 1; }
	int		GetMaxBurst() { return 1; }
	float	GetMinRestTime() { return 4.0; }
	float	GetMaxRestTime() { return 4.0; }

	void	NotifyRocketDied( void );

	bool	HasAnyAmmo( void );

#ifdef CLIENT_DLL

	// We need to render opaque and translucent pieces
	virtual RenderGroup_t	GetRenderGroup( void ) {	return RENDER_GROUP_TWOPASS;	}

	virtual void	NotifyShouldTransmit( ShouldTransmitState_t state );
	virtual int		DrawModel( int flags );
	virtual void	ViewModelDrawn( C_BaseViewModel *pBaseViewModel );
	virtual bool	IsTranslucent( void );

	void			GetWeaponAttachment( int attachmentId, Vector &outVector, Vector *dir = NULL );

#endif	//CLIENT_DLL

	CBaseEntity *GetMissile(void) { return m_hRocketMissile; }

#ifndef CLIENT_DLL
	DECLARE_ACTTABLE();
#endif
	
protected:

	CNetworkVar( bool, m_bInitialStateUpdate );
	CNetworkVar( bool, m_bGuiding );
	CNetworkVar( bool, m_bHideGuiding );

#ifndef CLIENT_DLL
	//CHandle<CRocketLaserDot>	m_hLaserDot;
	//CHandle<CRocketMissile>	m_hRocketMissile;
#endif
	CNetworkHandle(CBaseEntity, m_hRocketMissile);

private:
	
	CWeaponRocket( const CWeaponRocket & );
};

#endif // WEAPON_ROCKET_H
