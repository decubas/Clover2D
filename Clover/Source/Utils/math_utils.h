#ifndef __MATHUTILS_H__
#define __MATHUTILS_H__ 1

#define PI 3.141592f

#include <inttypes.h>
#include <array>
#include <initializer_list>
#include <functional>
#include <numeric>
#include <random>
#include <limits>

namespace MathUtils
{
	static float Clamp(float value, float minVal, float maxVal) {

		if (value <= minVal) return minVal;
		if (value >= maxVal) return maxVal;

		return value;
	}

	static float Radians(float degrees) {

		return degrees * (PI / 180.0f);
	}

	static float Degrees(float radians) {

		return radians * (180.0f / PI);
	}

	static float Abs(float x)
	{
		return x >= 0.0f ? x : -x;
	}

};

namespace dcMath
{
	class Random
	{
		static std::random_device rd;
		static std::mt19937 mt;
	public:
		
		static void init()
		{
			mt = std::mt19937(rd());
		}

		static int Random::irand(int a, int b)
		{
			std::uniform_int_distribution<int> intDist(a, b);
			return intDist(mt);
		}

		static float Random::frand(float a, float b)
		{
			std::uniform_real_distribution<float> floatDist(a, b);
			return floatDist(mt);;
		}
	};

#define IMPLEMENT_OPERATOR(sign)\
		TVec operator##sign(const TVec& other) const\
		{\
			TVec r(static_cast<T>(0));\
			for (size_t i = 0; i < N; i++)\
			{	r[i] = v[i] sign other[i];}\
			return r;\
		}\
		TVec operator##sign(const T val) const\
		{\
			TVec r(static_cast<T>(0)); \
			for (size_t i = 0; i < N; i++)\
			{r[i] = v[i] sign val; }\
			return r; \
		}

#define IMPLEMENT_EQUAL_OPERATOR(sign)\
		TVec operator##sign(const TVec& other)\
		{\
			for (size_t i = 0; i < N; i++)\
			{v[i] sign other[i];}\
			return *this;\
		}\
		TVec operator##sign(const T& val)\
		{\
			for (size_t i = 0; i < N; i++)\
			{v[i] sign val; }\
			return *this; \
		}

#define IMPLEMENT_OPERATORS()																					  \
		T& operator[](size_t i) { return v[i]; }																  \
		T operator[](size_t i) const { return v[i]; }															  \
		bool operator==(const TVec& other) const { return memcmp(v, other.v, N * sizeof(T));}					  \
		bool operator!=(const TVec& other) const { return !this->operator==(other);}							  \
		bool operator>=(const TVec& other) const { return magnitudeSqr(v) >= magnitudeSqr(other.v); }			  \
		bool operator<=(const TVec& other) const { return magnitudeSqr(v) <= magnitudeSqr(other.v); }			  \
		bool operator>(const TVec& other)  const { return magnitudeSqr(v) >  magnitudeSqr(other.v); }			  \
		bool operator<(const TVec& other)  const { return magnitudeSqr(v) <  magnitudeSqr(other.v); }			  \
		IMPLEMENT_OPERATOR(+);																					  \
		IMPLEMENT_OPERATOR(-);																					  \
		IMPLEMENT_OPERATOR(*);																					  \
		IMPLEMENT_OPERATOR(/ );																					  \
		IMPLEMENT_EQUAL_OPERATOR(+= );																			  \
		IMPLEMENT_EQUAL_OPERATOR(-= );																			  \
		IMPLEMENT_EQUAL_OPERATOR(*= );																			  \
		IMPLEMENT_EQUAL_OPERATOR(/= );																			  \
		template<int S>																							  \
		void operator=(const Vec<T, S>& other) { memcpy_s(v, N * sizeof(T), other.v, (S - (S - N)) * sizeof(T)); }
	
#define IMPLEMENT_CONSTRUCTORS()														\
		TVec() = default;																\
		TVec(const std::initializer_list<T>& list)										\
		{																				\
			for (size_t i = 0; i < N; i++)												\
			{	v[i] = list.begin()[i];	}												\
		}																				\
		TVec(const T& value) 															\
		{																				\
			for (size_t i = 0; i < N; i++)												\
			{	v[i] = value;	}														\
		}																				\
		TVec(const TVec& newV) { memcpy_s(v, N * sizeof(T), newV.v, N * sizeof(T)); }	\
																						\
		TVec(const Vec<T, N-1>& newV, T z)												\
		{																				\
			memcpy_s(v, N * sizeof(T), newV.v, (N-1) * sizeof(T));						\
			v[N-1] = z;																	\
		}																				

