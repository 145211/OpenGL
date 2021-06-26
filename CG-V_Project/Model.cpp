#include "model.h"

Model::Model() : position(glm::vec3(0.f)), rotation(glm::vec3(0.f)), scaling(glm::vec3(0.f)), origin(glm::vec3(0.f))
{

}

Model::Model(const char* fileName)
{
	loadModel(fileName);
}

Model::Model(const Model& m)
{
	this->vertexArray = m.vertexArray;
	this->position = m.position;
	this->rotation = m.rotation;
	this->scaling = m.scaling;
	this->origin = m.origin;
}

void Model::loadModel(const char* fileName)
{
	vertexArray.reserve(200000);
	//Vertex portions
	std::vector<glm::fvec4> vertex_positions;
	std::vector<glm::fvec2> vertex_texcoords;
	std::vector<glm::fvec4> vertex_normals;

	vertex_positions.reserve(200000);
	vertex_texcoords.reserve(200000);
	vertex_normals.reserve(200000);

	//Face vectors
	std::vector<GLint> vertex_position_indicies;
	std::vector<GLint> vertex_texcoord_indicies;
	std::vector<GLint> vertex_normal_indicies;

	vertex_position_indicies.reserve(300000);
	vertex_texcoord_indicies.reserve(300000);
	vertex_normal_indicies.reserve(300000);

	std::stringstream ss;
	std::ifstream in_file(fileName);
	std::string line = "";
	std::string prefix = "";
	glm::vec3 temp_vec3;
	glm::vec2 temp_vec2;
	GLint temp_glint = 0;

	//File open error check
	if (!in_file.is_open())
	{
		throw "ERROR::OBJLOADER::Could not open file.";
	}

	//Read one line at a time
	while (std::getline(in_file, line))
	{
		//Get the prefix of the line
		ss.clear();
		ss.str(line);
		ss >> prefix;

		if (prefix == "#")
		{

		}
		else if (prefix == "o")
		{

		}
		else if (prefix == "s")
		{

		}
		else if (prefix == "use_mtl")
		{

		}
		else if (prefix == "v") //Vertex position
		{
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			vertex_positions.push_back(glm::vec4(temp_vec3, 1.0f));
		}
		else if (prefix == "vt")
		{
			ss >> temp_vec2.x >> temp_vec2.y;
			vertex_texcoords.push_back(temp_vec2);
		}
		else if (prefix == "vn")
		{
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			vertex_normals.push_back(glm::vec4(temp_vec3, 0.0f));
		}
		else if (prefix == "f")
		{
			int counter = 0;
			while (ss >> temp_glint)
			{
				//Pushing indices into correct arrays
				if (counter == 0)
					vertex_position_indicies.push_back(temp_glint);
				else if (counter == 1)
					vertex_texcoord_indicies.push_back(temp_glint);
				else if (counter == 2)
					vertex_normal_indicies.push_back(temp_glint);

				//Handling characters
				if (ss.peek() == '/')
				{
					++counter;
					ss.ignore(1, '/');
				}
				else if (ss.peek() == ' ')
				{
					++counter;
					ss.ignore(1, ' ');
				}

				//Reset the counter
				if (counter > 2)
					counter = 0;
			}
		}
		else
		{

		}
	}

	//Build final vertex array (mesh)
	vertexArray.resize(vertex_position_indicies.size(), Vertex());

	//Load in all indices
	for (size_t i = 0; i < vertexArray.size(); ++i)
	{
		vertexArray[i].position = vertex_positions[vertex_position_indicies[i] - 1];
		vertexArray[i].texCoord = vertex_texcoords[vertex_texcoord_indicies[i] - 1];
		vertexArray[i].normal = vertex_normals[vertex_normal_indicies[i] - 1];
		vertexArray[i].color = glm::vec4(1.f, 1.f, 1.f, 1.0f);
	}

	//DEBUG
	//std::cout << "Nr of vertices: " << vertexArray.size() << "\n";

	//Loaded success
	std::cout << "OBJ file loaded!" << "\n";
}

