#pragma once

#include <string>
#include <utility>
#include <vector>

#include <opencv2/core.hpp>

class Cv_c4_option
{
public:
  Cv_c4_option(); // default
  Cv_c4_option(std::string file_path);

  Cv_c4_option(std::pair<cv::Vec3b,cv::Vec3b> p_board_selector,
               std::pair<cv::Vec3b,cv::Vec3b> p_red_selector,
               std::pair<cv::Vec3b,cv::Vec3b> p_green_selector,
               int p_distance_value,
               int p_blur,
               cv::Size p_size_remap,
               cv::Size p_size_remap_margin
               );


  virtual   ~Cv_c4_option();
  void save(std::string file_path);
  void load(std::string file_path);
  bool check();
  bool check_color();
  bool check_color_collision();
  bool check_color_value();

private:
  std::pair<cv::Vec3b,cv::Vec3b> blue_board;
  std::pair<cv::Vec3b,cv::Vec3b> red_piece;
  std::pair<cv::Vec3b,cv::Vec3b> green_piece;

  unsigned int distance_value;
  unsigned int blur;
  cv::Size size_remap;
  cv::Size size_remap_margin;

public:

  const cv::Size get_size_remap_margin() const {
    return size_remap_margin;
  }

  void set_size_remap_margin(const cv::Size size_remap_margin) {
    this->size_remap_margin = size_remap_margin;
  }

  const cv::Size get_size_remap() const {
    return size_remap;
  }

  void set_size_remap(const cv::Size size_remap) {
    this->size_remap = size_remap;
  }

  unsigned int get_blur() const {
    return blur;
  }

  void set_blur(const unsigned int blur) {
    this->blur = blur;
  }

  unsigned int get_distance_value() const {
    return distance_value;
  }

  void set_distance_value(const unsigned int distance_value) {
    this->distance_value = distance_value;
  }

  const std::pair<cv::Vec3b,cv::Vec3b> get_green_piece() const {
    return green_piece;
  }

  void set_green_piece(const std::pair<cv::Vec3b,cv::Vec3b>& green_piece) {
    this->green_piece = green_piece;
  }

  const std::pair<cv::Vec3b,cv::Vec3b> get_red_piece() const {
    return red_piece;
  }

  void set_red_piece(const std::pair<cv::Vec3b,cv::Vec3b>& red_piece) {
    this->red_piece = red_piece;
  }

  const std::pair<cv::Vec3b,cv::Vec3b> get_blue_board() const {
    return blue_board;
  }

  void set_blue_board(const std::pair<cv::Vec3b,cv::Vec3b>& blue_board) {
    this->blue_board = blue_board;
  }

};

namespace std {
  ostream& operator <<(ostream& flx, const pair<cv::Vec3b,cv::Vec3b>& v);
  ostream& operator <<(ostream& c, const Cv_c4_option& t );
}
