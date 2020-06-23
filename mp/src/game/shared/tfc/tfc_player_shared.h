//============= Copyright Concussion Studios, All rights reserved. =============//
//
// Purpose:
//
//=============================================================================//
#ifndef TFC_PLAYER_SHARED_H
#define TFC_PLAYER_SHARED_H
#ifdef _WIN32
#pragma once
#endif

#include "networkvar.h"
#include "weapon_tfcbase.h"
#include "basegrenade_shared.h"

#ifdef CLIENT_DLL
	class C_TFCPlayer;
#else
	class CTFCPlayer;
#endif

#ifdef CLIENT_DLL
	EXTERN_RECV_TABLE( DT_TFCPlayerShared );
#else
	EXTERN_SEND_TABLE( DT_TFCPlayerShared );
#endif

class CTFCPlayerShared
{
public:

#ifdef CLIENT_DLL

	friend class C_TFCPlayer;
	typedef C_TFCPlayer OuterClass;
	DECLARE_PREDICTABLE();
#else
	friend class CTFCPlayer;
	typedef CTFCPlayer OuterClass;
#endif
	
	DECLARE_EMBEDDED_NETWORKVAR()
	DECLARE_CLASS_NOBASE( CTFCPlayerShared );

	// Initialization.
	CTFCPlayerShared();
	void Init( OuterClass *pOuter );

	// State (TF_STATE_*).
	//int	GetState() const					{ return m_nPlayerState; }
	//void	SetState( int nState )				{ m_nPlayerState = nState; }
	//bool	InState( int nState )				{ return ( m_nPlayerState == nState ); }

	// Weapons.
	CWeaponTFCBase *GetActiveTFCWeapon() const;

	// Utility.
	int GetDesiredPlayerClassIndex( void ) { return m_iDesiredPlayerClass; }

private:

	// Vars that are networked.
	//CNetworkVar( int, m_nPlayerState );			// Player state.

	// Vars that are not networked.
	CNetworkVar( int, m_iDesiredPlayerClass );

	OuterClass			*m_pOuter;					// C_TFCPlayer or CTFCPlayer (client/server).
};	
#endif // TFC_PLAYER_SHARED_H
