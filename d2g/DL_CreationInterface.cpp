#include "StdAfx.h"
#include "DL_CreationInterface.h"

DL_CreationInterface::DL_CreationInterface(void)
{
	extrusion = new DL_Extrusion;
}

DL_CreationInterface::~DL_CreationInterface(void)
{
	delete extrusion;
}


/** Sets the current attributes for entities. */
void DL_CreationInterface::setAttributes(const DL_Attributes & attrib)
{
	attributes = attrib;
}

/** @return the current attributes used for new entities. */
DL_Attributes DL_CreationInterface::getAttributes(void)
{
	return attributes;
}

/** Sets the current attributes for entities. */
void DL_CreationInterface::setExtrusion(double dx, double dy, double dz, double elevation)
{
	extrusion->setDirection(dx,dy,dz);
	extrusion->setElevation(elevation);
}

/** @return the current attributes used for new entities. */
DL_Extrusion* DL_CreationInterface::getExtrusion(void)
{
	return extrusion;
}
