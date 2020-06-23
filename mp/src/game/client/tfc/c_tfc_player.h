//============= Copyright Concussion Studios, All rights reserved. =============//
//
// Purpose: Player for TFC.
//
//=============================================================================//
#ifndef C_TFC_PLAYER_H
#define C_TFC_PLAYER_H
#ifdef _WIN32
#pragma once
#endif

#include "tfc_playeranimstate.h"
#include "c_hl2mp_player.h"
#include "c_basetempentity.h"
#include "baseparticleentity.h"
#include "tfc_player_shared.h"

// ---------------------------------------------------------------------------------------------- //
// Purpose: Player animation event. Sent to the client when a player fires, jumps, reloads, etc..
// ---------------------------------------------------------------------------------------------- //
class C_TEPlayerAnimEvent : public C_BaseTempEntity
{
public:
	DECLARE_CLASS( C_TEPlayerAnimEvent, C_BaseTempEntity );
	DECLARE_CLIENTCLASS();

	virtual void PostDataUpdate( DataUpdateType_t updateType ) OVERRIDE;

public:
	CNetworkHandle( CBasePlayer, m_hPlayer );
	CNetworkVar( int, m_iEvent );
	CNetworkVar( int, m_nData );
};

// ---------------------------------------------------------------------------------------------- //
// Purpose: TFC player
// ---------------------------------------------------------------------------------------------- //
class C_TFCPlayer : public C_HL2MP_Player
{
public:
	DECLARE_CLASS( C_TFCPlayer, C_HL2MP_Player );
	DECLARE_CLIENTCLASS();
	DECLARE_PREDICTABLE();
	DECLARE_INTERPOLATION();

			C_TFCPlayer();
	virtual ~C_TFCPlayer();

	static C_TFCPlayer* GetLocalTFCPlayer();

	virtual const QAngle& GetRenderAngles() OVERRIDE;
	virtual void UpdateClientSideAnimation() OVERRIDE;
	virtual void PostDataUpdate( DataUpdateType_t updateType ) OVERRIDE;
	virtual void ProcessMuzzleFlashEvent() OVERRIDE;

// Called by shared code.
public:
	
	CTFCPlayerShared m_Shared;

	CWeaponTFCBase *Weapon_OwnsThisID( int iWeaponID );
	CWeaponTFCBase *GetActiveTFCWeapon( void ) const;

	virtual void DoAnimationEvent( PlayerAnimEvent_t event, int nData = 0 );

public:

	ITFCPlayerAnimState *m_PlayerAnimState;

	QAngle	m_angEyeAngles;
	CInterpolatedVar< QAngle >	m_iv_angEyeAngles;

private:
	C_TFCPlayer( const C_TFCPlayer & );
};

inline C_TFCPlayer* ToTFCPlayer( CBasePlayer *pPlayer )
{
	Assert( dynamic_cast< C_TFCPlayer* >( pPlayer ) != NULL );
	return static_cast< C_TFCPlayer* >( pPlayer );
}

#endif // C_TFC_PLAYER_H