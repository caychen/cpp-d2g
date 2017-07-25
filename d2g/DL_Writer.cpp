#include "StdAfx.h"
#include "DL_Writer.h"

DL_Writer::DL_Writer(DL_Codes::version version) : m_handle(0x30)
{
	this->version = version;
	modelSpaceHandle = 0;
	paperSpaceHandle = 0;
	paperSpace0Handle = 0;
}

DL_Writer::~DL_Writer(void)
{
}



/** Generic section for section 'name'.
*
* <pre>
*   0
*  SECTION
*   2
*  name
* </pre>
*/
void DL_Writer::section(const char* name) const
{
	dxfString(0, "SECTION");
	dxfString(2, name);
}

/**
* Section HEADER
*
* <pre>
*   0
*  SECTION
*   2
*  HEADER
* </pre>
*/
void DL_Writer::sectionHeader() const
{
	section("HEADER");
}

/**
* Section TABLES
*
* <pre>
*   0
*  SECTION
*   2
*  TABLES
* </pre>
*/
void DL_Writer::sectionTables() const
{
	section("TABLES");
}

/**
* Section BLOCKS
*
* <pre>
*   0
*  SECTION
*   2
*  BLOCKS
* </pre>
*/
void DL_Writer::sectionBlocks() const
{
	section("BLOCKS");
}

/**
* Section ENTITIES
*
* <pre>
*   0
*  SECTION
*   2
*  ENTITIES
* </pre>
*/
void DL_Writer::sectionEntities() const
{
	section("ENTITIES");
}

/**
* Section CLASSES
*
* <pre>
*   0
*  SECTION
*   2
*  CLASSES
* </pre>
*/
void DL_Writer::sectionClasses() const
{
	section("CLASSES");
}

/**
* Section OBJECTS
*
* <pre>
*   0
*  SECTION
*   2
*  OBJECTS
* </pre>
*/
void DL_Writer::sectionObjects() const
{
	section("OBJECTS");
}

/**
* End of a section.
*
* <pre>
*   0
*  ENDSEC
* </pre>
*/
void DL_Writer::sectionEnd() const
{
	dxfString(0, "ENDSEC");
}


/**
* Generic table for table 'name' with 'num' entries:
*
* <pre>
*   0
*  TABLE
*   2
*  name
*  70
*   num
* </pre>
*/
void DL_Writer::table(const char* name, int num, int handle) const
{
	dxfString(0, "TABLE");
	dxfString(2, name);

	if(version >= VER_2000)
	{
		dxfHex(5, handle);
		dxfString(100, "AcDbSymbolTable");
	}

	dxfInt(70, num);
}

/** Table for layers.
*
* @param num Number of layers in total.
*
* <pre>
*   0
*  TABLE
*   2
*  LAYER
*   70
*      num
* </pre>
*/
void DL_Writer::tableLayers(int num) const
{
	table("LAYER", num, 2);
}

/** Table for line types.
*
* @param num Number of line types in total.
*
* <pre>
*   0
*  TABLE
*   2
*  LTYPE
*   70
*      num
* </pre>
*/
void DL_Writer::tableLineTypes(int num) const
{
	//lineTypeHandle = 5;
	table("LTYPE", num, 5);
}

/** Table for application id.
*
* @param num Number of registered applications in total.
*
* <pre>
*   0
*  TABLE
*   2
*  APPID
*   70
*      num
* </pre>
*/
void DL_Writer::tableAppid(int num) const
{
	table("APPID", num, 9);
}

/**
* End of a table.
*
* <pre>
*   0
*  ENDTAB
* </pre>
*/
void DL_Writer::tableEnd() const
{
	dxfString(0, "ENDTAB");
}

/**
* End of the DXF file.
*
* <pre>
*   0
*  EOF
* </pre>
*/
void DL_Writer::dxfEOF() const
{
	dxfString(0, "EOF");
}

/**
* Comment.
*
* <pre>
*  999
*  text
* </pre>
*/
void DL_Writer::comment(const char* text) const
{
	dxfString(999, text);
}

/**
* Entity.
*
* <pre>
*   0
*  entTypeName
* </pre>
*
* @return Unique handle or 0.
*/
void DL_Writer::entity(const char* entTypeName) const
{
	dxfString(0, entTypeName);

	if(version >= VER_2000)
	{
		handle();
	}
}


/**
* Attributes of an entity.
*
* <pre>
*   8
*  layer
*  62
*  color
*  39
*  width
*   6
*  linetype
* </pre>
*/
void DL_Writer::entityAttributes(const DL_Attributes& attrib) const
{
	// layer name:
	dxfString(8, attrib.getLayer());

	// R12 doesn't accept BYLAYER values. The value has to be missing
	//   in that case.
	if(version >= VER_2000 ||
		attrib.getColor() != 256)
	{
		dxfInt(62, attrib.getColor());
	}

	if(version >= VER_2000)
	{
		dxfInt(370, attrib.getWidth());
	}

	if(version >= VER_2000 ||
		strcasecmp(attrib.getLineType().c_str(), "BYLAYER"))
	{
		dxfString(6, attrib.getLineType());
	}
}

/**
* Subclass.
*/
void DL_Writer::subClass(const char* sub) const
{
	dxfString(100, sub);
}


