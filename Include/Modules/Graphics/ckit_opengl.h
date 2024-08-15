#include "../../../Core/Basic/ckit_types.h"
#include "../../../Core/Basic/ckit_logger.h"
#include "../../../Core/Collections/ckit_vector.h"
#include "./glad/glad.h"
#include "./stb_image.h"

// Textures
#define TEXTURE_MAX 32

// Shaders
typedef struct CKIT_Shader {
	u32* textures;
	u32 id;
} CKIT_Shader;

CKIT_Shader ckit_shader_create() {
	CKIT_Shader ret = {0};

	ret.id = glCreateProgram();
	glAttachShader(ret.id, vertex);
	glAttachShader(ret.id, fragment);
	glLinkProgram(ret.id);
	// print linking errors if any
	glGetProgramiv(ret.id, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(ret.id, 512, NULL, infoLog);
		std::cout << "LINKING_FAILED\n" << infoLog << std::endl;
	}

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return ret;
}

void ckit_shader_add_source(CKIT_Shader *) {
	CKIT_Shader ret = {0};

	ret.id = glCreateProgram();
	glAttachShader(ret.id, vertex);
	glAttachShader(ret.id, fragment);
	glLinkProgram(ret.id);
	// print linking errors if any
	glGetProgramiv(ret.id, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(ret.id, 512, NULL, infoLog);
		std::cout << "LINKING_FAILED\n" << infoLog << std::endl;
	}

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return ret;
}

CKIT_Shader ckit_shader_create() {
	CKIT_Shader ret = {0};

	ret.id = glCreateProgram();
	glAttachShader(ret.id, vertex);
	glAttachShader(ret.id, fragment);
	glLinkProgram(ret.id);
	// print linking errors if any
	glGetProgramiv(ret.id, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(ret.id, 512, NULL, infoLog);
		std::cout << "LINKING_FAILED\n" << infoLog << std::endl;
	}

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return ret;
}


void ckit_shader_check_compile_erros(u32 shaderID, const char* type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
    } else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
    }
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