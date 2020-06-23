//============= Copyright Concussion Studios, All rights reserved. =============//
//
// Purpose:
//
//=============================================================================//
#ifndef WEAPON_TFC_CROWBAR_H
#define WEAPON_TFC_CROWBAR_H
#ifdef _WIN32
#pragma once
#endif

#include "weapon_tfcbasemelee.h"

#if defined( CLIENT_DLL )
	#define CTFCCrowbar C_TFCCrowbar
#endif

// ----------------------------------------------------------------------------- //
// CTFCCrowbar class definition.
// ----------------------------------------------------------------------------- //
class CTFCCrowbar : public CWeaponTFCBaseMelee
{
public:
	DECLARE_CLASS( CTFCCrowbar, CWeaponTFCBaseMelee );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();	
#ifndef CLIENT_DLL
	DECLARE_DATADESC();
#endif

	CTFCCrowbar();

	virtual TFCWeaponID GetWeaponID( void ) const { return WEAPON_CROWBAR; }

// Overrideables.
public:
#ifdef GAME_DLL
	// This is called first to determine if the axe should apply damage to the entity.
	virtual void AxeHit( CBaseEntity *pHit, bool bFirstSwing, trace_t &tr, float *flDamage, bool *bDoEffects ) OVERRIDE;
#endif

private:
	CTFCCrowbar( const CTFCCrowbar & ) {}
};

#endif // WEAPON_TFC_CROWBAR_H