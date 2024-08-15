#include "../../Core/Basic/ckit_types.h"
#include <glad/glad.h>

// Textures

// Shaders
typedef struct CKIT_Shader {
	u32* textures;
	u32 id;
} CKIT_Shader;

CKIT_Shader ckit_shader_create() {

}

void ckit_shader_use(CKIT_Shader* shader) {
	glUseProgram(shader->id);
}

/*
void *GetAnyGLFuncAddress(const char *name)
{
  void *p = (void *)wglGetProcAddress(name);
  if(p == 0 ||
    (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) ||
    (p == (void*)-1) )
  {
    HMODULE module = LoadLibraryA("opengl32.dll");
    p = (void *)GetProcAddress(module, name);
  }

  return p;
}
*/


// WGL (wglMakeCurrent())

// 