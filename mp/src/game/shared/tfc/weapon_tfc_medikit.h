//============= Copyright Concussion Studios, All rights reserved. =============//
//
// Purpose:
//
//=============================================================================//
#ifndef WEAPON_TFC_MEDIKIT_H
#define WEAPON_TFC_MEDIKIT_H
#ifdef _WIN32
#pragma once
#endif

#include "weapon_tfc_crowbar.h"

#if defined( CLIENT_DLL )
	#define CTFCMedikit C_TFCMedikit
#endif

// ----------------------------------------------------------------------------- //
// CTFCMedikit class definition.
// ----------------------------------------------------------------------------- //
class CTFCMedikit : public CTFCCrowbar
{
public:
	DECLARE_CLASS( CTFCMedikit, CTFCCrowbar );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
	
	CTFCMedikit();

	virtual TFCWeaponID GetWeaponID( void ) const { return TF_WEAPON_MEDIKIT; }

private:
	CTFCMedikit( const CTFCMedikit & ) {}
};
#endif // WEAPON_TFC_MEDIKIT_H