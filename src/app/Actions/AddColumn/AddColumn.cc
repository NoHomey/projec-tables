#include "AddColumn.h"
#include "../ParseTableName/ParseTableName.h"
#include "../Message/Message.h"
#include "../../../Parsers/ColumnTypeParser/ColumnTypeParser.h"
#include "../../Messages/MissingColumnType/MissingColumnType.h"
#include "../../Messages/InvalidColumnType/InvalidColumnType.h"
#include "../../Messages/NewColumnAdded/NewColumnAdded.h"

AddColumn AddColumn:: instance;

ConstString AddColumn::actionString{"AddColumn"};

AddColumn::AddColumn() noexcept
: Base{AddColumnState::ParseTableName} { }

Action* AddColumn::addColumn() noexcept {
    instance.setState(AddColumnState::ParseTableName);
    return &instance;
}

Action* AddColumn::parseTableName() {
    return Base::parseTableName<AddColumn>(AddColumnState::ParseColumnType, AddColumnState::TableNotFound);
}

Action* AddColumn::parseColumnType() {
    ColumnTypeParser::ParseResult result;
    try {
        result = ColumnTypeParser::parse(command);
    } catch(const ColumnTypeParser::InvalidColumnType& error) {
        return Message::showMessage(InvalidColumnType::inject(error.getToken()));
    } catch(const Exception& error) {
        return Message::showMessage(MissingColumnType::missingColumnType());
    }
    Action::command = result.getRest();
    arguments.push(static_cast<TableTypes::Integer>(result.getColumnType()));
    setState(AddColumnState::AddNewColumn);
    return this;
}

Action* AddColumn::addNewColumn() {
    const ColumnMetaData::ColumnType columnType
        = static_cast<ColumnMetaData::ColumnType>(arguments[1].asInteger());
    currentTable->addColumn({columnType});
    return Message::showMessage(NewColumnAdded::inject(currentTable->getName(), columnType));
}

Action* AddColumn::action() {
    switch(getState()) {
        case AddColumnState::ParseTableName: return parseTableName();
        case AddColumnState::TableNotFound: return tableNotFound();
        case AddColumnState::ParseColumnType: return parseColumnType();
        case AddColumnState::AddNewColumn: return addNewColumn();
        default: return nullptr;
    }
}