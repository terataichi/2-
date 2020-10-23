#include "TmxLoader.h"
#include <iostream>
#include <sstream>

TmxLoader::TmxLoader()
{
    version_ = TMX_VERSION;
}

TmxLoader::~TmxLoader()
{
}

bool TmxLoader::LoadTmx(std::string fileName)
{
    rapidxml::file<> file(fileName.c_str());
    doc.parse<0>(file.data());

    rapidxml::xml_node<>* root_node;
    root_node = doc.first_node("map");

    // バージョンチェック
    if (root_node->first_attribute("version")->value() != version_) {
        std::cout << "バージョンが違います" << std::endl;
        return false;
    }

    tmxFileName_ = fileName;
    //  ﾏｯﾌﾟの中身を保存
    for (rapidxml::xml_attribute<>* data = root_node->first_attribute();
        data != nullptr;
        data = data->next_attribute())
    {
        std::cout << data->name() << "[" << data->value() << "]" << std::endl;
    }
    //  欲しいマップ情報の格納
    mapData_.width = atoi(root_node->first_attribute("width")->value());
    mapData_.height = atoi(root_node->first_attribute("height")->value());
    mapData_.tileWidth = atoi(root_node->first_attribute("tilewidth")->value());
    mapData_.tileHeight = atoi(root_node->first_attribute("tileheight")->value());



    //  各レイヤー情報の取り出し
    for (rapidxml::xml_node<>* layer_node = root_node->first_node("layer");
        layer_node != nullptr;
        layer_node = layer_node->next_sibling("layer"))
    {
        LayerData layer;

        // 基本情報の格納
        layer.name = layer_node->first_attribute("name")->value();
        layer.id = atoi(layer_node->first_attribute("id")->value());
        layer.width = atoi(layer_node->first_attribute("width")->value());
        layer.heigth = atoi(layer_node->first_attribute("height")->value());


        std::cout << "レイヤー名：" << layer.name << std::endl;
        std::cout << "番号：" << layer.id << std::endl;
        std::cout << "縦のチップ数：" << layer.heigth << std::endl;
        std::cout << "横のチップ数：" << layer.width << std::endl;

        // チップデータの格納
        rapidxml::xml_node<>* data_node = layer_node->first_node("data");
        std::string data = data_node->value();

        std::istringstream iss{ data };
        std::string num;

        layer.chipData.resize(layer.width * layer.heigth + 1);
        //for (int i = 0; i <= layer.width * layer.heigth; i++)
        for (auto& data : layer.chipData)
        {
            getline(iss, num, ',');
            data = atoi(num.c_str());
            std::cout << num;
        }

        // 情報の追加
        layerData_.emplace_back(layer);
    }


    //  Tsxファイル名を保存
    rapidxml::xml_node<>* tileset_node = root_node->first_node("tileset");
    tsxFileName_ = tileset_node->first_attribute("source")->value();

    // Tsxファイルの読み込み
    std::string pass = fileName.substr(0, fileName.find_last_of("/") + 1);
    LoadTsx(pass + tsxFileName_);


    return true;
}

bool TmxLoader::LoadTsx(std::string fileName)
{
    std::string tsxFile = fileName;
    rapidxml::file<> file(tsxFile.c_str());
    doc.parse<0>(file.data());

    rapidxml::xml_node<>* root_node = doc.first_node("tileset");

    // バージョンチェック
    if (root_node->first_attribute("version")->value() != version_) {
        std::cout << "バージョンが違います" << std::endl;
        return false;
    }

    // イメージ情報の格納
    rapidxml::xml_node<>* tileset_node = root_node->first_node("image");
    // ファイルがどの階層にいるかわからないのでちゃんと探してから格納する
    std::string str = tileset_node->first_attribute("source")->value();
    ImageName_ = str.substr(str.find_first_of("I"));

    return true;
}

LayerVec TmxLoader::GetLayerData(void)
{
    return layerData_;
}

MapData TmxLoader::GetMapData(void)
{
    return mapData_;
}

std::string TmxLoader::GetImageName(void)
{
	return ImageName_;
}

std::string TmxLoader::GetTmxFileName(void)
{
    return tmxFileName_;
}
