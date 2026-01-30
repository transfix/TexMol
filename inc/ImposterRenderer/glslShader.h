#ifndef __SHADER_H
#define __SHADER_H

#include <string>

#if ( (defined(__MACH__)) && (defined(__APPLE__)) )   
#include <stdlib.h>
#include <OpenGL/gl.h>
//#include <GLUT/glut.h>
//#include <OpenGL/glext.h>
#else
#include <stdlib.h>
#include <glew/glew.h>
#include <GL/gl.h>
//#include <GL/glut.h>
//#include <GL/glext.h>
#endif

namespace CCVImposterRenderer {

  class glslShader {
  public:
	glslShader();
	glslShader(const char *vsFile, const char *fsFile);
	~glslShader();

	void initFromFile(const char *vsFile, const char *fsFile);
	void initFromString(const char *vs, const char *fs);
    
	void bind();
	void unbind();
	
	unsigned int id();
	
  private:
	unsigned int shader_id;
	unsigned int shader_vp;
	unsigned int shader_fp;
  };
}

#endif
