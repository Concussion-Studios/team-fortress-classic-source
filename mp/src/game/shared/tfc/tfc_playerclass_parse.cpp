//============= Copyright Concussion Studios, All rights reserved. =============//
//
// Purpose:
//
//=============================================================================//
#include "cbase.h"
#include "KeyValues.h"
#include "tfc_playerclass_parse.h"
#include "tier2/tier2.h"

#define PC_UNDEFINED_FILE			""
#define PC_SCOUT_FILE				"scripts/playerclasses/scout"
#define PC_SNIPER_FILE				"scripts/playerclasses/sniper"
#define PC_SOLDIER_FILE				"scripts/playerclasses/soldier"
#define PC_DEMOMAN_FILE				"scripts/playerclasses/demoman"
#define PC_MEDIC_FILE				"scripts/playerclasses/medic"
#define PC_HEAVYWEAPONS_FILE		"scripts/playerclasses/heavyweapons"
#define PC_PYRO_FILE				"scripts/playerclasses/pyro"
#define PC_SPY_FILE					"scripts/playerclasses/spy"
#define PC_ENGINEER_FILE			"scripts/playerclasses/engineer"
#define PC_CIVILIAN_FILE			"scripts/playerclasses/civilian"

const char *s_aPlayerClassFiles[] =
{
	PC_UNDEFINED_FILE,
	PC_SCOUT_FILE,
	PC_SNIPER_FILE,
	PC_SOLDIER_FILE,
	PC_DEMOMAN_FILE,
	PC_MEDIC_FILE,
	PC_HEAVYWEAPONS_FILE,
	PC_PYRO_FILE,
	PC_SPY_FILE,
	PC_ENGINEER_FILE,
	PC_CIVILIAN_FILE
};

TFCPlayerClassInfo_t s_aTFCPlayerClassData[ CLASS_COUNT_ALL ];

#ifdef CLIENT_DLL
ConVar mp_usehdmodels( "mp_usehdmodels", "0", NULL, "Enable the use of the 2001 tfc player models." );
#endif

