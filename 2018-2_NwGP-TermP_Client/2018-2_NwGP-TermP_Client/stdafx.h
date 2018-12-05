// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#define SERVERPORT	9000
#define BUFSIZE		512

#define SERVERIP	"127.0.0.1"//"192.168.20.208"
#define WM_SOCKET	WM_USER+1
#pragma once

#include "targetver.h"
#include "../../2018-2_NwGP-TermP_Server/NGP_Server/protocol.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:

#include <winsock2.h>
#include <windows.h>


// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// C++ 런타임 헤더 파일입니다.
#include <iostream>
#include <chrono>
#include <string>
#include <math.h>

#pragma comment(lib, "ws2_32")
#pragma pack(1)

// 캡션 FPS 출력 여부
// 항상 캡션에 FPS를 출력	(0: 비활성 | 1: 활성)
#define USE_CAPTIONFPS_ALWAYS	1

#if USE_CAPTIONFPS_ALWAYS
#define SHOW_CAPTIONFPS
#elif _DEBUG	// debug에서는 항상 실행
#define SHOW_CAPTIONFPS
#endif

#if defined(SHOW_CAPTIONFPS)
#define MAX_UPDATE_FPS 1.0 / 3.0
#endif

// 최대 FPS
#if _DEBUG
#define MAX_FRAMETIME 0.0
#else
// #define MAX_FRAMETIME 1.0 / 60.0
#define MAX_FRAMETIME 0.0
#endif

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <atlImage.h>
#include <random>

#define TITLE_MX_LENGTH 64
#define TITLESTRING TEXT("2018-2_NetworkGameProgramming_TermProject2")

// 초기 클라이언트 크기
#define CLIENT_WIDTH	1024
#define CLIENT_HEIGHT	768

#define DIR_UP			0x01
#define DIR_DOWN		0x02
#define DIR_LEFT		0x04
#define DIR_RIGHT		0x08

#define MAX_HP	100

#define USE_CONSOLE_WINDOW
#ifdef USE_CONSOLE_WINDOW	// 콘솔창을 띄우는 define
#include <stdio.h>
#include <iostream>
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 
#endif
#endif

template <typename val>
struct Vector2D {
	union {
		val value[2];
		struct { val x, y; };
		struct { val width, height; };
		struct { val w, h; };
	};
#define v value[0]
	//Vector2D& operator= (const POINT p) { x = p.x, y = p.y; return *this; }
	Vector2D& operator= (const Vector2D<val> p) { x = p.x, y = p.y; return *this; }
};

using Vec2i = Vector2D<unsigned int>;

template <typename val>
struct Vector3D {
	union {
		val value[3];
		struct { val x, y, z; };
		struct { val r, g, b; };
	};
#define v value[0]
};

template <typename val>
val clamp(val minimum, val value, val maximum) { return max(minimum, min(value, maximum)); }

enum ObjectState {
	idle = 0,
	walking = 1,
	melee_attack = 2
};