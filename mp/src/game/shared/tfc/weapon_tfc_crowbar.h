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

	CTFCCrowbar();

	virtual TFCWeaponID GetWeaponID( void ) const OVERRIDE { return TF_WEAPON_CROWBAR; }

	void SecondaryAttack( void ) OVERRIDE { return; }

	virtual float GetRange( void ) OVERRIDE { return 75.0f; }
	virtual float GetFireRate( void ) OVERRIDE { return 0.4f; }
	virtual void AddViewKick( void ) OVERRIDE;
	virtual float GetDamageForActivity( Activity hitActivity ) OVERRIDE { return 25.0f; };

private:
	CTFCCrowbar( const CTFCCrowbar & ) {}
};

#endif // WEAPON_TFC_CROWBAR_H