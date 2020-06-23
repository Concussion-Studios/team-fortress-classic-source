//============= Copyright Concussion Studios, All rights reserved. =============//
//
// Purpose:
//
//=============================================================================//

#include "cbase.h"
#include "tfc_gamerules.h"
#include "tfc_player_shared.h"
#include "takedamageinfo.h"
#include "weapon_tfcbase.h"

#ifdef CLIENT_DLL
	#include "c_tfc_player.h"
#else
	#include "tfc_player.h"
	void TE_PlayerAnimEvent( CBasePlayer *pPlayer, PlayerAnimEvent_t event, int nData );
#endif

#ifdef CLIENT_DLL
BEGIN_RECV_TABLE_NOBASE( CTFCPlayerShared, DT_TFCPlayerSharedLocal )
END_RECV_TABLE()

BEGIN_RECV_TABLE_NOBASE( CTFCPlayerShared, DT_TFCPlayerShared )
	//RecvPropInt( RECVINFO( m_nPlayerCond ) ),
	RecvPropInt( RECVINFO( m_iDesiredPlayerClass ) ),
	// Local Data.
	RecvPropDataTable( "tfcsharedlocaldata", 0, 0, &REFERENCE_RECV_TABLE(DT_TFCPlayerSharedLocal) ),
END_RECV_TABLE()

BEGIN_PREDICTION_DATA_NO_BASE( CTFCPlayerShared )
	//DEFINE_PRED_FIELD( m_nPlayerState, FIELD_INTEGER, FTYPEDESC_INSENDTABLE ),
END_PREDICTION_DATA()
#else
BEGIN_SEND_TABLE_NOBASE( CTFCPlayerShared, DT_TFCPlayerSharedLocal )
END_SEND_TABLE()

BEGIN_SEND_TABLE_NOBASE( CTFCPlayerShared, DT_TFCPlayerShared )
	//SendPropInt( SENDINFO( m_nPlayerCond ), TF_COND_LAST, SPROP_UNSIGNED | SPROP_CHANGES_OFTEN ),
	SendPropInt( SENDINFO( m_iDesiredPlayerClass ), Q_log2( CLASS_COUNT_ALL )+1, SPROP_UNSIGNED ),
	// Local Data.
	SendPropDataTable( "tfcsharedlocaldata", 0, &REFERENCE_SEND_TABLE( DT_TFCPlayerSharedLocal ), SendProxy_SendLocalDataTable ),	
END_SEND_TABLE()
#endif

// --------------------------------------------------------------------------------------------------- //
// Shared CTFCPlayer implementation.
// --------------------------------------------------------------------------------------------------- //
void CTFCPlayer::DoAnimationEvent( PlayerAnimEvent_t event, int nData )
{
	m_PlayerAnimState->DoAnimationEvent( event, nData );
#ifdef GAME_DLL
	TE_PlayerAnimEvent( this, event, nData );	// Send to any clients who can see this guy.  
#endif // GAME_DLL
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CWeaponTFCBase *CTFCPlayer::GetActiveTFCWeapon( void ) const
{
	CBaseCombatWeapon *pRet = GetActiveWeapon();
	if ( pRet )
	{
		Assert( dynamic_cast< CWeaponTFCBase* >( pRet ) != NULL );
		return static_cast< CWeaponTFCBase * >( pRet );
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CWeaponTFCBase *CTFCPlayer::Weapon_OwnsThisID( int iWeaponID )
{
	for (int i = 0;i < WeaponCount(); i++) 
	{
		CWeaponTFCBase *pWpn = ( CWeaponTFCBase *)GetWeapon( i );
		if ( pWpn == NULL )
			continue;

		if ( pWpn->GetWeaponID() == iWeaponID )
			return pWpn;
	}

	return NULL;
}

// --------------------------------------------------------------------------------------------------- //
// CTFCPlayerShared implementation.
// --------------------------------------------------------------------------------------------------- //
CTFCPlayerShared::CTFCPlayerShared()
{
//	m_nPlayerState.Set( TF_STATE_WELCOME );
}

void CTFCPlayerShared::Init( CTFCPlayer *pPlayer )
{
	m_pOuter = pPlayer;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CWeaponTFCBase *CTFCPlayerShared::GetActiveTFCWeapon() const
{
	return m_pOuter->GetActiveTFCWeapon();
}