//! @file ShaderProgram.cpp
/*!
 *	@date 	30.11.2012
 *	@author	Guido Schmidt
 */

#include "ShaderProgram.h"

namespace renderer
{

    namespace GLSL
    {
        std::string keywords[]  = {"atomic_uint", "attribute", "bool", "break", "bvec2", "bvec3", "bvec4","case","centroid","coherent","const","continue","default","discard","dmat2","dmat2x2","dmat2x3","dmat2x4","dmat3","dmat3x2","dmat3x3","dmat3x4","dmat4","dmat4x2","dmat4x3","dmat4x4","do","double","dvec2","dvec3","dvec4","else","flat","float","for","highp","if","iimage1D","iimage1DArray","iimage2D","iimage2DArray","iimage2DMS","iimage2DMSArray","iimage2DRect","iimage3D","iimageBuffer","iimageCube","iimageCubeArray","image1D","image1DArray","image2D","image2DArray","image2DMS","image2DMSArray","image2DRect","image3D","imageBuffer","imageCube","imageCubeArray","in","inout","int","invariant","isampler1D","isampler1DArray","isampler2D","isampler2DArray","isampler2DMS","isampler2DMSArray","isampler2DRect","isampler3D","isamplerBuffer","isamplerCube","isamplerCubeArray","ivec2","ivec3","ivec4","layout","lowp","mat2","mat2x2","mat2x3","mat2x4","mat3","mat3x2","mat3x3","mat3x4","mat4","mat4x2","mat4x3","mat4x4","mediump","noperspective","out","patch","precision","readonly","restrict","return","sample","sampler1D","sampler1DArray","sampler1DArrayShadow","sampler1DShadow","sampler2D","sampler2DArray","sampler2DArrayShadow","sampler2DMS","sampler2DMSArray","sampler2DRect","sampler2DRectShadow","sampler2DShadow","sampler3D","samplerBuffer","samplerCube","samplerCubeArray","samplerCubeArrayShadow","samplerCubeShadow","smooth","struct","subroutine","switch","uimage1D","uimage1DArray","uimage2D","uimage2DArray","uimage2DMS","uimage2DMSArray","uimage2DRect","uimage3D","uimageBuffer","uimageCube","uimageCubeArray","uint","uniform","usampler1D","usampler1DArray","usampler2D","usampler2DArray","usampler2DMS","usampler2DMSArray","usampler2DRect","usampler3D","usamplerBuffer","usamplerCube","usamplerCubeArray","uvec2","uvec3","uvec4","varying","vec2","vec3","vec4","void","volatile","while","writeonly"};
        std::string functions[] = {"gl_BackColor", "gl_BackLightModelProduct", "gl_BackLightProduct", "gl_BackMaterial", "gl_BackSecondaryColor", "gl_ClipDistance", "gl_ClipPlane", "gl_ClipVertex", "gl_Color", "gl_DepthRange", "gl_EyePlaneQ", "gl_EyePlaneR", "gl_EyePlaneS", "gl_EyePlaneT", "gl_Fog", "gl_FogCoord", "gl_FogFragCoord", "gl_FragColor", "gl_FragCoord", "gl_FragData", "gl_FragDepth", "gl_FrontColor", "gl_FrontFacing", "gl_FrontLightModelProduct", "gl_FrontLightProduct", "gl_FrontMaterial", "gl_FrontSecondaryColor", "gl_InstanceID", "gl_InvocationID", "gl_Layer", "gl_LightModel", "gl_LightSource", "gl_MaxAtomicCounterBindings", "gl_MaxAtomicCounterBufferSize", "gl_MaxClipDistances", "gl_MaxClipPlanes", "gl_MaxCombinedAtomicCounterBuffers", "gl_MaxCombinedAtomicCounters", "gl_MaxCombinedImageUniforms", "gl_MaxCombinedImageUnitsAndFragmentOutputs", "gl_MaxCombinedTextureImageUnits", "gl_MaxDrawBuffers", "gl_MaxFragmentAtomicCounterBuffers", "gl_MaxFragmentAtomicCounters", "gl_MaxFragmentImageUniforms", "gl_MaxFragmentInputComponents", "gl_MaxFragmentUniformComponents", "gl_MaxFragmentUniformVectors", "gl_MaxGeometryAtomicCounterBuffers", "gl_MaxGeometryAtomicCounters", "gl_MaxGeometryImageUniforms", "gl_MaxGeometryInputComponents", "gl_MaxGeometryOutputComponents", "gl_MaxGeometryOutputVertices", "gl_MaxGeometryTextureImageUnits", "gl_MaxGeometryTotalOutputComponents", "gl_MaxGeometryUniformComponents", "gl_MaxGeometryVaryingComponents", "gl_MaxImageSamples", "gl_MaxImageUnits", "gl_MaxLights", "gl_MaxPatchVertices", "gl_MaxProgramTexelOffset", "gl_MaxTessControlAtomicCounterBuffers", "gl_MaxTessControlAtomicCounters", "gl_MaxTessControlImageUniforms", "gl_MaxTessControlInputComponents", "gl_MaxTessControlOutputComponents", "gl_MaxTessControlTextureImageUnits", "gl_MaxTessControlTotalOutputComponents", "gl_MaxTessControlUniformComponents", "gl_MaxTessEvaluationAtomicCounterBuffers", "gl_MaxTessEvaluationAtomicCounters", "gl_MaxTessEvaluationImageUniforms", "gl_MaxTessEvaluationInputComponents", "gl_MaxTessEvaluationOutputComponents", "gl_MaxTessEvaluationTextureImageUnits", "gl_MaxTessEvaluationUniformComponents", "gl_MaxTessGenLevel", "gl_MaxTessPatchComponents", "gl_MaxTextureCoords", "gl_MaxTextureImageUnits", "gl_MaxTextureUnits", "gl_MaxVaryingComponents", "gl_MaxVaryingFloats", "gl_MaxVaryingVectors", "gl_MaxVertexAtomicCounterBuffers", "gl_MaxVertexAtomicCounters", "gl_MaxVertexAttribs", "gl_MaxVertexImageUniforms", "gl_MaxVertexOutputComponents", "gl_MaxVertexTextureImageUnits", "gl_MaxVertexUniformComponents", "gl_MaxVertexUniformVectors", "gl_MaxViewports", "gl_MinProgramTexelOffsegl_ModelViewMatrix", "gl_ModelViewMatrixInverse", "gl_ModelViewMatrixInverseTranspose", "gl_ModelViewMatrixTranspose", "gl_ModelViewProjectionMatrix", "gl_ModelViewProjectionMatrixInverse", "gl_ModelViewProjectionMatrixInverseTranspose", "gl_ModelViewProjectionMatrixTranspose", "gl_MultiTexCoord0", "gl_MultiTexCoord1", "gl_MultiTexCoord2", "gl_MultiTexCoord3", "gl_MultiTexCoord4", "gl_MultiTexCoord5", "gl_MultiTexCoord6", "gl_MultiTexCoord7", "gl_Normal", "gl_NormalMatrix", "gl_NormalScale", "gl_ObjectPlaneQ", "gl_ObjectPlaneR", "gl_ObjectPlaneS", "gl_ObjectPlaneT", "gl_PatchVerticesIn", "gl_PerVertex", "gl_Point", "gl_PointCoord", "gl_PointSize", "gl_Position", "gl_PrimitiveID", "gl_PrimitiveIDIn", "gl_ProjectionMatrix", "gl_ProjectionMatrixInverse", "gl_ProjectionMatrixInverseTranspose", "gl_ProjectionMatrixTranspose", "gl_SampleID", "gl_SampleMask", "gl_SampleMaskIn", "gl_SamplePosition", "gl_SecondaryColor", "gl_TessCoord", "gl_TessLevelInner", "gl_TessLevelOuter", "gl_TexCoord", "gl_TextureEnvColor", "gl_TextureMatrixInverseTranspose", "gl_TextureMatrixTranspose", "gl_Vertex", "gl_VertexID", "gl_ViewportIndex", "gl_in", "gl_out", "EmitStreamVertex", "EmitVertex", "EndPrimitive", "EndStreamPrimitive", "abs", "acos", "acosh", "all", "any", "asin", "asinh", "atan", "atanh", "atomicCounter", "atomicCounterDecrement", "atomicCounterIncrement", "barrier", "bitCount", "bitfieldExtract", "bitfieldInsert", "bitfieldReverse", "ceil", "clamp", "cos", "cosh", "cross", "dFdx", "dFdy", "degrees", "determinant", "distance", "dot", "equal", "exp", "exp2", "faceforward", "findLSB", "findMSB", "floatBitsToInt", "floatBitsToUint", "floor", "fma", "fract", "frexp", "ftransform", "fwidth", "greaterThan", "greaterThanEqual", "imageAtomicAdd", "imageAtomicAnd", "imageAtomicCompSwap", "imageAtomicExchange", "imageAtomicMax", "imageAtomicMin", "imageAtomicOr", "imageAtomicXor", "imageLoad", "imageStore", "imulExtended", "intBitsToFloat", "interpolateAtCentroid", "interpolateAtOffset", "interpolateAtSample", "inverse", "inversesqrt", "isinf", "isnan", "ldexp", "length", "lessThan", "lessThanEqual", "log", "log2", "matrixCompMult", "max", "memoryBarrier", "min", "mix", "mod", "modf", "noise1", "noise2", "noise3", "noise4", "normalize", "not", "notEqual", "outerProduct", "packDouble2x32", "packHalf2x16", "packSnorm2x16", "packSnorm4x8", "packUnorm2x16", "packUnorm4x8", "pow", "radians", "reflect", "refract", "round", "roundEven", "shadow1D", "shadow1DLod", "shadow1DProj", "shadow1DProjLod", "shadow2D", "shadow2DLod", "shadow2DProj", "shadow2DProjLod", "sign", "sin", "sinh", "smoothstep", "sqrt", "step", "tan", "tanh", "texelFetch", "texelFetchOffset", "texture", "texture1D", "texture1DLod", "texture1DProj", "texture1DProjLod", "texture2D", "texture2DLod", "texture2DProj", "texture2DProjLod", "texture3D", "texture3DLod", "texture3DProj", "texture3DProjLod", "textureCube", "textureCubeLod", "textureGather", "textureGatherOffset", "textureGatherOffsets", "textureGrad", "textureGradOffset", "textureLod", "textureLodOffset", "textureOffset", "textureProj", "textureProjGrad", "textureProjGradOffset", "textureProjLod", "textureProjLodOffset", "textureProjOffset", "textureQueryLod", "textureSize", "transpose", "trunc", "uaddCarry", "uintBitsToFloat", "umulExtended", "unpackDouble2x32", "unpackHalf2x16", "unpackSnorm2x16", "unpackSnorm4x8", "unpackUnorm2x16", "unpackUnorm4x8", "usubBorrow", "gl_TextureMatrix", "gl_TextureMatrixInverse"};


