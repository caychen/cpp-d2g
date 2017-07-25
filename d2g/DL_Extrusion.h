#pragma once


/****************************************************************************
** $Id: dl_extrusion.h 273 2005-02-28 18:14:39Z andrew $
**
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
** This file is part of the dxflib project.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid dxflib Professional Edition licenses may use 
** this file in accordance with the dxflib Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.ribbonsoft.com for further details.
**
** Contact info@ribbonsoft.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#include <math.h>


/**
* Storing and passing around attributes. Attributes
* are the layer name, color, width and line type.
*
* @author Andrew Mustun
*/
class DL_Extrusion
{
public:
	/**
	* Default constructor.
	*/
	DL_Extrusion(void);


	DL_Extrusion(double dx, double dy, double dz, double elevation);
public:
	/**
	* Destructor.
	*/
	~DL_Extrusion(void);
private:
	double* direction;
	double elevation;
	
public:
	void setDirection(double dx, double dy,double dz);
	double* getDirection(void) const;
	void getDirection(double dir[]) const;

	void setElevation(double elevation);
	double getElevation(void)const;

	DL_Extrusion operator = (const DL_Extrusion& extru);
};
//EOF
