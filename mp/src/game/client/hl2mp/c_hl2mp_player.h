//========= Copyright � 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
#ifndef HL2MP_PLAYER_H
#define HL2MP_PLAYER_H
#pragma once

#include "hl2mp_playeranimstate.h"
#include "c_basehlplayer.h"
#include "baseparticleentity.h"
#include "hl2mp_player_shared.h"
#include "beamdraw.h"
#include "c_basetempentity.h"
#include "flashlighteffect.h"

// ------------------------------------------------------------------------------------------------------------- //
// Tony; m_pFlashlightEffect is private, so just subclass. We may want to do some more stuff with it later anyway.
// ------------------------------------------------------------------------------------------------------------- //
class CHL2MPFlashlightEffect : public CFlashlightEffect
{
public:
	CHL2MPFlashlightEffect(int nIndex = 0) : CFlashlightEffect( nIndex  ) {}
	~CHL2MPFlashlightEffect() {};

	virtual void UpdateLight( const Vector &vecPos, const Vector &vecDir, const Vector &vecRight, const Vector &vecUp, int nDistance );
};

// -------------------------------------------------------------------------------- //
// Player animation event. Sent to the client when a player fires, jumps, reloads, etc..
// -------------------------------------------------------------------------------- //
class C_TEPlayerAnimEvent : public C_BaseTempEntity
{
public:
	DECLARE_CLASS( C_TEPlayerAnimEvent, C_BaseTempEntity );
	DECLARE_CLIENTCLASS();

	virtual void PostDataUpdate( DataUpdateType_t updateType );

public:
	CNetworkHandle( CBasePlayer, m_hPlayer );
	CNetworkVar( int, m_iEvent );
	CNetworkVar( int, m_nData );
};

// -------------------------------------------------------------------------------- //
// Ragdoll entities.
// -------------------------------------------------------------------------------- //
class C_HL2MPRagdoll : public C_BaseAnimatingOverlay
{
public:
	DECLARE_CLASS( C_HL2MPRagdoll, C_BaseAnimatingOverlay );
	DECLARE_CLIENTCLASS();
	
	C_HL2MPRagdoll();
	~C_HL2MPRagdoll();

	virtual void OnDataChanged( DataUpdateType_t type );

	int GetPlayerEntIndex() const;
	IRagdoll* GetIRagdoll() const {	return m_pRagdoll; }

	void ImpactTrace( trace_t *pTrace, int iDamageType, const char *pCustomImpactName );
	void UpdateOnRemove( void );
	virtual void SetupWeights( const matrix3x4_t *pBoneToWorld, int nFlexWeightCount, float *pFlexWeights, float *pFlexDelayedWeights );
	
private:
	
	C_HL2MPRagdoll( const C_HL2MPRagdoll & ) {}

	void Interp_Copy( C_BaseAnimatingOverlay *pDestinationEntity );
	void CreateHL2MPRagdoll( void );

private:

	EHANDLE	m_hPlayer;
	CNetworkVector( m_vecRagdollVelocity );
	CNetworkVector( m_vecRagdollOrigin );
};

// -------------------------------------------------------------------------------- //
// Player Class.
// -------------------------------------------------------------------------------- //
class C_HL2MP_Player : public C_BaseHLPlayer
{
public:
	DECLARE_CLASS( C_HL2MP_Player, C_BaseHLPlayer );

	DECLARE_CLIENTCLASS();
	DECLARE_PREDICTABLE();
	DECLARE_INTERPOLATION();


	C_HL2MP_Player();
	~C_HL2MP_Player( void );

	// Player avoidance
	void AvoidPlayers( CUserCmd *pCmd );
	float m_fNextThinkPushAway;
	virtual bool CreateMove( float flInputSampleTime, CUserCmd *pCmd );

	void ClientThink( void );
	void Respawn();

	static C_HL2MP_Player* GetLocalHL2MPPlayer();
	
	virtual int DrawModel( int flags );
	virtual void AddEntity( void );

