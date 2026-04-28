#pragma once

#include <filesystem>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Utils.hpp"
#include "stb_image.h"

/**
 * @brief The type of shader
 */
enum class ShaderType {
	Texture,   ///< Is a texture
	Unicolor,  ///< Has a unique color all over the mesh
	Custom,	   ///< Has a custom shader
	Light	   ///< Similar to Unicolor
};

/**
 * @brief The type of a texture
 */
enum class TextureType {
	Diffuse,  ///< Represents the diffuse
	Specular  ///< Represents the specular
};

/**
 * @brief Associates an OpenGL Id to a texture type
 */
struct LoadedTextures
{
	unsigned int oglID;	 ///< OpenGL id
	TextureType type;	 ///< Type
};

/**
 * @brief Returns the ID of the texture
 * @param[in] texturePath The path on the disk to the texture
 * @param[in] type The type of the choosen texture
 * @return The ID of the texture
 */
int TextureFromFile(const std::filesystem::path& texturePath, TextureType type);

/**
 * @brief Cast a TextureType to a string
 * @param[in] t The Texture type
 * @return The corresponding string
 */
std::string to_string(TextureType t);

/**
 * @brief The shader, ie. the visual that will be applied onto a mesh
 */
class Shader
{
   public:
	/**
	 * @brief Default constructor
	 */
	Shader(void);

	/**
	 * @brief Constructor
	 * @param[in] shaderType The type of shader wanted
	 * @param[in] isFont Indicates if the shader matches a font
	 */
	Shader(ShaderType shaderType, bool isFont = false);

	/**
	 * @brief Destructor
	 */
	~Shader();

	/**
	 * @brief Set the shader has being of unicolor type
	 * @param[in] color The color that will be shown by the shader
	 */
	void setColor(glm::vec3 color);

	/**
	 * @brief Set the shader has being of unicolor type
	 * @param[in] color The color that will be shown by the shader as a string
	 * @note Example "#FFA155"
	 */
	void setColor(std::string color);

	/**
	 * @brief Set the shader as a light
	 */
	void setLight(void);

	/**
	 * @brief Adds a texture to the shader, therefore making it of texture type
	 * @param[in] texturePath The path to the texture on the disl
	 */
	void addTexture(const std::filesystem::path& texturePath);

	/**
	 * @brief Sets the shader custom
	 * @param[in] vtxShdPath The custom vertex shader path
	 * @param[in] frgShdPath The custom fragment shader path
	 */
	void setCustom(const std::filesystem::path& vtxShdPath, const std::filesystem::path& frgShdPath);

	/**
	 * @brief Apply the given transformation to the shader
	 * @param[in] model The model matrix, corresponds to the world displacement
	 * @param[in] view The view matrix, corresponds to the camera's point of view
	 * @param[in] proejction The projection matrix, corresponds to the screen displacement
	 */
	void setTransformation(glm::mat4 model, glm::mat4 view, glm::mat4 projection) const;

	/**
	 * @brief Returns the id of an uniform specified by a string
	 * @param[in] uniform The string representing the wanted uniform
	 * @return The id
	 */
	int getUniform(const std::string& uniform) const;

	/**
	 * @brief Gets the shader's type
	 * @return The shader type
	 */
	ShaderType getType(void) const;

	/**
	 * @brief Compile the shader with the shader files set with the setFunctions
	 */
	void compile(void);

	/**
	 * @brief Binds the shader to the current mesh
	 */
	void use(void) const;

   private:
	/**
	 * @brief Groups up all the data describing a texture
	 */
	struct TextureInfo
	{
		unsigned char* data;  ///< The raw texture data
		int width;			  ///< The width
		int height;			  ///< The height
		int nrChannels;		  ///< The number of channels for one pixel

		unsigned int buffer;		 ///< The buffer id associated
		std::filesystem::path path;	 ///< The path of the texture on the disk
	};

	std::vector<TextureInfo> m_texturesInfo;  ///< The list of textures info for this shader
	bool m_font;							  ///< Indicates if the shader is used to display font

	glm::vec3 m_color;	///< The color used by the unicolor type

	ShaderType m_shaderType;  ///< The type of the shader

	std::filesystem::path m_vtxShdPath;								   ///< The path to the vertex shader
	std::filesystem::path m_frgShdPath;								   ///< The path to the fragment shader
	std::map<std::filesystem::path, LoadedTextures> m_loadedTextures;  ///< The list of all loaded textures of that shader

	unsigned int m_shdPrgId;  /// OpenGL, the shader program ID
	std::string m_name;		  ///< The name of the shader, used for debugging mostly

	/**
	 * @brief Gets the texture data from the given path
	 * @param[in] path The path to the texture to extract
	 * @return The content of the texture
	 */
	std::string _getFileContent(const std::filesystem::path& path);

	/**
	 * @brief Sets the shader with the correct shader files
	 * @param[in] vtxShdPath The vertex shader that corresponds to the wanted shader type
	 * @param[in] frgShdPath The fragment shader that corresponds to the wanted shader type
	 */
	void _setShaders(const std::filesystem::path& vtxShdPath, const std::filesystem::path& frgShdPath);

	/**
	 * @brief Delete the shaders associated to the vertex and fragment shader
	 * @param[in] vtx The vertex shader to delete
	 * @param[in] frg The fragment shader to delete
	 */
	void _deleteShaders(unsigned int vtx, unsigned frg);
};