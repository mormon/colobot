// * This file is part of the COLOBOT source code
// * Copyright (C) 2001-2008, Daniel ROUX & EPSITEC SA, www.epsitec.ch
// *
// * This program is free software: you can redistribute it and/or modify
// * it under the terms of the GNU General Public License as published by
// * the Free Software Foundation, either version 3 of the License, or
// * (at your option) any later version.
// *
// * This program is distributed in the hope that it will be useful,
// * but WITHOUT ANY WARRANTY; without even the implied warranty of
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// * GNU General Public License for more details.
// *
// * You should have received a copy of the GNU General Public License
// * along with this program. If not, see  http://www.gnu.org/licenses/.// taskinfo.h

#ifndef _TASKINFO_H_
#define	_TASKINFO_H_


#include "misc.h"


class CInstanceManager;
class CTerrain;
class CBrain;
class CPhysics;
class CObject;



class CTaskInfo : public CTask
{
public:
	CTaskInfo(CInstanceManager* iMan, CObject* object);
	~CTaskInfo();

	BOOL		EventProcess(const Event &event);

	Error		Start(char *name, float value, float power, BOOL bSend);
	Error		IsEnded();
	BOOL		Abort();

protected:
	CObject*	SearchInfo(float power);

protected:
	float			m_progress;
	float			m_speed;
	float			m_time;
	BOOL			m_bError;
};


#endif //_TASKINFO_H_
