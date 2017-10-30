
#pragma once

#include "Cv_c4.hpp"


// this class will help the user to configure the camera to the situation
class Cv_c4_option_helper
{
private:


public:
  Cv_c4 cv;
  Cv_c4_option_helper(const Cv_c4_option& opt);

  std::pair<cv::Vec3b,cv::Vec3b> chose(std::pair<cv::Vec3b,cv::Vec3b> base, cv::Mat img);


  int chose_int(int , cv::Mat img);

  void chose_blue(std::string img_path);
  void chose_green(std::string img_path);
  void chose_red(std::string img_path);
  void chose_distance(std::string img_path);

  virtual ~Cv_c4_option_helper();
};
