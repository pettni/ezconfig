// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

#pragma once

#include <yaml-cpp/yaml.h>

#include "eigen_fwd.hpp"

namespace YAML {

template<typename T, int Rows, int Cols, int Opts>
bool convert<Eigen::Matrix<T, Rows, Cols, Opts>>::decode(const Node & yaml, Eigen::Matrix<T, Rows, Cols, Opts> & obj)
{
  if constexpr (Cols == 1) {
    // compile-time vector
    const auto data = yaml.as<std::vector<T>>();
    if (Rows > 0 && data.size() != static_cast<std::size_t>(Rows)) {
      throw YAML::ParserException{
        yaml.Mark(),
        "Invalid size of numeric yaml vector: expected '" + std::to_string(Rows) + "' but got '"
          + std::to_string(data.size()) + "'",
      };
    }
    obj = Eigen::Map<const Eigen::Matrix<T, Rows, Cols>>(data.data(), static_cast<Eigen::Index>(data.size()));
  } else {
    // compile-time matrix
    const auto data = yaml.as<std::vector<std::vector<T>>>();
    if (data.empty()) { throw YAML::ParserException{yaml.Mark(), "Can not parse empty matrix"}; }
    obj.resize(static_cast<Eigen::Index>(data.size()), static_cast<Eigen::Index>(data[0].size()));
    for (auto i = 0u; const auto & data_row : data) {
      if (static_cast<Eigen::Index>(data_row.size()) != obj.cols()) {
        throw YAML::ParserException{yaml.Mark(), "Not all rows have the same length"};
      }
      obj.row(i++) =
        Eigen::Map<const Eigen::Matrix<T, Cols, 1>>(data_row.data(), static_cast<Eigen::Index>(data_row.size()));
    }
  }
  return true;
}

template<typename T, int Opts>
bool convert<Eigen::Quaternion<T, Opts>>::decode(const Node & yaml, Eigen::Quaternion<T, Opts> & obj)
{
  obj.w() = yaml["w"].as<T>();
  obj.x() = yaml["x"].as<T>();
  obj.y() = yaml["y"].as<T>();
  obj.z() = yaml["z"].as<T>();
  return true;
}

}  // namespace YAML
