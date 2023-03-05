#ifndef _CLOVER_H__
#define _CLOVER_H__ 1

#define _GLFW_WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <iostream>
#include <memory>
#include <utility>
#include <optional>
#include <algorithm>
#include <functional>
#include <fstream>
#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_set>
#include <typeindex>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <stdint.h>
#include <filesystem>
#include <unordered_map>



#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/matrix_decompose.inl"

// Own includes
//#include "Tools/Log.h"
//#include "Tools/FileManager.h"
//#include "Tools/FileSystem.h"
#include "Tools/UniqueId.h"
#include "Tools/Random.h"


#define MIN(a,b) (a < b ? a : b)
#define PI 3.141592
#define POOL_SIZE 10
#define DEFAULT_UNLIT "DefaultUnlit"
#define DEFAULT_LIT "DefaultLit"
#define DEPTH_TEXTURE_ID 0
#define DEFAULT_TEXTURES 0
#define DIFFUSE_TEXTURE "u_diffuse"
#define NORMAL_TEXTURE "u_normal"
//PBR
#define AMBIENT_OCLUSION_TEXTURE "u_ambient"
#define SPECULAR_TEXTURE "u_specular"
#define ROUGH_TEXTURE "u_rough"
#define HEIGHT_TEXTURE "u_height"
//Enable Settings
#define Cl_DISABLE -1
#define Cl_BLEND 0
#define Cl_DEPTH 1
// Cull face
#define Cl_FRONT 0x0404
#define Cl_BACK 0x0405
#define Cl_FRONT_AND_BACK 0x0408
// Draw Mode
#define Cl_POINT_DRAW_MODE 0x1B00
#define Cl_LINE_DRAW_MODE 0x1B01
#define Cl_DEFAULT_DRAW_MODE 0x1B02
#define Cl_POLYGON_DRAW_MODE 0x0009
// Other
#define AssetsPath "assets/"
#define Cl_ASSERT(x, ...) {if(!(x)) {__debugbreak();}}
#define BIT(x) (1 << x)

typedef enum
{
	kErrorCode_Ok = 0,
	kErrorCode_Memory = -10,
	kErrorCode_MemoryNull = -11,
	kErrorCode_NodeNULL = -12,

	kErrorCode_Size = -13,
	kErrorCode_NoneBytes = -14,
	kErrorCode_MemorySet = -15,
	kErrorCode_SrcNull = -16,
	kErrorCode_Malloc = -17,
	kErrorCode_Insert = -18,
	kErrorCode_Concat = -19,

	kErrorCode_vectorNULL = -20,
	kErrorCode_VectorFull = -21,
	kErrorCode_VectorEmpty = -22,

	kErrorCode_OutOfRange = -23,
	kErrorCode_CallbackNULL = -24,

	kErrorCode_ListNULL = -25,
	kErrorCode_ListFull = -26,
	kErrorCode_ListEmpty = -32,

	kErrorCode_stackNULL = -27,
	kErrorCode_stackEmpty = -28,
	kErrorCode_stackFull = -29,
	kErrorCode_storageNULL = -30,

	kErrorCode_resize = -31,

	kErrorCode_QueueNULL = -32,
	kErrorCode_QueueStorageNULL = -33

} ErrorCode;

typedef uint8_t 		u8;
typedef int_least8_t 	s8;
typedef uint16_t 		u16;
typedef int_least16_t 	s16;
typedef uint32_t 		u32;
typedef int_least32_t 	s32;
typedef uint64_t 		u64;
typedef int_least64_t	s64;

typedef u8 byte;

typedef size_t ClassID;
typedef s64 EntityID;
typedef u64 ComponentID;
typedef u64 SystemID;
typedef int Flags;

typedef u32 RenderID;
typedef u32 KeyID;
typedef u32 PostProcessID;


typedef glm::vec2  CVec2;
typedef glm::vec3  CVec3;
typedef glm::vec4  CVec4;
typedef glm::mat3  CMat3;
typedef glm::mat4  CMat4;

typedef std::filesystem::path CPath;
typedef std::string 	CString;


template<typename T, typename T2>
using CPair = std::pair<T, T2>;

template<typename T>
using CVector = std::vector<T>;

