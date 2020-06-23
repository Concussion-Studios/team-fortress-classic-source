//============= Copyright Concussion Studios, All rights reserved. =============//
//
// Purpose: Player for TFC.
//
//=============================================================================//
#include "cbase.h"
#include "tfc_player.h"
#include "tfc_gamerules.h"
#include "keyvalues.h"
#include "viewport_panel_names.h"
#include "client.h"
#include "team.h"
#include "weapon_tfcbase.h"
#include "datacache/imdlcache.h"

#define TFC_PLAYER_MODEL "models/player/pyro.mdl"

LINK_ENTITY_TO_CLASS( info_player_red, CPointEntity );
LINK_ENTITY_TO_CLASS( info_player_blue, CPointEntity );

// ------------------------------------------------------------------------------------------ //
// Purpose: CTEPlayerAnimEvent implementation.
// ------------------------------------------------------------------------------------------ //
IMPLEMENT_SERVERCLASS_ST_NOBASE( CTEPlayerAnimEvent_TFC, DT_TEPlayerAnimEvent_TFC )
	SendPropEHandle( SENDINFO( m_hPlayer ) ),
	SendPropInt( SENDINFO( m_iEvent ), Q_log2( PLAYERANIMEVENT_COUNT ) + 1, SPROP_UNSIGNED ),
	SendPropInt( SENDINFO( m_nData ), 32 )
END_SEND_TABLE()

static CTEPlayerAnimEvent_TFC g_TETFCPlayerAnimEvent( "PlayerAnimEvent" );

void TE_TFC_PlayerAnimEvent( CBasePlayer *pPlayer, PlayerAnimEvent_t event, int nData )
{
	CPVSFilter filter( pPlayer->EyePosition() );
	
	// The player himself doesn't need to be sent his animation events 
	// unless tfc_showanimstate wants to show them.
	//if ( cl_showanimstate.GetInt() == pPlayer->entindex() )
	{
		filter.RemoveRecipient( pPlayer );
	}

	g_TETFCPlayerAnimEvent.m_hPlayer = pPlayer;
	g_TETFCPlayerAnimEvent.m_iEvent = event;
	g_TETFCPlayerAnimEvent.m_nData = nData;
	g_TETFCPlayerAnimEvent.Create( filter, 0 );
}

// -------------------------------------------------------------------------------- //
// Tables.
// -------------------------------------------------------------------------------- //
LINK_ENTITY_TO_CLASS( player, CTFCPlayer );
PRECACHE_REGISTER(player);

IMPLEMENT_SERVERCLASS_ST( CTFCPlayer, DT_TFCPlayer )
	SendPropExclude( "DT_BaseAnimating", "m_flPoseParameter" ),
	SendPropExclude( "DT_BaseAnimating", "m_flPlaybackRate" ),	
	SendPropExclude( "DT_BaseAnimating", "m_nSequence" ),
	SendPropExclude( "DT_BaseEntity", "m_angRotation" ),
	SendPropExclude( "DT_BaseAnimatingOverlay", "overlay_vars" ),
	
	// cs_playeranimstate and clientside animation takes care of these on the client
	SendPropExclude( "DT_ServerAnimationData" , "m_flCycle" ),	
	SendPropExclude( "DT_AnimTimeMustBeFirst" , "m_flAnimTime" ),

	SendPropAngle( SENDINFO_VECTORELEM( m_angEyeAngles, 0 ), 11 ),
	SendPropAngle( SENDINFO_VECTORELEM( m_angEyeAngles, 1 ), 11 ),

	SendPropDataTable( SENDINFO_DT( m_Shared ), &REFERENCE_SEND_TABLE( DT_TFCPlayerShared ) )
END_SEND_TABLE()

CTFCPlayer::CTFCPlayer()
{
	m_PlayerAnimState = CreatePlayerAnimState( this );

	UseClientSideAnimation();
	m_angEyeAngles.Init();
//	m_pCurStateInfo = NULL;
	m_lifeState = LIFE_DEAD; // Start "dead".

	SetViewOffset( TFC_PLAYER_VIEW_OFFSET );

	SetContextThink( &CTFCPlayer::TFCPlayerThink, gpGlobals->curtime, "TFCPlayerThink" );
}

