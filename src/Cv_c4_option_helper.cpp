#include "Cv_c4_option_helper.hpp"
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <memory>
#include <functional>


Cv_c4_option_helper::Cv_c4_option_helper(const Cv_c4_option& opt):cv(opt) {

}

struct picker_context_range
{
  std::pair<cv::Vec<int,3>,cv::Vec<int,3>> p;
  cv::Mat img;
  Cv_c4* th;
};


struct picker_context_int
{
  int p;
  cv::Mat img;
  Cv_c4* th;
};

void update_callback_range(int a , void * v)
{
  picker_context_range p = *(picker_context_range*) v;
  std::pair<cv::Vec3b,cv::Vec3b> pc(p.p);

  cv::Mat img_hsv,img_filtred;

  cvtColor(p.img,img_hsv,cv::COLOR_BGR2HSV);
  cv::inRange(img_hsv, pc.first, pc.second, img_filtred);

  std::string source_window = "selector_range_display";
  cv::namedWindow( source_window, CV_WINDOW_NORMAL );
  cv::imshow( source_window, img_filtred );



}

void update_callback_int(int a , void * v)
{
  picker_context_int p = *(picker_context_int*) v;

  cv::Mat img_hsv, img_filtred;

  cvtColor(p.img, img_hsv, cv::COLOR_BGR2HSV);
  auto brd = p.th->isolate_Game(img_hsv);

  std::vector<std::vector<cv::Point> > lap;
  lap.push_back(brd);

  std::cout << "elem: " << brd.size() << "\n";

  cv::Mat cp_img = p.img.clone();

  cv::drawContours(cp_img,lap , 0, cv::Scalar(0,0,255));

  std::string source_window = "selector_int_display";
  cv::namedWindow( source_window, CV_WINDOW_NORMAL );
  cv::imshow(source_window, cp_img);

  std::cout <<  p.th->predict_board(p.img) << "\n";
}


void Cv_c4_option_helper::chose_blue(std::string img_path)
{
  cv::Mat c = cv::imread(img_path, CV_LOAD_IMAGE_COLOR);
  this->chose_blue(c);

}

void Cv_c4_option_helper::chose_red(std::string img_path)
{
  cv::Mat c = cv::imread(img_path, CV_LOAD_IMAGE_COLOR);
  this->chose_red(c);
}

void Cv_c4_option_helper::chose_green(std::string img_path)
{
  cv::Mat c = cv::imread(img_path, CV_LOAD_IMAGE_COLOR);
  this->chose_green(c);
}


void Cv_c4_option_helper::chose_distance(std::string img_path)
{
  cv::Mat c = cv::imread(img_path, CV_LOAD_IMAGE_COLOR);
  this->chose_distance(c);
}

void Cv_c4_option_helper::chose_blue(cv::Mat m){
  auto color = cv.opt.get_blue_board();
  cv.opt.set_blue_board(chose(color,m));
}

void Cv_c4_option_helper::chose_green(cv::Mat m){
  auto color = cv.opt.get_green_piece();
  cv.opt.set_green_piece(chose(color,m));
  }

void Cv_c4_option_helper::chose_red(cv::Mat m){
  auto color = cv.opt.get_red_piece();
  cv.opt.set_red_piece(chose(color,m));
}

void Cv_c4_option_helper::chose_distance(cv::Mat m)
{
  int d = 200;
  cv.opt.set_distance_value(chose_int(d,m));

}

std::pair<cv::Vec3b,cv::Vec3b>
Cv_c4_option_helper::chose(std::pair<cv::Vec3b,cv::Vec3b> base, cv::Mat img)
{

  picker_context_range p;
  p.img = img;
  p.p = base;
  p.th = &this->cv;

  std::string source_window = "selector_range";
  cv::namedWindow( source_window, CV_WINDOW_NORMAL );
  cv::imshow( source_window, p.img );

  int* h_blue_min=&(p.p.first[0]);
  int* s_blue_min=&(p.p.first[1]);
  int* v_blue_min=&(p.p.first[2]);

  int* h_blue_max=&(p.p.second[0]);
  int* s_blue_max=&(p.p.second[1]);
  int* v_blue_max=&(p.p.second[2]);

  cv::createTrackbar( "min h", source_window, h_blue_min, 255, update_callback_range, &p);
  cv::createTrackbar( "min s", source_window, s_blue_min, 255, update_callback_range, &p);
  cv::createTrackbar( "min v", source_window, v_blue_min, 255, update_callback_range, &p);

  cv::createTrackbar( "max h", source_window, h_blue_max, 255, update_callback_range, &p);
  cv::createTrackbar( "max s", source_window, s_blue_max, 255, update_callback_range, &p);
  cv::createTrackbar( "max v", source_window, v_blue_max, 255, update_callback_range, &p);

  update_callback_range( 0, &p);

  while (cv::waitKey(0) != 10);

  cv::destroyWindow(source_window);
  cv::destroyWindow("selector_range_display");

  std::pair<cv::Vec3b,cv::Vec3b> d(p.p);

  std::cout << d << "\n";
  return d;
}

int
Cv_c4_option_helper::chose_int(int i, cv::Mat img)
{
  picker_context_int p;
  p.img = img;
  p.p = i;
  p.th = &this->cv;

  std::string source_window = "selector_int";
  cv::namedWindow( source_window, CV_WINDOW_NORMAL );
  cv::imshow( source_window, p.img );
  int v;
  int* val=&(v);
  cv::createTrackbar( "distance", source_window, val, 400, update_callback_int, &p);

  update_callback_int( 0, &p);

  while (cv::waitKey(0) != 10);

  cv::destroyWindow(source_window);
  cv::destroyWindow("selector_int_display");

  return p.p;
}



Cv_c4_option_helper::~Cv_c4_option_helper() {

}
