#include "mathHelper.h"

namespace mathHelper
{
	void C_QUATScale( const Quaternion *q, Quaternion *r, float scale )
	{
		r->x = q->x * scale;
		r->y = q->y * scale;
		r->z = q->z * scale;
		r->w = q->w * scale;
	}
	void C_QUATNormalize( const Quaternion *src, Quaternion *unit )
	{
		float mag;

		mag = (src->x * src->x) + (src->y * src->y) + (src->z * src->z) + (src->w * src->w);

		if ( mag >= QUAT_EPSILON )
		{
			mag = 1.0f / sqrt(mag);
	    
			unit->x = src->x * mag;
			unit->y = src->y * mag;
			unit->z = src->z * mag;
			unit->w = src->w * mag;
		}
		else
		{
			unit->x = unit->y = unit->z = unit->w = 0.0F;    
		}
	}
	void C_QUATMtx( Quaternion *r, const Mtx m )
	{
		float tr,s;
		int i,j,k;
		int nxt[3] = {1,2,0};
		float q[3];

		tr = m[0][0] + m[1][1] + m[2][2];
		if( tr > 0.0f )
		{
			s = (float)sqrt(tr + 1.0f);
			r->w = s * 0.5f;
			s = 0.5f / s;
			r->x = (m[2][1] - m[1][2]) * s;
			r->y = (m[0][2] - m[2][0]) * s;
			r->z = (m[1][0] - m[0][1]) * s;
		}
		else 
		{
			i = 0;
			if (m[1][1] > m[0][0]) i = 1;
			if (m[2][2] > m[i][i]) i = 2;
			j = nxt[i];
			k = nxt[j];
			s = (float)sqrt( (m[i][i] - (m[j][j] + m[k][k])) + 1.0 );
			q[i] = s * 0.5f;
	        
			if (s!=0.0f)
				s = 0.5f / s;
	        
			r->w = (m[k][j] - m[j][k]) * s;
			q[j] = (m[i][j] + m[j][i]) * s;
			q[k] = (m[i][k] + m[k][i]) * s;

			r->x = q[0];
			r->y = q[1];
			r->z = q[2];
		}
	}
	void C_QUATLogN( const Quaternion *q, Quaternion *r )
	{
		float theta,scale;

		scale = q->x*q->x + q->y*q->y + q->z*q->z;

		scale = sqrt(scale);
		theta = atan2( scale, q->w );

		if ( scale > 0.0F )
			scale = theta/scale;

		r->x = scale*q->x;
		r->y = scale*q->y;
		r->z = scale*q->z;
		r->w = 0.0F;
	}

	void C_QUATLerp( const Quaternion *p, const Quaternion *q, Quaternion *r, float t )
	{
		r->x = t * ( q->x - p->x ) + p->x;
		r->y = t * ( q->y - p->y ) + p->y;
		r->z = t * ( q->z - p->z ) + p->z;
		r->w = t * ( q->w - p->w ) + p->w;
	}

