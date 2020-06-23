//============= Copyright Concussion Studios, All rights reserved. =============//
//
// Purpose:
//
//=============================================================================//
#ifndef WEAPON_TFC_GRENADELAUNCHER_H
#define WEAPON_TFC_GRENADELAUNCHER_H
#ifdef _WIN32
#pragma once
#endif

#include "weapon_tfcbase.h"

#if defined( CLIENT_DLL )
	#define CTFCGrenadeLauncher C_TFCGrenadeLauncher
#endif

#define GRENADE_DAMAGE 100.0f
#define GRENADE_RADIUS 250.0f
#define GRENADE_TIMER	2.5f //Seconds

// ----------------------------------------------------------------------------- //
// CTFCGrenadeLauncher class definition.
// ----------------------------------------------------------------------------- //
class CTFCGrenadeLauncher : public CWeaponTFCBase
{
public:
	DECLARE_CLASS( CTFCGrenadeLauncher, CWeaponTFCBase );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
#ifndef CLIENT_DLL
	DECLARE_DATADESC();
#endif

	CTFCGrenadeLauncher();

	virtual void	Precache( void );
	virtual void	PrimaryAttack( void );

	virtual TFCWeaponID GetWeaponID( void ) const { return WEAPON_GRENADELAUNCHER; }

protected:

	Vector	m_vecTossVelocity;
	float	m_flNextGrenadeCheck;
	
private:
	CTFCGrenadeLauncher( const CTFCGrenadeLauncher & ) {}
};

#endif // WEAPON_TFC_GRENADELAUNCHER_H