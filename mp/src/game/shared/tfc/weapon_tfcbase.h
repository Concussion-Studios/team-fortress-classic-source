//============= Copyright Concussion Studios, All rights reserved. =============//
//
// Purpose: Class Heirarchy for tfc weapons
//
//		  CWeaponTFCBase
//			|
//			|--> CWeaponTFCBaseMelee
//			|		|
//			|		|--> CTFCCrowbar
//			|		|--> CTFCKnife
//			|		|--> CTFCSpanner
//			|		|--> CTFCUmbrella
//			|		|--> CTFCMedikit
//			|
//			|--> CTFCMinigun
//			|--> CTFCGrenadeLauncher
//			|--> CTFCShotgun
//			|		|
//			|		|--> CTFCSuperShotgun
//			|
//			|--> CTFCNailgun
//			|		|
//			|		|--> CTFCSuperNailgun
//
//=============================================================================//
#ifndef WEAPON_TFCBASE_H
#define WEAPON_TFCBASE_H
#ifdef _WIN32
#pragma once
#endif

#include "hl2mp_player_shared.h"
#include "weapon_hl2mpbase.h"

#if defined( CLIENT_DLL )
	#define CWeaponTFCBase C_WeaponTFCBase
#endif

class CHL2MP_Player;

class CWeaponTFCBase : public CWeaponHL2MPBase
{
public:
	DECLARE_CLASS( CWeaponTFCBase, CWeaponHL2MPBase );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
#ifndef CLIENT_DLL
	DECLARE_DATADESC();
#endif

	CWeaponTFCBase();

	virtual bool IsPredicted() const OVERRIDE { return true; }

#ifndef CLIENT_DLL
	virtual void Spawn() OVERRIDE;
	virtual Vector GetSoundEmissionOrigin() const OVERRIDE;
	virtual bool PhysicsSplash( const Vector &centerPoint, const Vector &normal, float rawSpeed, float scaledSpeed ) OVERRIDE;
#endif

	CHL2MP_Player* GetPlayerOwner() const;

	// Get TFC-specific weapon data.
	//CTFCWeaponInfo const	&GetTFCWpnData() const;

	// Get specific TFC weapon ID (ie: WEAPON_RPG, etc)
	virtual TFCWeaponID GetWeaponID( void ) const {	Assert( false ); return TF_WEAPON_NONE; }

	// return true if this weapon is an instance of the given weapon type (ie: "IsA" WEAPON_TRANQ)
	bool IsA( TFCWeaponID id ) const { return GetWeaponID() == id; }

private:
	
	CWeaponTFCBase( const CWeaponTFCBase & );
};

#endif // WEAPON_TFCBASE_H