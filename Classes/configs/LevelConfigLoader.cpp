#include "LevelConfigLoader.h"
#include "json/rapidjson.h"
#include "json/document.h"

USING_NS_CC;
using namespace RAPIDJSON_NAMESPACE;

LevelConfig* LevelConfigLoader::loadLevelConfig(int levelId, const std::string& configPath)
{
    // 1. 重置错误日志
    _errorLog.clear();

    // 2. 构造配置文件路径（如 "configs/levels/level_1.json"）
    std::string fileName = StringUtils::format("level_%d.json", levelId);
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(configPath + fileName);

    // 3. 检查文件是否存在
    if (!FileUtils::getInstance()->isFileExist(fullPath))
    {
        _errorLog = StringUtils::format("配置文件不存在：%s", fullPath.c_str());
        CCLOGERROR("[LevelConfigLoader] %s", _errorLog.c_str());
        return nullptr;
    }

    // 4. 读取文件内容
    std::string fileContent = FileUtils::getInstance()->getStringFromFile(fullPath);
    if (fileContent.empty())
    {
        _errorLog = StringUtils::format("配置文件为空：%s", fullPath.c_str());
        CCLOGERROR("[LevelConfigLoader] %s", _errorLog.c_str());
        return nullptr;
    }

    // 5. 解析 JSON 数据
    Document doc;
    doc.Parse<kParseDefaultFlags>(fileContent.c_str());

    // 6. 检查 JSON 格式合法性
    if (doc.HasParseError())
    {
        _errorLog = StringUtils::format(
            "JSON 解析错误（文件：%s，错误码：%d，偏移量：%u）",
            fullPath.c_str(),
            doc.GetParseError(),
            doc.GetErrorOffset()
        );
        CCLOGERROR("[LevelConfigLoader] %s", _errorLog.c_str());
        return nullptr;
    }

    // 7. 解析 JSON 数据为 LevelConfig
    return parseLevelConfig(doc);
}

LevelConfig* LevelConfigLoader::parseLevelConfig(const RAPIDJSON_NAMESPACE::Value& root)
{
    // 1. 创建 LevelConfig 实例
    LevelConfig* levelConfig = LevelConfig::create();
    if (!levelConfig)
    {
        _errorLog = "LevelConfig 实例创建失败";
        CCLOGERROR("[LevelConfigLoader] %s", _errorLog.c_str());
        return nullptr;
    }

    int cardId = 0;

    // 2. 解析主牌区配置（Playfield）
    if (root.HasMember("Playfield") && root["Playfield"].IsArray())
    {
        const auto& playfieldArray = root["Playfield"];
        for (SizeType i = 0; i < playfieldArray.Size(); ++i)
        {
            const auto& cardJson = playfieldArray[i];
            CardConfig cardConfig;

            if (parseCardConfig(cardJson, cardConfig))
            {
                cardConfig.cardId = cardId++;
                levelConfig->addPlayfieldConfig(cardConfig);
            }
            else
            {
                // 解析单张卡牌失败，释放配置并返回 nullptr
                CC_SAFE_DELETE(levelConfig);
                return nullptr;
            }
        }
    }
    else
    {
        _errorLog = "JSON 缺少 Playfield 数组或格式错误";
        CCLOGERROR("[LevelConfigLoader] %s", _errorLog.c_str());
        CC_SAFE_DELETE(levelConfig);
        return nullptr;
    }

    // 3. 解析备用牌堆配置（Stack）
    if (root.HasMember("Stack") && root["Stack"].IsArray())
    {
        const auto& stackArray = root["Stack"];
        for (SizeType i = 0; i < stackArray.Size(); ++i)
        {
            const auto& cardJson = stackArray[i];
            CardConfig cardConfig;

            if (parseCardConfig(cardJson, cardConfig))
            {
                cardConfig.cardId = cardId++;
                levelConfig->addStackConfig(cardConfig);
            }
            else
            {
                // 解析单张卡牌失败，释放配置并返回 nullptr
                CC_SAFE_DELETE(levelConfig);
                return nullptr;
            }
        }
    }
    else
    {
        _errorLog = "JSON 缺少 Stack 数组或格式错误";
        CCLOGERROR("[LevelConfigLoader] %s", _errorLog.c_str());
        CC_SAFE_DELETE(levelConfig);
        return nullptr;
    }

    // 4. 解析成功，返回配置实例
    CCLOG("[LevelConfigLoader] 关卡配置解析成功（主牌区：%zu 张，备用牌堆：%zu 张）",
        levelConfig->getPlayfieldConfigs().size(),
        levelConfig->getStackConfigs().size());
    return levelConfig;
}