void CTFCPlayer::TFCPlayerThink()
{
	//if ( m_pCurStateInfo && m_pCurStateInfo->pfnThink )
	//	(this->*m_pCurStateInfo->pfnThink)();

	SetContextThink( &CTFCPlayer::TFCPlayerThink, gpGlobals->curtime, "TFCPlayerThink" );
}

CTFCPlayer::~CTFCPlayer()
{
	m_PlayerAnimState->Release();
}

CTFCPlayer *CTFCPlayer::CreatePlayer( const char *className, edict_t *ed )
{
	CTFCPlayer::s_PlayerEdict = ed;
	return (CTFCPlayer*)CreateEntityByName( className );
}

void CTFCPlayer::PostThink()
{
	BaseClass::PostThink();

	QAngle angles = GetLocalAngles();
	angles[PITCH] = 0;
	SetLocalAngles( angles );
	
	// Store the eye angles pitch so the client can compute its animation state correctly.
	m_angEyeAngles = EyeAngles();

    m_PlayerAnimState->Update( m_angEyeAngles[YAW], m_angEyeAngles[PITCH] );
}

void CTFCPlayer::Precache()
{
	int i;

	// Precache the models, hd models and arms models as well.
	for ( i = 0; i < CLASS_COUNT_ALL; i++ )
	{
		const char *pszModel = GetClassData( i )->m_szModelName;
		if ( pszModel && pszModel[0] )
			PrecacheModel( pszModel );

		const char *pszHDModel = GetClassData( i )->m_szHDModelName;
		if ( pszHDModel && pszHDModel[0] )
				PrecacheModel( pszHDModel );

		const char *pszArmsModel = GetClassData( i )->m_szArmsModel;
		if ( pszArmsModel && pszArmsModel[0] )
				PrecacheModel( pszArmsModel );
	}
	
	// Precache player class sounds
	for ( i = FIRST_NORMAL_CLASS; i < CLASS_COUNT_ALL; ++i )
	{
		TFCPlayerClassInfo_t *pInfo = GetClassData( i );

		PrecacheScriptSound( pInfo->m_szDeathSound );
		PrecacheScriptSound( pInfo->m_szMeleeDeathSound );
		PrecacheScriptSound( pInfo->m_szExplosionDeathSound );
	}

	PrecacheScriptSound( "Player.Spawn" );

	BaseClass::Precache();
}


void CTFCPlayer::InitialSpawn( void )
{
	BaseClass::InitialSpawn();

	//State_Enter( STATE_WELCOME );
}


void CTFCPlayer::Spawn()
{
	MDLCACHE_CRITICAL_SECTION();

	//UpdateModel();

	SetMoveType( MOVETYPE_WALK );
	CHL2_Player::Spawn();

	// Kind of lame, but CBasePlayer::Spawn resets a lot of the state that we initially want on.
	// So if we're in the welcome state, call its enter function to reset 
	/*if ( m_Shared.State_Get() == STATE_WELCOME )
	{
		State_Enter_WELCOME();
	}*/

	// If they were dead, then they're respawning. Put them in the active state.
	/*if ( m_Shared.State_Get() == STATE_DYING )
	{
		State_Transition( STATE_ACTIVE );
	}*/

	// If they're spawning into the world as fresh meat, give them items and stuff.
	if ( /*m_Shared.State_Get() == STATE_ACTIVE*/ !IsObserver() )
	{
		EmitSound( "Player.Spawn" );

		pl.deadflag = false;
		RemoveSolidFlags( FSOLID_NOT_SOLID );

		RemoveEffects( EF_NODRAW );
		
		GiveDefaultItems();
	}

	m_Local.m_iHideHUD = 0;
	
	AddFlag( FL_ONGROUND ); // set the player on the ground at the start of the round.

	//m_impactEnergyScale = HL2MPPLAYER_PHYSDAMAGE_SCALE;

	if ( TFCGameRules()->IsIntermission() )
		AddFlag( FL_FROZEN );
	else
		RemoveFlag( FL_FROZEN );

	//m_iSpawnInterpCounter = (m_iSpawnInterpCounter + 1) % 8;

	m_Local.m_bDucked = false;

	SetPlayerUnderwater( false );

	// This makes the surrounding box always the same size as the standing collision box
	// helps with parts of the hitboxes that extend out of the crouching hitbox, eg with the
	// heavyweapons guy
	Vector mins = VEC_HULL_MIN;
	Vector maxs = VEC_HULL_MAX;
	CollisionProp()->SetSurroundingBoundsType( USE_SPECIFIED_BOUNDS, &mins, &maxs );
}

