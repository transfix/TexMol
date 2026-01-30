#ifndef __SECONDARYSTRUCTURERENDERINGPROPERTIES_H__
#define __SECONDARYSTRUCTURERENDERINGPROPERTIES_H__

// SecondaryStructureRenderingProperties.h: interface for the SecondaryStructureRenderingProperties class.

#include "SecondaryStructureRenderingPropertiesBase.h"

class SecondaryStructureRenderingProperties : public SecondaryStructureRenderingPropertiesBase
{
	public:
		SecondaryStructureRenderingProperties(QWidget* parent = 0, const char* name = 0, WFlags fl = 0);
		virtual ~SecondaryStructureRenderingProperties();
		void addNewRenderingTypeInOrder(const char* name, int index);
};

#endif