	template<typename T, size_t N>
	struct Vec
	{
		typedef Vec<T, N> TVec;
		T v[N] = { 0 };

		IMPLEMENT_CONSTRUCTORS();
		IMPLEMENT_OPERATORS();
	};

	template<typename T>
	struct Vec<T, 2>
	{
		typedef Vec<T, 2> TVec;
		static const size_t N = 2;

		union
		{
			T v[2] = { 0 };
			struct { T x, y; };
			struct { T r, g; };
		};

		TVec(const T& _x, const T& _y)
			: x(_x), y(_y)
		{}

		IMPLEMENT_CONSTRUCTORS();
		IMPLEMENT_OPERATORS();
	};

	template<typename T>
	struct Vec<T, 3>
	{
		typedef Vec<T, 3> TVec;
		static const size_t N = 3;

		union
		{
			T v[3] = { 0 };
			struct { T x, y, z; };
			struct { T r, g, b; };
		};

		TVec(const T& _x, const T& _y, const T& _z)
			: x(_x), y(_y), z(_z)
		{}

		IMPLEMENT_CONSTRUCTORS();		
		IMPLEMENT_OPERATORS();

	};

	
	template<typename T>
	struct Vec<T, 4>
	{
		typedef Vec<T, 4> TVec;
		static const size_t N = 4;

		union
		{
			T v[4] = {0};
			struct { T x, y, z, w; };
			struct { T r, g, b, a; };
		};

		TVec(const T& _x, const T& _y, const T& _z, const T& _w)
			: x(_x), y(_y), z(_z), w(_w)
		{}

		IMPLEMENT_CONSTRUCTORS();
		IMPLEMENT_OPERATORS();
	};


#undef IMPLEMENT_CONSTRUCTORS
#undef IMPLEMENT_OPERATORS
#undef IMPLEMENT_OPERATOR
#undef IMPLEMENT_EQUAL_OPERATOR

	template<typename T, int N>
	struct Mat
	{
		typedef Mat<T, N> TMat;
		typedef Vec<T, N> ColType;
		//typedef std::array<ColType, S> MatType;

		ColType m [N]{ 0 };

		TMat() = default;
		TMat(const TMat& other)
		{
			for (size_t i = 0; i < N; i++)
				m[i] = other.m[i];
		}
		TMat(const ColType& other)
		{
			for (size_t i = 0; i < N; i++)
				m[i] = other[i];
		}
		TMat(const T& value) 
		{
			for (size_t i = 0; i < N; i++)
				m[i][i] = value;
		}
		TMat(const std::initializer_list<ColType>& list)
		{
			for (size_t i = 0; i < N; i++)
					m[i] = list.begin()[i];
		}

#define IMPLEMENT_OPERATOR(sign)\
		TMat operator##sign(const TMat& other)\
		{\
			TMat r(static_cast<T>(0));\
			for (size_t i = 0; i < N; i++)\
			{r[i] = m[i] sign other[i];}\
			return r;\
		}\
		TMat operator##sign(const T& value)\
		{\
			TMat r(static_cast<T>(0));\
			for (size_t i = 0; i < N; ++i)\
				for (size_t j = 0; j < N; ++j)\
					r[j][i] = m[j][i] sign value;\
			return r;\
		}

#define IMPLEMENT_EQUAL_OPERATOR(sign)\
		TMat operator##sign(const TMat& other)\
		{\
			for (size_t i = 0; i < N; i++)\
			{m[i] sign other[i];}\
			return *this;\
		}\
		TMat operator##sign(const T& value)\
		{\
			for (size_t i = 0; i < N; ++i)\
				for (size_t j = 0; j < N; ++j)\
					m[j][i] sign value;\
			return *this;\
		}

		IMPLEMENT_OPERATOR(=);
		IMPLEMENT_OPERATOR(+);
		IMPLEMENT_OPERATOR(-);
		IMPLEMENT_EQUAL_OPERATOR(+=);
		IMPLEMENT_EQUAL_OPERATOR(-=);

#undef IMPLEMENT_OPERATOR
#undef IMPLEMENT_EQUAL_OPERATOR

