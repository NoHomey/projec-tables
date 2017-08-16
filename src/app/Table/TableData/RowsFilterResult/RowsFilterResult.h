#pragma once

#include "../../../../Container/DynamicArray/DynamicArray.htd"
#include "../../TableTypes.h"

class RowsFilterResult {
public:
    static RowsFilterResult allRows(size_t all) noexcept;

public:
    RowsFilterResult() noexcept;

    RowsFilterResult(TableTypes::Row unFilteredRowsCount);

    RowsFilterResult(const RowsFilterResult& other) = delete;

    RowsFilterResult(RowsFilterResult&& other) noexcept = default;

    RowsFilterResult& operator=(const RowsFilterResult& other) = delete;
    
    RowsFilterResult& operator=(RowsFilterResult&& other) noexcept = default;

public:
    void addRow(TableTypes::Row row) noexcept;

    void optimize();

    size_t count() const noexcept;

    TableTypes::Row operator[](TableTypes::Row row) const noexcept;

    bool isEmpty() const noexcept;

    template<typename ForEachInDynamicArrayFunction>
    void forEach(ForEachInDynamicArrayFunction& function) const;
    
    template<typename ForEachInDynamicArrayFunction>
    void forEach(const ForEachInDynamicArrayFunction& function) const; 

private:
    template<typename ForEachInDynamicArrayFunction>
    void forEachRow(ForEachInDynamicArrayFunction function) const;

private:
    RowsFilterResult(size_t allCount) noexcept;

private:
    static const size_t minUnused = 32;

private:
    size_t allCount;

    DynamicArray<TableTypes::Row> filteredRows;
};

template<typename ForEachInDynamicArrayFunction>
void RowsFilterResult::forEachRow(ForEachInDynamicArrayFunction function) const {
    if(allCount > 0) {
        for(TableTypes::Row row = 0; row < allCount; ++ row) {
            function(row, row);
        }
    } else {
        filteredRows.forEach(function);
    }
}

template<typename ForEachInDynamicArrayFunction>
void RowsFilterResult::forEach(ForEachInDynamicArrayFunction& function) const {
    forEachRow<ForEachInDynamicArrayFunction&>(function);
}

template<typename ForEachInDynamicArrayFunction>
void RowsFilterResult::forEach(const ForEachInDynamicArrayFunction& function) const {
    forEachRow<const ForEachInDynamicArrayFunction&>(function);
}