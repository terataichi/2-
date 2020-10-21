#include "TileMap.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "../_debug/_DebugConOut.h"

// rappidXml
#include "../TileMap/rapidxml.hpp"
#include "../TileMap/rapidxml_utils.hpp"

bool TileMap::LoadTmx(std::string fileName)
{
    std::string version = TMX_VERSION;                  // 現在のファイルのVersionを定義して格納しておく
    rapidxml::xml_document<> doc;                       // 
    rapidxml::file<>file(fileName.c_str());

    doc.parse<0>(file.data());


    rapidxml::xml_node<>* root_node;
    root_node = doc.first_node("map");

    for (rapidxml::xml_attribute<>* data = root_node->first_attribute();
        data != nullptr;
        data = data->next_attribute())
    {
        std::cout << data->name() << "[" << data->value() << "]" << std::endl;
    }

    if (root_node->first_attribute("compression") > 0) {
        std::cout << root_node->first_attribute("compression")->value();
        //std::cout << "Layer Compression: " << layer.data.compression << std::endl;
    }
    //std::cout << root_node->first_attribute("compression")->value()<< std::endl;

    root_node->first_attribute();

	return true;
}

TileMap::TileMap()
{


}

TileMap::~TileMap()
{
}
