#ifdef WIN32
#include <Windows.h>
#endif
#include <GLES2/gl2.h>
#include <malloc.h>
#include "defines.h"

void __gluMultMatrixVecd(const GLfloat matrix[16], const GLfloat in[4], GLfloat out[4])
{
    int i;

    for (i=0; i<4; i++) {
        out[i] = 
            in[0] * matrix[0*4+i] +
            in[1] * matrix[1*4+i] +
            in[2] * matrix[2*4+i] +
            in[3] * matrix[3*4+i];
    }
}
void __gluMultMatricesd(const GLfloat a[16], const GLfloat b[16],GLfloat r[16])
{
    int i, j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            r[i*4+j] = 
                a[i*4+0]*b[0*4+j] +
                a[i*4+1]*b[1*4+j] +
                a[i*4+2]*b[2*4+j] +
                a[i*4+3]*b[3*4+j];
        }
    }
}
int __gluInvertMatrixd(const GLfloat m[16], GLfloat invOut[16])
{
    float inv[16], det;
    int i;

    inv[0] =   m[5]*m[10]*m[15] - m[5]*m[11]*m[14] - m[9]*m[6]*m[15]
             + m[9]*m[7]*m[14] + m[13]*m[6]*m[11] - m[13]*m[7]*m[10];
    inv[4] =  -m[4]*m[10]*m[15] + m[4]*m[11]*m[14] + m[8]*m[6]*m[15]
             - m[8]*m[7]*m[14] - m[12]*m[6]*m[11] + m[12]*m[7]*m[10];
    inv[8] =   m[4]*m[9]*m[15] - m[4]*m[11]*m[13] - m[8]*m[5]*m[15]
             + m[8]*m[7]*m[13] + m[12]*m[5]*m[11] - m[12]*m[7]*m[9];
    inv[12] = -m[4]*m[9]*m[14] + m[4]*m[10]*m[13] + m[8]*m[5]*m[14]
             - m[8]*m[6]*m[13] - m[12]*m[5]*m[10] + m[12]*m[6]*m[9];
    inv[1] =  -m[1]*m[10]*m[15] + m[1]*m[11]*m[14] + m[9]*m[2]*m[15]
             - m[9]*m[3]*m[14] - m[13]*m[2]*m[11] + m[13]*m[3]*m[10];
    inv[5] =   m[0]*m[10]*m[15] - m[0]*m[11]*m[14] - m[8]*m[2]*m[15]
             + m[8]*m[3]*m[14] + m[12]*m[2]*m[11] - m[12]*m[3]*m[10];
    inv[9] =  -m[0]*m[9]*m[15] + m[0]*m[11]*m[13] + m[8]*m[1]*m[15]
             - m[8]*m[3]*m[13] - m[12]*m[1]*m[11] + m[12]*m[3]*m[9];
    inv[13] =  m[0]*m[9]*m[14] - m[0]*m[10]*m[13] - m[8]*m[1]*m[14]
             + m[8]*m[2]*m[13] + m[12]*m[1]*m[10] - m[12]*m[2]*m[9];
    inv[2] =   m[1]*m[6]*m[15] - m[1]*m[7]*m[14] - m[5]*m[2]*m[15]
             + m[5]*m[3]*m[14] + m[13]*m[2]*m[7] - m[13]*m[3]*m[6];
    inv[6] =  -m[0]*m[6]*m[15] + m[0]*m[7]*m[14] + m[4]*m[2]*m[15]
             - m[4]*m[3]*m[14] - m[12]*m[2]*m[7] + m[12]*m[3]*m[6];
    inv[10] =  m[0]*m[5]*m[15] - m[0]*m[7]*m[13] - m[4]*m[1]*m[15]
             + m[4]*m[3]*m[13] + m[12]*m[1]*m[7] - m[12]*m[3]*m[5];
    inv[14] = -m[0]*m[5]*m[14] + m[0]*m[6]*m[13] + m[4]*m[1]*m[14]
             - m[4]*m[2]*m[13] - m[12]*m[1]*m[6] + m[12]*m[2]*m[5];
    inv[3] =  -m[1]*m[6]*m[11] + m[1]*m[7]*m[10] + m[5]*m[2]*m[11]
             - m[5]*m[3]*m[10] - m[9]*m[2]*m[7] + m[9]*m[3]*m[6];
    inv[7] =   m[0]*m[6]*m[11] - m[0]*m[7]*m[10] - m[4]*m[2]*m[11]
             + m[4]*m[3]*m[10] + m[8]*m[2]*m[7] - m[8]*m[3]*m[6];
    inv[11] = -m[0]*m[5]*m[11] + m[0]*m[7]*m[9] + m[4]*m[1]*m[11]
             - m[4]*m[3]*m[9] - m[8]*m[1]*m[7] + m[8]*m[3]*m[5];
    inv[15] =  m[0]*m[5]*m[10] - m[0]*m[6]*m[9] - m[4]*m[1]*m[10]
             + m[4]*m[2]*m[9] + m[8]*m[1]*m[6] - m[8]*m[2]*m[5];

    det = m[0]*inv[0] + m[1]*inv[4] + m[2]*inv[8] + m[3]*inv[12];
    if (det == 0)
        return GL_FALSE;

    det = 1.0f / det;

    for (i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;
	return GL_TRUE;
}
int MYgluUnProject(GLfloat winx, GLfloat winy, GLfloat winz,
		const GLfloat modelMatrix[16], 
		const GLfloat projMatrix[16],
        const GLint viewport[4],
	    GLfloat *objx, GLfloat *objy, GLfloat *objz)
{
    float finalMatrix[16];
    float in[4];
    float out[4];

    __gluMultMatricesd(modelMatrix, projMatrix, finalMatrix);
    if (!__gluInvertMatrixd(finalMatrix, finalMatrix)) return(GL_FALSE);

    in[0]=winx;
    in[1]=winy;
    in[2]=winz;
    in[3]=1.0;

    /* Map x and y from window coordinates */
    in[0] = (in[0] - viewport[0]) / viewport[2];
    in[1] = (in[1] - viewport[1]) / viewport[3];

    /* Map to range -1 to 1 */
    in[0] = in[0] * 2 - 1;
    in[1] = in[1] * 2 - 1;
    in[2] = in[2] * 2 - 1;

    __gluMultMatrixVecd(finalMatrix, in, out);
    if (out[3] == 0.0) return(GL_FALSE);
    out[0] /= out[3];
    out[1] /= out[3];
    out[2] /= out[3];
    *objx = out[0];
    *objy = out[1];
    *objz = out[2];
    return(GL_TRUE);
}

/*
mathHelper::VEC3 GetOGLPos(int x, int y)
{
	GLint viewport[4];
	GLfloat modelview[16];
	GLfloat projection[16];
	GLfloat winX, winY, winZ;
	GLfloat posX, posY, posZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	MYgluUnProject( winX, winY, 0, modelview, projection, viewport, &posX, &posY, &posZ);

	return mathHelper::VEC3(posX, posY, posZ);
}
mathHelper::VEC3 GetOGLPosOnScreen(float x, float y,float z)
{
	GLint viewport[4];
	GLfloat modelview[16];
	GLfloat projection[16];
	GLfloat winX, winY, winZ;
	GLfloat posX, posY, posZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

//	winX = (float)x;
//	winY = (float)viewport[3] - (float)y;
//	glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	gluProject( x, y, z, modelview, projection, viewport, &posX, &posY, &posZ);

	return mathHelper::VEC3((float)posX, (float)posY, (float)posZ);
}
*/
mathHelper::VEC2 ClosestPointOnSegment(mathHelper::VEC2& A, mathHelper::VEC2& B, mathHelper::VEC2& P)
{
    mathHelper::VEC2 D = B - A;
	mathHelper::VEC2 m = P - A;
    float numer = mathHelper::VEC2Dot(&m,&D);
    if (numer <= 0.0f)
        return A;
    float denom = mathHelper::VEC2Dot(&D,&D);
    if (numer >= denom)
        return B;
    return A + (numer/denom) * D;
}

mathHelper::VEC2 closest_point_on_segment_AB_to_point_P(mathHelper::VEC2 A, mathHelper::VEC2 B, mathHelper::VEC2 P)
{
	mathHelper::VEC2 U = B - A;
	mathHelper::VEC2 m = P - A;
	float t = mathHelper::VEC2Dot(&m,&U) / mathHelper::VEC2Dot(&U,&U);
	if ( t < 0 ) t = 0; if ( t > 1 ) t = 1;
	return A + U * t;
}
float PointDistanceFromLine(mathHelper::VEC2 A, mathHelper::VEC2 B, mathHelper::VEC2 P)
{
	mathHelper::VEC2 Pt = closest_point_on_segment_AB_to_point_P(A, B, P);

	return sqrtf((P-Pt).LenSq());
}
int GetFileSize(const char* path)
{
	FILE *pFile = NULL;
	int Size = 0;
	if(pFile = fopen(path, "rb" ))
	{
		fseek( pFile, 0, SEEK_END );
		Size = ftell( pFile );
		fclose( pFile );
	}
	return Size;
}
char* LoadFile(const char* szFileName,int& iLen)
{
	char* m_pMem = 0;
	iLen = GetFileSize(szFileName);
	if(iLen)
	{
		m_pMem = (char*)malloc(iLen + 1);
		if(FILE *pFile = fopen(szFileName, "rb" ))
		{
			int l = fread(m_pMem,1,iLen,pFile);
			iLen = l;
			m_pMem[iLen] = 0;
			fclose( pFile );
		}
	}

	return m_pMem;
}
