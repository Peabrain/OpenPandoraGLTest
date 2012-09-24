#pragma once
#include <math.h>

//#include "nw4r/math/config.h"
//#include "nw4r/math/triangular.h"

#include <cstring>

namespace mathHelper
{
	typedef float Mtx[3][4];
	typedef float (*MtxPtr)[4];
	typedef float Mtx44[4][4];
	typedef float (*Mtx44Ptr)[4];
	typedef float ROMtx[4][3];
	typedef float (*ROMtxPtr)[3];

	#define NW4R_MATH_DEG_TO_RAD(fDeg) (3.1415926535897932384626433832795f * fDeg / 180.0f)

	typedef struct
	{

		float x, y, z, w;

	} Quaternion, *QuaternionPtr, Qtrn, *QtrnPtr;

	typedef struct
	{

		float x, y, z;
	} Vec, *VecPtr, Point3d, *Point3dPtr;

	struct _VEC2
	{
		float x;
		float y;
	};

	struct _VEC3
	{
		float x;
		float y;
		float z;
		void operator += (const _VEC3 &ref)
		{
			x += ref.x;
			y += ref.y;
			z += ref.z;
		}
	};

	struct _VEC4
	{
		float x;
		float y;
		float z;
		float w;
	};

	struct _QUAT
	{
		float x;
		float y;
		float z;
		float w;
	};

	struct _MTX22
	{
		union
		{
			struct
			{
				float _00, _01;
				float _10, _11;
			};
			float m[2][2];
			float a[4];
		};
	};

	struct _MTX33
	{
		union
		{
			struct
			{
				float _00, _01, _02;
				float _10, _11, _12;
				float _20, _21, _22;
			};
			float m[3][3];
			float a[9];
		};
	};

	struct _MTX34
	{
		union
		{
			struct
			{
				float _00, _01, _02, _03;
				float _10, _11, _12, _13;
				float _20, _21, _22, _23;
			};
			float m[3][4];
			float a[12];
			Mtx mtx;
		};
	};

	struct _MTX44
	{
		union
		{
			struct
			{
				float _00, _01, _02, _03;
				float _10, _11, _12, _13;
				float _20, _21, _22, _23;
				float _30, _31, _32, _33;
			};
			float m[4][4];
			float a[16];
			Mtx44 mtx;
		};
	};



	/* =======================================================================
			Class definitions
	   ======================================================================== */
	struct VEC2;
	struct VEC3;
	struct VEC4;
	struct QUAT;
	struct QUAT16;
	struct MTX22;
	struct MTX34;
	struct MTX44;


	/* ------------------------------------------------------------------------
		VEC2
	   ------------------------------------------------------------------------ */
	struct VEC2 : public _VEC2
	{
	public:
		typedef VEC2 self_type;
		typedef float  value_type;
	public:
		VEC2() {}
		VEC2(const float* p) { x = p[0]; y = p[1]; }
		VEC2(float fx, float fy) { x = fx; y = fy; }
		VEC2(const _VEC2& v) { x = v.x; y = v.y; }

		operator float*() { return &x; }
		operator const float*() const { return &x; }

		self_type& operator+=(const self_type& rhs) { x += rhs.x; y += rhs.y; return *this; }
		self_type& operator-=(const self_type& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
		self_type& operator*=(float f) { x *= f; y *= f; return *this; }
		self_type& operator/=(float f) { float r = 1.f / f; x *= r; y *= r; return *this; }

		self_type operator+() const { return *this; }
		self_type operator-() const { return self_type(-x, -y); }

		self_type operator+(const self_type& rhs) const { return self_type(x + rhs.x, y + rhs.y); }
		self_type operator-(const self_type& rhs) const { return self_type(x - rhs.x, y - rhs.y); }
		self_type operator*(float f) const { return self_type(f * x, f * y); }
		self_type operator/(float f) const { float r = 1.f / f; return self_type(r * x, r * y); }
		bool operator==(const self_type& rhs) const { return x == rhs.x && y == rhs.y; }
		bool operator!=(const self_type& rhs) const { return x != rhs.x || y != rhs.y; }

		float LenSq() const { return x * x + y * y; }

		void Report(bool bNewline = true, const char* name = 0) const;
	};



	/* ------------------------------------------------------------------------
		VEC3
	   ------------------------------------------------------------------------ */
	struct VEC3 : public _VEC3
	{
	public:
		typedef VEC3 self_type;
		typedef float  value_type;
	public:
		VEC3() {}
		VEC3(const float* p) { x = p[0]; y = p[1]; z = p[2]; }
		VEC3(float fx, float fy, float fz) { x = fx; y = fy; z = fz; }
		VEC3(const _VEC3& v) { x = v.x; y = v.y; z = v.z; }
		VEC3(const Vec& v) { x = v.x; y = v.y; z = v.z; }

		operator float*() { return &x; }
		operator const float*() const { return &x; }
	#if 1
		operator Vec*() { return (Vec*)&x; }
		operator const Vec*() const { return (const Vec*)&x; }
	#endif

		self_type& operator+=(const self_type& rhs);
		self_type& operator-=(const self_type& rhs);
		self_type& operator*=(float f);
		self_type& operator/=(float f) { return operator*=(1.f / f); }

		self_type operator+() const { return *this; }
		self_type operator-() const { return self_type(-x, -y, -z); }

		// The optimal implementation of binary operators depends on whether the return value is optimized.
		self_type operator+(const self_type& rhs) const;
		self_type operator-(const self_type& rhs) const;
		self_type operator*(float f) const;
		self_type operator/(float f) const { float r = 1.f / f; return operator*(r); }

		bool operator==(const self_type& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
		bool operator!=(const self_type& rhs) const { return x != rhs.x || y != rhs.y || z != rhs.z; }

		float LenSq() const { return x * x + y * y + z * z; }

		void Report(bool bNewline = true, const char* name = 0) const;
	};



	/* ------------------------------------------------------------------------
		VEC4
	   ------------------------------------------------------------------------ */
	struct VEC4 : public _VEC4
	{
	public:
		typedef VEC4 self_type;
		typedef float  value_type;
	public:
		VEC4() {}
		VEC4(const float* p) { x = p[0]; y = p[1]; z = p[2]; w = p[3]; }
		VEC4(float fx, float fy, float fz, float fw) { x = fx; y = fy; z = fz; w = fw; }
		VEC4(const _VEC4& v) { x = v.x; y = v.y; z = v.z; w = v.w; }

		operator float*() { return &x; }
		operator const float*() const { return &x; }

		self_type& operator+=(const self_type& rhs);
		self_type& operator-=(const self_type& rhs);
		self_type& operator*=(float f);
		self_type& operator/=(float f);

		self_type operator+() const { return *this; }
		self_type operator-() const { return self_type(-x, -y, -z, -w); }

		self_type operator+(const self_type& rhs) const;
		self_type operator-(const self_type& rhs) const;
		self_type operator*(float f) const;
		self_type operator/(float f) const { float r = 1.f / f; return operator*(r); }

		bool operator==(const self_type& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w; }
		bool operator!=(const self_type& rhs) const { return x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w; }

		float LenSq() const { return x * x + y * y + z * z + w * w; }

		void Report(bool bNewline = true, const char* name = 0) const;
	};



	/* ------------------------------------------------------------------------
		QUAT
	   ------------------------------------------------------------------------ */
	struct QUAT : public _QUAT
	{
	public:
		typedef QUAT self_type;
		typedef float  value_type;
	public:
		QUAT() {}
		QUAT(const float* p) { x = p[0]; y = p[1]; z = p[2]; w = p[3]; }
		QUAT(const _QUAT& rhs) { x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w; }
		QUAT(float fx, float fy, float fz, float fw) { x = fx; y = fy; z = fz; w = fw; }

		operator float*() { return &x; }
		operator const float*() const { return &x; }
	#if 0
		// We've refrained from using the cast operators because the function names would be the same as DolphinSDK.
		operator Quaternion*() { return (Quaternion*)&x; }
		operator const Quaternion*() const { return (const Quaternion*)&x; }
	#endif

		self_type& operator+=(const self_type& rhs);
		self_type& operator-=(const self_type& rhs);
		self_type& operator*=(float f);
		self_type& operator/=(float f) { return operator*=(1.f / f); }

