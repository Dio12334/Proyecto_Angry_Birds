#include "../include/Mesh.h"
#include "../include/Renderer.h"
#include "../include/Texture.h"
#include "../include/VertexArray.h"
#include <cmath>
#include <fstream>
#include <rapidjson/rapidjson.h>
#include <sstream>
#include <rapidjson/document.h>
#include <SDL2/SDL_log.h>

Mesh::Mesh():
	_box(Vector3::Infinity,  Vector3::NegInfinity),
	_vertexArray(nullptr),
	_radius(0.0f),
	_specPower(100.0f)
{

}

Mesh::~Mesh(){

}

bool Mesh::load(const std::string &fileName, class Renderer *renderer){
	std::ifstream file(fileName);
	if(!file.is_open()){
		SDL_Log("File not found: Mesh %s", fileName.c_str());
		return false;
	}
	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if(!doc.IsObject()){
		SDL_Log("Mesh %s is not valid json", fileName.c_str());
		return false;
	}

	int ver = doc["version"].GetInt();

	if(ver != 1){
		SDL_Log("Mesh %s not version 1", fileName.c_str());
		return false;
	}

	_shaderName = doc["shader"].GetString();

	size_t vertSize = 8;

	const rapidjson::Value& textures = doc["textures"];
	if(!textures.IsArray() || textures.Size() < 1){
		SDL_Log("Mesh %s has no textures, there should be at least one", fileName.c_str());
		return false;
	}

	_specPower = static_cast<float>(doc["specularPower"].GetDouble());

	for(rapidjson::SizeType i = 0; i < textures.Size(); ++i){
		std::string texName = textures[i].GetString();
		Texture* t = renderer->getTexture(texName);
		if(t == nullptr){
			t = renderer->getTexture(texName);
			if(t == nullptr){
				t = renderer->getTexture("Assets/Default.png");
			}
		}
		_textures.emplace_back(t);
	}

	const rapidjson::Value& vertsJson = doc["vertices"];
	if(!vertsJson.IsArray() || vertsJson.Size() < 1){
		SDL_Log("Mesh %s has no vertices", fileName.c_str());
		return false;
	}

	std::vector<float> vertices;
	vertices.reserve(vertsJson.Size() * vertSize);
	_radius = 0.0f;
	for(rapidjson::SizeType i = 0; i < vertsJson.Size(); ++i){
		const rapidjson::Value& vert = vertsJson[i];
		if(!vert.IsArray() || vert.Size() != 8){
			SDL_Log("Unexpected vertex format for %s", fileName.c_str());
			return false;
		}
		Vector3 pos(vert[0].GetDouble(), vert[1].GetDouble(), vert[2].GetDouble());
		_radius = std::max(_radius, pos.LengthSq());
		_box.updateMinMax(pos);

		for(rapidjson::SizeType i = 0; i < vert.Size(); ++i){
			vertices.emplace_back(static_cast<float>(vert[i].GetDouble()));
		}
	}

	_radius = std::sqrt(_radius);
	const rapidjson::Value& indJson = doc["indices"];
	if(!indJson.IsArray() || indJson.Size() < 1){
		SDL_Log("Mesh %s has no indices", fileName.c_str());
		return false;
	}

	std::vector<unsigned int> indices;
	indices.reserve(indJson.Size() * 3);
	for(rapidjson::SizeType i = 0; i < indJson.Size(); ++i){
		const rapidjson::Value& ind = indJson[i];
		if(!ind.IsArray() || ind.Size() != 3){
			SDL_Log("Invalid indices for %s", fileName.c_str());
			return false;
		}

		indices.emplace_back(ind[0].GetUint());
		indices.emplace_back(ind[1].GetUint());
		indices.emplace_back(ind[2].GetUint());
	}
	_vertexArray = new VertexArray(vertices.data(), static_cast<unsigned>(vertices.size())/vertSize, 
			indices.data(), static_cast<unsigned>(indices.size()));
	return true;
}

void Mesh::unload(){
	delete _vertexArray;
	_vertexArray = nullptr;
}

Texture* Mesh::getTexture(size_t index){
	if(index < _textures.size()){
		return _textures[index];
	}
	else
		return nullptr;
}
