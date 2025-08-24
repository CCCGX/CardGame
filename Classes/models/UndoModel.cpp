#include "UndoModel.h"

USING_NS_CC;

UndoModel* UndoModel::s_instance = nullptr;

UndoModel* UndoModel::getInstance()
{
	if (s_instance == nullptr)
	{
		s_instance = new (std::nothrow) UndoModel();
		CCASSERT(s_instance, "SingletonManager 实例创建失败!");
	}
	return s_instance;
}

UndoModel::~UndoModel()
{
}

void UndoModel::pushActionRecord(const ActionRecord& record)
{
	_actionStack.push(record);
}

ActionRecord UndoModel::popActionRecord()
{
	if(_actionStack.empty())
		return ActionRecord();
	const auto& res = _actionStack.top();
	_actionStack.pop();
	return res;
}