		self_type operator+() const { return *this; }
		self_type operator-() const { return self_type(-x, -y, -z, -w); }

		self_type operator+(const self_type& rhs) const;
		self_type operator-(const self_type& rhs) const;
		self_type operator*(float f) const;
		self_type operator/(float f) const { return operator*(1.f / f); }

		bool operator==(const self_type& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w; }
		bool operator!=(const self_type& rhs) const { return x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w; }

		void Report(bool bNewline = true, const char* name = 0) const;
	};



	/* ------------------------------------------------------------------------
		MTX33
		Exists primarily for normal matrices.
		For now, implement only those that are definitely required.
	   ------------------------------------------------------------------------ */
	struct MTX33 : public _MTX33
	{
	public:
		typedef MTX33 self_type;
		typedef float   value_type;
	public:
		MTX33() {}
		MTX33(const float* p);
		MTX33(const MTX34& rhs);
		MTX33(float x00, float x01, float x02,
			  float x10, float x11, float x12,
			  float x20, float x21, float x22)
		{
			_00 = x00; _01 = x01; _02 = x02;
			_10 = x10; _11 = x11; _12 = x12;
			_20 = x20; _21 = x21; _22 = x22;
		}

		// cast operators
		operator float*() { return &_00; }
		operator const float*() const { return &_00; }

		bool operator==(const self_type& rhs) const { return std::memcmp(this, &rhs, sizeof(MTX33)) == 0; }
		bool operator!=(const self_type& rhs) const { return std::memcmp(this, &rhs, sizeof(MTX33)) != 0; }

		void Report(bool bNewline = true, const char* name = 0) const;
	};



	/* ------------------------------------------------------------------------
		MTX34
	   ------------------------------------------------------------------------ */
	struct MTX34 : public _MTX34
	{
	public:
		typedef MTX34 self_type;
		typedef float   value_type;
		typedef const float (*ConstMtxPtr)[4];
	public:
		MTX34() {}
		MTX34(const float* p);
		MTX34(const Mtx& rhs);
		MTX34(const MTX33& rhs);
		MTX34(float x00, float x01, float x02, float x03,
			  float x10, float x11, float x12, float x13,
			  float x20, float x21, float x22, float x23)
		{
			_00 = x00; _01 = x01; _02 = x02; _03 = x03;
			_10 = x10; _11 = x11; _12 = x12; _13 = x13;
			_20 = x20; _21 = x21; _22 = x22; _23 = x23;
		}

		// cast operators
		operator float*() { return &_00; }
		operator const float*() const { return &_00; }
	#if 1
		operator MtxPtr() { return (MtxPtr)&_00; }
		operator ConstMtxPtr() const { return (ConstMtxPtr)&_00; }
	#endif

		self_type& operator+=(const self_type& rhs);
		self_type& operator-=(const self_type& rhs);
	    
		self_type& operator*=(float f);
		self_type& operator/=(float f) { return operator*=(1.f / f); }

		self_type operator+() const { return *this; }
		self_type operator-() const
		{
			return MTX34(-_00, -_01, -_02, -_03,
						 -_10, -_11, -_12, -_13,
						 -_20, -_21, -_22, -_23);
		}

		//
		self_type operator+(const self_type& rhs) const;
		self_type operator-(const self_type& rhs) const;

		self_type operator*(float f) const;
		self_type operator/(float f) const { return *this * (1.f / f); }

		bool operator==(const self_type& rhs) const { return std::memcmp(this, &rhs, sizeof(MTX34)) == 0; }
		bool operator!=(const self_type& rhs) const { return std::memcmp(this, &rhs, sizeof(MTX34)) != 0; }

		void Report(bool bNewline = true, const char* name = 0) const;
	};



	/* ------------------------------------------------------------------------
		MTX44
	   ------------------------------------------------------------------------ */
	struct MTX44 : public _MTX44
	{
	public:
		typedef MTX44 self_type;
		typedef float   value_type;
		typedef const float (*ConstMtx44Ptr)[4];
	public:
		MTX44() {}
		MTX44(const float* p);
		MTX44(const MTX44& rhs);
		MTX44(const MTX34& rhs);
		MTX44(const Mtx44& rhs);
		MTX44(float x00, float x01, float x02, float x03,
			  float x10, float x11, float x12, float x13,
			  float x20, float x21, float x22, float x23,
			  float x30, float x31, float x32, float x33)
		{
			_00 = x00; _01 = x01; _02 = x02; _03 = x03;
			_10 = x10; _11 = x11; _12 = x12; _13 = x13;
			_20 = x20; _21 = x21; _22 = x22; _23 = x23;
			_30 = x30; _31 = x31; _32 = x32; _33 = x33;
		}

		//
		operator float*() { return &_00; }
		operator const float*() const { return &_00; }
	#if 1
		operator Mtx44Ptr() { return (Mtx44Ptr)&_00; }
		operator ConstMtx44Ptr() const { return (ConstMtx44Ptr)&_00; }
	#endif

	#if 0
		// Shouldn't be a problem to not implement this for now.
		self_type& operator+=(const self_type& rhs);
		self_type& operator-=(const self_type& rhs);
		self_type& operator*=(const self_type& rhs);
	    
		self_type& operator*=(float f);
		self_type& operator/=(float f);

		self_type operator+() const { return *this; }
		self_type operator-() const;

		//
		self_type operator+(const self_type& rhs) const;
		self_type operator-(const self_type& rhs) const;
		self_type operator*(const self_type& rhs) const;

		self_type operator*(float f) const;
		self_type operator/(float f) const;
	#endif

		bool operator==(const self_type& rhs) const { return std::memcmp(this, &rhs, sizeof(MTX44)) == 0; }
		bool operator!=(const self_type& rhs) const { return std::memcmp(this, &rhs, sizeof(MTX44)) != 0; }

		void Report(bool bNewline = true, const char* name = 0) const;
	};



	/* ------------------------------------------------------------------------
		VEC2 functions.
	   ------------------------------------------------------------------------ */
	// VEC2* VEC2Add(VEC2* pOut, const VEC2* p1, const VEC2* p2)
	//
	// Description: Calculates the sum of two vectors.
	// Arguments: pOut: Pointer to the buffer that receives the calculation result. The same vector may be specified for both p1 and p2.
	//          p1: Pointer to the left side value.
	//          p2: Pointer to the right side value.
	// Return value: Returns pOut.
	inline VEC2*
	VEC2Add(VEC2* pOut, const VEC2* p1, const VEC2* p2) { pOut->x = p1->x + p2->x; pOut->y = p1->y + p2->y; return pOut; }

	// VEC2* VEC2Sub(VEC2* pOut, const VEC2* p1, const VEC2* p2)
	//
	// Description:  Calculates the difference of two vectors.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result. The same vector may be specified for both p1 and p2.
	//          p1:  Pointer to the left side value.
	//          p2:  Pointer to the right side value.
	// Returns:  Returns pOut.
	inline VEC2*
	VEC2Sub(VEC2* pOut, const VEC2* p1, const VEC2* p2) { pOut->x = p1->x - p2->x; pOut->y = p1->y - p2->y; return pOut; }

	// VEC2* VEC2Scale(VEC2* pOut, const VEC2* p, float scale)
	//
	// Description:  Calculates the scalar multiple of a vector.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result. May point to the same vector as p.
	//          p:  Pointer to the left side value.
	//          scale:  Multiplication factor.
	// Returns:  Returns pOut.
	inline VEC2*
	VEC2Scale(VEC2* pOut, const VEC2* p, float scale) { pOut->x = p->x * scale; pOut->y = p->y * scale; return pOut; }

	// VEC2* VEC2Lerp(VEC2* pOut, const VEC2* p1, const VEC2* p2, float t)
	//
	// Description: Calculates the linear interpolation between vectors.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result. The same vector may be specified for both p1 and p2.
	//          p1:  Pointer to the vector taken as the start point of linear interpolation.
	//          p2:  Pointer to the vector taken as the end point of linear interpolation.
	//          t:  Linear interpolation parameter. If 0.0, p1 is the result; if 1.0, p2 is the result.
	// Returns:  Returns pOut.
	inline VEC2*
	VEC2Lerp(VEC2* pOut, const VEC2* p1, const VEC2* p2, float t)
	{
		// (1-t)*p1 + t*p2
		pOut->x = p1->x + t * (p2->x - p1->x);
		pOut->y = p1->y + t * (p2->y - p1->y);
		return pOut;
	}

