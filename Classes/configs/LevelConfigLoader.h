#pragma once
#include "cocos2d.h"
#include "LevelConfig.h"
#include "json/document.h"

/**
 * @class LevelConfigLoader
 * @brief 读取json文件 关卡配置加载器类
 * @职责 负责加载和解析游戏关卡的配置文件（JSON格式），将配置数据转换为LevelConfig实例，
 *       包含卡牌配置的解析与合法性验证，同时记录解析过程中的错误信息用于调试
 * @使用场景 用于游戏启动或关卡切换时，加载对应关卡的配置数据（如初始卡牌分布、关卡规则等），
 *           为游戏初始化提供必要的配置信息
 *           供LevelConfig调用，提供数据。
 */
class LevelConfigLoader
{
public:
    /**
     * @brief 默认构造函数
     */
    LevelConfigLoader() = default;

    /**
     * 加载指定关卡的配置文件
     * @param levelId 关卡ID（对应配置文件命名，如 "level_1.json"）
     * @param configPath 配置文件根路径（默认："configs/levels/"）
     * @return 成功返回 LevelConfig 实例，失败返回 nullptr
     */
    LevelConfig* loadLevelConfig(int levelId, const std::string& configPath = "configs/levels/");

private:
    /**
     * 解析 JSON 根对象，生成 LevelConfig 数据
     * @param root JSON 根对象
     * @return 成功返回 LevelConfig 实例，失败返回 nullptr
     */
    LevelConfig* parseLevelConfig(const RAPIDJSON_NAMESPACE::Value& root);

    /**
     * 解析单个卡牌配置（Playfield 或 Stack 中的卡牌）
     * @param cardJson 单张卡牌的 JSON 对象
     * @param outConfig 输出参数：解析后的 CardConfig 结构
     * @return 解析成功返回 true，失败返回 false
     */
    bool parseCardConfig(const RAPIDJSON_NAMESPACE::Value& cardJson, CardConfig& outConfig);

    /**
     * 验证卡牌配置的合法性（点数、花色范围）
     * @param face 卡牌点数
     * @param suit 卡牌花色
     * @return 合法返回 true，非法返回 false
     */
    bool validateCardConfig(int face, int suit);

private:
    /**
     * @brief 错误日志字符串
     * @用途 存储配置加载和解析过程中产生的错误信息，用于调试和定位配置文件或解析逻辑的问题
     */
    std::string _errorLog;
};