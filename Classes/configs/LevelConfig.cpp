#include "LevelConfig.h"

USING_NS_CC;

LevelConfig* LevelConfig::create()
{
    LevelConfig* config = new (std::nothrow) LevelConfig();
    if (config)
    {
        return config;
    }
    CC_SAFE_DELETE(config);
    return nullptr;
}

const std::vector<CardConfig>& LevelConfig::getPlayfieldConfigs() const
{
    return _playfieldConfigs;
}

void LevelConfig::addPlayfieldConfig(const CardConfig& config)
{
    _playfieldConfigs.emplace_back(config);
}

const std::vector<CardConfig>& LevelConfig::getStackConfigs() const
{
    return _stackConfigs;
}

void LevelConfig::addStackConfig(const CardConfig& config)
{
    _stackConfigs.emplace_back(config);
}
