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
// * along with this program. If not, see  http://www.gnu.org/licenses/.// slider.h

#ifndef _SLIDER_H_
#define	_SLIDER_H_


#include "control.h"
#include "struct.h"


class CD3DEngine;
class CButton;



class CSlider : public CControl
{
public:
	CSlider(CInstanceManager* iMan);
	~CSlider();

	BOOL		Create(FPOINT pos, FPOINT dim, int icon, EventMsg eventMsg);

	void		SetPos(FPOINT pos);
	void		SetDim(FPOINT dim);

	BOOL		SetState(int state, BOOL bState);
	BOOL		SetState(int state);
	BOOL		ClearState(int state);

	BOOL		EventProcess(const Event &event);
	void		Draw();

	void		SetLimit(float min, float max);

	void		SetVisibleValue(float value);
	float		RetVisibleValue();

	void		SetArrowStep(float step);
	float		RetArrowStep();

protected:
	void		MoveAdjust();
	void		AdjustGlint();
	void		DrawVertex(FPOINT pos, FPOINT dim, int icon);

protected:
	CButton*	m_buttonLeft;
	CButton*	m_buttonRight;

	float		m_min;
	float		m_max;
	float		m_visibleValue;
	float		m_step;

	BOOL		m_bHoriz;
	float		m_marginButton;

	BOOL		m_bCapture;
	FPOINT		m_pressPos;
	float		m_pressValue;

	EventMsg	m_eventUp;
	EventMsg	m_eventDown;
};


#endif //_SLIDER_H_