	void C_QUATExp( const Quaternion *q, Quaternion *r ) 
	{
		float theta, scale;

		theta = sqrt( q->x*q->x + q->y*q->y + q->z*q->z );
		scale = 1.0;

		if ( theta > QUAT_EPSILON )
			scale = (float)sin(theta)/theta;
	    
		r->x = scale * q->x;
		r->y = scale * q->y;
		r->z = scale * q->z;
		r->w = (float)cos(theta);
	}
	float C_QUATDotProduct( const Quaternion *p, const Quaternion *q ) 
	{
		return (q->x*p->x + q->y*p->y + q->z*p->z + q->w*p->w);
	}
	void C_QUATMultiply( const Quaternion *p, const Quaternion *q, Quaternion *pq )
	{
		Quaternion *r;
		Quaternion  pqTmp;

		if ( p == pq || q == pq )
		{
			r = &pqTmp;
		}
		else
		{
			r = pq;
		}

		r->w = p->w*q->w - p->x*q->x - p->y*q->y - p->z*q->z;
		r->x = p->w*q->x + p->x*q->w + p->y*q->z - p->z*q->y;
		r->y = p->w*q->y + p->y*q->w + p->z*q->x - p->x*q->z;
		r->z = p->w*q->z + p->z*q->w + p->x*q->y - p->y*q->x;
	    
		if ( r == &pqTmp )
		{
			*pq = pqTmp;
		}
	}
	void C_QUATInverse( const Quaternion *src, Quaternion *inv )
	{
		float mag, norminv;

		mag = ( src->x*src->x + src->y*src->y + src->z*src->z + src->w*src->w );

		if ( mag == 0.0f )
		{
			mag = 1.0f;
		}

		// [Inverse] = [Conjugate] / [Magnitude]
		norminv = 1.0f / mag;
		inv->x = -src->x * norminv;
		inv->y = -src->y * norminv;
		inv->z = -src->z * norminv;
		inv->w =  src->w * norminv;
	}
	void C_QUATAdd( const Quaternion *p, const Quaternion *q, Quaternion *r )
	{
		r->x = p->x + q->x;
		r->y = p->y + q->y;
		r->z = p->z + q->z;
		r->w = p->w + q->w;
	}
	void C_QUATDivide( const Quaternion *p, const Quaternion *q, Quaternion *r )
	{
		Quaternion qtmp;
		C_QUATInverse(q, &qtmp);
		C_QUATMultiply(&qtmp, p, r);
	}
	void C_QUATSlerp( const Quaternion *p, const Quaternion *q, Quaternion *r, float t )
	{
		float theta, sin_th, cos_th, tp, tq;
	    
		cos_th = p->x * q->x + p->y * q->y + p->z * q->z + p->w * q->w;
		tq     = 1.0F;
	    
		if ( cos_th < 0.0F )
		{
			cos_th = -cos_th;
			tq     = -tq;
		}

		if ( cos_th <= 1.0F - QUAT_EPSILON )
		{
			theta  = acos(cos_th);
			sin_th = sin(theta);
			tp     = sin((1.0f - t) * theta) / sin_th;
			tq    *= sin( t * theta ) / sin_th;
		}
		else
		{
			// cos(theta) is close to 1.0F -> linear
			tp = 1.0f - t;
			tq = tq * t;
		}

		r->x = tp * p->x + tq * q->x;
		r->y = tp * p->y + tq * q->y;
		r->z = tp * p->z + tq * q->z;
		r->w = tp * p->w + tq * q->w;

	}
	void C_QUATSquad( const Quaternion *p, const Quaternion *a, const Quaternion *b,
					  const Quaternion *q, Quaternion *r, float t )
	{
		Quaternion pq, ab;
		float t2;

		t2 = 2 * t * ( 1.0F - t );
		C_QUATSlerp(p, q, &pq, t);
		C_QUATSlerp(a, b, &ab, t);
		C_QUATSlerp(&pq, &ab, r, t2);
	}
	void C_QUATSubtract( const Quaternion *p, const Quaternion *q, Quaternion *r )
	{
		r->x = p->x - q->x;
		r->y = p->y - q->y;
		r->z = p->z - q->z;
		r->w = p->w - q->w;
	}

	MTX34* C_MTX34RotX(MTX34* pOut, float fRadX)
	{
		C_MTXIdentity(pOut->mtx);
		pOut->_11 = cos(fRadX);
		pOut->_12 = -sin(fRadX);
		pOut->_21 = sin(fRadX);
		pOut->_22 = cos(fRadX);
		return pOut;
	}
	MTX34* C_MTX34RotY(MTX34* pOut, float fRadY)
	{
		C_MTXIdentity(pOut->mtx);
		pOut->_00 = cos(fRadY);
		pOut->_02 = sin(fRadY);
		pOut->_20 = -sin(fRadY);
		pOut->_22 = cos(fRadY);
		return pOut;
	}
	MTX34* C_MTX34RotZ(MTX34* pOut, float fRadZ)
	{
		C_MTXIdentity(pOut->mtx);
		pOut->a[0] = cos(fRadZ);
		pOut->a[1] = sin(fRadZ);
		pOut->a[4] = -pOut->a[1];
		pOut->a[5] = pOut->a[0];
/*		pOut->_00 = cos(fRadZ);
		pOut->_01 = -sin(fRadZ);
		pOut->_10 = sin(fRadZ);
		pOut->_11 = cos(fRadZ);
		return pOut;
*/
/*		float s = sin(fRadZ);
		float c = cos(fRadZ);
		pOut->_00 = s;pOut->_02 = c;
		pOut->_10 = s * c;pOut->_11 = c;pOut->_12 = -s * s;
		pOut->_20 = c * c;pOut->_21 = -s;pOut->_22 = -c * s;
*/
		return pOut;
	}

