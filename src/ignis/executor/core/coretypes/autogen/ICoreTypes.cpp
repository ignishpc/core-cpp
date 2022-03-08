
#include "ignis/executor/core/ICoreTypes.h"

extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type000;
extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type001;
extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type002;
extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type003;
extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type004;
extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type005;
extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type006;
extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type007;
extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type008;
extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type009;
extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type010;
extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type011;
extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type012;
extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type013;
extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type014;
extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type015;
extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type016;
extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type017;
extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type018;
extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type019;
extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type020;
extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type021;
extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type022;
extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type023;
extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type024;
extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type025;
extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type026;
extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type027;
extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type028;
extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type029;
extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type030;
extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type031;
extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> type032;
extern std::shared_ptr<ignis::executor::core::selector::ISelectorGroup> function000;
extern std::shared_ptr<ignis::executor::core::selector::ISelectorGroup> function001;
extern std::shared_ptr<ignis::executor::core::selector::ISelectorGroup> function002;
extern std::shared_ptr<ignis::executor::core::selector::ISelectorGroup> function003;
extern std::shared_ptr<ignis::executor::core::selector::ISelectorGroup> function004;
extern std::shared_ptr<ignis::executor::core::selector::ISelectorGroup> function005;
extern std::shared_ptr<ignis::executor::core::selector::ISelectorGroup> function006;
extern std::shared_ptr<ignis::executor::core::selector::ISelectorGroup> function007;


std::vector<std::shared_ptr<ignis::executor::core::selector::ITypeSelector>> ignis::executor::core::ICoreTypes::defaultTypes() {
    return {
        type000, type001, type002, type003, type004, type005, type006, type007, type008, type009, type010, type011, type012, type013, type014, type015, type016, type017, type018, type019, type020, type021, type022, type023, type024, type025, type026, type027, type028, type029, type030, type031, type032
    };
}

std::vector<std::shared_ptr<ignis::executor::core::selector::ISelectorGroup>> ignis::executor::core::ICoreTypes::defaultFunctions() {
    return {
        function000, function001, function002, function003, function004, function005, function006, function007
    };
}
