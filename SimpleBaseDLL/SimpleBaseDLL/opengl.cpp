#include "pch.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment(lib, "OpenGL32.lib")

typedef void(APIENTRY* glBegin_t)(GLenum);
typedef void(APIENTRY* glEnd_t)(void);
typedef void(APIENTRY* glVertex3fv_t)(const GLfloat* v);
typedef void(APIENTRY* glClear_t)(GLbitfield);

glBegin_t pglBegin = NULL;
glEnd_t pglEnd = NULL;
glVertex3fv_t pglVertex3fv = NULL;
glClear_t pglClear = NULL;

GLenum g_glMode;

void APIENTRY Hooked_glBegin(GLenum mode)
{
	g_glMode = mode;

	// GetASynckeyState(VK_Z)
	if (GetKeyState(VK_RCONTROL) || GetKeyState(VK_RSHIFT)) {
		if (mode == GL_TRIANGLES || mode == GL_TRIANGLE_STRIP || mode == GL_TRIANGLE_FAN)
		{
			glDepthRange(0, 0.5);
		}
		else {
			glDepthRange(0.5, 1);
		}
	}


	(*pglBegin)(mode);
}

void APIENTRY Hooked_glEnd(void)
{

	(*pglEnd)();
}

 void APIENTRY Hooked_glVertex3fv(GLfloat* v)
{

	(*pglVertex3fv)(v);
 }

void APIENTRY Hooked_glClear(GLbitfield mask)
{
	(*pglClear)(mask);
}

void* DetourFunc(BYTE* src, const BYTE* dst, const int len)
{
	BYTE* jmp = (BYTE*)malloc(len + 5);
	DWORD dwback;
	VirtualProtect(src, len, PAGE_READWRITE, &dwback);
	memcpy(jmp, src, len);
	jmp += len;
	jmp[0] = 0xE9;
	*(DWORD*)(jmp + 1) = (DWORD)(src + len - jmp) - 5;
	src[0] = 0xE9;
	*(DWORD*)(src + 1) = (DWORD)(dst - src) - 5;
	VirtualProtect(src, len, dwback, &dwback);
	return (jmp - len);
}

void HookOpenGL(void)
{
	HMODULE hmOpenGL = GetModuleHandleA("opengl32.dll");
	pglBegin = (glBegin_t)DetourFunc((LPBYTE)GetProcAddress(hmOpenGL, "glBegin"), (LPBYTE)&Hooked_glBegin, 5);
	pglEnd = (glEnd_t)DetourFunc((LPBYTE)GetProcAddress(hmOpenGL, "glEnd"), (LPBYTE)&Hooked_glEnd, 6);
	pglVertex3fv = (glVertex3fv_t)DetourFunc((LPBYTE)GetProcAddress(hmOpenGL, "glVertex3fv"), (LPBYTE)&Hooked_glVertex3fv, 5);
	pglClear = (glClear_t)DetourFunc((LPBYTE)GetProcAddress(hmOpenGL, "glClear"), (LPBYTE)&Hooked_glClear, 5);
	// gl hooks hier
}