	Quaternion EulerToQuat(float roll, float pitch, float yaw)
	{
		Quaternion q;
		float cr, cp, cy, sr, sp, sy, cpcy, spsy;

		// compute all trigonometric values used to compute the quaternion
		cr = cos(roll/2);
		cp = cos(pitch/2);
		cy = cos(yaw/2);

		sr = sin(roll/2);
		sp = sin(pitch/2);
		sy = sin(yaw/2);

		cpcy = cp * cy;
		spsy = sp * sy;

		// combine values to generate the vector and scalar for the quaternion
		q.w = cr * cpcy + sr * spsy;
		q.x = sr * cpcy - cr * spsy;
		q.y = cr * sp * cy + sr * cp * sy;
		q.z = cr * cp * sy - sr * sp * cy;
		return q;
	}

	MTX34* C_MTX34RotXYZ(MTX34* pOut, float fRadX, float fRadY, float fRadZ)
	{
		MTX34 m,mx,my,mz;//,mxI,mzI;
		C_MTXIdentity(m);
		C_MTXIdentity(mx);
		C_MTXIdentity(my);
		C_MTXIdentity(mz);
		C_MTX34RotX(&mx,fRadX);
		C_MTX34RotY(&my,fRadY);
		C_MTX34RotZ(&mz,fRadZ);

		float x = fRadY;
		float y = fRadZ;
		float z = fRadX;
		m.m[0][0] = cosf(x) * cos(y);
		m.m[0][1] = cosf(x) * sin(y);
		m.m[0][2] = -sinf(x);

		m.m[1][0] = sinf(z) * sinf(x) * cosf(y) - cosf(z) * sinf(y);
		m.m[1][1] = sinf(z) * sinf(x) * sinf(y) + cosf(z) * cosf(y);
		m.m[1][2] = cosf(x) * sinf(z);

		m.m[2][0] = cosf(z) * sinf(x) * cosf(y) + sinf(z) * sinf(y);
		m.m[2][1] = cosf(z) * sinf(x) * sinf(y) - sinf(z) * cosf(y);
		m.m[2][2] = cosf(x) * cosf(z);

//		C_MTXConcat(mz,my,m);
//		C_MTXInverse(mx,mxI);
//		C_MTXInverse(mz,mzI);
//		C_MTXMult(m,mz,mx);
//		C_MTXInverse(m,m);
//		C_MTXConcat(m,mx,m);
		C_MTXInverse(m,m);
//		C_MTXConcat(m,my,m);
//		C_MTXInverse(m,m);
//		C_MTXInverse(m,m);
//		C_MTXInverse(m,m);
//		C_MTXInverse(m,m);
//		C_MTXConcat(m,mx,m);
//		C_MTXInverse(m,m);
		*pOut = m;
		return pOut;
	}
	MTX34* C_MTX34RotAxisRad(MTX34* pOut, const VEC3* pAxis, float fRad)
	{
		return pOut;
	}
	void C_MTXLookAt ( Mtx m, const Point3d *camPos, const Vec *camUp, const Point3d *target )
	{

		Vec vLook,vRight,vUp;

		// compute unit target vector
		// use negative value to look down (-Z) axis
		vLook.x = camPos->x - target->x;
		vLook.y = camPos->y - target->y;
		vLook.z = camPos->z - target->z;
		VECNormalize( &vLook,&vLook );


		// vRight = camUp x vLook
		VECCrossProduct    ( camUp, &vLook, &vRight );
		VECNormalize( &vRight,&vRight );


		// vUp = vLook x vRight
		VECCrossProduct( &vLook, &vRight, &vUp );
		// Don't need to normalize vUp since it should already be unit length
		// VECNormalize( &vUp, &vUp );


		m[0][0] = vRight.x;
		m[0][1] = vRight.y;
		m[0][2] = vRight.z;
		m[0][3] = -( camPos->x * vRight.x + camPos->y * vRight.y + camPos->z * vRight.z );

		m[1][0] = vUp.x;
		m[1][1] = vUp.y;
		m[1][2] = vUp.z;
		m[1][3] = -( camPos->x * vUp.x + camPos->y * vUp.y + camPos->z * vUp.z );

		m[2][0] = vLook.x;
		m[2][1] = vLook.y;
		m[2][2] = vLook.z;
		m[2][3] = -( camPos->x * vLook.x + camPos->y * vLook.y + camPos->z * vLook.z );


	}
	void C_MTXCopy ( const Mtx src, Mtx dst )
	{

		if( src == dst )
		{
			return;
		}


		dst[0][0] = src[0][0];    dst[0][1] = src[0][1];    dst[0][2] = src[0][2];    dst[0][3] = src[0][3];

		dst[1][0] = src[1][0];    dst[1][1] = src[1][1];    dst[1][2] = src[1][2];    dst[1][3] = src[1][3];

		dst[2][0] = src[2][0];    dst[2][1] = src[2][1];    dst[2][2] = src[2][2];    dst[2][3] = src[2][3];

	}
	void C_MTXConcat ( const Mtx a, const Mtx b, Mtx ab )
	{
		Mtx mTmp;
		MtxPtr m;

		if( (ab == a) || (ab == b) )
		{
			m = mTmp;
		}

		else
		{
			m = ab;
		}

		// compute (a x b) -> m

		m[0][0] = a[0][0]*b[0][0] + a[0][1]*b[1][0] + a[0][2]*b[2][0];
		m[0][1] = a[0][0]*b[0][1] + a[0][1]*b[1][1] + a[0][2]*b[2][1];
		m[0][2] = a[0][0]*b[0][2] + a[0][1]*b[1][2] + a[0][2]*b[2][2];
		m[0][3] = a[0][0]*b[0][3] + a[0][1]*b[1][3] + a[0][2]*b[2][3] + a[0][3];

		m[1][0] = a[1][0]*b[0][0] + a[1][1]*b[1][0] + a[1][2]*b[2][0];
		m[1][1] = a[1][0]*b[0][1] + a[1][1]*b[1][1] + a[1][2]*b[2][1];
		m[1][2] = a[1][0]*b[0][2] + a[1][1]*b[1][2] + a[1][2]*b[2][2];
		m[1][3] = a[1][0]*b[0][3] + a[1][1]*b[1][3] + a[1][2]*b[2][3] + a[1][3];

		m[2][0] = a[2][0]*b[0][0] + a[2][1]*b[1][0] + a[2][2]*b[2][0];
		m[2][1] = a[2][0]*b[0][1] + a[2][1]*b[1][1] + a[2][2]*b[2][1];
		m[2][2] = a[2][0]*b[0][2] + a[2][1]*b[1][2] + a[2][2]*b[2][2];
		m[2][3] = a[2][0]*b[0][3] + a[2][1]*b[1][3] + a[2][2]*b[2][3] + a[2][3];


		// overwrite a or b if needed
		if(m == mTmp)
		{
			C_MTXCopy( mTmp, ab );
		}
	}
	void C_MTXConcatArray ( const Mtx a, const Mtx* srcBase, Mtx* dstBase, int count )
	{
		int i;

		for ( i = 0 ; i < count ; i++ )
		{
			C_MTXConcat(a, *srcBase, *dstBase);
	        
			srcBase++;
			dstBase++;
		}
	}
	void C_MTXTranspose ( const Mtx src, Mtx xPose )
	{
		Mtx mTmp;
		MtxPtr m;

		if(src == xPose)
		{
			m = mTmp;
		}
		else
		{
			m = xPose;
		}


		m[0][0] = src[0][0];   m[0][1] = src[1][0];      m[0][2] = src[2][0];     m[0][3] = 0.0f;
		m[1][0] = src[0][1];   m[1][1] = src[1][1];      m[1][2] = src[2][1];     m[1][3] = 0.0f;
		m[2][0] = src[0][2];   m[2][1] = src[1][2];      m[2][2] = src[2][2];     m[2][3] = 0.0f;


		// copy back if needed
		if( m == mTmp )
		{
			C_MTXCopy( mTmp, xPose );
		}
	}

