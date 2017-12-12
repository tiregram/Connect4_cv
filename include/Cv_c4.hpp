#pragma once

#include <string>
#include <vector>

#include <opencv2/core.hpp>

#include "Cv_c4_option.hpp"

enum CV_BOARD_STATE{
  CV_RED,
  CV_GREEN,
  CV_UNDEFINED,
  CV_VIDE,
};

class Cv_c4
{
public:
  Cv_c4_option opt;
  Cv_c4() = delete;

  Cv_c4(const Cv_c4_option& opt);
  virtual ~Cv_c4();

  std::vector<CV_BOARD_STATE> predict_board(cv::Mat image) ;
  std::vector<CV_BOARD_STATE> predict_board(std::string img) ;
  CV_BOARD_STATE get_piece_color(cv::Vec3b color);

  void remap_game(const cv::Mat& image,
                  cv::Mat& imageremap,
                  const std::vector<cv::Point>&  quadri);


  std::vector<CV_BOARD_STATE>  get_board_value(std::vector<cv::Point>& list_center,
                                                      cv::Mat imageblur );

  // get the center of function
  cv::Point center(const std::vector<cv::Point>& contour);
  // get all center
  std::vector<std::vector<cv::Point> > get_piece(const cv::Mat & blue_board_Game);
  std::vector<cv::Point> centers(const std::vector<std::vector<cv::Point> >& contours);
  std::vector<cv::Point> isolate_Game(const cv::Mat& image_where_search_hsv);
  std::vector<cv::Point> set_correct_rotation_for_board(const std::vector<cv::Point> &  cnt);
};

std::ostream& operator<<(std::ostream&, const std::vector<CV_BOARD_STATE>& flx);