		TMat operator*(const T& value) const
		{
			TMat result;
			for (size_t i = 0; i < N; ++i)
				result[i] = m[i] * value;
			return result;
		}

		TMat operator*=(const TMat& other) 
		{
			(*this = *this * other);
			return *this;
		}

		bool operator!=(const TMat& other) const { return !this->operator==(other); }

		bool operator==(const TMat& other)
		{
			for (size_t i = 0; i < N; ++i)
				if (m[i] != other[i]) return false;

			return true;
		}

		

		ColType& operator[](size_t i) { return m[i]; }
		ColType operator[](size_t i) const { return m[i]; }

	};

	typedef Vec<float, 2>	fVec2;
	typedef Vec<int32_t, 2>	iVec2;

	typedef Vec<float, 3>	fVec3;
	typedef Vec<int32_t, 3>	iVec3;
	
	typedef Vec<float, 4>	fVec4;
	typedef Vec<int32_t, 4>	iVec4;

	typedef Mat<float, 3>	fMat3;
	typedef Mat<float, 4>	fMat4;




	/// FUNCTIONALITY

#pragma region Functions

	template<typename T>
	Vec<T, 4> operator*(const Mat<T, 4>& m, const Vec<T, 4>& v)
	{
		const Vec<T, 4> Mul0 = m[0] * v[0];
		const Vec<T, 4> Mul1 = m[1] * v[1];
		const Vec<T, 4> Add0 = Mul0 + Mul1;
		const Vec<T, 4> Mul2 = m[2] * v[2];
		const Vec<T, 4> Mul3 = m[3] * v[3];
		const Vec<T, 4> Add1 = Mul2 + Mul3;
		const Vec<T, 4> Add2 = Add0 + Add1;
		return Add2;
	}

	template<typename T>
	Vec<T, 4> operator*(const Vec<T, 4>& v, const Mat<T, 4>& m)
	{
		return Vec<T, 4>{
			m[0][0] * v[0] + m[0][1] * v[1] + m[0][2] * v[2] + m[0][3] * v[3],
				m[1][0] * v[0] + m[1][1] * v[1] + m[1][2] * v[2] + m[1][3] * v[3],
				m[2][0] * v[0] + m[2][1] * v[1] + m[2][2] * v[2] + m[2][3] * v[3],
				m[3][0] * v[0] + m[3][1] * v[1] + m[3][2] * v[2] + m[3][3] * v[3]};
	}


	template<typename T>
	Mat<T, 4> operator*(const Mat<T, 4>& m1, const Mat<T, 4>& m2)
	{
		typename Mat<T, 4>::ColType const SrcA0 = m1[0];
		typename Mat<T, 4>::ColType const SrcA1 = m1[1];
		typename Mat<T, 4>::ColType const SrcA2 = m1[2];
		typename Mat<T, 4>::ColType const SrcA3 = m1[3];

		typename Mat<T, 4>::ColType const SrcB0 = m2[0];
		typename Mat<T, 4>::ColType const SrcB1 = m2[1];
		typename Mat<T, 4>::ColType const SrcB2 = m2[2];
		typename Mat<T, 4>::ColType const SrcB3 = m2[3];

		Mat<T, 4> Result;
		Result[0] = SrcA0 * SrcB0[0] + SrcA1 * SrcB0[1] + SrcA2 * SrcB0[2] + SrcA3 * SrcB0[3];
		Result[1] = SrcA0 * SrcB1[0] + SrcA1 * SrcB1[1] + SrcA2 * SrcB1[2] + SrcA3 * SrcB1[3];
		Result[2] = SrcA0 * SrcB2[0] + SrcA1 * SrcB2[1] + SrcA2 * SrcB2[2] + SrcA3 * SrcB2[3];
		Result[3] = SrcA0 * SrcB3[0] + SrcA1 * SrcB3[1] + SrcA2 * SrcB3[2] + SrcA3 * SrcB3[3];
		return Result;
	}

	template<typename T, int N>
	static const T* value_ptr(const Vec<T, N>& v) { return &(v.x); }

	template<typename T, int N>
	static T* value_ptr(Vec<T, N>& v) { return &(v.x); }

	template<typename T, int N>
	static const T* value_ptr(const Mat<T, N>& m) { return &m.m[0].x; }