bool LevelConfigLoader::parseCardConfig(const RAPIDJSON_NAMESPACE::Value& cardJson, CardConfig& outConfig)
{
    // 1. 检查卡牌 JSON 是否为对象
    if (!cardJson.IsObject())
    {
        _errorLog = "卡牌配置不是 JSON 对象";
        CCLOGERROR("[LevelConfigLoader] %s", _errorLog.c_str());
        return false;
    }

    // 2. 解析卡牌点数（CardFace）
    if (!cardJson.HasMember("CardFace") || !cardJson["CardFace"].IsInt())
    {
        _errorLog = "卡牌配置缺少 CardFace 或格式错误";
        CCLOGERROR("[LevelConfigLoader] %s", _errorLog.c_str());
        return false;
    }
    const int cardFaceInt = cardJson["CardFace"].GetInt();

    // 3. 解析卡牌花色（CardSuit）
    if (!cardJson.HasMember("CardSuit") || !cardJson["CardSuit"].IsInt())
    {
        _errorLog = "卡牌配置缺少 CardSuit 或格式错误";
        CCLOGERROR("[LevelConfigLoader] %s", _errorLog.c_str());
        return false;
    }
    const int cardSuitInt = cardJson["CardSuit"].GetInt();

    // 4. 验证点数和花色的合法性
    if (!validateCardConfig(cardFaceInt, cardSuitInt))
    {
        CCLOGERROR("[LevelConfigLoader] %s", _errorLog.c_str());
        return false;
    }

    // 5. 解析卡牌位置（Position）
    if (!cardJson.HasMember("Position") || !cardJson["Position"].IsObject())
    {
        _errorLog = "卡牌配置缺少 Position 或格式错误";
        CCLOGERROR("[LevelConfigLoader] %s", _errorLog.c_str());
        return false;
    }
    const auto& posJson = cardJson["Position"];
    if (!posJson.HasMember("x") || !posJson["x"].IsInt() ||
        !posJson.HasMember("y") || !posJson["y"].IsInt())
    {
        _errorLog = "Position 缺少 x/y 或格式错误";
        CCLOGERROR("[LevelConfigLoader] %s", _errorLog.c_str());
        return false;
    }
    const float x = static_cast<float>(posJson["x"].GetInt());
    const float y = static_cast<float>(posJson["y"].GetInt());

    // 6. 赋值给输出参数
    outConfig.cardFace = static_cast<CardFaceType>(cardFaceInt);
    outConfig.cardSuit = static_cast<CardSuitType>(cardSuitInt);
    outConfig.position = Vec2(x, y);

    return true;
}

bool LevelConfigLoader::validateCardConfig(int face, int suit)
{
    // 验证点数范围（CFT_ACE(0) ~ CFT_KING(12)）
    if (face < CFT_ACE || face >= CFT_NUM_CARD_FACE_TYPES)
    {
        _errorLog = StringUtils::format("卡牌点数非法：%d（合法范围：%d ~ %d）",
            face, CFT_ACE, CFT_NUM_CARD_FACE_TYPES - 1);
        return false;
    }

    // 验证花色范围（CST_CLUBS(0) ~ CST_SPADES(3)）
    if (suit < CST_CLUBS || suit >= CST_NUM_CARD_SUIT_TYPES)
    {
        _errorLog = StringUtils::format("卡牌花色非法：%d（合法范围：%d ~ %d）",
            suit, CST_CLUBS, CST_NUM_CARD_SUIT_TYPES - 1);
        return false;
    }

    return true;
}

