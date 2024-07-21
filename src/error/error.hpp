#pragma once

#include <string>

#include "../location/location.hpp"

class Error {
  public:
    std::string message;
    Error(const std::string &message) : message(message) {}
    virtual std::string to_string();
};

class LocationError : public Error {
  public:
    Location loc;
    LocationError(const Location &loc, const std::string &message)
        : Error(message), loc(loc) {}
    std::string to_string();
};