	// float VEC2Dot(const VEC2* p1, const VEC2* p2) 
	//
	// Description:  Calculates the dot product of two vectors.
	// Arguments:  p1:  Pointer to the left side value.
	//          p2:  Pointer to the right side value.
	// Returns:  Returns the dot product of p1 and p2.
	inline float
	VEC2Dot(const VEC2* p1, const VEC2* p2) { return p1->x * p2->x + p1->y * p2->y; }

	// float VEC2LenSq(const VEC2* p) 
	//
	// Description:  Calculates the square of the vector length.
	// Arguments:  p:  Pointer to the target vector.
	// Returns:  Returns the square of the length of p.
	inline float
	VEC2LenSq(const VEC2* p) { return p->x * p->x + p->y * p->y; }

	//  float VEC2Len(const VEC2* p)
	//
	// Description:  Calculates the length of a vector.
	// Arguments:  p:  Pointer to the target vector.
	// Returns:  Returns the length of p.
	inline float
	VEC2Len(const VEC2* p) { return sqrt(p->x * p->x + p->y * p->y); }

	// float VEC2DistSq(const VEC2* p1, const VEC2* p2)
	//
	// Description:  Calculates the square of the distance between two vectors.
	// Arguments:  p1:  Pointer to the left side value.
	//          p2:  Pointer to the right side value.
	// Returns:  Returns the square of the distance between p1 and p2.
	inline float
	VEC2DistSq(const VEC2* p1, const VEC2* p2) { VEC2 tmp; return VEC2LenSq(VEC2Sub(&tmp, p1, p2)); }

	VEC2* VEC2Maximize(VEC2* pOut, const VEC2* p1, const VEC2* p2);
	VEC2* VEC2Minimize(VEC2* pOut, const VEC2* p1, const VEC2* p2);
	VEC2* VEC2Normalize(VEC2* pOut, const VEC2* p);

	inline VEC2
	operator*(float f, const VEC2& rhs) { return VEC2(f * rhs.x, f * rhs.y); }


	/* ------------------------------------------------------------------------
		VEC3 functions.
	   ------------------------------------------------------------------------ */
	// VEC3* VEC3Add(VEC3* pOut, const VEC3* p1, const VEC3* p2)
	//
	// Description:  Calculates the sum of two vectors.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result. The same vector may be specified for both p1 and p2.
	//          p1:  Pointer to the left side value.
	//          p2:  Pointer to the right side value.
	// Return value:  Returns pOut.
	inline VEC3*
	VEC3Add(register VEC3* pOut, const register VEC3* p1, const register VEC3* p2)
	{
		pOut->x = p1->x + p2->x;
		pOut->y = p1->y + p2->y;
		pOut->z = p1->z + p2->z;
		return pOut;
	}


	// VEC3* VEC3Sub(VEC3* pOut, const VEC3* p1, const VEC3* p2)
	//
	// Description:  Calculates the difference of two vectors.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result. The same vector may be specified for both p1 and p2.
	//          p1:  Pointer to the left side value.
	//          p2:  Pointer to the right side value.
	// Returns:  Returns pOut.
	inline VEC3*
	VEC3Sub(register VEC3* pOut, const register VEC3* p1, const register VEC3* p2)
	{
		pOut->x = p1->x - p2->x;
		pOut->y = p1->y - p2->y;
		pOut->z = p1->z - p2->z;
		return pOut;
	}

	// VEC3* VEC3Scale(VEC3* pOut, const VEC3* p, float scale)
	//
	// Description:  Calculates the scalar multiple of a vector.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result. May point to the same vector as p.
	//          p:  Pointer to the left side value.
	//          scale:  Multiplication factor.
	// Return value:  Returns pOut.
	inline VEC3*
	VEC3Scale(register VEC3* pOut, const register VEC3* p, register float scale)
	{
		pOut->x = scale * p->x;
		pOut->y = scale * p->y;
		pOut->z = scale * p->z;
		return pOut;
	}

	// VEC3* VEC3Lerp(VEC3* pOut, const VEC3* p1, const VEC3* p2, float t)
	//
	// Description: Calculates the linear interpolation between two vectors.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result. The same vector may be specified for both p1 and p2.
	//          p1:  Pointer to the vector taken as the start point of linear interpolation.
	//          p2:  Pointer to the vector taken as the end point of linear interpolation.
	//          t:  Linear interpolation parameter. If 0.0, p1 is the result; if 1.0, p2 is the result.
	// Return value:  Returns pOut.
	inline VEC3*
	VEC3Lerp(register VEC3* pOut, const register VEC3* p1, const register VEC3* p2, register float t)
	{
		pOut->x = p1->x + t * (p2->x - p1->x);
		pOut->y = p1->y + t * (p2->y - p1->y);
		pOut->z = p1->z + t * (p2->z - p1->z);
		return pOut;
	}

	// float VEC3Dot(const VEC3* p1, const VEC3* p2)
	//
	// Description:  Calculates the dot product of two vectors.
	// Arguments:  p1:  Pointer to the left side value.
	//          p2:  Pointer to the right side value.
	// Returns:  Returns the dot product of p1 and p2.
	inline float
	VEC3Dot(const register VEC3* p1, const register VEC3* p2)
	{
		return p1->x * p2->x + p1->y * p2->y + p1->z * p2->z;
	}


	// float VEC3LenSq(const VEC3* p)
	//
	// Description:  Calculates the square of the vector length.
	// Arguments:  p:  Pointer to the target vector.
	// Returns:  Returns the square of the length of p.
	inline float
	VEC3LenSq(const register VEC3* p)
	{
		return p->x * p->x + p->y * p->y + p->z * p->z;
	}
	void VECCrossProduct ( const Vec *a, const Vec *b, Vec *axb );
	float VECSquareMag ( const Vec *v );
	float VECMag ( const Vec *v );
	void VECNormalize ( const Vec *src, Vec *unit );
	float VECSquareDistance( const Vec *a, const Vec *b );

	/*#if defined(NW4R_MATH_BROADWAY)
	#define NW4R_VECCROSS      PSVECCrossProduct
	#define NW4R_VECMAG        PSVECMag
	#define NW4R_VECNORMALIZE  PSVECNormalize
	#define NW4R_VECSQDIST     PSVECSquareDistance
	#else
	#define NW4R_VECCROSS      C_VECCrossProduct
	#define NW4R_VECMAG        C_VECMag
	#define NW4R_VECNORMALIZE  C_VECNormalize
	#define NW4R_VECSQDIST     C_VECSquareDistance
	#endif
	*/
	// VEC3* VEC3Cross(VEC3* pOut, const VEC3* p1, const VEC3* p2)
	//
	// Description: Calculates the cross product of vectors.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result. The same vector may be specified for both p1 and p2.
	//          p1:  Pointer to the left side value.
	//          p2:  Pointer to the right side value.
	// Returns:  Returns pOut.
	inline VEC3*
	VEC3Cross(VEC3* pOut, const VEC3* p1, const VEC3* p2) { VECCrossProduct(*p1, *p2, *pOut); return pOut; }

	// float VEC3Len(const VEC3* p)
	//
	// Description:  Calculates the length of a vector.
	// Arguments:  p:  Pointer to the target vector.
	// Returns:  Returns the length of p.
	inline float
	VEC3Len(const VEC3* p) { return VECMag(*p); }

	// VEC3* VEC3Normalize(VEC3* pOut, const VEC3* p)
	//
	// Description: Normalizes a vector.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result. May point to the same vector as p.
	//          p: Pointer to the target vector.
	// Returns:  Returns pOut.
	inline VEC3*
	VEC3Normalize(VEC3* pOut, const VEC3* p) { VECNormalize(*p, *pOut); return pOut; }

	// float VEC3DistSq(const VEC3* p1, const VEC3* p2)
	//
	// Description:  Calculates the square of the distance between two vectors.
	// Arguments:  p1:  Pointer to the left side value.
	//          p2:  Pointer to the right side value.
	// Returns:  Returns the square of the distance between p1 and p2.
	inline float
	VEC3DistSq(const VEC3* p1, const VEC3* p2) { return VECSquareDistance(*p1, *p2); }

