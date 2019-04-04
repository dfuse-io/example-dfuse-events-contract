#pragma once

#include <string>

#include <eosio/eosio.hpp>

using eosio::action;
using eosio::datastream;
using eosio::name;
using eosio::permission_level;
using std::string;

class [[eosio::contract("tests")]] tests: public eosio::contract {
    public:
        tests(name receiver, name code, datastream<const char*> ds)
        :eosio::contract(receiver, code, ds)
        {}

        [[eosio::action]]
        void cfuinline(const string& data);

        [[eosio::action]]
        void cfainline(const string& data);
};
