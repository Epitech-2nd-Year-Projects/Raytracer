/**
 * @file OutputException.hpp
 * @brief Defines custom exceptions for the raytracer output operations.
 */

#pragma once

#include "Exceptions/RaytracerException.hpp"
#include <filesystem>
#include <string>

namespace Raytracer::Exceptions {

/**
 * @class FileException
 * @brief Base class for file-related exceptions.
 */
class FileException : public RaytracerException {
public:
  /**
   * @brief Constructor with filename and error message.
   * @param filename Path to the problematic file.
   * @param message Error description.
   */
  FileException(const std::filesystem::path &filename,
                std::string message) noexcept
      : RaytracerException(std::move(message)), m_filename(filename) {
    m_message = "File '" + m_filename.string() + "': " + m_message;
  }

  /**
   * @brief Get the filename that caused the exception.
   * @return Path to the problematic file.
   */
  [[nodiscard]] const std::filesystem::path &getFilename() const noexcept {
    return m_filename;
  }

protected:
  std::filesystem::path m_filename;
};

/**
 * @class OutputFileException
 * @brief Exception for output file operations.
 */
class OutputFileException : public FileException {
public:
  /**
   * @brief Constructor for output file errors.
   * @param filename Output file path.
   * @param message Error description.
   */
  OutputFileException(const std::filesystem::path &filename,
                      std::string message) noexcept
      : FileException(filename, std::move(message)) {}
};
} // namespace Raytracer::Exceptions