	/*#undef NW4R_VECMAG
	#undef NW4R_VECCROSS
	#undef NW4R_VECNORMALIZE
	#undef NW4R_VECSQDIST
	#undef NW4R_VECSQMAG
	*/
	VEC3* VEC3Maximize(VEC3* pOut, const VEC3* p1, const VEC3* p2);
	VEC3* VEC3Minimize(VEC3* pOut, const VEC3* p1, const VEC3* p2);

	//



	inline VEC3&
	VEC3::operator+=(const VEC3& rhs) { (void)VEC3Add(this, this, &rhs); return *this; }
	inline VEC3&
	VEC3::operator-=(const VEC3& rhs) { (void)VEC3Sub(this, this, &rhs); return *this; }
	inline VEC3&
	VEC3::operator*=(float f) { (void)VEC3Scale(this, this, f); return *this; }
	inline VEC3
	VEC3::operator+(const VEC3& rhs) const { VEC3 tmp; (void)VEC3Add(&tmp, this, &rhs); return tmp; }
	inline VEC3
	VEC3::operator-(const VEC3& rhs) const { VEC3 tmp; (void)VEC3Sub(&tmp, this, &rhs); return tmp; }
	inline VEC3
	VEC3::operator*(float f) const { VEC3 tmp; (void)VEC3Scale(&tmp, this, f); return tmp; }
	inline VEC3
	operator*(float f, const VEC3& rhs) { VEC3 tmp; (void)VEC3Scale(&tmp, &rhs, f); return tmp; }



	/* ------------------------------------------------------------------------
		VEC4 functions.
	   ------------------------------------------------------------------------ */
	VEC4* VEC4Add(VEC4* pOut, const VEC4* p1, const VEC4* p2);
	VEC4* VEC4Sub(VEC4* pOut, const VEC4* p1, const VEC4* p2);
	VEC4* VEC4Scale(VEC4* pOut, const VEC4* p, float scale);
	VEC4* VEC4Lerp(VEC4* pOut, const VEC4* p1, const VEC4* p2, float t);
	float   VEC4Dot(const VEC4* p1, const VEC4* p2);
	float   VEC4LenSq(const VEC4* p);
	float   VEC4Len(const VEC4* p);
	VEC4* VEC4Normalize(VEC4* pOut, const VEC4* p);
	float   VEC4DistSq(const VEC4* p1, const VEC4* p2);
	VEC4* VEC4Maximize(VEC4* pOut, const VEC4* p1, const VEC4* p2);
	VEC4* VEC4Minimize(VEC4* pOut, const VEC4* p1, const VEC4* p2);

	inline VEC4&
	VEC4::operator+=(const VEC4& rhs) { (void)VEC4Add(this, this, &rhs); return *this; }
	inline VEC4&
	VEC4::operator-=(const VEC4& rhs) { (void)VEC4Sub(this, this, &rhs); return *this; }
	inline VEC4&
	VEC4::operator*=(float f) { (void)VEC4Scale(this, this, f); return *this; }
	inline VEC4
	VEC4::operator+(const VEC4& rhs) const { VEC4 tmp; (void)VEC4Add(&tmp, this, &rhs); return tmp; }
	inline VEC4
	VEC4::operator-(const VEC4& rhs) const { VEC4 tmp; (void)VEC4Sub(&tmp, this, &rhs); return tmp; }
	inline VEC4
	VEC4::operator*(float f) const { VEC4 tmp; (void)VEC4Scale(&tmp, this, f); return tmp; }
	inline VEC4
	operator*(float f, const VEC4& rhs) { VEC4 tmp; (void)VEC4Scale(&tmp, &rhs, f); return tmp; }



	/* ------------------------------------------------------------------------
		QUAT functions.
	   ------------------------------------------------------------------------ */
	/*#if defined(NW4R_MATH_BROADWAY)

	#define NW4R_QUATADD       PSQUATAdd
	#define NW4R_QUATDIVIDE    PSQUATDivide
	#define NW4R_QUATDOT       PSQUATDotProduct
	#define NW4R_QUATEXP       C_QUATExp
	#define NW4R_QUATINVERSE   PSQUATInverse
	#define NW4R_QUATLERP      C_QUATLerp
	#define NW4R_QUATLOGN      C_QUATLogN
	#define NW4R_QUATMTX       C_QUATMtx
	#define NW4R_QUATMULT      PSQUATMultiply
	#define NW4R_QUATNORMALIZE PSQUATNormalize
	// QUATRotAxisRad
	#define NW4R_QUATSCALE     PSQUATScale
	#define NW4R_QUATSLERP     C_QUATSlerp
	#define NW4R_QUATSQUAD     C_QUATSquad
	#define NW4R_QUATSUB       PSQUATSubtract

	#else

	#define NW4R_QUATADD       C_QUATAdd
	#define NW4R_QUATDIVIDE    C_QUATDivide
	#define NW4R_QUATDOT       C_QUATDotProduct
	#define NW4R_QUATEXP       C_QUATExp
	#define NW4R_QUATINVERSE   C_QUATInverse
	#define NW4R_QUATLERP      C_QUATLerp
	#define NW4R_QUATLOGN      C_QUATLogN
	#define NW4R_QUATMTX       C_QUATMtx
	#define NW4R_QUATMULT      C_QUATMultiply
	#define NW4R_QUATNORMALIZE C_QUATNormalize
	// QUATRotAxisRad
	#define NW4R_QUATSCALE     C_QUATScale
	#define NW4R_QUATSLERP     C_QUATSlerp
	#define NW4R_QUATSQUAD     C_QUATSquad
	#define NW4R_QUATSUB       C_QUATSubtract

	#endif
	*/

	#define QUAT_EPSILON        0.00001f

	void C_QUATScale( const Quaternion *q, Quaternion *r, float scale );
	void C_QUATNormalize( const Quaternion *src, Quaternion *unit );
	void C_QUATMtx( Quaternion *r, const Mtx m );
	void C_QUATLogN( const Quaternion *q, Quaternion *r );
	void C_QUATLerp( const Quaternion *p, const Quaternion *q, Quaternion *r, float t );
	void C_QUATExp( const Quaternion *q, Quaternion *r ) ;
	float C_QUATDotProduct( const Quaternion *p, const Quaternion *q ) ;
	void C_QUATMultiply( const Quaternion *p, const Quaternion *q, Quaternion *pq );
	void C_QUATInverse( const Quaternion *src, Quaternion *inv );
	void C_QUATAdd( const Quaternion *p, const Quaternion *q, Quaternion *r );
	void C_QUATDivide( const Quaternion *p, const Quaternion *q, Quaternion *r );
	void C_QUATSlerp( const Quaternion *p, const Quaternion *q, Quaternion *r, float t );
	void C_QUATSquad( const Quaternion *p, const Quaternion *a, const Quaternion *b, const Quaternion *q, Quaternion *r, float t );
	void C_QUATSubtract( const Quaternion *p, const Quaternion *q, Quaternion *r );
	MTX34* C_MTX34RotXYZ(MTX34* pOut, float fRadX, float fRadY, float fRadZ);
	MTX34* C_MTX34RotAxisRad(MTX34* pOut, const VEC3* pAxis, float fRad);
	MTX34* C_MTX34RotX(MTX34* pOut, float fRadX);
	MTX34* C_MTX34RotY(MTX34* pOut, float fRadY);
	MTX34* C_MTX34RotZ(MTX34* pOut, float fRadZ);
	void C_MTXLookAt( Mtx m, const Point3d *camPos, const Vec *camUp, const Point3d *target );
	void C_MTXCopy ( const Mtx src, Mtx dst );
	void C_MTXConcat ( const Mtx a, const Mtx b, Mtx ab );
	void C_MTXConcatArray ( const Mtx a, const Mtx* srcBase, Mtx* dstBase, int count );
	void C_MTXTranspose ( const Mtx src, Mtx xPose );
	void C_MTXIdentity ( Mtx m );
	void C_MTXScaleApply ( const Mtx src, Mtx dst, float xS, float  yS, float zS );
	void C_MTXTransApply ( const Mtx src, Mtx dst, float xT, float yT, float zT );
	void C_MTXQuat ( Mtx m, const Quaternion *q );
	int C_MTXInverse ( const Mtx src, Mtx inv );
	int C_MTXInvXpose ( const Mtx src, Mtx invX );
	void C_MTXMultVec ( const Mtx m, const Vec *src, Vec *dst );
	void C_MTXMultVecArray ( const Mtx m, const Vec *srcBase, Vec *dstBase, int count );
	void C_MTXMultVecArray1 ( const Mtx m, const Vec *srcBase, Vec *dstBase, int count );
	void C_MTXMultNorm ( const Mtx m, const Vec *src, Vec *dst );

