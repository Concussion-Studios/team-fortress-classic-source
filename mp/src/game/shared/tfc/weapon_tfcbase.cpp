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
#include "cbase.h"
#include "in_buttons.h"
#include "takedamageinfo.h"
#include "weapon_tfcbase.h"
#include "ammodef.h"
#include "tfc_gamerules.h"

#if defined( CLIENT_DLL )
	#include "c_tfc_player.h"
#else
	#include "tfc_player.h"
	#include "te_effect_dispatch.h"
#endif

// ----------------------------------------------------------------------------- //
// CWeaponTFCBase tables.
// ----------------------------------------------------------------------------- //
IMPLEMENT_NETWORKCLASS_ALIASED( WeaponTFCBase, DT_WeaponTFCBase )

BEGIN_NETWORK_TABLE( CWeaponTFCBase, DT_WeaponTFCBase )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CWeaponTFCBase ) 
END_PREDICTION_DATA()

#ifdef GAME_DLL
BEGIN_DATADESC( CWeaponTFCBase )
END_DATADESC()
#endif

// ----------------------------------------------------------------------------- //
// CWeaponTFCBase implementation. 
// ----------------------------------------------------------------------------- //
CWeaponTFCBase::CWeaponTFCBase()
{
	SetPredictionEligible( true );
	AddSolidFlags( FSOLID_TRIGGER ); // Nothing collides with these but it gets touches.
}

CTFCPlayer* CWeaponTFCBase::GetPlayerOwner() const
{
	return dynamic_cast< CTFCPlayer* >( GetOwner() );
}

/*const CTFCWeaponInfo &CWeaponTFCBase::GetTFCWpnData() const
{
	const FileWeaponInfo_t *pWeaponInfo = &GetWpnData();
	const CTFCWeaponInfo *pTFCInfo;

#ifdef _DEBUG
	pTFCInfo = dynamic_cast< const CTFCWeaponInfo* >( pWeaponInfo );
	Assert( pTFCInfo );
#else
	pTFCInfo = static_cast< const CTFCWeaponInfo* >( pWeaponInfo );
#endif

	return *pTFCInfo;
}*/

#ifdef CLIENT_DLL

#else // CLIENT_DLL

void CWeaponTFCBase::Spawn()
{
	BaseClass::Spawn();

	// Set this here to allow players to shoot dropped weapons
	SetCollisionGroup( COLLISION_GROUP_WEAPON );
		
	// Move it up a little bit, otherwise it'll be at the guy's feet, and its sound origin 
	// will be in the ground so its EmitSound calls won't do anything.
	SetLocalOrigin( Vector( 0, 0, 5 ) );
}
	
bool CWeaponTFCBase::DefaultReload( int iClipSize1, int iClipSize2, int iActivity )
{
	if ( BaseClass::DefaultReload( iClipSize1, iClipSize2, iActivity ) )
	{
		SendReloadSoundEvent();
		return true;
	}
	else
	{
		return false;
	}
}

void CWeaponTFCBase::SendReloadSoundEvent()
{
	CTFCPlayer *pPlayer = dynamic_cast< CTFCPlayer* >( GetOwner() );
	if ( !pPlayer )
		return;

	// Send a message to any clients that have this entity to play the reload.
	CPASFilter filter( pPlayer->GetAbsOrigin() );
	filter.RemoveRecipient( pPlayer );

	UserMessageBegin( filter, "ReloadEffect" );
		WRITE_SHORT( pPlayer->entindex() );
	MessageEnd();

	// Make the player play his reload animation.
	pPlayer->DoAnimationEvent( PLAYERANIMEVENT_RELOAD );
}

Vector CWeaponTFCBase::GetSoundEmissionOrigin() const
{
	CBasePlayer *pPlayer = GetPlayerOwner();
	if ( pPlayer )
		return pPlayer->WorldSpaceCenter();
	else
		return WorldSpaceCenter();
}

bool CWeaponTFCBase::PhysicsSplash( const Vector &centerPoint, const Vector &normal, float rawSpeed, float scaledSpeed )
{
	if ( rawSpeed > 20 )
	{
		float size = 4.0f;
		if ( GetWeaponID() != WEAPON_TRANQ )
			size += 2.0f;

		// adjust splash size based on speed
		size += RemapValClamped( rawSpeed, 0, 400, 0, 3 );

		CEffectData	data;
 		data.m_vOrigin = centerPoint;
		data.m_vNormal = normal;
		data.m_flScale = random->RandomFloat( size, size + 1.0f );

		if ( GetWaterType() & CONTENTS_SLIME )
			data.m_fFlags |= FX_WATER_IN_SLIME;

		DispatchEffect( "gunshotsplash", data );

		return true;
	}

	return false;
}
#endif