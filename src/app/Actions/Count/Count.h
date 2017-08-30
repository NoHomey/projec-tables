#pragma once

#include "../ActionUsingTableNameColumnAndValue/ActionUsingTableNameColumnAndValue.htd"

class Count: public ActionUsingTableNameColumnAndValue<Count> {
private:
    friend ActionUsingTableNameColumnAndValue<Count>;

    using Base = ActionUsingTableNameColumnAndValue<Count>;

public:
    static Action* controller() noexcept;

private:
    Count() noexcept = default;

private:
    static Action* finalAction(TableTypes::Column column, Argument& argument);

public:
    static ConstString actionString;

private:
    static Count instance;
};
