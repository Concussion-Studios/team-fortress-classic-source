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

	CWeaponTFCBaseMelee();

	// Get specific TFC weapon ID (ie: WEAPON_AC, etc)
	virtual TFCWeaponID GetWeaponID( void ) const {	Assert( false ); return TF_WEAPON_NONE; }
	
	//Attack functions
	virtual	void	PrimaryAttack( void ) OVERRIDE;
	virtual	void	SecondaryAttack( void ) OVERRIDE;
	
	virtual void	ItemPostFrame( void ) OVERRIDE;

	//Functions to select animation sequences 
	virtual Activity	GetPrimaryAttackActivity( void ) OVERRIDE { return ACT_VM_HITCENTER; }
	virtual Activity	GetSecondaryAttackActivity( void ) OVERRIDE { return ACT_VM_HITCENTER2; }

	virtual	float	GetFireRate( void )	OVERRIDE { return 0.2f; }
	virtual float	GetRange( void ) { return 32.0f; }
	virtual	float	GetDamageForActivity( Activity hitActivity ) { return 1.0f; }

	virtual	void	ImpactEffect( trace_t &trace );

	virtual	bool ImpactWater( const Vector &start, const Vector &end );
	virtual	void Swing( int bIsSecondary );
	virtual	void Hit( trace_t &traceHit, Activity nHitActivity );
	virtual	Activity ChooseIntersectionPointAndActivity( trace_t &hitTrace, const Vector &mins, const Vector &maxs, CBasePlayer *pOwner );

private:
	CWeaponTFCBaseMelee( const CWeaponTFCBaseMelee & ) {}
};
#endif // WEAPON_TFCBASE_MELEE_H