	void C_MTXIdentity ( Mtx m )
	{

		m[0][0] = 1.0f;     m[0][1] = 0.0f;  m[0][2] = 0.0f;  m[0][3] = 0.0f;

		m[1][0] = 0.0f;     m[1][1] = 1.0f;  m[1][2] = 0.0f;  m[1][3] = 0.0f;

		m[2][0] = 0.0f;     m[2][1] = 0.0f;  m[2][2] = 1.0f;  m[2][3] = 0.0f;

	}
	void C_MTXScaleApply ( const Mtx src, Mtx dst, float xS, float  yS, float zS )
	{
		dst[0][0] = src[0][0] * xS;     dst[0][1] = src[0][1] * xS;
		dst[0][2] = src[0][2] * xS;     dst[0][3] = src[0][3] * xS;

		dst[1][0] = src[1][0] * yS;     dst[1][1] = src[1][1] * yS;
		dst[1][2] = src[1][2] * yS;     dst[1][3] = src[1][3] * yS;

		dst[2][0] = src[2][0] * zS;     dst[2][1] = src[2][1] * zS;
		dst[2][2] = src[2][2] * zS;     dst[2][3] = src[2][3] * zS;
	}
	void C_MTXTransApply ( const Mtx src, Mtx dst, float xT, float yT, float zT )
	{
		if ( src != dst )
		{
			dst[0][0] = src[0][0];    dst[0][1] = src[0][1];    dst[0][2] = src[0][2];
			dst[1][0] = src[1][0];    dst[1][1] = src[1][1];    dst[1][2] = src[1][2];
			dst[2][0] = src[2][0];    dst[2][1] = src[2][1];    dst[2][2] = src[2][2];
		}
	    
		dst[0][3] = src[0][3] + xT;
		dst[1][3] = src[1][3] + yT;
		dst[2][3] = src[2][3] + zT;
	}
	void C_MTXQuat ( Mtx m, const Quaternion *q )
	{

		float s,xs,ys,zs,wx,wy,wz,xx,xy,xz,yy,yz,zz;

		s = 2.0f / ( (q->x * q->x) + (q->y * q->y) + (q->z * q->z) + (q->w * q->w) );

		xs = q->x *  s;     ys = q->y *  s;  zs = q->z *  s;
		wx = q->w * xs;     wy = q->w * ys;  wz = q->w * zs;
		xx = q->x * xs;     xy = q->x * ys;  xz = q->x * zs;
		yy = q->y * ys;     yz = q->y * zs;  zz = q->z * zs;


		m[0][0] = 1.0f - (yy + zz);
		m[0][1] = xy   - wz;
		m[0][2] = xz   + wy;
		m[0][3] = 0.0f;

		m[1][0] = xy   + wz;
		m[1][1] = 1.0f - (xx + zz);
		m[1][2] = yz   - wx;
		m[1][3] = 0.0f;

		m[2][0] = xz   - wy;
		m[2][1] = yz   + wx;
		m[2][2] = 1.0f - (xx + yy);
		m[2][3] = 0.0f;

	}
	int C_MTXInverse ( const Mtx src, Mtx inv )
	{
		Mtx mTmp;
		MtxPtr m;
		float det;

		if( src == inv )
		{
			m = mTmp;
		}
		else
		{
			m = inv;
		}


		// compute the determinant of the upper 3x3 submatrix
		det =   src[0][0]*src[1][1]*src[2][2] + src[0][1]*src[1][2]*src[2][0] + src[0][2]*src[1][0]*src[2][1]
			  - src[2][0]*src[1][1]*src[0][2] - src[1][0]*src[0][1]*src[2][2] - src[0][0]*src[2][1]*src[1][2];


		// check if matrix is singular
		if( det == 0.0f )
		{
			return 0;
		}


		// compute the inverse of the upper submatrix:

		// find the transposed matrix of cofactors of the upper submatrix
		// and multiply by (1/det)

		det = 1.0f / det;


		m[0][0] =  (src[1][1]*src[2][2] - src[2][1]*src[1][2]) * det;
		m[0][1] = -(src[0][1]*src[2][2] - src[2][1]*src[0][2]) * det;
		m[0][2] =  (src[0][1]*src[1][2] - src[1][1]*src[0][2]) * det;

		m[1][0] = -(src[1][0]*src[2][2] - src[2][0]*src[1][2]) * det;
		m[1][1] =  (src[0][0]*src[2][2] - src[2][0]*src[0][2]) * det;
		m[1][2] = -(src[0][0]*src[1][2] - src[1][0]*src[0][2]) * det;

		m[2][0] =  (src[1][0]*src[2][1] - src[2][0]*src[1][1]) * det;
		m[2][1] = -(src[0][0]*src[2][1] - src[2][0]*src[0][1]) * det;
		m[2][2] =  (src[0][0]*src[1][1] - src[1][0]*src[0][1]) * det;


		// compute (invA)*(-C)
		m[0][3] = -m[0][0]*src[0][3] - m[0][1]*src[1][3] - m[0][2]*src[2][3];
		m[1][3] = -m[1][0]*src[0][3] - m[1][1]*src[1][3] - m[1][2]*src[2][3];
		m[2][3] = -m[2][0]*src[0][3] - m[2][1]*src[1][3] - m[2][2]*src[2][3];

		// copy back if needed
		if( m == mTmp )
		{
			C_MTXCopy( mTmp,inv );
		}

		return 1;
	}
	int C_MTXInvXpose ( const Mtx src, Mtx invX )
	{
		Mtx mTmp;
		MtxPtr m;
		float det;

		if( src == invX )
		{
			m = mTmp;
		}
		else
		{
			m = invX;
		}

		// compute the determinant of the upper 3x3 submatrix
		det =   src[0][0]*src[1][1]*src[2][2] + src[0][1]*src[1][2]*src[2][0] + src[0][2]*src[1][0]*src[2][1]
			  - src[2][0]*src[1][1]*src[0][2] - src[1][0]*src[0][1]*src[2][2] - src[0][0]*src[2][1]*src[1][2];

		// check if matrix is singular
		if( det == 0.0f )
		{
			return 0;
		}

		// compute the inverse-transpose of the upper submatrix:

		// find the transposed matrix of cofactors of the upper submatrix
		// and multiply by (1/det)

		det = 1.0f / det;

		m[0][0] =  (src[1][1]*src[2][2] - src[2][1]*src[1][2]) * det;
		m[0][1] = -(src[1][0]*src[2][2] - src[2][0]*src[1][2]) * det;
		m[0][2] =  (src[1][0]*src[2][1] - src[2][0]*src[1][1]) * det;

		m[1][0] = -(src[0][1]*src[2][2] - src[2][1]*src[0][2]) * det;
		m[1][1] =  (src[0][0]*src[2][2] - src[2][0]*src[0][2]) * det;
		m[1][2] = -(src[0][0]*src[2][1] - src[2][0]*src[0][1]) * det;

		m[2][0] =  (src[0][1]*src[1][2] - src[1][1]*src[0][2]) * det;
		m[2][1] = -(src[0][0]*src[1][2] - src[1][0]*src[0][2]) * det;
		m[2][2] =  (src[0][0]*src[1][1] - src[1][0]*src[0][1]) * det;


		// the fourth columns should be all zero
		m[0][3] = 0.0F;
		m[1][3] = 0.0F;
		m[2][3] = 0.0F;

		// copy back if needed
		if( m == mTmp )
		{
			C_MTXCopy( mTmp,invX );
		}

		return 1;
	}
	void C_MTXMultVec ( const Mtx m, const Vec *src, Vec *dst )
	{
		Vec vTmp;

		// A Vec has a 4th implicit 'w' coordinate of 1
		vTmp.x = m[0][0]*src->x + m[0][1]*src->y + m[0][2]*src->z + m[0][3];
		vTmp.y = m[1][0]*src->x + m[1][1]*src->y + m[1][2]*src->z + m[1][3];
		vTmp.z = m[2][0]*src->x + m[2][1]*src->y + m[2][2]*src->z + m[2][3];

		// Copy back
		dst->x = vTmp.x;
		dst->y = vTmp.y;
		dst->z = vTmp.z;
	}
	void C_MTXMultNorm ( const Mtx m, const Vec *src, Vec *dst )
	{
		Vec vTmp;

		// A Vec has a 4th implicit 'w' coordinate of 1
		vTmp.x = m[0][0]*src->x + m[0][1]*src->y + m[0][2]*src->z;
		vTmp.y = m[1][0]*src->x + m[1][1]*src->y + m[1][2]*src->z;
		vTmp.z = m[2][0]*src->x + m[2][1]*src->y + m[2][2]*src->z;

		// Copy back
		dst->x = vTmp.x;
		dst->y = vTmp.y;
		dst->z = vTmp.z;
	}
	void C_MTXMultVecArray ( const Mtx m, const Vec *srcBase, Vec *dstBase, int count )
	{
		int i;
		Vec vTmp;

		for(i=0; i< count; i++)
		{
			// Vec has a 4th implicit 'w' coordinate of 1
			vTmp.x = m[0][0]*srcBase->x + m[0][1]*srcBase->y + m[0][2]*srcBase->z + m[0][3];
			vTmp.y = m[1][0]*srcBase->x + m[1][1]*srcBase->y + m[1][2]*srcBase->z + m[1][3];
			vTmp.z = m[2][0]*srcBase->x + m[2][1]*srcBase->y + m[2][2]*srcBase->z + m[2][3];

			// Copy back
			dstBase->x = vTmp.x;
			dstBase->y = vTmp.y;
			dstBase->z = vTmp.z;

			srcBase++;
			dstBase++;
		}
	}
	void C_MTXMultVecArray1 ( const Mtx m, const Vec *srcBase, Vec *dstBase, int count )
	{
		int i;
		Vec vTmp;

		for(i=0; i< count; i++)
		{
			// Vec has a 4th implicit 'w' coordinate of 1
			Vec v;
			v.x = srcBase->x + m[0][3];
			v.y = srcBase->y + m[1][3];
			v.z = srcBase->z + m[2][3];
			vTmp.x = m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z;
			vTmp.y = m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z;
			vTmp.z = m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z;

			// Copy back
			dstBase->x = vTmp.x;
			dstBase->y = vTmp.y;
			dstBase->z = vTmp.z;

			srcBase++;
			dstBase++;
		}
	}
	void VECCrossProduct ( const Vec *a, const Vec *b, Vec *axb )
	{
		Vec vTmp;
		vTmp.x =  ( a->y * b->z ) - ( a->z * b->y );
		vTmp.y =  ( a->z * b->x ) - ( a->x * b->z );
		vTmp.z =  ( a->x * b->y ) - ( a->y * b->x );
		axb->x = vTmp.x;
		axb->y = vTmp.y;
		axb->z = vTmp.z;
	}
	float VECSquareMag ( const Vec *v )
	{
		float sqmag;
		sqmag = (v->x * v->x) + (v->y * v->y) + (v->z * v->z);
		return sqmag;
	}
	float VECMag ( const Vec *v )
	{
		return sqrt( VECSquareMag(v) );
	}
	void VECNormalize ( const Vec *src, Vec *unit )
	{
		float mag;
		mag = (src->x * src->x) + (src->y * src->y) + (src->z * src->z);
		mag = 1.0f / sqrt(mag);
		unit->x = src->x * mag;
		unit->y = src->y * mag;
		unit->z = src->z * mag;
	}
	float VECSquareDistance( const Vec *a, const Vec *b )
	{
		Vec diff;

		diff.x = a->x - b->x;
		diff.y = a->y - b->y;
		diff.z = a->z - b->z;

		return (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);
	}
	void	quaternion_rotate_about_x(mathHelper::QUAT *quat, float angle)
	{
		mathHelper::QUAT result;
		float a = angle * 0.5f;
		float w = cos(a);
		float scale = sin(a);

		result.w = quat->w * w - quat->x * scale;
		result.x = quat->w * scale + quat->x * w;
		result.y = quat->y * w + quat->z * scale;
		result.z = quat->z * w - quat->y * scale;
	    
		quat->w = result.w;
		quat->x = result.x;
		quat->y = result.y;
		quat->z = result.z;
	}
	void	quaternion_rotate_about_y(mathHelper::QUAT *quat, float angle)
	{
		mathHelper::QUAT result;
		float a = angle * 0.5f;
		float w = cos(a);
		float scale = sin(a);

		result.w = quat->w * w - quat->y * scale;
		result.x = quat->x * w - quat->z * scale;
		result.y = quat->w * scale + quat->y * w;
		result.z = quat->z * w + quat->x * scale;
	    
		quat->w = result.w;
		quat->x = result.x;
		quat->y = result.y;
		quat->z = result.z;
	}
	void	quaternion_rotate_about_z(mathHelper::QUAT *quat, float angle)
	{
		mathHelper::QUAT result;
		float a = angle * 0.5f;
		float w = cos(a);
		float scale = sin(a);
	    
		result.w = quat->w * w - quat->z * scale;
		result.x = quat->x * w + quat->y * scale;
		result.y = quat->y * w - quat->x * scale;
		result.z = quat->w * scale + quat->z * w;
	    
		quat->w = result.w;
		quat->x = result.x;
		quat->y = result.y;
		quat->z = result.z;
	}
	mathHelper::QUAT applyRoll(float roll1,float climb1)
	{
		mathHelper::QUAT q_rotation;
		q_rotation.w = 1.0;
		q_rotation.x = 0.0;
		q_rotation.y = 0.0;
		q_rotation.z = 0.0;

		if ((roll1 == 0.0)&&(climb1 == 0.0))
			return q_rotation;

		if (roll1)
			quaternion_rotate_about_z( &q_rotation, -roll1);
		if (climb1)
			quaternion_rotate_about_x( &q_rotation, -climb1);

		QUATNormalize(&q_rotation,&q_rotation);
	//	q_rotation.normalise();

	//    quaternion_normalise(&q_rotation);	// probably not strictly necessary but good to do to keep q_rotation sane
	//	Ogre::Matrix3 rotMatrix;
	//	q_rotation.ToRotationMatrix(rotMatrix);
	//    quaternion_into_gl_matrix(q_rotation, rotMatrix);


		return q_rotation;
	}
	MTX44* MTX44Transpose(MTX44* pOut, const MTX34* p)
	{
		pOut->_00 = p->_00;pOut->_10 = p->_01;pOut->_20 = p->_02;pOut->_30 = p->_03;
		pOut->_01 = p->_10;pOut->_11 = p->_11;pOut->_21 = p->_12;pOut->_31 = p->_13;
		pOut->_02 = p->_20;pOut->_12 = p->_21;pOut->_22 = p->_22;pOut->_32 = p->_23;
		pOut->_03 = 0;pOut->_13 = 0;pOut->_23 = 0;pOut->_33 = 1;

		return pOut;
	}
	MTX44* MTX44Mult(MTX44* pOut, const MTX44* p1, const MTX34* p2)
	{
		for(int n = 0;n < 4;n++)
		{
			pOut->m[0][n] = p1->m[0][n] * p2->m[0][0] + p1->m[1][n] * p2->m[1][0] + p1->m[2][n] * p2->m[2][0];
			pOut->m[1][n] = p1->m[0][n] * p2->m[0][1] + p1->m[1][n] * p2->m[1][1] + p1->m[2][n] * p2->m[2][1];
			pOut->m[2][n] = p1->m[0][n] * p2->m[0][2] + p1->m[1][n] * p2->m[1][2] + p1->m[2][n] * p2->m[2][2];
			pOut->m[3][n] = p1->m[0][n] * p2->m[0][3] + p1->m[1][n] * p2->m[1][3] + p1->m[2][n] * p2->m[2][3] + p1->m[3][n];
		}
		return pOut;
	}
	MTX44* MTX44Mult(MTX44* pOut, const MTX44* p1, const MTX44* p2)
	{
		for(int n = 0;n < 4;n++)
		{
			pOut->m[0][n] = p1->m[0][n] * p2->m[0][0] + p1->m[1][n] * p2->m[1][0] + p1->m[2][n] * p2->m[2][0] + p1->m[3][n] * p2->m[3][0];
			pOut->m[1][n] = p1->m[0][n] * p2->m[0][1] + p1->m[1][n] * p2->m[1][1] + p1->m[2][n] * p2->m[2][1] + p1->m[3][n] * p2->m[3][1];
			pOut->m[2][n] = p1->m[0][n] * p2->m[0][2] + p1->m[1][n] * p2->m[1][2] + p1->m[2][n] * p2->m[2][2] + p1->m[3][n] * p2->m[3][2];
			pOut->m[3][n] = p1->m[0][n] * p2->m[0][3] + p1->m[1][n] * p2->m[1][3] + p1->m[2][n] * p2->m[2][3] + p1->m[3][n] * p2->m[3][3];
		}
		return pOut;
	}
}
