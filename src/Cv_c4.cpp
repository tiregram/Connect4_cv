#include "Cv_c4.hpp"

#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>


Cv_c4::Cv_c4(const Cv_c4_option& opt):opt(opt) {
  std::cout <<"-> Init Opencv"  << "\n";
}

std::vector<CV_BOARD_STATE> Cv_c4::predict_board(std::string img_path) {
  cv::Mat image;

  image = cv::imread(img_path, CV_LOAD_IMAGE_COLOR);

  if(image.data == NULL )
    {
      throw "Could not open or find the image";
    }

  return  this->predict_board(image);
}
std::vector<CV_BOARD_STATE> Cv_c4::predict_board(cv::Mat image)  {


  cv::Mat imagecvt;
  cv::Mat imageblur;


  cvtColor(image,imagecvt , cv::COLOR_BGR2HSV);

  // find the game
  auto t = isolate_Game(imagecvt);

  // correct rotation
  auto t1 =set_correct_rotation_for_board(t);

  // remap
  cv::Mat imageremap;
  remap_game(imagecvt,
             imageremap,
             t1);

  // can be blur
  cv::blur(imageremap,imageblur,cv::Size(10,10));

  cv::Mat imageB;

  cv::inRange(imageremap,
              this->opt.get_blue_board().first,
              this->opt.get_blue_board().second,
              imageB);

  auto list_center = centers(get_piece(imageB));

  std::vector<CV_BOARD_STATE> b = this->get_board_value(list_center, imageremap);

  return b;
}

std::vector<CV_BOARD_STATE>  Cv_c4::get_board_value(std::vector<cv::Point>& list_center,
                                                    cv::Mat imageblur ) {

  // TODO: out
  std::vector<CV_BOARD_STATE> board(6*7);
  for (unsigned int i = 0; i < board.size(); i++) {
    board[i]= CV_VIDE;
  }

  // board collect position and value /////////////////////////////////////////
  double w = this->opt.get_size_remap().width;
  double h = this->opt.get_size_remap().height;

  for (unsigned int i = 0 ; i < list_center.size(); i++) {

    int x = (list_center[i].x)/w*7;
    int y = (list_center[i].y)/h*6;

    cv::Vec3b c = imageblur.at<cv::Vec3b>(list_center[i]);

    CV_BOARD_STATE value_board = this->get_piece_color(c);

    board[x+7*y] = value_board;
  }

  return board;
}



CV_BOARD_STATE Cv_c4::get_piece_color(cv::Vec3b color) {

  if(this->opt.get_red_piece().first  < color &&
     color < this->opt.get_red_piece().second )
        {
          return CV_RED;
        }

  if(this->opt.get_green_piece().first  < color &&
     color < this->opt.get_green_piece().second )
      {
        return CV_GREEN;
      }

    return CV_VIDE;
}

Cv_c4::~Cv_c4() {

}

std::ostream& operator <<(std::ostream& flx, const std::vector<CV_BOARD_STATE>& board) {

  flx<< "╔═╦═╦═╦═╦═╦═╦═╗" <<std::endl;

  for(int y = 0; y < 6 ;y++ )
    {
      flx << "║";

      for(int x = 0; x <7 ;x++ )
        {
          int v = board[x+y*7];
          std::string color_elem;

          if(v == CV_VIDE || v == CV_UNDEFINED )
            color_elem = " ";
          else if( v == CV_RED)
            color_elem= "\033[1;31m◉\033[0m";
          else if( v == CV_GREEN)
            color_elem= "\033[1;32m◉\033[0m";

          flx << color_elem << "║";
        }

      flx << "\n";
    }

  flx<< "╚═╩═╩═╩═╩═╩═╩═╝" <<std::endl;

  return flx;
}

