#pragma once

#include <string>

#include "../location.hpp"

class Error {
  public:
    std::string message;
    Error(std::string message) : message(message) {}
    virtual std::string to_string();
};

class LocationError : public Error {
  public:
    Location loc;
    LocationError(Location loc, std::string message)
        : Error(message), loc(loc) {}
    std::string to_string();
};