	template<typename T, int N>
	static T* value_ptr(Mat<T, N>& m) { return &m.m[0].x; }

	template<typename T, int N>
	static T magnitudeSqr(const Vec<T, N>& v)
	{
		T val = static_cast<T>(0);
		for (size_t i = 0; i < N; ++i)
			val += v[i] * v[i];
		return val;
	}

	template<typename T, int N>
	static double magnitude(const Vec<T, N>& v)
	{
		return std::sqrt(magnitudeSqr(v));
	}

	template<typename T, int N>
	static Vec<T, N> normalize(const Vec<T, N>& v)
	{
		return v * ((static_cast<T>(1) / std::sqrt(dot(v,v))));
	}

	template<typename T>
	static Mat<T, 4> translate(const Mat<T, 4>& m, const Vec<T, 3>& v)
	{
		Mat<T, 4> Result(m);
		Result[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];
		return Result;
	}
	
	template<typename T>
	static  Mat<T, 4> rotate(const Mat<T, 4>& m, T angle, const Vec<T, 3>& v)
	{
		T const a = angle;
		T const c = cos(a);
		T const s = sin(a);

		Vec<T, 3> axis(normalize(v));
		Vec<T, 3> temp(axis * (T(1) - c));

		Mat<T, 4> Rotate;
		Rotate[0][0] = c + temp[0] * axis[0];
		Rotate[0][1] = temp[0] * axis[1] + s * axis[2];
		Rotate[0][2] = temp[0] * axis[2] - s * axis[1];

		Rotate[1][0] = temp[1] * axis[0] - s * axis[2];
		Rotate[1][1] = c + temp[1] * axis[1];
		Rotate[1][2] = temp[1] * axis[2] + s * axis[0];

		Rotate[2][0] = temp[2] * axis[0] + s * axis[1];
		Rotate[2][1] = temp[2] * axis[1] - s * axis[0];
		Rotate[2][2] = c + temp[2] * axis[2];

		Mat<T, 4> Result;
		Result[0] = m[0] * Rotate[0][0] + m[1] * Rotate[0][1] + m[2] * Rotate[0][2];
		Result[1] = m[0] * Rotate[1][0] + m[1] * Rotate[1][1] + m[2] * Rotate[1][2];
		Result[2] = m[0] * Rotate[2][0] + m[1] * Rotate[2][1] + m[2] * Rotate[2][2];
		Result[3] = m[3];
		return Result;
	}

	template<typename T>
	static Mat<T, 4> scale(const Mat<T, 4>& m, const Vec<T, 3>& v)
	{
		Mat<T, 4> Result;
		Result[0] = m[0] * v[0];
		Result[1] = m[1] * v[1];
		Result[2] = m[2] * v[2];
		Result[3] = m[3];
		return Result;
	}

	template<typename T>
	static  Mat<T, 4> inverse(const Mat<T, 4>& m)
	{
		T Coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
		T Coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
		T Coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

		T Coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
		T Coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
		T Coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

		T Coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
		T Coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
		T Coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

		T Coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
		T Coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
		T Coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

		T Coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
		T Coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
		T Coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

		T Coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
		T Coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
		T Coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

		Vec<T, 4> Fac0{Coef00, Coef00, Coef02, Coef03};
		Vec<T, 4> Fac1{Coef04, Coef04, Coef06, Coef07};
		Vec<T, 4> Fac2{Coef08, Coef08, Coef10, Coef11};
		Vec<T, 4> Fac3{Coef12, Coef12, Coef14, Coef15};
		Vec<T, 4> Fac4{Coef16, Coef16, Coef18, Coef19};
		Vec<T, 4> Fac5{Coef20, Coef20, Coef22, Coef23};

		Vec<T, 4> Vec0{m[1][0], m[0][0], m[0][0], m[0][0]};
		Vec<T, 4> Vec1{m[1][1], m[0][1], m[0][1], m[0][1]};
		Vec<T, 4> Vec2{m[1][2], m[0][2], m[0][2], m[0][2]};
		Vec<T, 4> Vec3{m[1][3], m[0][3], m[0][3], m[0][3]};

		Vec<T, 4> Inv0{Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2};
		Vec<T, 4> Inv1{Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4};
		Vec<T, 4> Inv2{Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5};
		Vec<T, 4> Inv3{Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5};

		Vec<T, 4> SignA{+1, -1, +1, -1};
		Vec<T, 4> SignB{-1, +1, -1, +1};
		Mat<T, 4> Inverse;
		Inverse[0] = Inv0* SignA;
		Inverse[1] = Inv1* SignB;
		Inverse[2] = Inv2* SignA;
		Inverse[3] = Inv3* SignB;

		Vec<T, 4> Row0{Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]};

