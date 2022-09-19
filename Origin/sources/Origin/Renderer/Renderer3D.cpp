#include "pch.h"
#include "Renderer3D.h"
#include "Origin\Scene\Component\Camera.h"
#include "RenderCommand.h"

#include "Platform\OpenGL\OpenGL_Shader.h"
#include "stb_image.h"

#include <glm\gtc\matrix_transform.hpp>


namespace Origin
{

	struct Renderer3D_Data
	{
		// Skybox Data
		std::shared_ptr<Shader> skyShader;
		std::shared_ptr<VertexArray> skyVA;
		std::shared_ptr<VertexBuffer> skyVB;
		uint32_t skyTextureID;
	};

	static Renderer3D_Data s_3Ddata;

	void Renderer3D::Init()
	{
		// Skybox
		SkyboxInit();
	}

	void Renderer3D::BeginScene(const Camera& camera, glm::mat4& transform)
	{
		glm::mat4 viewProjection = camera.GetProjection() * glm::inverse(transform);

		StartBatch();
	}

	void Renderer3D::BeginScene(const EditorCamera& camera)
	{
		s_3Ddata.skyShader->SetMatrix("u_ViewProjection", camera.GetViewProjection());
		s_3Ddata.skyShader->Bind();

		StartBatch();
	}

	void Renderer3D::EndScene()
	{
		Flush();
	}

	void Renderer3D::Flush()
	{

	}

	void Renderer3D::Shutdown()
	{
	}

	void Renderer3D::SkyboxInit()
	{
		//Skybox
		///  Should start with the right side [ GL_CUBE_MAP_POSITIVE_X ]

		float vertices[] = {

			 1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f, // 0 [RIGHT] top left	
			 1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f, // 1 [RIGHT] top right
			 1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f, // 2 [RIGHT] bottom right
			 1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f, // 3 [RIGHT] bottom left

			-1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, // 4 [LEFT] top left	
			-1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, // 5 [LEFT] top right
			-1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, // 6 [LEFT] bottom right
			-1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, // 7 [LEFT] bottom left

			-1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, // 8 [TOP] front left
			 1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f, // 9 [TOP] front right
			 1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f, //10 [TOP] back right
			-1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, //11 [TOP] back left

			-1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, //12 [BOTTOM] front left
			 1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f, //13 [BOTTOM] front right
			 1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f, //14 [BOTTOM] back right
			-1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, //15 [BOTTOM] back left

			-1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, //16 [BACK] bottom left
			 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, //17 [BACK] bottom right
			 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f, //18 [BACK] top right
			-1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, //19 [BACK] top left

			-1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, //20 [FRONT] bottom left
			 1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, //21 [FRONT] bottom right
			 1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, //22 [FRONT] top right
			-1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, //23 [FRONT] top left
		};

		s_3Ddata.skyVA = VertexArray::Create();
		s_3Ddata.skyVB = VertexBuffer::Create(vertices, sizeof(vertices));

		BufferLayout layout = {
			{ ShaderDataType::Float3, "aPos"},
			{ ShaderDataType::Float3, "aTexCoords"},
		};

		s_3Ddata.skyVB->SetLayout(layout);
		s_3Ddata.skyVA->AddVertexBuffer(s_3Ddata.skyVB);

		uint32_t* indices = new uint32_t[36];

		uint32_t Offset = 0;
		for (uint32_t i = 0; i < 36; i += 6)
		{
			indices[i + 0] = Offset + 0;
			indices[i + 1] = Offset + 1;
			indices[i + 2] = Offset + 2;

			indices[i + 3] = Offset + 2;
			indices[i + 4] = Offset + 3;
			indices[i + 5] = Offset + 0;

			Offset += 4;
		}

		std::shared_ptr<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, 36);
		s_3Ddata.skyVA->SetIndexBuffer(indexBuffer);
		delete[] indices;

		

		std::string path[6] =
		{
			"assets/skybox/default/right.jpg",
			"assets/skybox/default/left.jpg",
			"assets/skybox/default/top.jpg",
			"assets/skybox/default/bottom.jpg",
			"assets/skybox/default/back.jpg",
			"assets/skybox/default/front.jpg",
		};

		int width, height, bpp;
		stbi_uc* data = nullptr;

		stbi_set_flip_vertically_on_load(false);
		GLenum dataFormat = 0, internalFormat = 0;

		// set the file path
		for (uint32_t i = 0; i < 6; i++)
		{
			data = stbi_load(path[i].c_str(), &height, &width, &bpp, 0);

			switch (bpp)
			{
			case 3:
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;

				break;

			case 4:
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
				break;
			}

			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
				OGN_CORE_TRACE("Texture \"{0}\" Successfully Loaded", path[i]);
			}
			else
			{
				OGN_CORE_ERROR("Failed to load Texture: {0}", path[i]);
				stbi_image_free(data);
			}
		}
		OGN_CORE_TRACE("Bits Per Pixel : {0}", bpp);
		OGN_CORE_TRACE("Internal Format: {0}, Data Format: {1}", internalFormat, dataFormat);

		// generate texture
		glGenTextures(1, &s_3Ddata.skyTextureID);
		glBindTexture(GL_TEXTURE_2D, s_3Ddata.skyTextureID);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		s_3Ddata.skyShader = Shader::Create("assets/shaders/Skybox.glsl");
		s_3Ddata.skyShader->Bind();
	}

	void Renderer3D::DrawSkybox()
	{
		glDepthFunc(GL_LESS);
		s_3Ddata.skyShader->Bind();
		s_3Ddata.skyVA->Bind();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, s_3Ddata.skyTextureID);

		glDrawElements(GL_TRIANGLES, s_3Ddata.skyVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);

		s_3Ddata.skyVA->Unbind();
		s_3Ddata.skyShader->Unbind();
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glDepthFunc(GL_LESS);
	}

	void Renderer3D::StartBatch()
	{
	}

	void Renderer3D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	
}