/**
* Layer (must be in the TABLES section LAYER).
*
* <pre>
*   0
*  LAYER
* </pre>
*/
void DL_Writer::tableLayerEntry(unsigned long int h)  const
{
	dxfString(0, "LAYER");

	if(version >= VER_2000)
	{
		if(h == 0)
		{
			handle();
		}
		else
		{
			dxfHex(5, h);
		}

		dxfString(100, "AcDbSymbolTableRecord");
		dxfString(100, "AcDbLayerTableRecord");
	}
}


/**
* Line type (must be in the TABLES section LTYPE).
*
* <pre>
*   0
*  LTYPE
* </pre>
*/
void DL_Writer::tableLineTypeEntry(unsigned long int h )  const
{
	dxfString(0, "LTYPE");

	if(version >= VER_2000)
	{
		if(h == 0)
		{
			handle();
		}
		else
		{
			dxfHex(5, h);
		}

		//dxfHex(330, 0x5);
		dxfString(100, "AcDbSymbolTableRecord");
		dxfString(100, "AcDbLinetypeTableRecord");
	}
}

/**
* Appid (must be in the TABLES section APPID).
*
* <pre>
*   0
*  APPID
* </pre>
*/
void DL_Writer::tableAppidEntry(unsigned long int h )  const
{
	dxfString(0, "APPID");

	if(version >= VER_2000)
	{
		if(h == 0)
		{
			handle();
		}
		else
		{
			dxfHex(5, h);
		}

		//dxfHex(330, 0x9);
		dxfString(100, "AcDbSymbolTableRecord");
		dxfString(100, "AcDbRegAppTableRecord");
	}
}


/**
* Block (must be in the section BLOCKS).
*
* <pre>
*   0
*  BLOCK
* </pre>
*/
void DL_Writer::sectionBlockEntry(unsigned long int h )  const
{
	dxfString(0, "BLOCK");

	if(version >= VER_2000)
	{
		if(h == 0)
		{
			handle();
		}
		else
		{
			dxfHex(5, h);
		}

		//dxfHex(330, blockHandle);
		dxfString(100, "AcDbEntity");

		if(h == 0x1C)
		{
			dxfInt(67, 1);
		}

		dxfString(8, "0");                 // TODO: Layer for block
		dxfString(100, "AcDbBlockBegin");
	}
}

/**
* End of Block (must be in the section BLOCKS).
*
* <pre>
*   0
*  ENDBLK
* </pre>
*/
void DL_Writer::sectionBlockEntryEnd(unsigned long int h )  const
{
	dxfString(0, "ENDBLK");

	if(version >= VER_2000)
	{
		if(h == 0)
		{
			handle();
		}
		else
		{
			dxfHex(5, h);
		}

		//dxfHex(330, blockHandle);
		dxfString(100, "AcDbEntity");

		if(h == 0x1D)
		{
			dxfInt(67, 1);
		}

		dxfString(8, "0");                 // TODO: Layer for block
		dxfString(100, "AcDbBlockEnd");
	}
}


void DL_Writer::color(int col = 256) const
{
	dxfInt(62,col);
}


void DL_Writer::lineType(const char *lt) const
{
	dxfString(6,lt);
}


void DL_Writer::lineTypeScale(double scale) const
{
	dxfReal(48,scale);
}


void DL_Writer::lineWeight(int lw) const
{
	dxfInt(370,lw);
}


void DL_Writer::coord(int gc, double x, double y, double z) const
{
	dxfReal(gc,x);
	dxfReal(gc + 10,y);
	dxfReal(gc + 20,z);
}


void DL_Writer::coordTriplet(int gc, const double* value) const
{
	if(value)
	{
		dxfReal(gc, *value++);
		dxfReal(gc + 10, *value++);
		dxfReal(gc + 20, *value++);
	}
}


void DL_Writer::resetHandle() const
{
	m_handle  = 1;
}


/**
* Writes a unique handle and returns it.
*/
unsigned long DL_Writer::handle(int gc ) const
{
	// handle has to be hex
	dxfHex(gc, m_handle);
	return m_handle++;
}

/**
* @return Next handle that will be written.
*/
unsigned long DL_Writer::getNextHandle() const
{
	return m_handle;
}


/**
* Increases handle, so that the handle returned remains available.
*/
unsigned long DL_Writer::incHandle() const
{
	return m_handle++;
}

/**
* Sets the handle of the model space. Entities refer to
* this handle.
*/
void DL_Writer::setModelSpaceHandle(unsigned long h)
{
	modelSpaceHandle = h;
}


unsigned long DL_Writer::getModelSpaceHandle()
{
	return modelSpaceHandle;
}


/**
* Sets the handle of the paper space. Some special blocks refer to
* this handle.
*/
void DL_Writer::setPaperSpaceHandle(unsigned long h)
{
	paperSpaceHandle = h;
}


unsigned long DL_Writer::getPaperSpaceHandle()
{
	return paperSpaceHandle;
}

/**
* Sets the handle of the paper space 0. Some special blocks refer to
* this handle.
*/
void DL_Writer::setPaperSpace0Handle(unsigned long h)
{
	paperSpace0Handle = h;
}


unsigned long DL_Writer::getPaperSpace0Handle()
{
	return paperSpace0Handle;
}
