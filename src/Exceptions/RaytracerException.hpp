/**
 * @file RaytracerException.hpp
 * @brief Defines custom exceptions for the raytracer.
 */

#pragma once

#include <exception>
#include <string>

namespace Raytracer::Exceptions {

/**
 * @class RaytracerException
 * @brief Base class for all raytracer exceptions.
 */
class RaytracerException : public std::exception {
public:
  /**
   * @brief Constructor with error message.
   * @param message Error description.
   */
  explicit RaytracerException(std::string message) noexcept
      : m_message(std::move(message)) {}

  /**
   * @brief Get the error message.
   * @return C-string containing the error message.
   */
  [[nodiscard]] const char *what() const noexcept override {
    return m_message.c_str();
  }

protected:
  std::string m_message;
};
} // namespace Raytracer::Exceptions