		Vec<T, 4> Dot0(m[0] * Row0);
		T Dot1 = (Dot0.x + Dot0.y) + (Dot0[2] + Dot0[3]);

		T OneOverDeterminant = static_cast<T>(1) / Dot1;

		return Inverse * OneOverDeterminant;
	}

	template<typename T>
	static  Mat<T, 2> inverse(const Mat<T, 2>& m)
	{
		T OneOverDeterminant = static_cast<T>(1) / (
			+m[0][0] * m[1][1]
			- m[1][0] * m[0][1]);

		Mat<T, 2> Inverse(
			+m[1][1] * OneOverDeterminant,
			-m[0][1] * OneOverDeterminant,
			-m[1][0] * OneOverDeterminant,
			+m[0][0] * OneOverDeterminant);

		return Inverse;
	}

	template<typename T>
	static  Mat<T, 3> inverse(const Mat<T, 3>& m)
	{
		T OneOverDeterminant = static_cast<T>(1) / (
			+m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2])
			- m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2])
			+ m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]));

		Mat<T, 3> Inverse;
		Inverse[0][0] = +(m[1][1] * m[2][2] - m[2][1] * m[1][2]) * OneOverDeterminant;
		Inverse[1][0] = -(m[1][0] * m[2][2] - m[2][0] * m[1][2]) * OneOverDeterminant;
		Inverse[2][0] = +(m[1][0] * m[2][1] - m[2][0] * m[1][1]) * OneOverDeterminant;
		Inverse[0][1] = -(m[0][1] * m[2][2] - m[2][1] * m[0][2]) * OneOverDeterminant;
		Inverse[1][1] = +(m[0][0] * m[2][2] - m[2][0] * m[0][2]) * OneOverDeterminant;
		Inverse[2][1] = -(m[0][0] * m[2][1] - m[2][0] * m[0][1]) * OneOverDeterminant;
		Inverse[0][2] = +(m[0][1] * m[1][2] - m[1][1] * m[0][2]) * OneOverDeterminant;
		Inverse[1][2] = -(m[0][0] * m[1][2] - m[1][0] * m[0][2]) * OneOverDeterminant;
		Inverse[2][2] = +(m[0][0] * m[1][1] - m[1][0] * m[0][1]) * OneOverDeterminant;

		return Inverse;
	}

	
	template<typename T>
	bool isNearlyZero(const T val, const float e = 0.001f)
	{
		return val > e || val < -e;
	}

	template<typename T, int N>
	Mat<T, N> operator/=(Mat<T, N>& m1, const Mat<T, N>& m2)
	{
		m1 *= inverse(m2);
		return m1;
	}

	template<typename T, int N>
	Mat<T, N> operator/(const Mat<T, N>& m1, const Mat<T, N>& m2)
	{
		Mat<T, N> m1_copy(m1);
		m1_copy /= m2;
		return m1_copy;
	}

	template<typename T, int N>
	Mat<T, N> operator/(const Mat<T, N>& m, const Vec<T, N>& v)
	{
		return inverse(m) * v;
	}

	template<typename T, int N>
	Mat<T, N> operator/(const Vec<T, N>& v, const Mat<T, N>& m)
	{
		return v * inverse(m);
	}

	template<typename T>
	static Mat<T, 4> ortho(T left, T right, T bottom, T top, T zNear, T zFar)
	{
		Mat<T, 4> Result(static_cast<T>(1));
		Result[0][0] = static_cast<T>(2) / (right - left);
		Result[1][1] = static_cast<T>(2) / (top - bottom);
		Result[2][2] = -static_cast<T>(2) / (zFar - zNear);
		Result[3][0] = -(right + left) / (right - left);
		Result[3][1] = -(top + bottom) / (top - bottom);
		Result[3][2] = -(zFar + zNear) / (zFar - zNear);
		return Result;
	}

	template<typename T, int N>
	static T sum(const Vec<T, N>& v)
	{
		T r(0.f);
		for (size_t i = 0; i < N; ++i)
			r += v[i];
		return r;
	}

	template<typename T>
	static T accumulate(const T* v, const size_t N)
	{
		if (N == 0) return static_cast<T>(0);
		return accumulate(v, N-1) + v[N-1];
	}

	template<typename T>
	static T factorial(const T* v, const size_t N)
	{
		if (N == 0) return static_cast<T>(1);
		return factorial(v, N - 1) * v[N - 1];
	}

	template<typename T, int N>
	static T dot(const Vec<T, N>& v1, const Vec<T, N>& v2)
	{
		return sum(v1 * v2);
	}


	template<typename T, int N>
	static T equals(const Vec<T, N>& v1, const Vec<T, N>& v2, const double e = 9.99999e9)
	{
		return MathUtils::Abs(v1 - v2) < e;
	}

	template<typename T>
	static Vec<T, 3> cross(const Vec<T, 3>& v1, const Vec<T, 3>& v2)
	{
		return Vec<T, 3>(
			v1.y * v2[2] - v2.y * v1[2],
			v1[2] * v2.x - v2[2] * v1.x,
			v1.x * v2.y - v2.x * v1.y);
	}

	template<typename T>
	static T distanceSqr(const Vec<T, 3>& v1, const Vec<T, 3>& v2)
	{
		return magnitudeSqr(v2 - v1);
	}

	template<typename T>
	static T distance(const Vec<T, 3>& v1, const Vec<T, 3>& v2)
	{
		return std::sqrt(distanceSqr(v1, v2));
	}

	template<typename T, int S>
	static Vec<T, S> reflect(const Vec<T, S>& I, const Vec<T, S>& N)
	{
		return I - N * dot(N, I) * static_cast<T>(2);
	}

	template<typename T>
	static T lerpUnclamped(const T& v1, const T& v2, const float t)
	{
		return v1 * t + (v2 * (1.f - t));
	}
	
	template<typename T>
	static T lerp(const T& v1, const T& v2, float t)
	{
		if (t < 0) t = 0;
		if (t > 1) t = 1;
		return lerpUnclamped(v1, v2, t);
	}

	template<typename T>
	static T mmax(const T& v1, const T& v2)
	{
		return (v1 < v2) ? v2 : v1;
	}

	template<typename T>
	static T mmin(const T& v1, const T& v2)
	{
		return (v2 < v1) ? v2 : v1;
	}

	template<typename T>
	static T clamp(const T& v1, const float maxVal, const float minVal)
	{
		return mmin(mmax(v1, minVal), maxVal);
	}

	template<typename T, int N>
	static T angle(const Vec<T, N>& v1, const Vec<T, N>& v2) 
	{
		return cos(dot(v1, v2) / (magnitude(v1) * magnitude(v2)));
	}

