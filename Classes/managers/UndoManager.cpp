#include "UndoManager.h"

UndoManager::UndoManager()
{
	_undoModel = UndoModel::getInstance();
}

void UndoManager::recordPlayfieldCardClick(int cardId)
{
	ActionRecord record = ActionRecord();
	record.action = CLICK_PLAY_FIELD;
	record.cardId = cardId;
	_undoModel->pushActionRecord(record);
}

void UndoManager::recordStackCardClick(int cardId)
{
	ActionRecord record = ActionRecord();
	record.action = CLICK_STACK_CARD;
	record.cardId = cardId;
	_undoModel->pushActionRecord(record);
}

ActionRecord UndoManager::removeActionRecord()
{
	return _undoModel->popActionRecord();
}
