// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

#pragma once

#include <yaml-cpp/yaml.h>

#include "eigen_fwd.hpp"

namespace YAML {

template<typename T, int Rows, int Cols, int Opts>
bool convert<Eigen::Matrix<T, Rows, Cols, Opts>>::decode(const Node & yaml, Eigen::Matrix<T, Rows, Cols, Opts> & obj)
{
  if constexpr (Cols == 1) {
    if (yaml.IsSequence()) {
      if (Rows > 0 && yaml.size() != static_cast<std::size_t>(Rows)) {
        throw YAML::ParserException{
          yaml.Mark(),
          "Invalid size of numeric yaml vector: expected '" + std::to_string(Rows) + "' but got '"
            + std::to_string(yaml.size()) + "'",
        };
      }
      // compile-time vector
      const auto data = yaml.as<std::vector<T>>();
      obj = Eigen::Map<const Eigen::Matrix<T, Rows, Cols>>(data.data(), static_cast<Eigen::Index>(data.size()));
    } else if (yaml.IsMap()) {
      // count x,y,z keys
      auto counter = [](const auto & item) {
        const auto k = item.first.template as<std::string>();
        return k == "x" || k == "y" || k == "z";
      };
      Eigen::Index N = std::count_if(std::begin(yaml), std::end(yaml), counter);
      obj.resize(N);
      for (auto i = 0u; i < N; ++i) { obj(i) = yaml[char('x' + i)].as<T>(); }
    } else {
      throw YAML::ParserException{yaml.Mark(), "Expected sequence or map"};
    }
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
  if (yaml["w"]) {
    obj.w() = yaml["w"].as<T>();
    obj.x() = yaml["x"].as<T>();
    obj.y() = yaml["y"].as<T>();
    obj.z() = yaml["z"].as<T>();
  } else if (yaml["qw"]) {
    obj.w() = yaml["qw"].as<T>();
    obj.x() = yaml["qx"].as<T>();
    obj.y() = yaml["qy"].as<T>();
    obj.z() = yaml["qz"].as<T>();
  } else {
    throw YAML::ParserException{yaml.Mark(), "Expected key 'w' or 'qw'"};
  }
  return true;
}

}  // namespace YAML