void CTFCPlayer::InitClass( void )
{
	// Set initial health and armor based on class.
	SetMaxHealth( GetPlayerClass()->GetMaxHealth() );
	SetHealth( GetMaxHealth() );

	SetArmorValue( GetPlayerClass()->GetMaxArmor() );

	// Give default items for class.
	GiveDefaultItems();
}

void CTFCPlayer::GiveDefaultItems()
{
	// Get the player class data.
	TFCPlayerClassInfo_t *pInfo = m_PlayerClass.GetData();

	RemoveAllAmmo();

	// Give ammo. Must be done before weapons, so weapons know the player has ammo for them.
	for ( int iAmmo = 0; iAmmo < AMMO_LAST; ++iAmmo )
		GiveAmmo( pInfo->m_aAmmoMax[iAmmo], iAmmo, false );

	// Give weapons.
	for ( int iWeapon = 0; iWeapon < TFC_MAX_WEAPON_SLOTS; ++iWeapon )
	{
		if ( pInfo->m_aWeapons[iWeapon] != WEAPON_NONE )
		{
			int iWeaponID = pInfo->m_aWeapons[iWeapon];
			const char *pszWeaponName = WeaponIDToAlias( iWeaponID );

			CWeaponTFCBase *pWeapon = (CWeaponTFCBase *)GetWeapon( iWeapon );

			//If we already have a weapon in this slot but is not the same type then nuke it (changed classes)
			if ( pWeapon && pWeapon->GetWeaponID() != iWeaponID )
			{
				Weapon_Detach( pWeapon );
				UTIL_Remove( pWeapon );
			}

			pWeapon = (CWeaponTFCBase *)Weapon_OwnsThisID( iWeaponID );
			if ( pWeapon )
			{
				pWeapon->ChangeTeam( GetTeamNumber() );
				pWeapon->GiveDefaultAmmo();
			}
			else
			{
				pWeapon = (CWeaponTFCBase *)GiveNamedItem( pszWeaponName );
				if ( pWeapon )
					pWeapon->DefaultTouch( this );
			}
		}
		else
		{
			//I shouldn't have any weapons in this slot, so get rid of it
			CWeaponTFCBase *pCarriedWeapon = (CWeaponTFCBase *)GetWeapon( iWeapon );

			//Don't nuke builders since they will be nuked if we don't need them later.
			if ( pCarriedWeapon /*&& pCarriedWeapon->GetWeaponID() != TF_WEAPON_BUILDER*/ )
			{
				Weapon_Detach( pCarriedWeapon );
				UTIL_Remove( pCarriedWeapon );
			}
		}
	}

	SetActiveWeapon( NULL );
	Weapon_Switch( Weapon_GetSlot( 0 ) );
	Weapon_SetLast( Weapon_GetSlot( 1 ) );
}

int CTFCPlayer::GetAutoTeam( void )
{
	int iTeam = TEAM_SPECTATOR;

	//CTCFTeam *pBlue = TFCTeamMgr()->GetTeam( TF_TEAM_BLUE );
	//CTCFTeam *pRed  = TFCTeamMgr()->GetTeam( TF_TEAM_RED );

	CTeam *pBlue = GetGlobalTeam( TEAM_BLUE );
	CTeam *pRed  = GetGlobalTeam( TEAM_RED );

	if ( pBlue && pRed )
	{
		if ( pBlue->GetNumPlayers() < pRed->GetNumPlayers() )
			iTeam = TEAM_BLUE;
		else if ( pRed->GetNumPlayers() < pBlue->GetNumPlayers() )
			iTeam = TEAM_RED;
		else
			iTeam = RandomInt( 0, 1 ) ? TEAM_RED : TEAM_BLUE;
	}

	return iTeam;
}