template<typename key, typename value>
using CMap = std::unordered_map<key, value>;


template<typename T>
ClassID getClassID()
{
	return std::type_index(typeid(T)).hash_code();
}

template<typename T>
CString getClassName()
{
	return std::type_index(typeid(T)).name();
}

template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename ... Args>
constexpr Scope<T> CreateScope(Args&& ... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

enum class ShaderDataType {
	None = 0,
	Float,
	Float2,
	Float3,
	Float4,
	UInt,
	Int,
	Int2,
	Int3,
	Int4,
	Mat3,
	Mat4,
	Bool
};


static u32 ShaderDataTypeSize(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Float:  return 4;
	case ShaderDataType::Float2: return 4 * 2;
	case ShaderDataType::Float3: return 4 * 3;
	case ShaderDataType::Float4: return 4 * 4;
	case ShaderDataType::UInt:   return 4;
	case ShaderDataType::Int:    return 4;
	case ShaderDataType::Int2:   return 4 * 2;
	case ShaderDataType::Int3:   return 4 * 3;
	case ShaderDataType::Int4:   return 4 * 4;
	case ShaderDataType::Mat3:   return 3 * 3;
	case ShaderDataType::Mat4:   return 4 * 4;
	case ShaderDataType::Bool:   return 1;
	}

	return 0;
}

static size_t SizeOfShaderDataType(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Float:  return sizeof(float);
	case ShaderDataType::Float2: return sizeof(float) * 2;
	case ShaderDataType::Float3: return sizeof(float) * 3;
	case ShaderDataType::Float4: return sizeof(float) * 4;
	case ShaderDataType::UInt:   return sizeof(u32);
	case ShaderDataType::Int:    return sizeof(s32);
	case ShaderDataType::Int2:   return sizeof(s32) * 2;
	case ShaderDataType::Int3:   return sizeof(s32) * 3;
	case ShaderDataType::Int4:   return sizeof(s32) * 4;
	case ShaderDataType::Mat3:   return sizeof(CMat3);
	case ShaderDataType::Mat4:   return sizeof(CMat4);
	case ShaderDataType::Bool:   return sizeof(bool);
	}

	return 0;
}



enum Type {
	T_NONE = 0x0,
	T_FLOAT_1,
	T_FLOAT_2,
	T_FLOAT_3,
	T_FLOAT_4,

	T_DOUBLE_1,
	T_DOUBLE_2,
	T_DOUBLE_3,
	T_DOUBLE_4,

	T_INT_1,
	T_INT_2,
	T_INT_3,
	T_INT_4,

	T_UINT_1,
	T_UINT_2,
	T_UINT_3,
	T_UINT_4,

	T_BYTE_1,
	T_BYTE_2,
	T_BYTE_3,
	T_BYTE_4,

	T_UBYTE_1,
	T_UBYTE_2,
	T_UBYTE_3,
	T_UBYTE_4,

	T_SHORT_1,
	T_SHORT_2,
	T_SHORT_3,
	T_SHORT_4,

	T_USHORT_1,
	T_USHORT_2,
	T_USHORT_3,
	T_USHORT_4,

	T_MAT_4x4,
	T_MAT_3x3,
	T_MAT_2x2,

	T_SAMPLER_1D,
	T_SAMPLER_2D,
	T_SAMPLER_3D,

	// aliases
	T_FLOAT = T_FLOAT_1,
	T_DOUBLE = T_DOUBLE_1,
	T_INT = T_INT_1,
	T_UINT = T_UINT_1,
	T_BYTE = T_BYTE_1,
	T_UBYTE = T_UBYTE_1,
	T_SHORT = T_SHORT_1,
	T_USHORT = T_USHORT_1,

	//--------------------------
	T_MAX_DEFINED // max value of defined types
};
using KeyCode = uint16_t;

namespace Key
{
	enum : KeyCode
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	};

}

using MouseCode = uint16_t;

namespace Mouse
{
	enum : MouseCode
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	};
}



class Cl_Object
{
public:
	Cl_Object() = default;
	~Cl_Object() = default;

	virtual void ShowEditor() {};
	UniqueID GetInternalID() { return internal_id; }
private:
	UniqueID internal_id;
};






#endif