	// QUAT* QUATAdd(QUAT* pOut, const QUAT* q1, const QUAT* q2)
	//
	// Description: Calculates the sum of two quaternions.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result. The same quaternion may be specified for both q1 and q2.
	//          q1:  Pointer to the left side value.
	//          q2:  Pointer to the right side value.
	// Returns:  Returns pOut.
	inline QUAT*
	QUATAdd(QUAT* pOut, const QUAT* q1, const QUAT* q2)
	{ C_QUATAdd((const Quaternion*)q1, (const Quaternion*)q2, (Quaternion*)pOut); return pOut; }

	// QUAT* QUATDivide(QUAT* pOut, const QUAT* q1, const QUAT* q2)
	//
	// Description: Calculates the quotient of quaternions.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result. The same quaternion may be specified for both q1 and q2.
	//          q1:  Pointer to the left side value.
	//          q2:  Pointer to the right side value.
	// Returns:  Returns pOut.
	inline QUAT*
	QUATDivide(QUAT* pOut, const QUAT* q1, const QUAT* q2)
	{ C_QUATDivide((const Quaternion*)q1, (const Quaternion*)q2, (Quaternion*)pOut); return pOut; }

	// float QUATDot(const QUAT* q1, const QUAT* q2)
	//
	// Description: Calculates the inner product of quaternions.
	// Arguments:  q1:  Pointer to the left side value.
	//          q2:  Pointer to the right side value.
	// Return values: Returns the inner product of q1 and q2.
	inline float
	QUATDot(const QUAT* q1, const QUAT* q2)
	{ return C_QUATDotProduct((const Quaternion*)q1, (const Quaternion*)q2); }

	// QUAT* QUATExp(QUAT* pOut, const QUAT* q)
	//
	// Description: Raises e, the base of natural logs, to the power of a quaternion.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result. May point to the same quaternion as q.
	//          q: Pointer to quaternion that is the exponent value. It must be a pure quaternion (w=0).
	// Returns:  Returns pOut.
	inline QUAT*
	QUATExp(QUAT* pOut, const QUAT* q)
	{ C_QUATExp((const Quaternion*)q, (Quaternion*)pOut); return pOut; }

	// QUAT* QUATInverse(QUAT* pOut, const QUAT* q)
	//
	// Description: Calculates the inverse of a quaternion.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result. May point to the same quaternion as q.
	//          q:  Pointer to the left side value.
	// Returns:  Returns pOut.
	inline QUAT*
	QUATInverse(QUAT* pOut, const QUAT* q)
	{ C_QUATInverse((const Quaternion*)q, (Quaternion*)pOut); return pOut; }

	// QUAT* QUATLerp(QUAT* pOut, const QUAT* q1, const QUAT* q2, float t)
	//
	// Description: Calculates the linear interpolation between two quaternions.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result. The same quaternion may be specified for both q1 and q2.
	//          q1: Pointer to the quaternion taken as the start point of linear interpolation.
	//          q2: Pointer to the quaternion taken as the end point of linear interpolation.
	//          t:  Linear interpolation parameter. If 0.0, q1 is the result; if 1.0, q2 is the result.
	// Return value:  Returns pOut.
	inline QUAT*
	QUATLerp(QUAT* pOut, const QUAT* q1, const QUAT* q2, float t)
	{ C_QUATLerp((const Quaternion*)q1, (const Quaternion*)q2, (Quaternion*)pOut, t); return pOut; }

	// QUAT* QUATLogN(QUAT* pOut, const QUAT* q)
	//
	// Description: Calculates the natural logarithm of a quaternion.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result. May point to the same quaternion as q.
	//          q: Pointer to quaternion that is the antilogarithm.
	// Returns:  Returns pOut.
	inline QUAT*
	QUATLogN(QUAT* pOut, const QUAT* q)
	{ C_QUATLogN((const Quaternion*)q, (Quaternion*)pOut); return pOut; }

	// QUAT* MTX34ToQUAT(QUAT* pOut, const MTX34* pM)
	//
	// Description: Creates a quaternion based on a rotation matrix.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result.
	//          pM: Pointer to rotation matrix that is the basis of the quaternion.
	// Returns:  Returns pOut.
	inline QUAT*
	MTX34ToQUAT(QUAT* pOut, const MTX34* pM)
	{ C_QUATMtx((Quaternion*)pOut, *pM); return pOut; }

	// QUAT* QUATMult(QUAT* pOut, const QUAT* q1, const QUAT* q2)
	//
	// Description: Calculates the product of quaternions.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result. The same quaternion may be specified for both q1 and q2.
	//          q1:  Pointer to the left side value.
	//          q2:  Pointer to the right side value.
	// Returns:  Returns pOut.
	inline QUAT*
	QUATMult(QUAT* pOut, const QUAT* q1, const QUAT* q2)
	{ C_QUATMultiply((const Quaternion*)q1, (const Quaternion*)q2, (Quaternion*)pOut); return pOut; }

	// QUAT* QUATNormalize(QUAT* pOut, const QUAT* q)
	//
	// Description: Normalizes a quaternion.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result. May point to the same quaternion as q.
	//          q: Pointer to the quaternion to be normalized.
	// Returns:  Returns pOut.
	inline QUAT*
	QUATNormalize(QUAT* pOut, const QUAT* q)
	{ C_QUATNormalize((const Quaternion*)q, (Quaternion*)pOut); return pOut; }

	// QUAT* QUATScale(QUAT* pOut, const QUAT* q, float scale)
	//
	// Description: Calculates a real number multiple of a quaternion.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result. May point to the same quaternion as q.
	//          q:  Pointer to the left side value.
	//          scale: Multiplication factor
	// Returns:  Returns pOut.
	inline QUAT*
	QUATScale(QUAT* pOut, const QUAT* q, float scale)
	{ C_QUATScale((const Quaternion*)q, (Quaternion*)pOut, scale); return pOut; }

	// QUAT* QUATSlerp(QUAT* pOut, const QUAT* q1, const QUAT* q2, float t)
	//
	// Description: Calculates the spherical surface linear interpolation between two quaternions.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result. The same quaternion may be specified for both q1 and q2.
	//          q1: Pointer to quaternion taken as the start point of spherical surface linear interpolation
	//          q2: Pointer to the quaternion taken as the end point of spherical surface linear interpolation
	//          t:  Spherical linear interpolation parameter. If 0.0, q1 is the result; if 1.0, q2 is the result.
	// Returns:  Returns pOut.
	inline QUAT*
	QUATSlerp(QUAT* pOut, const QUAT* q1, const QUAT* q2, float t)
	{ C_QUATSlerp((const Quaternion*)q1, (const Quaternion*)q2, (Quaternion*)pOut, t); return pOut; }

	// QUAT* QUATSquad(QUAT* pOut, const QUAT* p, const QUAT* a, const QUAT* b, const QUAT* q, float t)
	//
	// Description: Calculates the spherical surface cubic interpolation between two quaternions.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result. The same quaternion as p, a, b, or q may be specified.
	//          p: Pointer to the quaternion taken as the start point of spherical surface cubic interpolation
	//          a: Pointer to the quaternion used to control the spherical 3D interpolation
	//          b:  Pointer to the quaternion used to control the spherical cubic interpolation.
	//          q: Pointer to the quaternion taken as the end point of spherical cubic interpolation.
	//          t:  Spherical linear interpolation parameter. If 0.0, q1 is the result; if 1.0, q2 is the result.
	// Return value:  Returns pOut.
	inline QUAT*
	QUATSquad(QUAT* pOut, const QUAT* p, const QUAT* a, const QUAT* b, const QUAT* q, float t)
	{ C_QUATSquad((const Quaternion*)p, (const Quaternion*)a, (const Quaternion*)b, (const Quaternion*)q, (Quaternion*)pOut, t); return pOut; }

