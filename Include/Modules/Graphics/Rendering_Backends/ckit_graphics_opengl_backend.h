#include "../../../Core/Basic/ckit_types.h"
#include "../../../Core/Basic/ckit_logger.h"
#include "../../../Core/Collections/ckit_vector.h"
#include "./glad/glad.h"
#include "./stb_image.h"
#include <windows.h>

// Textures
#define TEXTURE_MAX 32

// Shaders
typedef enum CKIT_ShaderType{
	CKIT_VERTEX_SHADER,
	CKIT_FRAGMENT_SHADER
} CKIT_ShaderType;

typedef struct CKIT_ShaderDescriptor {
	CKIT_ShaderType type;
	const char* path; 
} CKIT_ShaderDescriptor;

typedef struct CKIT_Shader {
	u32* textures;
	u32 id;
} CKIT_Shader;

void ckit_shader_check_compile_erros(u32 shaderID, const char* type) {
    int success;
    char info_log[1024];
    if (type != "PROGRAM") {
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shaderID, 1024, NULL, info_log);
			LOG_ERROR("ERROR::SHADER_COMPILATION_ERROR of type: %s\n", type);
			LOG_ERROR("%s -- --------------------------------------------------- --\n", info_log);
		}
    } else {
		glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderID, 1024, NULL, info_log);
			LOG_ERROR("ERROR::PROGRAM_LINKING_ERROR of type: %s\n", type);
			LOG_ERROR("%s -- --------------------------------------------------- --\n", info_log);
		}
    }
}

// Date: August 15, 2024
// NOTE(Jovanni): You might not even need this shader_discriptors
CKIT_Shader ckit_shader_create(CKIT_ShaderDescriptor* shader_descriptor, u32 shader_descriptor_count) {
	CKIT_Shader ret = {0};
	u32* shader_source_ids = NULLPTR; 

	ret.id = glCreateProgram();
	for (int i = 0; i < shader_descriptor_count; i++) {
		const char* shader_source = ckit_os_read_entire_file(shader_descriptor[i].path);
		u32 source_id;

		switch (shader_descriptor[i].type) {
			case CKIT_VERTEX_SHADER: {
				source_id = glCreateShader(GL_VERTEX_SHADER);
				glShaderSource(source_id, 1, &shader_source, NULL);
        		glCompileShader(source_id);
        		ckit_shader_check_compile_erros(source_id, "VERTEX");
				glAttachShader(ret.id, source_id);
			} break;

			case CKIT_FRAGMENT_SHADER: {
				source_id = glCreateShader(GL_FRAGMENT_SHADER);
				glShaderSource(source_id, 1, &shader_source, NULL);
        		glCompileShader(source_id);
        		ckit_shader_check_compile_erros(source_id, "FRAGMENT");
				glAttachShader(ret.id, source_id);
			} break;
		}

		ckit_vector_push(shader_source_ids, source_id);
	}
	glLinkProgram(ret.id);

	Boolean success = FALSE;
	glGetProgramiv(ret.id, GL_LINK_STATUS, &success);
	if (!success) {
		char info_log[1028] = {0};
		glGetProgramInfoLog(ret.id, 512, NULL, info_log);
		LOG_ERROR("LINKING_FAILED\n");
	}

	for (int i = 0; i < ckit_vector_count(shader_source_ids); i++) {
		glDeleteShader(shader_source_ids[i]);
	}

	return ret;
}

CKIT_Shader ckit_shader_add_texture(CKIT_Shader* shader) {
	ckit_assert_msg(ckit_vector_count(shader->textures) <= TEXTURE_MAX, "Texture max hit!");

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

	ckit_vector_push(shader->textures, texture);
}

CKIT_Shader ckit_shader_bind_textures(CKIT_Shader* shader) {
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

void ckit_wgl_context_create(HDC dc_handle) {
	HGLRC opengl_context = wglCreateContext(dc_handle);
	wglMakeCurrent(dc_handle, opengl_context);
}

// WGL (wglMakeCurrent())

// 