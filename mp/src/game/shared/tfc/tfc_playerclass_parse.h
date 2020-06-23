//============= Copyright Concussion Studios, All rights reserved. =============//
//
// Purpose:
//
//=============================================================================//
#ifndef TFC_PLAYERCLASS_PARSE_H
#define TFC_PLAYERCLASS_PARSE_H
#ifdef _WIN32
#pragma once
#endif

#include "playerclass_info_parse.h"
#include "networkvar.h"

#define MAX_PLAYERCLASS_SOUND_LENGTH	128

#ifdef CLIENT_DLL
	EXTERN_RECV_TABLE( DT_TFCPlayerClassShared );
#else
	EXTERN_SEND_TABLE( DT_TFCPlayerClassShared );
#endif

//-----------------------------------------------------------------------------
// Cache structure for the TFC player class data
//-----------------------------------------------------------------------------
struct TFCPlayerClassInfo_t
{
	char		m_szClassName[MAX_PLAYERCLASS_NAME_LENGTH];
	char		m_szModelName[MAX_PLAYERCLASS_NAME_LENGTH];
	char		m_szHDModelName[MAX_PLAYERCLASS_NAME_LENGTH];
	char		m_szArmsModel[ MAX_PLAYERCLASS_NAME_LENGTH];
	char		m_szLocalizableName[MAX_PLAYERCLASS_NAME_LENGTH];
	float		m_flMaxSpeed;
	int			m_nMaxHealth;
	int			m_nMaxArmor;
	int			m_aWeapons[TFC_MAX_WEAPON_SLOTS];
	int			m_aGrenades[TFC_MAX_GRENADE_SLOTS];
	int			m_aAmmoMax[AMMO_LAST];

	bool		m_bParsed;

#ifdef GAME_DLL
	// sounds
	char		m_szDeathSound[MAX_PLAYERCLASS_SOUND_LENGTH];
	char		m_szMeleeDeathSound[MAX_PLAYERCLASS_SOUND_LENGTH];
	char		m_szExplosionDeathSound[MAX_PLAYERCLASS_SOUND_LENGTH];
#endif

	TFCPlayerClassInfo_t();
	const char *GetModelName() const;
	void Parse( const char *pszClassName );

private:

	void ParseData( KeyValues *pKeyValuesData );
};

void InitPlayerClasses( void );
TFCPlayerClassInfo_t *GetClassData( int iClass );

//-----------------------------------------------------------------------------
// TFC Player Class
//-----------------------------------------------------------------------------
class CTFCPlayerClassShared
{
public:

	CTFCPlayerClassShared();

	DECLARE_EMBEDDED_NETWORKVAR()
	DECLARE_CLASS_NOBASE( CTFCPlayerClassShared );

	bool		Init( int iClass );
	bool		IsClass( int iClass ) const						{ return ( m_iClass == iClass ); }
	int			GetClassIndex( void )							{ return m_iClass; }

	const char	*GetName( void ) const							{ return GetClassData( m_iClass )->m_szClassName; }
	const char	*GetModelName( void ) const						{ return GetClassData( m_iClass )->GetModelName(); }
	const char  *GetArmsModelName( void ) const					{ return GetClassData( m_iClass )->m_szArmsModel; }
	float		GetMaxSpeed( void )								{ return GetClassData( m_iClass )->m_flMaxSpeed; }
	int			GetMaxHealth( void )							{ return GetClassData( m_iClass )->m_nMaxHealth; }
	int			GetMaxArmor( void )								{ return GetClassData( m_iClass )->m_nMaxArmor; }

	TFCPlayerClassInfo_t  *GetData( void )						{ return GetClassData( m_iClass ); }

protected:

	CNetworkVar( int,	m_iClass );
};

#endif // TFC_PLAYERCLASS_PARSE_H