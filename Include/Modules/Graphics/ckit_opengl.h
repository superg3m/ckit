#include "../../../Core/Basic/ckit_types.h"
#include "../../../Core/Basic/ckit_logger.h"
#include "../../../Core/Collections/ckit_vector.h"
#include "./glad/glad.h"
#include "./stb_image.h"

// Textures

// Shaders
typedef struct CKIT_Shader {
	u32* textures;
	u32 id;
} CKIT_Shader;

CKIT_Shader ckit_shader_create() {

}

CKIT_Shader ckit_shader_add_texture(CKIT_Shader* shader) {
  u32 texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nrChannels;
  u8 *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    LOG_ERROR("Failed to load texture\n");
  }
  stbi_image_free(data);  
}

CKIT_Shader ckit_shader_bind_texture(CKIT_Shader* shader) {
  for (u32 i = 0; i < ckit_vector_count(shader->textures); i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, shader->textures[i]);
  }
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