	Quaternion EulerToQuat(float roll, float pitch, float yaw);
	// QUAT* QUATSub(QUAT* pOut, const QUAT* q1, const QUAT* q2)
	//
	// Description: Calculates the difference of quaternions.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result. The same quaternion may be specified for both q1 and q2.
	//          q1:  Pointer to the left side value.
	//          q2:  Pointer to the right side value.
	// Returns:  Returns pOut.
	inline QUAT*
	QUATSub(QUAT* pOut, const QUAT* q1, const QUAT* q2)
	{ C_QUATSubtract((const Quaternion*)q1, (const Quaternion*)q2, (Quaternion*)pOut); return pOut; }


	/*#undef NW4R_QUATADD
	#undef NW4R_QUATDIVIDE
	#undef NW4R_QUATDOT
	#undef NW4R_QUATEXP
	#undef NW4R_QUATINVERSE
	#undef NW4R_QUATLERP
	#undef NW4R_QUATLOGN
	#undef NW4R_QUATMTX
	#undef NW4R_QUATMULT
	#undef NW4R_QUATNORMALIZE
	// QUATRotAxisRad
	#undef NW4R_QUATSCALE
	#undef NW4R_QUATSLERP
	#undef NW4R_QUATSQUAD
	#undef NW4R_QUATSUB
	*/
	inline QUAT&
	QUAT::operator+=(const QUAT& rhs) { (void)QUATAdd(this, this, &rhs); return *this; }
	inline QUAT&
	QUAT::operator-=(const QUAT& rhs) { (void)QUATSub(this, this, &rhs); return *this; }
	inline QUAT&
	QUAT::operator*=(float f) { (void)QUATScale(this, this, f); return *this; }
	inline QUAT
	QUAT::operator+(const QUAT& rhs) const { QUAT tmp; (void)QUATAdd(&tmp, this, &rhs); return tmp; }
	inline QUAT
	QUAT::operator-(const QUAT& rhs) const { QUAT tmp; (void)QUATSub(&tmp, this, &rhs); return tmp; }
	inline QUAT
	QUAT::operator*(float f) const { QUAT tmp; (void)QUATScale(&tmp, this, f); return tmp; }
	inline QUAT
	operator*(float f, const QUAT& rhs) { QUAT tmp; (void)QUATScale(&tmp, &rhs, f); return tmp; }



	/* ------------------------------------------------------------------------
		MTX33 functions
	   ------------------------------------------------------------------------ */
	MTX33* MTX33Copy(MTX33* pOut, const MTX33* p);
	MTX33* MTX33Zero(MTX33* pOut);
	MTX33* MTX33Identity(MTX33* pOut);
	MTX33* MTX34ToMTX33(MTX33* pOut, const MTX34* pM);
	MTX33* MTX33MAdd(MTX33* pOut, float t, const MTX33* p1, const MTX33* p2);

	int MTX34InvTranspose(MTX33* pOut, const MTX34* p);


	inline
	MTX33::MTX33(const float* p) { MTX33Copy(this, reinterpret_cast<const MTX33*>(p)); }

	inline
	MTX33::MTX33(const MTX34& rhs) { MTX34ToMTX33(this, &rhs); }



	/* ------------------------------------------------------------------------
		MTX34 functions
	   ------------------------------------------------------------------------ */
	MTX34* MTX34Zero(MTX34* pOut);
	bool MTX34IsIdentity(const MTX34* p);
	MTX34* MTX34Add(MTX34* pOut, const MTX34* p1, const MTX34* p2);
	MTX34* MTX34Sub(MTX34* pOut, const MTX34* p1, const MTX34* p2);
	MTX34* MTX34Mult(MTX34* pOut, const MTX34* p, float f);
	MTX34* MTX34Scale(MTX34* pOut, const MTX34* pM, const VEC3* pS);
	MTX34* MTX34Trans(MTX34* pOut, const MTX34* pM, const VEC3* pT);
	MTX34* MTX34MAdd(MTX34* pOut, float t, const MTX34* p1, const MTX34* p2);
	MTX34* MTX33ToMTX34(MTX34* pOut, const MTX33* pM);


	// For future implementation:
	//MTX34* MTX34Inv(MTX34* pOut, float* pDet, const MTX34* p);
	//MTX34* MTX34InvTranspose(MTX34* pOut, float* pDet, const MTX34* p);
	//float MTX34Det(const MTX34* p);

	//
	//
	//
	inline MTX34 operator*(float f, const MTX34& rhs)
	{
		MTX34 tmp;
		(void)MTX34Mult(&tmp, &rhs, f);
		return tmp;
	}

	// MTX34* MTX34RotXYZRad(MTX34* pOut, float fRadX, float fRadY, float fRadZ)
	//
	// Description:  Creates a rotation matrix.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result.
	//          fRadX: Angle of rotation about the x-axis in radians
	//          fRadY: Angle around the y-axis in radians
	//          fRadZ: Angle of rotation about the z-axis in radians
	// Returns:  Returns pOut.
	inline MTX34*
	MTX34RotXYZRad(MTX34* pOut, float fRadX, float fRadY, float fRadZ)
	{
		return C_MTX34RotXYZ(pOut, fRadX, fRadY, fRadZ);
	}

	// MTX34* MTX34RotXYZDeg(MTX34* pOut, float fDegX, float fDegY, float fDegZ)
	//
	// Description:  Creates a rotation matrix.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result.
	//          fRadX: Angle about the x-axis in units of 1/360th of a circle (degrees).
	//          fRadY: Angle about the y-axis in units of 1/360th of a circle (degrees).
	//          fRadZ: Angle about the z-axis in units of 1/360th of a circle (degrees).
	// Returns:  Returns pOut.
	inline MTX34*
	MTX34RotXYZDeg(MTX34* pOut, float fDegX, float fDegY, float fDegZ)
	{
		float rx = NW4R_MATH_DEG_TO_RAD(fDegX);
		float ry = NW4R_MATH_DEG_TO_RAD(fDegY);
		float rz = NW4R_MATH_DEG_TO_RAD(fDegZ);
		return C_MTX34RotXYZ(pOut, rx,ry,rz);
	}

	// MTX34* MTX34RotAxisRad(MTX34* pOut, const VEC3* pAxis, float fRad)
	//
	// Description:  Creates a rotation matrix for rotating about a specified axis.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result.
	//          pAxis:  Pointer to the vector specified as the axis of rotation.
	//          fRad: Rotation amount in radians.
	// Returns:  Returns pOut.
	inline MTX34*
	MTX34RotAxisRad(MTX34* pOut, const VEC3* pAxis, float fRad)
	{
		return C_MTX34RotAxisRad(pOut, pAxis, fRad);
	}

	inline MTX34*
	MTX34RotY(MTX34* pOut, float fRadY)
	{
		return C_MTX34RotY(pOut,fRadY);
	}
	inline MTX34*
	MTX34RotX(MTX34* pOut, float fRadX)
	{
		return C_MTX34RotX(pOut,fRadX);
	}
	inline MTX34*
	MTX34RotZ(MTX34* pOut, float fRadZ)
	{
		return C_MTX34RotZ(pOut,fRadZ);
	}

	// MTX34* MTX34RotAxisDeg(MTX34* pOut, const VEC3* pAxis, float fDeg)
	//
	// Description:  Creates a rotation matrix for rotating about a specified axis.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result.
	//          pAxis:  Pointer to the vector specified as the axis of rotation.
	//          fDeg: Amount of rotation in units of 1/360th of a circle (degrees).
	// Returns:  Returns pOut.
	inline MTX34*
	MTX34RotAxisDeg(MTX34* pOut, const VEC3* pAxis, float fDeg)
	{
		float fRad = NW4R_MATH_DEG_TO_RAD(fDeg);
		return C_MTX34RotAxisRad(pOut, pAxis, fRad);
	}

	// MTX34* MTX34LookAt(MTX34* pOut, const VEC3* pCamPos, const VEC3* pCamUp, const VEC3* pTarget)
	//
	// Description: Sets the camera matrix.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result.
	//          pCamPos: Pointer to the vector specified as the camera position.
	//          pCamUp: Pointer to vector specifing the camera's up direction.
	//          pTarget: Pointer to vector specifing the camera's viewpoint.
	// Return value:  Returns pOut.
	inline MTX34*
	MTX34LookAt(MTX34* pOut, const VEC3* pCamPos, const VEC3* pCamUp, const VEC3* pTarget)
	{
		C_MTXLookAt(*pOut, *pCamPos, *pCamUp, *pTarget);
		return pOut;
	}


