#include "StdAfx.h"
#include "DL_Attributes.h"

DL_Attributes::DL_Attributes(void)
: color(0)
, width(0)
{
	layer = "";
	lineType = "BYLAYER";
}


/**
* Constructor for DXF attributes.
*
* @param layer Layer name for this entity or NULL for no layer
*              (every entity should be on a named layer!).
* @param color Color number (0..256). 0 = BYBLOCK, 256 = BYLAYER.
* @param width Line thickness. Defaults to zero. -1 = BYLAYER, 
*               -2 = BYBLOCK, -3 = default width
* @param lineType Line type name or "BYLAYER" or "BYBLOCK". Defaults
*              to "BYLAYER"
*/

DL_Attributes::DL_Attributes(const std::string &layer, int color, int width, const std::string &lineType)
{
	setLayer(layer);
	setColor(color);
	setWidth(width);
	setLineType(lineType);
}

DL_Attributes::~DL_Attributes(void)
{
}


/**
* Sets the layer. If the given pointer points to NULL, the
*  new layer name will be an empty but valid string.
*/
void DL_Attributes::setLayer(const string& layer)
{
	this->layer = layer;
}

/**
* @return Layer name.
*/
string DL_Attributes::getLayer(void) const
{
	return layer;
}


/**
* Sets the color.
*
* @see DL_Codes, dxfColors
*/
void DL_Attributes::setColor(int color)
{
	this->color = color;
}


/**
* @return Color.
*
* @see DL_Codes, dxfColors
*/
int DL_Attributes::getColor(void) const
{
	return color;
}


/**
* Sets the width.
*/
void DL_Attributes::setWidth(int width)
{
	this->width = width;
}

/**
* @return Width.
*/
int DL_Attributes::getWidth(void) const
{
	return width;
}

/**
* Sets the line type. This can be any string and is not
*  checked to be a valid line type. 
*/
void DL_Attributes::setLineType(const string& lineType)
{
	this->lineType = lineType;
}

/**
* @return Line type.
*/
string DL_Attributes::getLineType(void) const
{
	if (lineType.length() == 0)
	{
		return "BYLAYER";
	}
	else 
		return lineType;
}


/**
* Copies attributes (deep copies) from another attribute object.
*/
DL_Attributes DL_Attributes::operator = (const DL_Attributes& attrib)
{
	setLayer(attrib.layer);
	setColor(attrib.color);
	setWidth(attrib.width);
	setLineType(attrib.lineType);
	return *this;
}