	// Should this object cast shadows?
	virtual ShadowType_t		ShadowCastType( void );
	virtual C_BaseAnimating *BecomeRagdollOnClient() { return NULL; }
	virtual const QAngle& GetRenderAngles();
	virtual bool ShouldDraw( void );
	virtual void OnDataChanged( DataUpdateType_t type );
	virtual float GetFOV( void );
	virtual void ItemPreFrame( void );
	virtual void ItemPostFrame( void );
	virtual float GetMinFOV()	const { return 5.0f; }
	virtual void NotifyShouldTransmit( ShouldTransmitState_t state );
	virtual void CreateLightEffects( void ) {}
	virtual bool ShouldReceiveProjectedTextures( int flags );
	virtual void PostDataUpdate( DataUpdateType_t updateType );
	virtual void DoImpactEffect( trace_t &tr, int nDamageType );
	IRagdoll* GetRepresentativeRagdoll() const;
	virtual void CalcView( Vector &eyeOrigin, QAngle &eyeAngles, float &zNear, float &zFar, float &fov );
	virtual const QAngle& EyeAngles( void );

	void	UpdateLookAt( void );
	int		GetIDTarget() const { return m_iIDEntIndex; }
	void	UpdateIDTarget( void );

	HL2MPPlayerState State_Get() const;

	virtual void UpdateClientSideAnimation();
	virtual void CalculateIKLocks( float currentTime );

	static void RecvProxy_CycleLatch( const CRecvProxyData *pData, void *pStruct, void *pOut );

	virtual float GetServerIntendedCycle() { return m_flServerCycle; }
	virtual void SetServerIntendedCycle( float cycle ) { m_flServerCycle = cycle; }

	void InitializePoseParams( void );

// shared code
public:

	virtual CHL2MPPlayerAnimState* GetAnimState() { return m_PlayerAnimState; }

	Vector GetAttackSpread( CBaseCombatWeapon *pWeapon, CBaseEntity *pTarget = NULL );
	virtual Vector GetAutoaimVector( float flDelta );
	virtual void TraceAttack( const CTakeDamageInfo &inputInfo, const Vector &vecDir, trace_t *ptr, CDmgAccumulator *pAccumulator );
	void DoAnimationEvent( PlayerAnimEvent_t event, int nData = 0 );
	//Tony; when model is changed, need to init some stuff.
	virtual CStudioHdr *OnNewModel( void );
	void SetAnimation( PLAYER_ANIM playerAnim );
	bool ShouldCollide( int collisionGroup, int contentsMask ) const;

private:
	
	C_HL2MP_Player( const C_HL2MP_Player & );
	CHL2MPPlayerAnimState *m_PlayerAnimState;

	QAngle	m_angEyeAngles;

	CInterpolatedVar< QAngle >	m_iv_angEyeAngles;

	EHANDLE	m_hRagdoll;

	int	m_headYawPoseParam;
	int	m_headPitchPoseParam;
	float m_headYawMin;
	float m_headYawMax;
	float m_headPitchMin;
	float m_headPitchMax;

	bool m_isInit;
	Vector m_vLookAtTarget;

	float m_flLastBodyYaw;
	float m_flCurrentHeadYaw;
	float m_flCurrentHeadPitch;

	int	  m_iIDEntIndex;

	CountdownTimer m_blinkTimer;

	int	  m_iSpawnInterpCounter;
	int	  m_iSpawnInterpCounterCache;

	virtual void	UpdateFlashlight( void ); //Tony; override.
	void ReleaseFlashlight( void );
	Beam_t	*m_pFlashlightBeam;

	CHL2MPFlashlightEffect *m_pHL2MPFlashLightEffect;

	CNetworkVar( HL2MPPlayerState, m_iPlayerState );	

	int m_cycleLatch; // The animation cycle goes out of sync very easily. Mostly from the player entering/exiting PVS. Server will frequently update us with a new one.
	float m_flServerCycle;
};

inline C_HL2MP_Player *ToHL2MPPlayer( CBaseEntity *pEntity )
{
	if ( !pEntity || !pEntity->IsPlayer() )
		return NULL;

	return dynamic_cast<C_HL2MP_Player*>( pEntity );
}
#endif //HL2MP_PLAYER_H
