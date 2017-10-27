#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <string>


using namespace cv;


void  color_filter(Mat image_in,
                   Mat& imageB,
                   Mat& imageR,
                   Mat& imageG,
                   bool display=true)
{
  Mat imagecvt;

  cvtColor(image_in,imagecvt , cv::COLOR_BGR2HSV);

  inRange(imagecvt, Scalar(  0,180,  0), Scalar( 30,255,255), imageR);
  inRange(imagecvt, Scalar( 34,100,  0), Scalar( 75,255,255), imageG);
  inRange(imagecvt, Scalar(100,100,  70), Scalar(140,255,200), imageB);

  if(display){
    namedWindow( "B", CV_WINDOW_NORMAL );
    namedWindow( "R", CV_WINDOW_NORMAL );
    namedWindow( "G", CV_WINDOW_NORMAL );

    imshow( "R", imageR );
    imshow( "B", imageB );
    imshow( "G", imageG );
  }
}

using namespace std;


// TODO selector
std::vector<std::vector<Point> >  get_piece(const Mat & blue_board_Game, bool display = true, Mat* draw_in=NULL )
{
  std::vector<std::vector<Point> > contours;

  findContours(blue_board_Game, contours, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);

  //Draw the contours
  cv::Scalar colors[3];
  colors[0] = cv::Scalar(255, 0, 0);
  colors[1] = cv::Scalar(0, 255, 0);
  colors[2] = cv::Scalar(0, 0, 255);

  Mat a;

  if (draw_in == NULL)
    a= Mat(blue_board_Game.size(), CV_8UC3, cv::Scalar(0,0,0));
  else
    a = *draw_in;

  std::vector<std::vector<Point> > cntaa;

  for (size_t idx = 0; idx < contours.size(); idx++) {

    double area  =contourArea(contours[idx]);
    double epsilon = 0.05*arcLength(contours[idx],true);

    std::vector<Point> approx;

    approxPolyDP(contours[idx], approx, epsilon,true);

    //    if(isContourConvex(approx))
      if(area>300 && area < 2000)
        {
          cntaa.push_back(contours[idx]);
          if (display || draw_in != NULL) {
            cv::drawContours(a, contours, idx, colors[idx % 3]);
          }
        }
  }

  if (display) {
    namedWindow( "Remap", CV_WINDOW_NORMAL );
    imshow( "Remap", a);
  }

  return cntaa;
}



// TODO with blue color
int get_piece_color(Vec3b color)
{

  if(0   < color[0] && color[0] < 25  &&
     170 < color[1] && color[1] < 255 &&
     0   < color[2] && color[2] < 200)
    {
      return 1;
    }
  // inRange(imagecvt, Scalar(34,100,0), Scalar(75,255,255) ,imageG);
  if(34 < color[0] && color[0] < 75 &&
     100  < color[1] && color[1] < 255 &&
     0   < color[2] && color[2] < 255)
    {
      return 2;
    }

  return 3;
}

// get the center of function
Point center(std::vector<Point> contour)
{
  auto a = cv::moments(contour);

  int val_x=a.m10/a.m00;
  int val_y=a.m01/a.m00;

  return Point(val_x,val_y);
}

void set_correct_rotation_for_board(std::vector<cv::Point>& cnt)
{
  if(cnt.size() != 4)
    {
      throw "ERROR size need 4 but " + cnt.size();
    }

  if(not(cnt[3].x > cnt[1].x and cnt[3].y < cnt[1].y))
    {
      auto swap = cnt[3];
      cnt[3] = cnt[1];
      cnt[1] = swap;
    }
}

// isolate the board game with a contour
std::vector<Point> isolate_Game(cv::Mat image_where_search_hsv,
                      bool display=false,
                      cv::Mat * draw_in = NULL)
{
  cv::Mat blue_selection;

  inRange(image_where_search_hsv, Scalar(100,100,0), Scalar(130,255,255) ,blue_selection);


  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  findContours( blue_selection, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
  RNG rng(12345);

  Mat a;
  if (draw_in == NULL)
    a= Mat(image_where_search_hsv.size(), CV_8UC3, cv::Scalar(0,0,0));
  else
    a = *draw_in;


  unsigned int best_i = 0;
  double best = 0;

  for( unsigned int i = 0; i< contours.size(); i++ )
    {
      double a = contourArea(contours[i]);
      if(a > best )
        {
          best_i = i;
          best = a;
        }
    }

  Scalar color1 = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );

  auto t = boundingRect(contours[best_i]);

  rectangle( a, t.tl(), t.br(), color1, 2, 8, 0 );

  Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
  drawContours( a, contours, best_i, color, 2, 8, hierarchy, 0, Point() );

  //convex hull try
  std::vector<Point> llo;
  approxPolyDP(contours[best_i],llo,100,true);

  std::vector<std::vector<Point> > elo;
  elo.push_back(llo);

  drawContours( a, elo, 0, Scalar(0,0,255), 2, 8, hierarchy, 0, Point() );
  if(display){
    namedWindow( "origin", CV_WINDOW_NORMAL );
    imshow( "origin", a);
  }

  return llo;
}


// get all center
std::vector<Point>  centers(std::vector<std::vector<cv::Point> > contours) {

  std::vector<Point> list_center;

  for(auto contour : contours ){
    list_center.push_back(center(contour));
  }

  return list_center;

}