bool UseHDModels()
{
#ifdef CLIENT_DLL 
	return mp_usehdmodels.GetBool();
#else
	return false;
#endif
}

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
TFCPlayerClassInfo_t::TFCPlayerClassInfo_t()
{
	m_szClassName[0] = '\0';
	m_szModelName[0] = '\0';
	m_szHDModelName[0] = '\0';
	m_szArmsModel[0] = '\0';
	m_szLocalizableName[0] = '\0';
	m_flMaxSpeed = 0.0f;
	m_nMaxHealth = 0;
	m_nMaxArmor = 0;

#ifdef GAME_DLL
	m_szDeathSound[0] = '\0';
	m_szMeleeDeathSound[0] = '\0';
	m_szExplosionDeathSound[0] = '\0';
#endif

	for ( int iWeapon = 0; iWeapon < TFC_MAX_WEAPON_SLOTS; ++iWeapon )
		m_aWeapons[iWeapon] = WEAPON_NONE;

	for ( int iGrenade = 0; iGrenade < TFC_MAX_GRENADE_SLOTS; ++iGrenade )
		m_aGrenades[iGrenade] = WEAPON_NONE;

	for ( int iAmmo = 0; iAmmo < AMMO_LAST; ++iAmmo )
		m_aAmmoMax[iAmmo] = AMMO_DUMMY;

	m_bParsed = false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void TFCPlayerClassInfo_t::Parse( const char *szName )
{
	// Have we parsed this file already?
	if ( m_bParsed )
		return;

	// No filesystem at this point????  Hmmmm......
	// Parse class file.
	const unsigned char *pKey = NULL;

	if ( g_pGameRules )
		pKey = g_pGameRules->GetEncryptionKey();

	KeyValues *pKV = ReadEncryptedKVFile( filesystem, szName, pKey );
	if ( pKV )
	{
		ParseData( pKV );
		pKV->deleteThis();
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *TFCPlayerClassInfo_t::GetModelName() const
{
#ifdef CLIENT_DLL
	if ( UseHDModels() )
	{
		if ( m_szHDModelName[0] != '\0' )
			return m_szHDModelName;
	}

	return m_szModelName;
#else
	return m_szModelName;
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void TFCPlayerClassInfo_t::ParseData( KeyValues *pKeyValuesData )
{
	// Attributes.
	Q_strncpy( m_szClassName, pKeyValuesData->GetString( "name" ), MAX_PLAYERCLASS_NAME_LENGTH );

	Q_strncpy( m_szHDModelName, pKeyValuesData->GetString( "model_hd" ), MAX_PLAYERCLASS_NAME_LENGTH );
	Q_strncpy( m_szModelName, pKeyValuesData->GetString( "model" ), MAX_PLAYERCLASS_NAME_LENGTH );
	Q_strncpy( m_szArmsModel, pKeyValuesData->GetString("model_arms" ), MAX_PLAYERCLASS_NAME_LENGTH );
	Q_strncpy( m_szLocalizableName, pKeyValuesData->GetString( "localize_name" ), MAX_PLAYERCLASS_NAME_LENGTH );

	m_flMaxSpeed = pKeyValuesData->GetFloat( "speed_max" );
	m_nMaxHealth = pKeyValuesData->GetInt( "health_max" );
	m_nMaxArmor = pKeyValuesData->GetInt( "armor_max" );

	// Weapons.
	int i;
	char buf[32];
	for ( i=0;i<TFC_MAX_WEAPON_SLOTS;i++ )
	{
		Q_snprintf( buf, sizeof(buf), "weapon%d", i+1 );		
		m_aWeapons[i] = AliasToWeaponID( pKeyValuesData->GetString( buf ) );
	}

	// Grenades.
	for ( i=0;i<TFC_MAX_GRENADE_SLOTS;i++ )
	{
		Q_snprintf( buf, sizeof(buf), "grenade%d", i+1 );		
		m_aWeapons[i] = AliasToWeaponID( pKeyValuesData->GetString( buf ) );
	}

	// Ammo Max.
	KeyValues *pAmmoKeyValuesData = pKeyValuesData->FindKey( "AmmoMax" );
	if ( pAmmoKeyValuesData )
	{
		for ( int iAmmo = 1; iAmmo < AMMO_LAST; ++iAmmo )
			m_aAmmoMax[iAmmo] = pAmmoKeyValuesData->GetInt( g_aAmmoNames[iAmmo], 0 );
	}

#ifdef GAME_DLL	
	// Death Sounds
	Q_strncpy( m_szDeathSound, pKeyValuesData->GetString( "sound_death", "Player.Death" ), MAX_PLAYERCLASS_SOUND_LENGTH );
	Q_strncpy( m_szMeleeDeathSound, pKeyValuesData->GetString( "sound_melee_death", "Player.MeleeDeath" ), MAX_PLAYERCLASS_SOUND_LENGTH );
	Q_strncpy( m_szExplosionDeathSound, pKeyValuesData->GetString( "sound_explosion_death", "Player.ExplosionDeath" ), MAX_PLAYERCLASS_SOUND_LENGTH );
#endif

	// The file has been parsed.
	m_bParsed = true;
}

//-----------------------------------------------------------------------------
// Purpose: Initialize the player class data (keep a cache).
//-----------------------------------------------------------------------------
void InitPlayerClasses( void )
{
	// Special case the undefined class.
	TFCPlayerClassInfo_t *pClassData = &s_aTFCPlayerClassData[ CLASS_UNDEFINED ];
	Assert( pClassData );
	Q_strncpy( pClassData->m_szClassName, "undefined", MAX_PLAYERCLASS_NAME_LENGTH );
	Q_strncpy( pClassData->m_szModelName, "models/player/scout.mdl", MAX_PLAYERCLASS_NAME_LENGTH );	// Undefined players still need a model
	Q_strncpy( pClassData->m_szArmsModel, "models/weapons/c_arms_scout.mdl", MAX_PLAYERCLASS_NAME_LENGTH );	// Undefined players still need hands
	Q_strncpy( pClassData->m_szLocalizableName, "undefined", MAX_PLAYERCLASS_NAME_LENGTH );

	// Initialize the classes.
	for ( int iClass = 1; iClass < CLASS_COUNT_ALL; ++iClass )
	{
		TFCPlayerClassInfo_t *pClassData = &s_aTFCPlayerClassData[iClass];
		Assert( pClassData );
		pClassData->Parse( s_aPlayerClassFiles[iClass] );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Helper function to get player class data.
//-----------------------------------------------------------------------------
TFCPlayerClassInfo_t *GetClassData( int iClass )
{
	Assert ( ( iClass >= 0 ) && ( iClass < CLASS_COUNT_ALL ) );
	return &s_aTFCPlayerClassData[iClass];
}

//=============================================================================
//
// Shared player class data.
//

#ifdef CLIENT_DLL
BEGIN_RECV_TABLE_NOBASE( CTFCPlayerClassShared, DT_TFCPlayerClassShared )
	RecvPropInt( RECVINFO( m_iClass ) ),
END_RECV_TABLE()
#else
BEGIN_SEND_TABLE_NOBASE( CTFCPlayerClassShared, DT_TFCPlayerClassShared )
	SendPropInt( SENDINFO( m_iClass ), Q_log2( CLASS_COUNT_ALL )+1, SPROP_UNSIGNED ),
END_SEND_TABLE()
#endif

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CTFCPlayerClassShared::CTFCPlayerClassShared()
{
	m_iClass.Set( CLASS_UNDEFINED  );
}

//-----------------------------------------------------------------------------
// Purpose: Initialize the player class.
//-----------------------------------------------------------------------------
bool CTFCPlayerClassShared::Init( int iClass )
{
	Assert ( ( iClass >= CLASS_UNDEFINED ) && ( iClass <= CLASS_COUNT_ALL ) );
	m_iClass = iClass;
	return true;
}