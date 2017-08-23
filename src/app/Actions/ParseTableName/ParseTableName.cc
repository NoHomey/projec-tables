#include "ParseTableName.h"
#include "../../../Parsers/TableNameParser/TableNameParser.h"
#include "../../Messages/MissingTableName/MissingTableName.h"
#include "../../Messages/InvalidTableName/InvalidTableName.h"

ParseTableName::ParseTableName(ConstString& commandName) noexcept
: commandName{commandName} { }

Action* ParseTableName::action() {
    CharSequenceParser::ParseResult result;
    try {
        result = TableNameParser::parse(command);
    } catch(const TableNameParser::InvalidTableName& error) {
        return showMessage(new InvalidTableName(error.getToken()));
    } catch(const Exception& error) {
        return showMessage(new MissingTableName(commandName, arguments.size()));
    }
    Action::command = result.getRest();
    arguments.push(result.getExtracted());
    return nullptr;
}