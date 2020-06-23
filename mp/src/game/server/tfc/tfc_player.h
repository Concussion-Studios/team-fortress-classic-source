//============= Copyright Concussion Studios, All rights reserved. =============//
//
// Purpose: Player for TFC.
//
//=============================================================================//
#ifndef TFC_PLAYER_H
#define TFC_PLAYER_H
#ifdef _WIN32
#pragma once
#endif

#include "basetempentity.h"
#include "hl2mp_player.h"
#include "server_class.h"
#include "tfc_playeranimstate.h"
#include "tfc_player_shared.h"
#include "tfc_playerclass.h"

class CTFCPlayer;

// -------------------------------------------------------------------------------- //
// Player animation event. Sent to the client when a player fires, jumps, reloads, etc..
// -------------------------------------------------------------------------------- //
class CTEPlayerAnimEvent : public CBaseTempEntity
{
public:
	DECLARE_CLASS( CTEPlayerAnimEvent, CBaseTempEntity );
	DECLARE_SERVERCLASS();

	CTEPlayerAnimEvent( const char *name ) : CBaseTempEntity( name ) {}

	CNetworkHandle(CBasePlayer, m_hPlayer );
	CNetworkVar( int, m_iEvent );
	CNetworkVar( int, m_nData );
};

// ---------------------------------------------------------------------------------------------- //
// Purpose: TFC player
// ---------------------------------------------------------------------------------------------- //
class CTFCPlayer : public CHL2MP_Player
{
public:
	DECLARE_CLASS( CTFCPlayer, CHL2MP_Player );
	DECLARE_SERVERCLASS();


			CTFCPlayer();
	virtual ~CTFCPlayer( void );

	static CTFCPlayer *CreatePlayer( const char *className, edict_t *ed );
	static CTFCPlayer* Instance( int iEnt );

	virtual void PostThink() OVERRIDE;
	virtual void InitialSpawn() OVERRIDE;
	virtual void Spawn() OVERRIDE;
	virtual void Precache() OVERRIDE;
	virtual bool ClientCommand( const CCommand &args ) OVERRIDE;
	virtual int	TakeHealth( float flHealth, int bitsDamageType ) OVERRIDE;
	virtual void Event_Killed( const CTakeDamageInfo &info ) OVERRIDE;

// Called by shared code.
public:
	
	CTFCPlayerShared m_Shared;

	CWeaponTFCBase *Weapon_OwnsThisID( int iWeaponID );
	CWeaponTFCBase *GetActiveTFCWeapon( void ) const;

	virtual void DoAnimationEvent( PlayerAnimEvent_t event, int nData = 0 );


	// Player class implementation.
public:

	CTFCPlayerClass *GetPlayerClass( void ) 		{ return &m_PlayerClass; }
	int GetDesiredPlayerClassIndex( void )			{ return m_Shared.m_iDesiredPlayerClass; }
	void SetDesiredPlayerClassIndex( int iClass )	{ m_Shared.m_iDesiredPlayerClass = iClass; }

	void InitClass( void );
	void HandleCommand_JoinClass( const char *pClassName );

	CTFCPlayerClass m_PlayerClass;

	// Player teams implementation.
public:

	int GetAutoTeam( void );
	virtual void ChangeTeam( int iTeamNum ) OVERRIDE;
	void ForceChangeTeam( int iTeamNum );
	void HandleCommand_JoinTeam( const char *pTeamName );
	void HandleCommand_JoinTeam_NoMenus( const char *pTeamName );

// State management.
private:

	// TODO

private:

	void GiveDefaultItems();

	void TFCPlayerThink();

private:

	// Copyed from EyeAngles() so we can send it to the client.
	CNetworkQAngle( m_angEyeAngles );

	ITFCPlayerAnimState *m_PlayerAnimState;
};

inline CTFCPlayer *ToTFCPlayer( CBaseEntity *pEntity )
{
	if ( !pEntity || !pEntity->IsPlayer() )
		return NULL;

#ifdef _DEBUG
	Assert( dynamic_cast<CTFCPlayer*>( pEntity ) != 0 );
#endif
	return static_cast< CTFCPlayer* >( pEntity );
}

#endif	// TFC_PLAYER_H