void Cv_c4::remap_game(const cv::Mat& image,
                       cv::Mat& imageremap,
                       const std::vector<cv::Point>&  quadri) {



  int h = this->opt.get_size_remap().height;
  int w = this->opt.get_size_remap().width;
  int h_margin =  this->opt.get_size_remap_margin().height;
  int w_margin = this->opt.get_size_remap_margin().width;

  //build the matrix to go
  std::vector<cv::Point2f> matrix_destination;
  matrix_destination.push_back(cv::Point2f(w_margin, h_margin));
  matrix_destination.push_back(cv::Point2f(w - w_margin, h_margin));
  matrix_destination.push_back(cv::Point2f(w_margin, h - h_margin));
  matrix_destination.push_back(cv::Point2f(w - w_margin, h - h_margin));

  // build the matrix were you are
  std::vector<cv::Point2f> matrix_source;
  matrix_source.push_back(cv::Point2f(quadri[2]));
  matrix_source.push_back(cv::Point2f(quadri[3]));
  matrix_source.push_back(cv::Point2f(quadri[1]));
  matrix_source.push_back(cv::Point2f(quadri[0]));

  // Opencv function to find the perspaective function
  auto matrice_changement = getPerspectiveTransform(matrix_source, matrix_destination);

  // wrap Perspaetive see the open cv doc
  warpPerspective(image, imageremap, matrice_changement, this->opt.get_size_remap());
}

std::vector<cv::Point> Cv_c4::set_correct_rotation_for_board(const std::vector<cv::Point> &  cnt)
{
  std::vector<cv::Point> cnt_result(4);

  if(cnt.size() != 4)
    {
      throw "ERROR size need 4 but " + std::to_string(cnt.size());
    }

  auto cnt_center = center(cnt);

  for(auto& point : cnt)
    {
      auto vec = point - cnt_center;
      int indice =-1;

      if (vec.x > 0 and vec.y > 0)
        indice = 0;

      if (vec.x < 0 and vec.y > 0)
        indice = 1;

      if (vec.x < 0 and vec.y < 0)
        indice = 2;

      if (vec.x > 0 and vec.y < 0)
        indice = 3;

      cnt_result[indice] =point;
    }

  return cnt_result;
}


// get the center of function
cv::Point Cv_c4::center(const std::vector<cv::Point>& contour)
{
  auto a = cv::moments(contour);

  int val_x=a.m10/a.m00;
  int val_y=a.m01/a.m00;

  return cv::Point(val_x,val_y);
}


std::vector<std::vector<cv::Point> > Cv_c4::get_piece(const cv::Mat& blue_board_Game)
{
  std::vector<std::vector<cv::Point> > contours;

  cv::findContours(blue_board_Game,
                   contours,
                   CV_RETR_TREE,
                   CV_CHAIN_APPROX_NONE);

  std::vector<std::vector<cv::Point> > cntaa;

  for (unsigned int idx = 0; idx < contours.size(); idx++) {

    double area  =cv::contourArea(contours[idx]);
    double epsilon = 0.05 * cv::arcLength(contours[idx],true);

    std::vector<cv::Point> approx;

    cv::approxPolyDP(contours[idx], approx, epsilon,true);

    if(area>50 && area < 1000)
      {
        cntaa.push_back(contours[idx]);
      }
  }


  return cntaa;
}

std::vector<cv::Point> Cv_c4::isolate_Game(const cv::Mat& image_where_search_hsv) 
{
  cv::Mat blue_selection;

  cv::inRange(image_where_search_hsv,
              this->opt.get_blue_board().first,
              this->opt.get_blue_board().second,
              blue_selection);

  std::vector<std::vector<cv::Point> > contours;
  std::vector<cv::Vec4i> hierarchy;

  cv::findContours( blue_selection, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

  unsigned int best_i = 0;
  double best = 0;

  if(contours.size() == 0)
    throw "ERROR: no blue ellement detected";

  for( unsigned int i = 0; i< contours.size(); i++ )
    {
      double a = cv::contourArea(contours[i]);
      if(a > best )
        {
          best_i = i;
          best = a;
        }
    }

  //  auto t = cv::boundingRect(contours[best_i]);

  //convex hull try

  std::vector<cv::Point> llo;

  cv::approxPolyDP(contours[best_i],llo,this->opt.get_distance_value(),true);

  return llo;
}


// get all center
std::vector<cv::Point> Cv_c4::centers(const std::vector<std::vector<cv::Point> >& contours) {

  std::vector<cv::Point> list_center;

  for(auto contour : contours ){
    list_center.push_back(center(contour));
  }

  return list_center;
}

