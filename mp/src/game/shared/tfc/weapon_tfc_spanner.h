//============= Copyright Concussion Studios, All rights reserved. =============//
//
// Purpose:
//
//=============================================================================//
#ifndef WEAPON_TFC_SPANNER_H
#define WEAPON_TFC_SPANNER_H
#ifdef _WIN32
#pragma once
#endif

#include "weapon_tfc_crowbar.h"

#if defined( CLIENT_DLL )
	#define CTFCSpanner C_TFCSpanner
#endif

// ----------------------------------------------------------------------------- //
// CTFCSpanner class definition.
// ----------------------------------------------------------------------------- //
class CTFCSpanner : public CTFCCrowbar
{
public:
	DECLARE_CLASS( CTFCSpanner, CTFCCrowbar );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

public:
	
	CTFCSpanner();

	virtual TFCWeaponID GetWeaponID( void ) const { return TF_WEAPON_SPANNER; }

private:
	
	CTFCSpanner( const CTFCSpanner & ) {}
};

#endif // WEAPON_TFC_SPANNER_H
