//========= Copyright © 1996-2004, Valve LLC, All rights reserved. ============
//
//=============================================================================
#ifndef C_TFC_PLAYERCLASS_H
#define C_TFC_PLAYERCLASS_H
#ifdef _WIN32
#pragma once
#endif

#include "tfc_playerclass_parse.h"

//-----------------------------------------------------------------------------
// TFC Player Class
//-----------------------------------------------------------------------------
class C_TFCPlayerClass : public CTFCPlayerClassShared
{
public:

	C_TFCPlayerClass();
	~C_TFCPlayerClass();
};

#endif // C_TFC_PLAYERCLASS_H