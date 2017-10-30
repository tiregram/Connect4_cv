#include "Cv_c4.hpp"
#include "Cv_c4_option.hpp"
#include "Cv_c4_option_helper.hpp"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Hello
#include <boost/test/unit_test.hpp>
#include <vector>
#include <iostream>

typedef std::vector<unsigned int> Vui;

BOOST_AUTO_TEST_CASE(CV_opt_Test)
{
  auto b = std::make_pair<cv::Vec3b,cv::Vec3b>(cv::Vec3b(3,100,70),
                                               cv::Vec3b(15,255,200));

  auto r = std::make_pair<cv::Vec3b,cv::Vec3b>(cv::Vec3b(30,75,70),
                                               cv::Vec3b(80,255,200));

  auto g = std::make_pair<cv::Vec3b,cv::Vec3b>(cv::Vec3b(100,0,0),
                                               cv::Vec3b(140,255,255));
  int dv;
  int blur;
  cv::Size s(200,200);
  cv::Size m(5,5);

  Cv_c4_option opt(b,r,g,dv,blur,s,m);
  std::cout<<opt;
  // // get check
  BOOST_CHECK_EQUAL(opt.get_blue_board(),b);
  BOOST_CHECK_EQUAL(opt.get_red_piece(),r);
  BOOST_CHECK_EQUAL(opt.get_green_piece(),g);

  BOOST_CHECK_EQUAL(opt.get_distance_value(),dv);
  BOOST_CHECK_EQUAL(opt.get_blur(),blur);

  BOOST_CHECK_EQUAL(opt.get_size_remap(),s);
  BOOST_CHECK_EQUAL(opt.get_size_remap_margin(),m);

  // // set check
  // TODO:

  // // get check
  // TODO:

  // // save check
  // TODO:

  // // load check
  // TODO:

  // // check check
  // TODO:

  // // exit check
  // TODO:
}


BOOST_AUTO_TEST_CASE(CV_helper_test)
{
  Cv_c4_option opt;

  //  std::string path_img="img/z2pro_print2_1.2M/IMG_20171027_175623.jpg";
  std::string path_img="img/z2pro_print2_1.2M/IMG_20171027_175710.jpg";
  Cv_c4_option_helper cv_h(opt);

  cv_h.chose_blue(path_img);
  cv_h.chose_red(path_img);
  cv_h.chose_green(path_img);
  cv_h.chose_distance(path_img);

  std::cout << cv_h.cv.opt << std::endl;
  cv_h.cv.opt.save("save.cvconf");

  Cv_c4_option opt_l;
  opt_l.load("save.cvconf");

}

BOOST_AUTO_TEST_CASE(CV_predictTest)
{
  Cv_c4_option opt;
  Cv_c4 cv(opt);

  std::string path_img="img/z2pro_print2_1.2M/IMG_20171027_175623.jpg";
  opt.set_distance_value(50);

  auto a = cv.predict_board(path_img);
  std::cout <<a  << "\n";
}
