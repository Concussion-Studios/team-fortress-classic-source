//========= Copyright © 1996-2004, Valve LLC, All rights reserved. ============
//
//=============================================================================
#ifndef TFC_PLAYERCLASS_H
#define TFC_PLAYERCLASS_H
#ifdef _WIN32
#pragma once
#endif

#include "tfc_playerclass_parse.h"

//-----------------------------------------------------------------------------
// TFC Player Class
//-----------------------------------------------------------------------------
class CTFCPlayerClass : public CTFCPlayerClassShared
{
public:

	CTFCPlayerClass();
	~CTFCPlayerClass();
};

#endif // TFC_PLAYERCLASS_H