void Model::assimpLoadModel(std::string fileName) // do dokończenia
{
	vertexArray.reserve(200000);

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
	std::cout << importer.GetErrorString() << std::endl;

	aiMesh* mesh;

	for (unsigned int j = 0; j < (scene->mNumMeshes); j++) {

		mesh = scene->mMeshes[j];

		for (int i = 0; i < mesh->mNumVertices; i++) {
			aiVector3D vertex = mesh->mVertices[i];
			aiVector3D normal = mesh->mNormals[i];

			// liczba zdefiniowanych zestawów wsp. teksturowania (zestawów jest max 8)
			unsigned int liczba_zest = mesh->GetNumUVChannels();
			// Liczba składowych wsp. teksturowania dla 0 zestawu
			unsigned int wymiar_wsp_tex = mesh->mNumUVComponents[0];
			aiVector3D texCoord = mesh->mTextureCoords[0][i];

			vertexArray.push_back(Vertex{
				glm::vec4(vertex.x, vertex.y, vertex.z, 1),
				glm::vec4(1, 1, 1, 1),
				glm::vec2(texCoord.x, texCoord.y),
				glm::vec4(normal.x, normal.y, normal.z, 0) });
		}
	}


	// wczytywanie i liczenie faces
	//for (int i = 0; i < mesh->mNumFaces; ++i) {
	//	aiFace& face = mesh->mFaces[i];
	//	for (int j = 0; j < face.mNumIndices; j++) {
	//		indices.push_back(face.mIndices[j]);
	//		// tutaj w oryginale indices.push_back(face.mIndices[j]);
	//	}
	//}
	
	std::cout << "No. of vertices: " << vertexArray.size() << std::endl;
	std::cout << "Loaded model: " << fileName << std::endl;

	
	// liczenie materiałów
	//aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	//for (int i = 0; i < 19; ++i) {
	//	std::cout << i << " " << material->GetTextureCount((aiTextureType)i) << std::endl;
	//}

	// wczytywanie tekstur
	//for (int i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); ++i) {
	//	aiString str; // nazwa pliku
	//	aiTextureMapping mapping; // jak wygenerowano wsp tekstrurowania (optional)
	//	unsigned int uvMapping; // numer zestawu wsp. teksturowania (optional)
	//	ai_real blend; // współczynnik połączenia kolorów z kolejną teksturą (optional)
	//	aiTextureOp op; // sposób łączenia kolorów z kolejną teksturą (optional)
	//	aiTextureMapMode mapMode; // sposób adresowania tekstury (optional)
	//	material->GetTexture(aiTextureType_DIFFUSE, i, &str);
	//}
}

//void Model::assimpProcessNode(aiNode* node, const aiScene* scene)
//{
//	// process all the node's meshes (if any)
//	for (unsigned int i = 0; i < node->mNumMeshes; i++)
//	{
//		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//		meshes.push_back(processMesh(mesh, scene));
//	}
//	// then do the same for each of its children
//	for (unsigned int i = 0; i < node->mNumChildren; i++)
//	{
//		processNode(node->mChildren[i], scene);
//	}
//}

size_t Model::arraySize()
{
	return vertexArray.size();
}

size_t Model::indicesSize()
{
	return indices.size();
}

Vertex* Model::vertexData()
{
	return vertexArray.data();
}

GLuint* Model::indicesData()
{
	return indices.data();
}

void Model::setPosition(glm::vec3 position)
{
	this->position = position;
}

void Model::setRotation(glm::vec3 rotation)
{
	this->rotation = rotation;
}

void Model::setScaling(glm::vec3 scaling)
{
	this->scaling = scaling;
}

glm::vec3 Model::getPosition() const
{
	return this->position;
}

glm::vec3 Model::getRotation() const
{
	return this->rotation;
}

glm::vec3 Model::getScaling() const
{
	return this->scaling;
}

glm::vec3 Model::getOrigin() const
{
	return this->origin;
}
