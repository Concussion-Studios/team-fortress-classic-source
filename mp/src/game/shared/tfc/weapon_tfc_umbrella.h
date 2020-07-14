//============= Copyright Concussion Studios, All rights reserved. =============//
//
// Purpose:
//
//=============================================================================//
#ifndef WEAPON_TFC_UMBRELLA_H
#define WEAPON_TFC_UMBRELLA_H
#ifdef _WIN32
#pragma once
#endif

#include "weapon_tfc_crowbar.h"

#if defined( CLIENT_DLL )
	#define CTFCUmbrella C_TFCUmbrella
#endif

// ----------------------------------------------------------------------------- //
// CTFCUmbrella class definition.
// ----------------------------------------------------------------------------- //
class CTFCUmbrella : public CTFCCrowbar
{
public:
	DECLARE_CLASS( CTFCUmbrella, CTFCCrowbar );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();	
	
	CTFCUmbrella();

	virtual TFCWeaponID GetWeaponID( void ) const { return TF_WEAPON_UMBRELLA; }

private:
	CTFCUmbrella( const CTFCUmbrella & ) {}
};
#endif // WEAPON_TFC_Umbrella_H