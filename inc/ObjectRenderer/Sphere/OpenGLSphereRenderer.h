// OpenGLSphereRenderer.h: interface for the OpenGLSphereRenderer class.

#ifndef CCV_OPENGL_SPHERE_RENDERER_H
#define CCV_OPENGL_SPHERE_RENDERER_H

#include <GL/glew.h>

#include <ObjectRenderer/Sphere/SphereRendererBase.h>

namespace ObjectRendererLibrary
{
	const float XC = (float).52573111;
	const float ZC = (float).85065080;
	const float YC = (float)0.0;

	static GLuint faces[20][3] =
	{
		{0,4,1},{0,9,4},{9,5,4},{4,5,8},{4,8,1},
		{8,10,1},{8,3,10},{5,3,8},{5,2,3},{2,7,3},
		{7,10,3},{7,6,10},{7,11,6},{11,0,6},{0,1,6},
		{6,1,10},{9,0,11},{9,11,2},{9,2,5},{7,2,11}
	};

	static GLfloat vdata[12][3] =
	{
		{-XC,YC,ZC},{XC,YC,ZC},{-XC,YC,-ZC},{XC,YC,-ZC},
		{YC,ZC,XC},{YC,ZC,-XC},{YC,-ZC,XC},{YC,-ZC,-XC},
		{ZC,XC,YC},{-ZC,XC,YC},{ZC,-XC,YC},{-ZC,-XC,YC}
	};

	const int NUMBER_OF_LEVELS = 1;

	class OpenGLSphereRenderer : public SphereRendererBase
	{
		public:
			OpenGLSphereRenderer();
			virtual ~OpenGLSphereRenderer();
			virtual bool initRenderer();
			virtual void clearObjects();
			virtual void render(TransformationParameters* transformationParameters);
			virtual void setNumberOfLevels(unsigned int numberOfLevels);
			static bool getSphere(int level, float* sphereVertices, float* sphereNormals);

		protected:
			static void GenerateCurve(float* v1, float* v2, float* v3, int s, float* sphereVertices, float* sphereNormals, int* curPos);
			static int getNumberOfVerticesPerSphere(int numLevels);
			bool prepareBuffers();
			void renderOnce();
			void deleteData();
			bool fillUpBuffers();
			float* m_Vertices;
			float* m_Colors;
			float* m_Normals;
			// VBO availability checked via GLEW_VERSION_1_5
			bool checkForVertexBuffers();
			int m_NumberOfVertices;
			GLuint m_VertexBuffer;
			GLuint m_NormalBuffer;
			GLuint m_ColorBuffer;
			unsigned int m_NumberOfLevels;
	};
};

#endif