// main
void draw_supose_game(Vec<int,2> vy, Vec<int,2> vx, Point po, Mat* draw_in=NULL, bool display=false) {

  // line
  for (int i =0; i < 7; i++) {
    for (int j = 0; j < 6; j++) {
      if (i != 0) {
        line(*draw_in,
             po + (i-1) * Point(vx) + j * Point(vy) ,
             po + i     * Point(vx) + j     * Point(vy) ,
             Scalar(0,0,255));
      }
      if (j != 0) {
        line(*draw_in,
             po + i * Point(vx) + (j-1) * Point(vy) ,
             po + i     * Point(vx) + j     * Point(vy) ,
             Scalar(0,255,0));
      }
    }
  }

  if(display){
    namedWindow( "origin", CV_WINDOW_NORMAL );
    imshow( "origin", *draw_in);
  }
}

void remap_game(const Mat image,
                Mat& imageremap,
                const std::vector<Point>&  quadri,
                cv::Size size,
                int margin =2,
                bool display=false) {



  int h = size.height;
  int w = size.width;

  //build the matrix to go
  std::vector<Point2f> matrix_destination;
  matrix_destination.push_back(Point2f(margin,margin));
  matrix_destination.push_back(Point2f(w-margin,margin));
  matrix_destination.push_back(Point2f(margin,h-margin));
  matrix_destination.push_back(Point2f(w-margin,h-margin));

  // build the matrix were you are
  std::vector<Point2f> matrix_source;
  matrix_source.push_back(Point2f(quadri[0]));
  matrix_source.push_back(Point2f(quadri[3]));
  matrix_source.push_back(Point2f(quadri[1]));
  matrix_source.push_back(Point2f(quadri[2]));

  // opencv function to find the perspaective function
  auto matrice_changement = getPerspectiveTransform(matrix_source, matrix_destination);

  // wrap Perspaetive see the open cv doc
  warpPerspective(image,imageremap,matrice_changement,size);



  if(display){
    namedWindow( "image_remap", CV_WINDOW_NORMAL );
    imshow( "image_remap", imageremap);
  }
}

std::vector<int>  get_board_value(std::vector<cv::Point>& list_center, Mat imageblur,bool display=false ) {


  std::vector<int> board(6*7);
  for (int i = 0; i < board.size(); i++) {
    board[i]= 3;
  }

  cv::Mat color_rgb(1,list_center.size(),CV_8UC3);
  cv::Mat color_hsv;

  for(int i = 0 ; i < list_center.size(); i++)
    {
      color_rgb.at<cv::Vec3b>(i) =  imageblur.at<cv::Vec3b>(list_center[i]);
    }

  cvtColor(color_rgb,color_hsv,cv::COLOR_BGR2HSV);

  for (int i = 0 ; i < list_center.size(); i++) {

    int x = (list_center[i].x)/400.0*7;
    int y = (list_center[i].y)/400.0*6;
    Vec3b c =  color_hsv.at<cv::Vec3b>(i);

    int value_board = get_piece_color(c);

    std::string x_y_value = (//std::to_string(x)+","+
                             //std::to_string(y)+"-"+
                             std::to_string(value_board));

    cv::putText(imageblur, x_y_value  , list_center[i], 1, 1, Scalar(255,255,0));

    // avoid the blue detection
    if(value_board != 3)
      board[x+7*y] = value_board;
  }
  return board;
  //  return board;
}

int main( int argc, char** argv )
{

  if( argc != 2)
    {
      cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
      return -1;
    }

  Mat image;
  Mat imageblur;
  Mat imagecvt;

  image = imread(argv[1], CV_LOAD_IMAGE_COLOR);

  if(! image.data )
    {
      cout <<  "Could not open or find the image" << std::endl ;
      return -1;
    }

  // cvtColor(image,imageGr , cv::COLOR_BGR2GRAY);
  blur(image,imageblur,cv::Size(5,5));
  cvtColor(imageblur,imagecvt , cv::COLOR_BGR2HSV);

  // find the game
  auto t = isolate_Game(imagecvt,false);
  // correct rotation
  set_correct_rotation_for_board(t);

  // remap
  Mat imageremap;
  remap_game(image,imageremap,t,cv::Size(400,400),2,false);

  // can be blur
  blur(imageremap,imageblur,cv::Size(10,10));

  Mat imageR;
  Mat imageG;
  Mat imageB;

  color_filter(imageblur,imageB,imageR,imageG,true);

  //Find the contours. Use the contour Output Mat so the original image doesn't get overwritten
  //dilate(imageB, imageBr,Mat(),Point(-1,-1),2);
  Mat a(imageB.size(), CV_8UC3, cv::Scalar(0,0,0));

  auto list_center = centers(get_piece(imageB,false ,&a));

  auto board = get_board_value(list_center, imageblur);

  cout<< "╔═╦═╦═╦═╦═╦═╦═╗" <<std::endl;

  for(int y = 0; y < 6 ;y++ )
    {
      cout << "║";

      for(int x = 0; x <7 ;x++ )
        {
          int v = board[x+y*7];
          std::string color_elem;

          if(v ==3 )
            color_elem = " ";
          else if( v== 1)
            color_elem= "\033[1;31m◉\033[0m";
          else if( v== 2)
            color_elem= "\033[1;32m◉\033[0m";

          cout << color_elem << "║";
        }
      cout << "\n";
    }

  cout<< "╚═╩═╩═╩═╩═╩═╩═╝" <<std::endl;

  return 0;

}
