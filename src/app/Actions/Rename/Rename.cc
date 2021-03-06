#include "Rename.h"
#include "../ParseTableName/ParseTableName.h"
#include "../../Messages/TableRenamed/TableRenamed.h"
#include "../../Messages/NoRename/NoRename.h"
#include "../../Messages/NewNameIsNotUnique/NewNameIsNotUnique.h"

Rename Rename::instance;

ConstString Rename::actionString{"Rename"};

ConstString Rename::description{"Command usage: 'Rename <Table> <New name>'. Command description: Renames Table with name <Table> to <New name>."};

Rename::Rename() noexcept
: Base{RenameState::ParseTableName} { }

Action* Rename::controller() noexcept {
    return &instance;
}

Action* Rename::parseTableName() {
    return Base::parseTableName<Rename>(RenameState::ParseNewTableName, RenameState::TableNotFound);
}

Action* Rename::parseNewTableName() {
    if(!ParseTableName::parseTableName(actionString)) {
        return nullptr;
    }
    ConstString& newName = arguments[1].asTemporaryString();
    Table* table = allTables.getTableByName(newName);
    if(table != nullptr) {
        setState(currentTable->getName() == newName ? RenameState::WarnForNoRename : RenameState::NewNameIsNotUnique);
    } else {
        setState(RenameState::RenameTable);
    }
    return this;
}

Action* Rename::renameTable() {
    ConstString& newName = arguments[1].asTemporaryString();
    FixedSizeString oldName = currentTable->rename(FixedSizeString::fromString(newName));
    return showMessage(new TableRenamed(std::move(oldName), currentTable->getName()));
}

Action* Rename::newNameIsNotUnique() {
    return showMessage(new NewNameIsNotUnique(currentTable->getName(),
        allTables.getTableByName(arguments[1].asTemporaryString())->getName()
    ));
}

Action* Rename::warnForNoRename() {
    return showMessage(new NoRename(currentTable->getName()));
}

Action* Rename::action() {
    switch(getState()) {
        case RenameState::ParseTableName: return parseTableName();
        case RenameState::TableNotFound: return tableNotFound();
        case RenameState::ParseNewTableName: return parseNewTableName();
        case RenameState::NewNameIsNotUnique: return newNameIsNotUnique();
        case RenameState::WarnForNoRename: return warnForNoRename();
        case RenameState::RenameTable: return renameTable();
        default: return nullptr;
    }
}

Action* Rename::controlAction() noexcept {
    setState(RenameState::ParseTableName);
    return this;
}