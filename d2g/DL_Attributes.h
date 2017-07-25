#pragma once


/****************************************************************************
** $Id: dl_attributes.h 2334 2005-03-27 23:37:52Z andrew $
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


#include <string>
using std::string;

#include "DL_Codes.h"


/**
* Storing and passing around attributes. Attributes
* are the layer name, color, width and line type.
*
* @author Andrew Mustun
*/

class DL_Attributes
{
public:
	/**
	* Default constructor.
	*/
	DL_Attributes(void);
	DL_Attributes(const string& layer,int color,int width,const string&linetype);
	
	void setLayer(const string& layer);
	string getLayer(void) const;

	void setColor(int color);
	int getColor(void) const;

	void setWidth(int width);
	int getWidth(void) const;

	void setLineType(const string& lineType);
	string getLineType(void) const;

	DL_Attributes operator = (const DL_Attributes& attrib);

public:
	~DL_Attributes(void);
private:
	string layer;
	int color;
	int width;
	string lineType;
};

//EOF