	/*#if defined(NW4R_MATH_BROADWAY)

	#define NW4R_MTXCOPY           PSMTXCopy
	#define NW4R_MTXCONCAT         PSMTXConcat
	#define NW4R_MTXCONCATARRAY    PSMTXConcatArray
	#define NW4R_MTXTRANSPOSE      PSMTXTranspose
	#define NW4R_MTXIDENTITY       PSMTXIdentity
	#define NW4R_MTXSCALEAPPLY     PSMTXScaleApply
	#define NW4R_MTXTRANSAPPLY     PSMTXTransApply
	#define NW4R_MTXQUAT           PSMTXQuat
	#define NW4R_MTXINVERSE        PSMTXInverse
	#define NW4R_MTXINVXPOSE       PSMTXInvXpose
	#define NW4R_MTXMULTVEC        PSMTXMultVec
	#define NW4R_MTXMULTVECARRAY   PSMTXMultVecArray

	#else

	#define NW4R_MTXCOPY           C_MTXCopy
	#define NW4R_MTXCONCAT         C_MTXConcat
	#define NW4R_MTXCONCATARRAY    C_MTXConcatArray
	#define NW4R_MTXTRANSPOSE      C_MTXTranspose
	#define NW4R_MTXIDENTITY       C_MTXIdentity
	#define NW4R_MTXSCALEAPPLY     C_MTXScaleApply
	#define NW4R_MTXTRANSAPPLY     C_MTXTransApply
	#define NW4R_MTXQUAT           C_MTXQuat
	#define NW4R_MTXINVERSE        C_MTXInverse
	#define NW4R_MTXINVXPOSE       C_MTXInvXpose
	#define NW4R_MTXMULTVEC        C_MTXMultVec
	#define NW4R_MTXMULTVECARRAY   C_MTXMultVecArray

	#endif
	*/
	// MTX34* MTX34Copy(MTX34* pOut, const MTX34* p)
	//
	// Description: Copies the specified matrix.
	// Arguments:  pOut: Pointer to the matrix being copied to
	//          p: Pointer to the matrix being copied from
	// Return value:  Returns pOut.
	inline MTX34*
	MTX34Copy(MTX34* pOut, const MTX34* p) { C_MTXCopy(*p, *pOut); return pOut; }

	// MTX34* MTX34Mult(MTX34* pOut, const MTX34* p1, const MTX34* p2)
	//
	// Description: Calculates the product of two matrices.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result. The same matrix may be specified for both p1 and p2.
	//          p1: Pointer to the left side value.
	//          p2: Pointer to the right side value.
	// Return value:  Returns pOut.
	inline MTX34*
	MTX34Mult(MTX34* pOut, const MTX34* p1, const MTX34* p2) { C_MTXConcat(*p1, *p2, *pOut); return pOut; }

	MTX44* MTX44Mult(MTX44* pOut, const MTX44* p1, const MTX34* p2);
	MTX44* MTX44Mult(MTX44* pOut, const MTX44* p1, const MTX44* p2);

	// MTX34* MTX34MultArray(MTX34* pOut, const MTX34* p1, const MTX34* pSrc, u32 count)
	//
	// Description: Multiplies a matrix with an array of matrices from the left.
	// Arguments:  pOut:  Pointer to the start of the array that receives the calculated results.
	//          p1: Pointer to the matrix that is the left-side value.
	//          pSrc: Pointer to the start of the array of matrices used as the right side value.
	//          count : Number of elements in the matrix array that is the right-side value.
	// Return value:  Returns pOut.
	inline MTX34*
	MTX34MultArray(MTX34* pOut, const MTX34* p1, const MTX34* pSrc, int count) { C_MTXConcatArray(*p1, (const Mtx*)pSrc, (Mtx*)pOut, count); return pOut; }

	// MTX34* MTX34Transpose(MTX34* pOut, const MTX34* p)
	//
	// Description: Creates the transpose matrix of a matrix.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result. May point to the same matrix as p.
	//          p: Pointer to the matrix to transpose.
	// Return value:  Returns pOut.
	inline MTX34*
	MTX34Transpose(MTX34* pOut, const MTX34* p) { C_MTXTranspose(*p, *pOut); return pOut; }

	// MTX34* MTX34Identity(MTX34* pOut)
	//
	// Description: Creates an identity matrix.
	// Arguments:  pOut: Pointer to the buffer that stores the identity matrix.
	// Return value:  Returns pOut.
	inline MTX34*
	MTX34Identity(MTX34* pOut) { C_MTXIdentity(*pOut); return pOut; }

	// MTX34* MTX34Scale(MTX34* pOut, const VEC3* pS, const MTX34* pM)
	//
	// Description: Applies a scalar transformation to a matrix. Multiply the scale matrix from the left.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result. May point to the same matrix as pM.
	//          pS: Pointer to the vector storing the scale values for each of the three axis directions.
	//          pM:  Pointer to the source matrix.
	// Return value:  Returns pOut.
	inline MTX34*
	MTX34Scale(MTX34* pOut, const VEC3* pS, const MTX34* pM) { C_MTXScaleApply(*pM, *pOut, pS->x, pS->y, pS->z); return pOut; }

	// MTX34* MTX34Trans(MTX34* pOut, const VEC3* pT, const MTX34* pM)
	//
	// Description: Applies a translation to a matrix. Multiplies the translation matrix from the left.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result. May point to the same matrix as pM.
	//          pT: Pointer to the vector storing the amount of translation in each of the three axis directions.
	//          pM:  Pointer to the source matrix.
	// Return value:  Returns pOut.
	inline MTX34*
	MTX34Trans(MTX34* pOut, const VEC3* pT, const MTX34* pM) { C_MTXTransApply(*pM, *pOut, pT->x, pT->y, pT->z); return pOut; }

	// MTX34* QUATToMTX34(MTX34* pOut, const QUAT* p)
	//
	// Description: Creates a rotation matrix from a quaternion.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result.
	//          p: Pointer to quaternion that is the source
	// Return value:  Returns pOut.
	inline MTX34*
	QUATToMTX34(MTX34* pOut, const QUAT* p) { C_MTXQuat(*pOut, (const Quaternion*)p); return pOut; }

	// u32 MTX34Inv(MTX34* pOut, const MTX34* p)
	//
	// Description: Calculates the inverse matrix of a matrix.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result. May point to the same matrix as p.
	//          p:  Pointer to the source matrix.
	// Returns:  Returns 1 if the inverse matrix exists; otherwise, returns 0.
	inline int
	MTX34Inv(MTX34* pOut, const MTX34* p) { return C_MTXInverse(*p, *pOut); }

	// u32 MTX34InvTranspose(MTX34* pOut, const MTX34* p) 
	//
	// Description: Calculates the inverse transpose matrix of a matrix.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result. May point to the same matrix as p.
	//          p:  Pointer to the source matrix.
	// Returns:  Returns 1 if the inverse matrix exists; otherwise, returns 0.
	inline int
	MTX34InvTranspose(MTX34* pOut, const MTX34* p) { return C_MTXInvXpose(*p, *pOut); }

	// VEC3* VEC3Transform(VEC3* pOut, const MTX34* pM, const VEC3* pV)
	//
	// Description:  Transforms a vector using a matrix. The fourth element of the vector is calculated as 1.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result. May point to the same vector as pV.
	//          pM:  Pointer to the transformation matrix.
	//          pV:  Pointer to the source vector.
	// Return value:  Returns pOut.
	inline VEC3*
	VEC3Transform(VEC3* pOut, const MTX34* pM, const VEC3* pV) { C_MTXMultVec(*pM, *pV, *pOut); return pOut; }

	// VEC3* VEC3TransformCoord(VEC3* pOut, const MTX34* pM, const VEC3* pV)
	//
	// Description:  Transforms a vector using a matrix. The fourth element of the vector is calculated as 1.
	// Arguments:  pOut:  Pointer to the buffer that receives the calculation result. May point to the same vector as pV.
	//          pM:  Pointer to the transformation matrix.
	//          pV:  Pointer to the source vector.
	// Return value:  Returns pOut.
	inline VEC3*
	VEC3TransformCoord(VEC3* pOut, const MTX34* pM, const VEC3* pV) { C_MTXMultVec(*pM, *pV, *pOut); return pOut; }

