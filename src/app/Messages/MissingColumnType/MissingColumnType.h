#pragma once

#include "../../../Components/Info/InfoModel/InfoModel.h"
#include "../../../String/ConstString/ConstString.h"

class MissingColumnType: public InfoModel {
public:
    static MissingColumnType* missingColumnType();

public:
    void output(CharOutputStream& outputStream) const final;

private:
    MissingColumnType() noexcept = default;

    MissingColumnType(const MissingColumnType& other) = delete;

    MissingColumnType(MissingColumnType&& other) = delete;

    MissingColumnType& operator=(const MissingColumnType& other) = delete;

    MissingColumnType& operator=(MissingColumnType&& other) = delete;

private:
    static MissingColumnType instance;

    static ConstString text;
};