        std::vector<std::string> split(const char *str, char c = ' ')
        {
            std::vector<std::string> result;

            do
            {
                const char *begin = str;

                while(*str != c && *str)
                    str++;

                result.push_back(std::string(begin, str));
            } while (0 != *str++);

            return result;
        }

        bool has_any_digits(const std::string& s)
        {
            return std::any_of(s.begin(), s.end(), ::isdigit);
        }

		std::string ReadShaderSourceFormatted(std::string filename, unsigned int &lineCount)
        {
			std::string shaderSrc;
            std::string line;
            std::string lineFormatted;
            std::string formatted;
            unsigned int lineNumber = 0;

            std::ifstream fileIn(filename.c_str());
            if(filename.length() != 0 && fileIn.is_open()){
                while(!fileIn.eof()){;
                    std::string startTag = "<div>";
                    std::string endTag   = "</div>";

                    getline(fileIn, line);

                    std::vector<std::string> tokens = split(line.c_str());
                    lineFormatted = "";
                    std::string whiteSpace = "";
                    for(unsigned int i=0; i < tokens.size(); i++)
                    {
                        std::string::size_type p = 0;

                        while (p < tokens[i].size())
                        {
                          // If character at p is space erase it, otherwise go to next
                          if (isspace(tokens[i][p]))
                          {
                            tokens[i].erase(p, 1);
                            whiteSpace += "<span class='whitespace'> </span>";
                          }
                          else
                            ++p;
                        }

                        for(auto string : keywords)
                        {
                            std::size_t found = string.find(tokens[i]);
                            if(found != std::string::npos)
                            {
                                tokens[i] = "<span class='keyword'>" + tokens[i] + " </span>";
                            }
                        }

                        for(auto string : functions)
                        {
                            if(string == tokens[i])
                            {
                                tokens[i] = "<span class='function'>" + tokens[i] + " </span>";
                            }
                        }
                        if(has_any_digits(tokens[i]))
                        {
                            tokens[i] = "<span class='number'>" + tokens[i] + " </span>";
                        }

                        lineFormatted += tokens[i] + " ";
                    }


                    if(lineFormatted[0] == '/' && lineFormatted[1] == '/')
                    {
                        startTag = "<div class='comment'>";
                    }
                    if(lineFormatted[0] == '#')
                    {
                        startTag = "<div class='version'>";
                    }

                    lineFormatted += endTag + "\n";
                    formatted = startTag + whiteSpace + lineFormatted;

                    shaderSrc += formatted;

                    lineNumber++;
                }
                fileIn.close();
            }
            else
            {
                std::cout << "ERROR (ShaderProgram): Unable to read shader source code from '";
                std::cout << filename;
                std::cout << "'\n";
            }

			lineCount = lineNumber;
            return shaderSrc;
        }

