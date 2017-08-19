#include "ShowTables.h"
#include "../../ModelAdaptors/ListTablesModelAdaptor/ListTablesModelAdaptor.h"
#include "../../../Components/List/ListComponent/ListComponent.h"

ShowTables ShowTables::instance;

Action* ShowTables::showTables() noexcept {
    return &instance; 
}

Action* ShowTables::action() {
    setComponent(ListComponent::inject(ListTablesModelAdaptor::adapt(allTables)));
    return nullptr;
}