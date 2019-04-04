#include "tests.hpp"

void tests::cfuinline(
    const string& data
) {
    require_auth(_self);

    action action(permission_level{_self, "active"_n}, "dfuseiohooks"_n, "event"_n, std::make_tuple(std::string(""), data));
    action.send();
}

void tests::cfainline(
    const string& data
) {
    action action(std::vector<permission_level>(), "dfuseiohooks"_n, "event"_n, std::make_tuple(std::string(""), data));
    action.send_context_free();
}

