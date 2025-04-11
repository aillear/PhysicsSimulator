#pragma once

#include <format>
#include <stdexcept>

class BodyAreaError : public std::logic_error {
  public:
    BodyAreaError(float area)
        : std::logic_error("Create Body Error: "), area_(area) {
        ;
    }
    const char *what() const noexcept override {
        formatted_message_ = std::format("{}Invalid body area {}.",
                                         std::logic_error::what(), area_);
        return formatted_message_.c_str();
    }

    ~BodyAreaError() = default;
  private:
    mutable std::string formatted_message_;

  private:
    float area_;
};

class MaterialDensityError : public std::logic_error {
  public:
    MaterialDensityError(float density)
        : std::logic_error("Create Material Error: "), density_(density) {
        ;
    }
    const char *what() const noexcept override {
        formatted_message_ = std::format("{}Invalid material density {}.",
                                         std::logic_error::what(), density_);
        return formatted_message_.c_str();
    }

    ~MaterialDensityError() = default;
  private:
    mutable std::string formatted_message_;

  private:
    float density_;
};
