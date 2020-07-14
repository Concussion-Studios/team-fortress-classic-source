//============= Copyright Concussion Studios, All rights reserved. =============//
//
// Purpose:
//
//=============================================================================//
#ifndef WEAPON_TFC_SUPER_NAILGUN_H
#define WEAPON_TFC_SUPER_NAILGUN_H
#ifdef _WIN32
#pragma once
#endif

#include "weapon_tfc_nailgun.h"

#if defined( CLIENT_DLL )
	#define CTFCSuperNailgun C_TFCSuperNailgun
#endif

// ----------------------------------------------------------------------------- //
// CTFCSuperNailgun class definition.
// ----------------------------------------------------------------------------- //
class CTFCSuperNailgun : public CTFCNailgun
{
public:
	DECLARE_CLASS( CTFCSuperNailgun, CTFCNailgun );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
#ifndef CLIENT_DLL
	DECLARE_DATADESC();
#endif
	
	CTFCSuperNailgun();

	virtual TFCWeaponID GetWeaponID( void ) const { return TF_WEAPON_SUPER_NAILGUN; }
	virtual void PrimaryAttack();

private:
	
	CTFCSuperNailgun( const CTFCSuperNailgun & ) {}
};
#endif // WEAPON_TFC_SUPER_NAILGUN_H