	// VEC3* VEC3TransformCoordArray(VEC3* pOut, const MTX34* pM, const VEC3* pV, u32 count)
	//
	// Description: Transforms an array of vectors using the specified matrix. The fourth element of each vector is calculated as 1.
	// Arguments:  pOut:  Pointer to the start of the array that receives the calculated results.
	//          pM:  Pointer to the transformation matrix.
	//          pV: Pointer to the start of the source vector array.
	//          count : Number of elements in the vector array to be transformed.
	// Return value:  Returns pOut.
	inline VEC3*
		VEC3TransformCoordArray(VEC3* pOut, const MTX34* pM, const VEC3* pV, int count) { C_MTXMultVecArray(*pM, *pV, *pOut, count); return pOut; }
	inline VEC3*
		VEC3TransformCoordArray1(VEC3* pOut, const MTX34* pM, const VEC3* pV, int count) { C_MTXMultVecArray1(*pM, *pV, *pOut, count); return pOut; }

	/*#undef NW4R_MTX34COPY
	#undef NW4R_MTXCONCAT
	#undef NW4R_MTXCONCATARRAY
	#undef NW4R_MTXTRANSPOSE
	#undef NW4R_MTXIDENTITY
	#undef NW4R_MTXSCALEAPPLY
	#undef NW4R_MTXTRANSAPPLY
	#undef NW4R_MTXQUAT
	#undef NW4R_MTXINVERSE
	#undef NW4R_MTXINVXPOSE
	#undef NW4R_MTXMULTVEC
	#undef NW4R_MTXMULTVECARRAY
	*/
	// Converts as (x, y, z, 0).
	inline VEC3* VEC3TransformNormal(VEC3* pOut, const MTX34* pM, const VEC3* pV) { C_MTXMultNorm(*pM, *pV, *pOut); return pOut; }
	VEC3* VEC3TransformNormalArray(VEC3* pOutArray, const MTX34* pM, const VEC3* pArray, int n);


	//
	// MTX34 member functions.
	//

	inline
	MTX34::MTX34(const float* p) { (void)MTX34Copy(this, (MTX34*)p); }

	inline
	MTX34::MTX34(const Mtx& rhs) { (void)MTX34Copy(this, (MTX34*)&rhs); }

	inline
	MTX34::MTX34(const MTX33& rhs) { MTX33ToMTX34(this, &rhs); }

	inline MTX34&
	MTX34::operator+=(const self_type& rhs) { return *MTX34Add(this, this, &rhs); }

	inline MTX34&
	MTX34::operator-=(const self_type& rhs) { return *MTX34Sub(this, this, &rhs); }

	inline MTX34&
	MTX34::operator*=(float f) { return *MTX34Mult(this, this, f); }

	inline MTX34
	MTX34::operator+(const MTX34& rhs) const { MTX34 tmp; return *MTX34Add(&tmp, this, &rhs); }

	inline MTX34
	MTX34::operator-(const MTX34& rhs) const { MTX34 tmp; return *MTX34Sub(&tmp, this, &rhs); }

	inline MTX34
	MTX34::operator*(float f) const { MTX34 tmp; return *MTX34Mult(&tmp, this, f); }



	/* ------------------------------------------------------------------------
		MTX44 functions
	   ------------------------------------------------------------------------ */
	MTX44* MTX44Zero(MTX44* pOut);
	MTX44* MTX44Identity(MTX44* pOut);
	MTX44* MTX44Copy(MTX44* pOut, const MTX44* p);
	MTX44* MTX44Transpose(MTX44* pOut, const MTX34* p);
	bool MTX44IsIdentity(const MTX44* p);

	// MTX44* MTX44Frustum(MTX44* pOut, float t, float b, float l, float r, float n, float f)
	//
	// Description: Creates a projection matrix based on the view frustum in the near clipping plane.
	// Arguments:  pOut:  Pointer to the matrix storing the projection matrix.
	//          t:  y-coordinate of the upper side of the view frustum on the near clipping plane.
	//          b:  y-coordinate of the bottom side of the view frustum on the near clipping plane.
	//          l:  x-coordinate of the left side of the view frustum on the near clipping plane.
	//          r:  x-coordinate of the right side of the view frustum on the near clipping plane.
	//          n:  Distance to the near clipping plane.
	//          f:  Distance to the far clipping plane.
	// Returns:  Returns pOut.
	/*inline MTX44*
	MTX44Frustum(MTX44* pOut, float t, float b, float l, float r, float n, float f)
	{
		MTXFrustum(*pOut, t, b, l, r, n, f);
		return pOut;
	}
	*/
	// MTX44* MTX44Ortho(MTX44* pOut, float t, float b, float l, float r, float n, float f)
	//
	// Description: Creates an orthogonal projection matrix.
	// Arguments:  pOut:  Pointer to the matrix storing the projection matrix.
	//          t:  y-coordinate of the upper side of the view frustum on the near clipping plane.
	//          b:  y-coordinate of the bottom side of the view frustum on the near clipping plane.
	//          l:  x-coordinate of the left side of the view frustum on the near clipping plane.
	//          r:  x-coordinate of the right side of the view frustum on the near clipping plane.
	//          n:  Distance to the near clipping plane.
	//          f:  Distance to the far clipping plane.
	// Returns:  Returns pOut.
	/*inline MTX44*
	MTX44Ortho(MTX44* pOut, float t, float b, float l, float r, float n, float f)
	{
		MTXOrtho(*pOut, t, b, l, r, n, f);
		return pOut;
	}
	*/
	// MTX44* MTX44Perspective(MTX44* pOut, float fovy, float aspect, float n, float f)
	//
	// Description: Creates a projection matrix from the view angle and aspect ratio.
	// Arguments:  pOut:  Pointer to the matrix storing the projection matrix.
	//          fovy: Viewing angle in the vertical direction.
	//          aspect: View aspect ratio (width/height).
	//          n:  Distance to the near clipping plane.
	//          f:  Distance to the far clipping plane.
	// Return value:  Returns pOut.
	/*inline MTX44*
	MTX44Perspective(MTX44* pOut, float fovy, float aspect, float n, float f)
	{
		MTXPerspective(*pOut, fovy, aspect, n, f);
		return pOut;
	}
	*/


	// Converts as (x, y, z, 1).
	VEC4* VEC3Transform(VEC4* pOut, const MTX44* pM, const VEC3* pV);
	VEC4* VEC3TransformArray(VEC4* pOut, const MTX44* pM, const VEC3* pV, int count);

	VEC3* VEC3TransformCoord(VEC3* pOut, const MTX44* pM, const VEC3* pV);
	VEC3* VEC3TransformCoordArray(VEC3* pOut, const MTX44* pM, const VEC3* pV, int count);

	//VEC3* VEC3TransformNormal(VEC3* pOut, const MTX44* pM, const VEC3* pV);
	//VEC3* VEC3TransformNormalArray(VEC3* pOut, const MTX44* pM, const VEC3* pV, u32 count);

	VEC4* VEC4Transform(VEC4* pOut, const MTX44* pM, const VEC4* pV);
	VEC4* VEC4TransformArray(VEC4* pOut, const MTX44* pM, const VEC4* pV, int n);

	inline
	MTX44::MTX44(const float* p) { (void)MTX44Copy(this, (MTX44*)p); }

	inline
	MTX44::MTX44(const MTX34& rhs)
	{
		(void)MTX34Copy((MTX34*)this, (MTX34*)&rhs);
		_30 = _31 = _32 = 0.f; _33 = 1.f;
	}

	inline
	MTX44::MTX44(const MTX44& rhs) { (void)MTX44Copy(this, &rhs); }

	inline
	MTX44::MTX44(const Mtx44& rhs) { (void)MTX44Copy(this, (MTX44*)&rhs); }

	void	quaternion_rotate_about_x(mathHelper::QUAT *quat, float angle);
	void	quaternion_rotate_about_y(mathHelper::QUAT *quat, float angle);
	void	quaternion_rotate_about_z(mathHelper::QUAT *quat, float angle);
	mathHelper::QUAT applyRoll(float roll1,float climb1);
}
