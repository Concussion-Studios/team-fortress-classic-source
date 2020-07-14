//============= Copyright Concussion Studios, All rights reserved. =============//
//
// Purpose:
//
//=============================================================================//
#ifndef WEAPON_TFC_NAILGUN_H
#define WEAPON_TFC_NAILGUN_H
#ifdef _WIN32
#pragma once
#endif

#include "weapon_tfcbase.h"

#if defined( CLIENT_DLL )
	#define CTFCNailgun C_TFCNailgun
#endif

// ----------------------------------------------------------------------------- //
// CTFCNailgun class definition.
// ----------------------------------------------------------------------------- //
class CTFCNailgun : public CWeaponTFCBase
{
public:
	DECLARE_CLASS( CTFCNailgun, CWeaponTFCBase );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();	
#ifndef CLIENT_DLL
	DECLARE_DATADESC();
#endif
	
	CTFCNailgun();

	virtual TFCWeaponID GetWeaponID( void ) const { return TF_WEAPON_NAILGUN; }
	virtual void PrimaryAttack();
	virtual void SecondaryAttack() { return; }

private:
	
	CTFCNailgun( const CTFCNailgun & ) {}

};
#endif // WEAPON_TFC_NAILGUN_H