#include "TileMap.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "../_debug/_DebugConOut.h"

// rappidXml
#include "../TileMap/rapidxml.hpp"
#include "../TileMap/rapidxml_utils.hpp"

bool MapLoader::
TileMap::LoadTmx(std::string fileName)
{
    std::string version = TMX_VERSION;                  // 現在のファイルのVersionを定義して格納しておく
    rapidxml::xml_document<> doc;                       // 
    rapidxml::file<>file(fileName.c_str());

    doc.parse<0>(file.data());


    rapidxml::xml_node<>* root_node;
    root_node = doc.first_node("map");

    //  各情報の取り出し
    for (rapidxml::xml_attribute<>* data = root_node->first_attribute();
        data != nullptr;
        data = data->next_attribute())
    {
        std::cout << data->name() << "[" << data->value() << "]" << std::endl;
    }


    //  各レイヤー情報の取り出し
    for (rapidxml::xml_node<>* layer_node = root_node->first_node("layer");
        layer_node != nullptr;
        layer_node = layer_node->next_sibling("layer"))
    {
        MapLoader::LayerData layer;

        // 基本情報の格納
        layer.name = layer_node->first_attribute("name")->value();
        layer.id = atoi(layer_node->first_attribute("id")->value());
        layer.width = atoi(layer_node->first_attribute("width")->value());
        layer.heigth = atoi(layer_node->first_attribute("height")->value());

        std::cout << "レイヤー名：" <<layer.name << std::endl;
        std::cout << "番号："<<layer.id << std::endl;
        std::cout << "縦のチップ数："<<layer.heigth << std::endl;
        std::cout << "横のチップ数：" <<layer.width << std::endl;

        // チップデータの格納
        rapidxml::xml_node<>* data_node = layer_node->first_node("data");
        std::string data = data_node->value();
        std::istringstream iss{ data };
        std::string num;

        layer.chipData.resize(layer.width * layer.heigth);
        for (int i = 0; i < layer.width * layer.heigth; i++)
        {
            getline(iss, num, ',');
            layer.chipData.push_back(atoi(num.c_str()));
            std::cout << num;
        }
    }

    root_node->first_attribute();

	return true;
}

LayerMap MapLoader::TileMap::GetLayerData(void)
{
    return layer_;
}

MapLoader::MapData MapLoader::TileMap::GetMapData(void)
{
    return mapData_;
}

MapLoader::
TileMap::TileMap()
{


}

MapLoader::
TileMap::~TileMap()
{
}
