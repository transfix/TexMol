// SurfaceRenderingProperties.h: interface for the SurfaceRenderingProperties class.

#if !defined(AFX_SURFACERENDERINGPROPERTIES_H__9AB7858F_4BEE_4FC6_A973_B5CE3995BE5F__INCLUDED_)
#define AFX_SURFACERENDERINGPROPERTIES_H__9AB7858F_4BEE_4FC6_A973_B5CE3995BE5F__INCLUDED_

#include "SurfaceRenderingPropertiesBase.h"

class SurfaceRenderingProperties : public SurfaceRenderingPropertiesBase
{
	public:
		SurfaceRenderingProperties(QWidget* parent = 0, const char* name = 0, WFlags fl = 0);
		virtual ~SurfaceRenderingProperties();
		void addNewRenderingTypeInOrder(const char* name, int index);
};

#endif // !defined(AFX_SURFACERENDERINGPROPERTIES_H__9AB7858F_4BEE_4FC6_A973_B5CE3995BE5F__INCLUDED_)
