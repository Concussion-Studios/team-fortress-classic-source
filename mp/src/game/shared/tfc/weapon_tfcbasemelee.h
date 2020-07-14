//============= Copyright Concussion Studios, All rights reserved. =============//
//
// Purpose:
//
//=============================================================================//
#ifndef WEAPON_TFCBASE_MELEE_H
#define WEAPON_TFCBASE_MELEE_H
#ifdef _WIN32
#pragma once
#endif

#include "effect_dispatch_data.h"
#include "weapon_tfcbase.h"

#if defined( CLIENT_DLL )
	#define CWeaponTFCBaseMelee C_WeaponTFCBaseMelee
#endif

// ----------------------------------------------------------------------------- //
// class definition.
// ----------------------------------------------------------------------------- //
class CWeaponTFCBaseMelee : public CWeaponTFCBase
{
public:
	DECLARE_CLASS( CWeaponTFCBaseMelee, CWeaponTFCBase );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
#ifndef CLIENT_DLL
	DECLARE_DATADESC();
#endif

	CWeaponTFCBaseMelee();

	virtual void Spawn() OVERRIDE;
	virtual void PrimaryAttack() OVERRIDE;
	virtual void WeaponIdle() OVERRIDE;
	virtual void ItemPostFrame() OVERRIDE;
	virtual bool Holster( CBaseCombatWeapon *pSwitchingTo = NULL ) OVERRIDE;
	virtual bool HasPrimaryAmmo() OVERRIDE { return true; }
	virtual bool CanBeSelected() OVERRIDE { return true; }
	virtual void Smack();

	// Get specific TFC weapon ID (ie: WEAPON_AK47, etc)
	virtual TFCWeaponID GetWeaponID( void ) const {	Assert( false ); return TF_WEAPON_NONE; }

// Overrideables.
public:
#ifdef GAME_DLL
	// This is called first to determine if the axe should apply damage to the entity.
	virtual void AxeHit( CBaseEntity *pHit, bool bFirstSwing, trace_t &tr, float *flDamage, bool *bDoEffects ) { Assert(!"Derivate classes must implement this!."); }
#endif

public:
	trace_t m_trHit;
	EHANDLE m_pTraceHitEnt;
	float m_flStoredPrimaryAttack;

private:
	CWeaponTFCBaseMelee( const CWeaponTFCBaseMelee & ) {}
};
#endif // WEAPON_TFCBASE_MELEE_H