void CTFCPlayer::HandleCommand_JoinTeam( const char *pTeamName )
{
	int iTeam = TEAM_RED;
	if ( stricmp( pTeamName, "auto" ) == 0 )
	{
		iTeam = GetAutoTeam();
	}
	else if ( stricmp( pTeamName, "spectate" ) == 0 )
	{
		iTeam = TEAM_SPECTATOR;
	}
	else
	{
		for ( int i = 0; i < TEAM_COUNT; ++i )
		{
			if ( stricmp( pTeamName, g_aTeamNames[i] ) == 0 )
			{
				iTeam = i;
				break;
			}
		}
	}

	if ( iTeam == TEAM_SPECTATOR )
	{
		// Prevent this is the cvar is set
		if ( !mp_allowspectators.GetInt() && !IsHLTV() )
		{
			ClientPrint( this, HUD_PRINTCENTER, "#Cannot_Be_Spectator" );
			return;
		}
		
		if ( GetTeamNumber() != TEAM_UNASSIGNED && !IsDead() )
			CommitSuicide( false, true );

		ChangeTeam( TEAM_SPECTATOR );

		// do we have fadetoblack on? (need to fade their screen back in)
		if ( mp_fadetoblack.GetBool() )
		{
			color32_s clr = { 0,0,0,255 };
			UTIL_ScreenFade( this, clr, 0, 0, FFADE_IN | FFADE_PURGE );
		}
	}
	else
	{
		if ( iTeam == GetTeamNumber() )
			return;	// we wouldn't change the team

		// if this join would unbalance the teams, refuse
		// come up with a better way to tell the player they tried to join a full team!
		/*if ( TFGameRules()->WouldChangeUnbalanceTeams( iTeam, GetTeamNumber() ) )
		{
			ShowViewPortPanel( PANEL_TEAM );
			return;
		}*/

		ChangeTeam( iTeam );

		ShowViewPortPanel( ( iTeam == TEAM_RED ) ? PANEL_CLASS_RED : PANEL_CLASS_BLUE );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Join a team without using the game menus
//-----------------------------------------------------------------------------
void CTFCPlayer::HandleCommand_JoinTeam_NoMenus( const char *pTeamName )
{
	Msg( "Client command HandleCommand_JoinTeam_NoMenus: %s\n", pTeamName );

	int iTeam = TEAM_SPECTATOR;
	if ( Q_stricmp( pTeamName, "spectate" ) )
	{
		for ( int i = 0; i < TEAM_COUNT; ++i )
		{
			if ( stricmp( pTeamName, g_aTeamNames[i] ) == 0 )
			{
				iTeam = i;
				break;
			}
		}
	}

	ForceChangeTeam( iTeam );
}

//-----------------------------------------------------------------------------
// Purpose: Player has been forcefully changed to another team
//-----------------------------------------------------------------------------
void CTFCPlayer::ForceChangeTeam( int iTeamNum )
{
	int iNewTeam = iTeamNum;

	if ( iNewTeam == TEAM_AUTOASSIGN )
		iNewTeam = GetAutoTeam();

	if ( !GetGlobalTeam( iNewTeam ) )
	{
		Warning( "CTFCPlayer::ForceChangeTeam( %d ) - invalid team index.\n", iNewTeam );
		return;
	}

	int iOldTeam = GetTeamNumber();

	// if this is our current team, just abort
	if ( iNewTeam == iOldTeam )
		return;

	//RemoveAllObjects();
	//RemoveNemesisRelationships();

	BaseClass::ChangeTeam( iNewTeam );

	if ( iNewTeam == TEAM_UNASSIGNED )
	{
		//StateTransition( TF_STATE_OBSERVER );
	}
	else if ( iNewTeam == TEAM_SPECTATOR )
	{
		//m_bIsIdle = false;
		//StateTransition( TF_STATE_OBSERVER );

		RemoveAllWeapons();
		DestroyViewModels();
	}

	// Don't modify living players in any way
}

void CTFCPlayer::ChangeTeam( int iTeamNum )
{
	if ( !GetGlobalTeam( iTeamNum ) )
	{
		Warning( "CTFPlayer::ChangeTeam( %d ) - invalid team index.\n", iTeamNum );
		return;
	}

	int iOldTeam = GetTeamNumber();

	// if this is our current team, just abort
	if ( iTeamNum == iOldTeam )
		return;

	//RemoveAllObjects();
	//RemoveNemesisRelationships();

	BaseClass::ChangeTeam( iTeamNum );

	if ( iTeamNum == TEAM_UNASSIGNED )
	{
		//StateTransition( TF_STATE_OBSERVER );
		State_Transition( STATE_OBSERVER_MODE );
	}
	else if ( iTeamNum == TEAM_SPECTATOR )
	{
		//StateTransition( TF_STATE_OBSERVER );
		State_Transition( STATE_OBSERVER_MODE );

		RemoveAllWeapons();
		DestroyViewModels();
	}
	else // active player
	{
		if ( !IsDead() && (iOldTeam == TEAM_RED || iOldTeam == TEAM_BLUE) )
		{
			// Kill player if switching teams while alive
			CommitSuicide( false, true );
		}
		else if ( IsDead() && iOldTeam < FIRST_GAME_TEAM )
		{
			SetObserverMode( OBS_MODE_CHASE );

			// do we have fadetoblack on? (need to fade their screen back in)
			if ( mp_fadetoblack.GetBool() )
			{
				color32_s clr = { 0,0,0,255 };
				UTIL_ScreenFade( this, clr, 0, 0, FFADE_IN | FFADE_PURGE );
			}
		}
	}
}


void CTFCPlayer::HandleCommand_JoinClass( const char *pClassName )
{
	// can only join a class after you join a valid team
	if ( GetTeamNumber() <= LAST_SHARED_TEAM )
		return;

	// In case we don't get the class menu message before the spawn timer
	// comes up, fake that we've closed the menu.
	//SetClassMenuOpen( false );

	/*if ( TFGameRules()->InStalemate() )
	{
		if ( IsAlive() && !TFGameRules()->CanChangeClassInStalemate() )
		{
			ClientPrint(this, HUD_PRINTTALK, "#game_stalemate_cant_change_class" );
			return;
		}
	}*/

	int iClass = CLASS_UNDEFINED;
	/*bool bShouldNotRespawn = false;

	if ( ( TFGameRules()->State_Get() == GR_STATE_TEAM_WIN ) && ( TFGameRules()->GetWinningTeam() != GetTeamNumber() ) )
	{
		m_bAllowInstantSpawn = false;
		bShouldNotRespawn = true;
	}*/

	if ( stricmp( pClassName, "random" ) != 0 )
	{
		int i = 0;

		for ( i = CLASS_SCOUT ; i < CLASS_COUNT_ALL ; i++ )
		{
			if ( stricmp( pClassName, GetClassData( i )->m_szClassName ) == 0 )
			{
				iClass = i;
				break;
			}
		}

		if ( i >LAST_NORMAL_CLASS )
		{
			Warning( "HandleCommand_JoinClass( %s ) - invalid class name.\n", pClassName );
			return;
		}
	}
	else
	{
		// The player has selected Random class...so let's pick one for them.
		do{
			// Don't let them be the same class twice in a row
			iClass = random->RandomInt( FIRST_NORMAL_CLASS, LAST_NORMAL_CLASS );
		} while( iClass == GetPlayerClass()->GetClassIndex() );
	}

	// joining the same class?
	if ( iClass != CLASS_RANDOM && iClass == GetDesiredPlayerClassIndex() )
	{
		// If we're dead, and we have instant spawn, respawn us immediately. Catches the case
		// were a player misses respawn wave because they're at the class menu, and then changes
		// their mind and reselects their current class.
		if ( m_bAllowInstantSpawn && !IsAlive() )
			ForceRespawn();

		return;
	}

	SetDesiredPlayerClassIndex( iClass );

	// are they CLASS_RANDOM and trying to select the class they're currently playing as (so they can stay this class)?
	if ( iClass == GetPlayerClass()->GetClassIndex() )
	{
		// If we're dead, and we have instant spawn, respawn us immediately. Catches the case
		// were a player misses respawn wave because they're at the class menu, and then changes
		// their mind and reselects their current class.
		if ( m_bAllowInstantSpawn && !IsAlive() )
			ForceRespawn();

		return;
	}

	// We can respawn instantly if:
	//	- We're dead, and we're past the required post-death time
	//	- We're inside a respawn room
	//	- We're in the stalemate grace period
	/*bool bInRespawnRoom = PointInRespawnRoom( this, WorldSpaceCenter() );
	if ( bInRespawnRoom && !IsAlive() )
	{
		// If we're not spectating ourselves, ignore respawn rooms. Otherwise we'll get instant spawns
		// by spectating someone inside a respawn room.
		bInRespawnRoom = (GetObserverTarget() == this);
	}

	bool bDeadInstantSpawn = !IsAlive();
	if ( bDeadInstantSpawn && m_flDeathTime )
	{
		// In death mode, don't allow class changes to force respawns ahead of respawn waves
		float flWaveTime = TFGameRules()->GetNextRespawnWave( GetTeamNumber(), this );
		bDeadInstantSpawn = (gpGlobals->curtime > flWaveTime);
	}

	bool bInStalemateClassChangeTime = false;
	if ( TFGameRules()->InStalemate() )
	{
		// Stalemate overrides respawn rules. Only allow spawning if we're in the class change time.
		bInStalemateClassChangeTime = TFGameRules()->CanChangeClassInStalemate();
		bDeadInstantSpawn = false;
		bInRespawnRoom = false;
	}

	if ( bShouldNotRespawn == false && ( m_bAllowInstantSpawn || bDeadInstantSpawn || bInRespawnRoom || bInStalemateClassChangeTime ) )
	{
		ForceRespawn();
		return;
	}*/

	if( iClass == CLASS_RANDOM )
	{
		if( IsAlive() )
			ClientPrint( this, HUD_PRINTTALK, "#game_respawn_asrandom" );
		else
			ClientPrint( this, HUD_PRINTTALK, "#game_spawn_asrandom" );
	}
	else
	{
		if( IsAlive() )
			ClientPrint( this, HUD_PRINTTALK, "#game_respawn_as", GetClassData( iClass )->m_szLocalizableName );
		else
			ClientPrint( this, HUD_PRINTTALK, "#game_spawn_as", GetClassData( iClass )->m_szLocalizableName );
	}

	if ( IsAlive() /*&& ( GetHudClassAutoKill() == true ) && bShouldNotRespawn == false*/ )
		CommitSuicide( false, true );
}

bool CTFCPlayer::ClientCommand( const CCommand& args )
{
	const char *pcmd = args[0];

	if ( FStrEq( args[0], "spectate" ) )
	{
		if ( CBaseMultiplayerPlayer::ShouldRunRateLimitedCommand( args ) )
		{
			// instantly join spectators
			HandleCommand_JoinTeam_NoMenus( "spectate" );	
		}
		return true;
	}
	if ( FStrEq( pcmd, "jointeam" ) )
	{
		if ( CBaseMultiplayerPlayer::ShouldRunRateLimitedCommand( args ) )
		{
			if ( args.ArgC() >= 2 )
				HandleCommand_JoinTeam( args[1] );
		}
		return true;
	}
	else if ( FStrEq( pcmd, "jointeam_nomenus" ) )
	{
		if ( CBaseMultiplayerPlayer::ShouldRunRateLimitedCommand( args ) )
		{
			if ( args.ArgC() >= 2 )
				HandleCommand_JoinTeam_NoMenus( args[1] );
		}
		return true;
	}
	else if ( FStrEq( pcmd, "joinclass" ) ) 
	{
		if ( CBaseMultiplayerPlayer::ShouldRunRateLimitedCommand( args ) )
		{
			if ( args.ArgC() >= 2 )
				HandleCommand_JoinClass( args[1] );
		}
		return true;
	}

	return BaseClass::ClientCommand( args );
}

int CTFCPlayer::TakeHealth( float flHealth, int bitsDamageType )
{
	return BaseClass::TakeHealth( flHealth, bitsDamageType );
}

void CTFCPlayer::Event_Killed( const CTakeDamageInfo &info )
{
	DoAnimationEvent( PLAYERANIMEVENT_DIE );
	//State_Transition( STATE_DYING );	// Transition into the dying state.

	// Remove all items..
	RemoveAllItems( true );

	BaseClass::Event_Killed( info );
}