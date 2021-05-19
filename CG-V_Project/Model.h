#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <glm/glm.hpp>


class Model {
private:
	std::vector<float> vertices;
	std::vector<float> texCoords;
	std::vector<float> normals;
	std::vector<float> faces;
public:
	Model();
	Model(const char* fileName);
	~Model();

	void loadModel(const char* fileName);

	float* getVerticesArray();
	float* getTexCoordsArray();
	float* getNormalsArray();
	float* getFacesArray();

	size_t getVerticesSize();
	size_t getTexCoordsSize();
	size_t getNormalsSize();
	size_t getFacesSize();

	unsigned int getVertexCount();
};


#endif