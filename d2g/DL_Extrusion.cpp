#include "StdAfx.h"
#include "DL_Extrusion.h"


/**
* Default constructor.
*/
DL_Extrusion::DL_Extrusion(void)
{
	direction = new double[3];
	setDirection(0.0,0.0,1.0);

}


/**
* Constructor for DXF extrusion.
*
* @param direction Vector of axis along which the entity shall be extruded
*                  this is also the Z axis of the Entity coordinate system
* @param elevation Distance of the entities XY plane from the origin of the
*                  world coordinate system
*/
DL_Extrusion::DL_Extrusion(double dx, double dy, double dz, double elevation)
{

}

/**
* Destructor.
*/
DL_Extrusion::~DL_Extrusion(void)
{
	delete direction;
}


/**
* Sets the direction vector.
*/
void DL_Extrusion::setDirection(double dx, double dy,double dz)
{
	direction[0] = dx;
	direction[1] = dy;
	direction[2] = dz;
}


/**
* @return direction vector.
*/
double* DL_Extrusion::getDirection(void) const
{
	return direction;
}

/**
* @return direction vector.
*/
void DL_Extrusion::getDirection(double dir[])const
{
	dir[0] = direction[0];
	dir[1] = direction[1];
	dir[2] = direction[2];
}

/**
* Sets the elevation.
*/
void DL_Extrusion::setElevation(double elevation)
{
	this->elevation = elevation;
}

/**
* @return Elevation.
*/
double DL_Extrusion::getElevation(void) const
{
	return elevation;
}

DL_Extrusion DL_Extrusion::operator =(const DL_Extrusion& extru)
{
	setDirection(extru.direction[0],extru.direction[1],extru.direction[2]);
	setElevation(extru.elevation);
	return *this;
}