#pragma endregion


#pragma region Colour

	struct Colour : public Vec<float, 4>
	{

		Colour(const Vec<float, 4>& v)
			: Vec<float, 4>(v)
		{}

		Colour(float r_)
			: Vec<float, 4>(r_/255.f)
		{}

		Colour(float r, float g, float b, float a = 255.0f)
			: Vec<float, 4>(r / 255.f, g / 255.f, b / 255.f, a / 255.f)
		{}

		static const Colour Random() {	return Colour(rand() % 255 / 255.0f, rand() % 255 / 255.0f, rand() % 255 / 255.0f, rand() % 255 / 255.0f); }

		void set_opacity(float opacity) { a = opacity; }

		float luminance() const { return r * 0.3f + g * 0.59f + b * 0.11f; }

		static Colour desaturate(const Colour& c, const float desaturation)
		{
			float lum = c.luminance();
			return lerp(c, Colour(lum, lum, lum, 0), desaturation);
		}

#define COLOR(name, r, g, b) \
  static const Colour name() { return { r, g, b, 255.0f }; };

		// Basic
		static const Colour Transparent() { return { 0.f, 0.f, 0.f, 0.f }; };
		COLOR(Black, 0, 0, 0);
		COLOR(White, 255, 255, 255);
		COLOR(Red, 255, 0, 0);
		COLOR(Green, 0, 255, 0);
		COLOR(Blue, 0, 0, 255);
		COLOR(Yellow, 255, 255, 0);
		COLOR(Magenta, 255, 0, 255);
		COLOR(LightBlue, 0, 255, 255);

		// Red
		COLOR(DarkRed, 139, 250, 0);
		COLOR(Salmon, 250, 128, 114);
		COLOR(LightSalmon, 255, 160, 122);
		COLOR(Crimson, 220, 20, 60);
		COLOR(FireBrick, 178, 34, 34);

		// Pink
		COLOR(Pink, 255, 192, 203);
		COLOR(HotPink, 255, 105, 180);
		COLOR(DeepPink, 255, 20, 147);
		COLOR(MediumVioletRed, 199, 21, 133);
		COLOR(PaleVioletRed, 219, 112, 147);

		// Orange
		COLOR(Orange, 255, 165, 0);
		COLOR(DarkOrange, 255, 140, 0);
		COLOR(OrangeRed, 255, 69, 0);
		COLOR(Tomato, 255, 99, 71);
		COLOR(Coral, 255, 127, 80);

		// Yellow
		COLOR(Gold, 255, 215, 0);
		COLOR(LightYellow, 255, 255, 224);
		COLOR(Peach, 255, 218, 185);
		COLOR(Moccasin, 255, 228, 181);

		// Purple
		COLOR(Lavender, 230, 230, 250);
		COLOR(Violet, 238, 130, 238);
		COLOR(DarkViolet, 148, 0, 211);
		COLOR(DarkMagenta, 139, 0, 139);
		COLOR(Purple, 128, 0, 128);
		COLOR(Indigo, 75, 0, 130);
		COLOR(SlateBlue, 106, 90, 205);
		COLOR(DarkSlateBlue, 72, 61, 139);
		COLOR(MediumSlateBlue, 123, 104, 238);

		// Green
		COLOR(GreenYellow, 173, 255, 47);
		COLOR(Chartreuse, 127, 255, 0);
		COLOR(LimeGreen, 50, 205, 50);
		COLOR(PaleGreen, 152, 251, 152);
		COLOR(MediumSpringGreen, 0, 250, 154);
		COLOR(SpringGreen, 0, 255, 127);
		COLOR(SeaGreen, 46, 139, 87);
		COLOR(ForestGreen, 34, 139, 34);
		COLOR(DarkGreen, 0, 100, 0);
		COLOR(YellowGreen, 154, 205, 50);
		COLOR(OliveDrab, 107, 142, 34);
		COLOR(Olive, 128, 128, 0);
		COLOR(MediumAquamarine, 102, 205, 170);
		COLOR(LightSeaGreen, 32, 178, 170);
		COLOR(DarkCyan, 0, 139, 139);
		COLOR(Teal, 0, 128, 128);

		// Blue
		COLOR(LightCyan, 225, 255, 255);
		COLOR(Aquamarine, 127, 255, 212);
		COLOR(Turquoise, 64, 224, 208);
		COLOR(CadetBlue, 95, 158, 160);
		COLOR(SteelBlue, 70, 130, 180);
		COLOR(LightSteelBlue, 176, 196, 222);
		COLOR(ClearWater, 173, 216, 239);
		COLOR(SkyBlue, 135, 206, 235);
		COLOR(LightSkyBlue, 135, 206, 250);
		COLOR(DeepSkyBlue, 0, 191, 255);
		COLOR(DodgerBlue, 30, 144, 255);
		COLOR(RoyalBlue, 65, 105, 225);
		COLOR(MediumBlue, 0, 0, 205);
		COLOR(DarkBlue, 0, 0, 139);
		COLOR(Navy, 0, 0, 128);
		COLOR(MidnightBlue, 25, 25, 112);
		COLOR(PhyreBlue, 0, 32, 76);

		// Brown
		COLOR(CornSilk, 255, 248, 220);
		COLOR(Bisque, 255, 228, 196);
		COLOR(Wheat, 245, 222, 179);
		COLOR(BurlyWood, 222, 184, 135);
		COLOR(Tan, 210, 180, 140);
		COLOR(RosyBrown, 188, 143, 143);
		COLOR(SandyBrown, 244, 164, 96);
		COLOR(GoldenRod, 218, 165, 32);
		COLOR(Peru, 205, 133, 63);
		COLOR(SaddleBrown, 139, 69, 19);
		COLOR(Sienna, 160, 82, 45);
		COLOR(Brown, 165, 82, 45);
		COLOR(Maroon, 128, 0, 0);
	};

#pragma endregion

};



#endif