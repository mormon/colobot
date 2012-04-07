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
// * along with this program. If not, see  http://www.gnu.org/licenses/.// taskturn.cpp

#define STRICT
#define D3D_OVERLOADS

#include <windows.h>
#include <stdio.h>
#include <d3d.h>

#include "struct.h"
#include "d3dengine.h"
#include "math3d.h"
#include "event.h"
#include "misc.h"
#include "iman.h"
#include "terrain.h"
#include "object.h"
#include "physics.h"
#include "brain.h"
#include "task.h"
#include "taskturn.h"




// Constructeur de l'objet.

CTaskTurn::CTaskTurn(CInstanceManager* iMan, CObject* object)
					 : CTask(iMan, object)
{
	CTask::CTask(iMan, object);
}

// Destructeur de l'objet.

CTaskTurn::~CTaskTurn()
{
}


// Gestion d'un �v�nement.

BOOL CTaskTurn::EventProcess(const Event &event)
{
	if ( m_engine->RetPause() )  return TRUE;
	if ( event.event != EVENT_FRAME )  return TRUE;

	// Objet momentan�ment immobile (fourmi sur le dos) ?
	if ( m_object->RetFixed() )
	{
		m_physics->SetMotorSpeedX(0.0f);  // stoppe l'avance
		m_physics->SetMotorSpeedZ(0.0f);  // stoppe la rotation
		m_bError = TRUE;
		return TRUE;
	}

	return TRUE;
}


// Assigne le but � atteindre.
// Un angle positif effectue un virage � droite.

Error CTaskTurn::Start(float angle)
{
	m_startAngle = m_object->RetAngleY(0);
	m_finalAngle = m_startAngle+angle;

	if ( angle < 0.0f )
	{
		m_angle = angle+m_physics->RetCirStopLength();
		m_physics->SetMotorSpeedZ(-1.0f);  // tourne � gauche
		m_bLeft = TRUE;
	}
	else
	{
		m_angle = angle-m_physics->RetCirStopLength();
		m_physics->SetMotorSpeedZ(1.0f);  // tourne � droite
		m_bLeft = FALSE;
	}
	m_physics->SetMotorSpeedX(0.0f);
	m_physics->SetMotorSpeedY(0.0f);

	m_bError = FALSE;
	return ERR_OK;
}

// Indique si l'action est termin�e.

Error CTaskTurn::IsEnded()
{
	float	angle;

	if ( m_engine->RetPause() )  return ERR_CONTINUE;

	if ( m_bError )
	{
		return ERR_STOP;
	}

	angle = m_object->RetAngleY(0);

	if ( m_bLeft )
	{
		if ( angle <= m_startAngle+m_angle )
		{
			m_physics->SetMotorSpeedZ(0.0f);
//?			m_physics->SetCirMotionY(MO_MOTSPEED, 0.0f);
			m_physics->SetCirMotionY(MO_CURSPEED, 0.0f);
//?			m_physics->SetCirMotionY(MO_REASPEED, 0.0f);
			m_object->SetAngleY(0, m_finalAngle);
			return ERR_STOP;
		}
	}
	else
	{
		if ( angle >= m_startAngle+m_angle )
		{
			m_physics->SetMotorSpeedZ(0.0f);
//?			m_physics->SetCirMotionY(MO_MOTSPEED, 0.0f);
			m_physics->SetCirMotionY(MO_CURSPEED, 0.0f);
//?			m_physics->SetCirMotionY(MO_REASPEED, 0.0f);
			m_object->SetAngleY(0, m_finalAngle);
			return ERR_STOP;
		}
	}

	return ERR_CONTINUE;
}