        std::string ReadShaderSource(std::string filename)
        {
            std::string shaderSrc;
            std::string line;

            std::ifstream fileIn(filename.c_str());
            if(fileIn.is_open()){
                while(!fileIn.eof()){
                    getline(fileIn, line);
                    line += "\n";
                    shaderSrc += line;
                }
                fileIn.close();
            }
            else
            {
                std::cout << "ERROR (ShaderProgram): Unable to read shader source code from '";
                std::cout << filename;
                std::cout << "'\n";
            }
            return shaderSrc;
        }

        void PrintShaderInfoLog(GLuint shader)
        {
            GLint infoLogLength = 0;
            GLsizei charsWritten = 0;

            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
            char* log = (char*)malloc(infoLogLength);
            glGetShaderInfoLog(shader, infoLogLength, &charsWritten, log);
            std::cout << "> shader: ";
            std::cout << shader;
            std::cout << "\n";
            if(sizeof(log) == 4)
                std::cout << "No errors.";
            std::cout << log;
            std::cout << "\n\n";
            free(log);
        }

        void PrintShaderProgramInfoLog(GLuint shaderProgram)
        {
            GLint logLength;
            glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength);
            if(logLength > 0){
                char* log = (char*) malloc(logLength);
                GLsizei written;
                glGetProgramInfoLog(shaderProgram, logLength, &written, log);
                std::cout << "\n-- SHADER PROGRAMM LOG --------------------------------------------------\n";
                std::cout << "Shader program: ";
                std::cout << shaderProgram;
                std::cout << "\n";
                std::cout << log;
                std::cout << "\n";
                free(log);
            }
        }
    }

    ShaderProgram::ShaderProgram()
    {
        m_shaderProgram_ID = glCreateProgram();
        m_islinked = false;
        m_errorCheckValue = glGetError();
        m_activeAttributesWritten = false;
        m_activeUniformsWritten = false;
    }

    ShaderProgram::ShaderProgram(GLSL::GLSLShaderType shaderType0, std::string filename0, GLSL::GLSLShaderType shaderType1, std::string filename1)
    {
        m_shaderProgram_ID = glCreateProgram();
        m_activeAttributesWritten = false;
        m_activeUniformsWritten = false;

        m_shader_sources[shaderType0] = filename0;
        m_shader_sources[shaderType1] = filename1;

        std::cout << "\n-- SHADER PROGRAMM LOG --------------------------------------------------\n";


        addShader(shaderType0, m_shader_sources[shaderType0]);
        addShader(shaderType1, m_shader_sources[shaderType1]);
        glAttachShader(m_shaderProgram_ID, m_shader_IDs[GLSL::VERTEX]);
        glAttachShader(m_shaderProgram_ID, m_shader_IDs[GLSL::FRAGMENT]);
        link();

        std::cout << "\n";
    }

    ShaderProgram::~ShaderProgram()
    {

    }

    void ShaderProgram::addShader(GLSL::GLSLShaderType shaderType, std::string filename)
    {
        GLint Shader_ID;

        switch (shaderType) {
            case 0:
                Shader_ID = glCreateShader(GL_VERTEX_SHADER);
                break;
            case 1:
                Shader_ID = glCreateShader(GL_FRAGMENT_SHADER);
                break;
            case 2:
                Shader_ID = glCreateShader(GL_GEOMETRY_SHADER);
                break;
            case 3:
                Shader_ID = glCreateShader(GL_TESS_CONTROL_SHADER);
                break;
            case 4:
                Shader_ID = glCreateShader(GL_TESS_EVALUATION_SHADER);
                break;
        }

        std::string shaderSource = GLSL::ReadShaderSource(filename);
        const char* shaderSourcePointer = shaderSource.c_str();
        glShaderSource(Shader_ID, 1, &shaderSourcePointer, NULL);
        glCompileShader(Shader_ID);
        GLSL::PrintShaderInfoLog(Shader_ID);

        m_shader_IDs[shaderType] = Shader_ID;
    }

    void ShaderProgram::reloadAllShaders(void)
    {
       ReloadShader(GLSL::VERTEX);
       ReloadShader(GLSL::FRAGMENT);
       ReloadShader(GLSL::GEOMETRY);
       ReloadShader(GLSL::TESS_CONTROL);
       ReloadShader(GLSL::TESS_EVALUATION);
    }

    void ShaderProgram::ReloadShader(GLSL::GLSLShaderType shaderType)
    {
        std::string shaderSource = GLSL::ReadShaderSource(m_shader_sources[shaderType]);
        const char* shaderSourcePointer = shaderSource.c_str();
        glShaderSource(m_shader_IDs[shaderType], 1, &shaderSourcePointer, NULL);
        glCompileShader(m_shader_IDs[shaderType]);
        GLSL::PrintShaderInfoLog(m_shader_IDs[shaderType]);
        link();
    }

    void ShaderProgram::link(void)
    {
        glLinkProgram(m_shaderProgram_ID);

        GLint linkStatus;
        glGetProgramiv(m_shaderProgram_ID, GL_LINK_STATUS, &linkStatus);
        if(linkStatus == GL_FALSE){
            std::cout << "ERROR (ShaderProgram): Shader program linking failed!" << std::endl;
            //!GLSL::PrintShaderProgramInfoLog(m_shaderProgram_ID);
            m_islinked = false;
        }
        else{
            std::cout << "Successfully linked shader program." << std::endl;
            printActiveUniforms();
            m_islinked = true;
        }
    }

    bool ShaderProgram::isLinked(void)
    {
        return m_islinked;
    }

    void ShaderProgram::use(void)
    {
        glUseProgram(m_shaderProgram_ID);
    }

    void ShaderProgram::unuse(void){
        glUseProgram(0);
    }

    GLuint ShaderProgram::getUniform(std::string uniform_name)
    {
        GLuint location = glGetUniformLocation(m_shaderProgram_ID, uniform_name.c_str());
        return location;
    }

    void ShaderProgram::setUniform(GLuint uniform_location, const glm::mat4 &mat)
    {
        glUniformMatrix4fv(uniform_location, 1, GL_FALSE, &mat[0][0]);
    }

    void ShaderProgram::setUniform(GLuint uniform_location, const glm::vec3 &vec)
    {
        glUniform3f(uniform_location, vec[0], vec[1], vec[2]);
    }

    void ShaderProgram::setUniform(GLuint uniform_location, const glm::vec2 &vec)
    {
        glUniform2f(uniform_location, vec[0], vec[1]);
    }

    void ShaderProgram::setUniformArray3f(GLuint uniform_location, int count, GLfloat* value_ptr)
    {
        glUniform3fv(uniform_location, count, value_ptr);
    }

    void ShaderProgram::setUniformArrayMatrix4f(GLuint uniform_location, int count, GLfloat* value_ptr)
    {
        glUniformMatrix4fv(uniform_location, count, false, value_ptr);
    }

    void ShaderProgram::setUniform(GLuint uniform_location, const glm::vec4 &vec)
    {
        glUniform4f(uniform_location, vec[0], vec[1], vec[2], vec[3]);
    }

    void ShaderProgram::setUniform(GLuint uniform_location, int val)
    {
        glUniform1i(uniform_location, val);
    }

    void ShaderProgram::setUniform(GLuint uniform_location, float val)
    {
        glUniform1f(uniform_location, val);
    }

    void ShaderProgram::setUniformSampler(GLuint uniform_location, GLuint texture, GLint textureUnit)
    {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(uniform_location, textureUnit);
    }

    void ShaderProgram::setUniformCubemap(GLuint uniform_location, GLuint texture, GLint textureUnit)
    {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
        glUniform1i(uniform_location, textureUnit);
    }

    void ShaderProgram::bindAttributeLocation(GLuint location, std::string name)
    {
        glBindAttribLocation(m_shaderProgram_ID, location, name.c_str());
    }

    void ShaderProgram::printActiveAttributes(void)
    {
        if(!m_activeAttributesWritten){
            GLint maxLength, nAttributes;
            glGetProgramiv(m_shaderProgram_ID, GL_ACTIVE_ATTRIBUTES, &nAttributes);
            glGetProgramiv(m_shaderProgram_ID, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);

            GLchar* name = (GLchar*) malloc(maxLength);
            GLint written, size, location;
            GLenum type;

            std::cout << "-----------------------------------------------------" << std::endl;
            std::cout << "Attribute Index | Name" << std::endl;
            for(int i=0; i < nAttributes; i++){
                glGetActiveAttrib(m_shaderProgram_ID, i, maxLength, &written, &size, &type, name);
                location = glGetAttribLocation(m_shaderProgram_ID, name);
                std::cout << location << " | " << name << std::endl;
            }
            free(name);
            m_activeAttributesWritten = !m_activeAttributesWritten;
        }
    }

    void ShaderProgram::printActiveUniforms(void)
    {
        if(!m_activeUniformsWritten)
        {
            GLint maxLength, nUniforms;
            glGetProgramiv(m_shaderProgram_ID, GL_ACTIVE_UNIFORMS, &nUniforms);
            glGetProgramiv(m_shaderProgram_ID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength);

            GLchar* name = (GLchar*) malloc(maxLength);
            GLint size, location;
            GLsizei written;
            GLenum type;
            std::cout << "-----------------------------------------------------" << std::endl;
            std::cout << "Uniform Index | Name" << std::endl;
            for(int i=0; i < nUniforms; i++){
                glGetActiveUniform(m_shaderProgram_ID, i, maxLength, &written, &size, &type, name);
                location = glGetUniformLocation(m_shaderProgram_ID, name);
                std::cout << "unfiform " << location << " | " << name << std::endl;
            }
            free(name);
            m_activeUniformsWritten = !m_activeUniformsWritten;
        }
    }

    GLint ShaderProgram::getHandle(void)
    {
        return m_shaderProgram_ID;
    }

	std::string ShaderProgram::getShaderCodeOf(GLSL::GLSLShaderType shaderType, unsigned int &lineCount)
    {
		std::string shaderSource;
		if (!m_shader_sources[shaderType].empty())
			shaderSource = GLSL::ReadShaderSourceFormatted(m_shader_sources[shaderType], lineCount);
		else
			shaderSource = "No code for this shader!";
		return shaderSource;
    }
}
