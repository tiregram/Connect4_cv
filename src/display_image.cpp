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

  inRange(imagecvt, Scalar(  5,180,  0), Scalar( 25,255,255), imageR);
  inRange(imagecvt, Scalar( 34,100,  0), Scalar( 75,255,255), imageG);
  inRange(imagecvt, Scalar(100,100,  0), Scalar(140,255,255), imageB);

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

    if(isContourConvex(approx))
      if(area>20 && area < 200)
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
int piece_color(Vec3b color)
{

  if(7   < color[0] && color[0] < 25  &&
     100 < color[1] && color[1] < 255 &&
     0   < color[2] && color[2] < 255)
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
  cout << a.m10/a.m00<<","<<a.m01/a.m00<<std::endl;
  int val_x=a.m10/a.m00;
  int val_y=a.m01/a.m00;

  return Point(val_x,val_y);
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
  cout<<llo<<std::endl;
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

  Mat undefm;
  auto t = isolate_Game(imagecvt,false);

  // Matx<int,7,6> mat;


  // for(auto i : list_center ){

  //   Matx<int,2,1>  pi = (i-po);

  //   Matx<float,2,1> p(pi);

  //   Matx<int,2,1> aze = (m * p);

  //   int x = (int) round(aze(0,0));
  //   int y = (int) round(aze(1,0));

  //   Vec3b colora = imagecvt.at<Vec3b>(i);

  //   if(not( 0 <= x and x < 7 and
  //           0 <=y and  y < 6 ))
  //     {
  //       cout << "error size " <<x<<":"<<y<< "\n";
  //       continue;
  //     }

  //   mat(x,y) = piece_color(colora);

  // }

  // cout << mat.t()<<std::endl;



  // for (int i = 0; i < list_center.size(); i++) {
  //   line(a,list_center[i],list_center[i+1],colors[i%3]);
  // }

  // cv::drawContours(a, contours, top, colors[0 % 3]);
  // cv::drawContours(a, contours, top2, colors[1 % 3]);


  //  vector<Vec3f> circles;

  //  /// Apply the Hough Transform to find the circles
  //  for (int i = 50; i < 100; i= i+10) {
  //    for (int j = 50; j < 100; j= j+10) {
  //      HoughCircles( imageGr, circles, CV_HOUGH_GRADIENT, 1, 50 , i, j, 5, 500 );
  //      std::cout << circles.size() << "\n";
  //    }
  // }
  //  HoughCircles( imageR, circles, CV_HOUGH_GRADIENT, 1, 100, 200 , 18, 10, 300);

  // std::cout << circles.size() << "\n";
  // /// Draw the circles detected
  // for( size_t i = 0; i < circles.size(); i++ )
  //   {
  //     Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
  //     int radius = cvRound(circles[i][2]);
  //     // circle center
  //     circle( image, center, 3, Scalar(0,255,0), -1, 8, 0 );
  //     // circle outline
  //     circle( image, center, radius, Scalar(0,0,255), 3, 8, 0 );
  //   }

  Mat imageremap;
  std::vector<Point2f> p;
  p.push_back(Point2f(2,2));
  p.push_back(Point2f(198,2));
  p.push_back(Point2f(2,198));
  p.push_back(Point2f(198,198));

  std::vector<Point2f> p2;
  p2.push_back(Point2f(t[0]));
  p2.push_back(Point2f(t[3]));
  p2.push_back(Point2f(t[1]));
  p2.push_back(Point2f(t[2]));

  auto M = getPerspectiveTransform(p2, p);
  warpPerspective(image,imageremap,M,Size(200,200));

  // namedWindow( "omap", CV_WINDOW_NORMAL );
  // imshow( "omap", image);

  namedWindow( "map", CV_WINDOW_NORMAL );
  imshow( "map", imageremap);
  while(waitKey(0)!=10);


  //  blur(imageremap,imageblur,cv::Size(5,5));
  Mat imageR;
  Mat imageG;
  Mat imageB;

  color_filter(imageremap,imageB,imageR,imageG,true);

  //Find the contours. Use the contourOutput Mat so the original image doesn't get overwritten
  //dilate(imageB, imageBr,Mat(),Point(-1,-1),2);
  Mat a(imageB.size(), CV_8UC3, cv::Scalar(0,0,0));
  auto list_center = centers(get_piece(imageB,false ,&a));

  std::sort(list_center.begin(),
            list_center.end(),
            [](Point a,Point b)
            {
              return a.y * 10 + a.x < b.y *  10 + b.x;
            }
            );

  Point po = list_center[0];
  Point px = list_center[1];
  Point py = list_center[7];

  Vec<int,2> vx = px- po;
  Vec<int,2> vy = py- po;

  Matx<float,2,2> v(vx[0],vy[0],
                    vx[1],vy[1]);

  Matx<float,2,2> m = v.inv();

  cout << m << "\n";


  cout <<"vx"<< vx << "\n";
  cout <<"vy"<< vy << "\n";

  draw_supose_game(vy, vx, po,&a,false);

  Matx<int,7,6> mat;
  Mat imagecvt2;
  cvtColor(imageremap,imagecvt2 , cv::COLOR_BGR2HSV);

  for(auto i : list_center ){

    Matx<int,2,1>  pi = (i-po);

    Matx<float,2,1> p(pi);

    Matx<int,2,1> aze = (m * p);

    int x = (int) round(aze(0,0));
    int y = (int) round(aze(1,0));

    Vec3b colora = imagecvt2.at<Vec3b>(i);

    if(not( 0 <= x and x < 7 and
            0 <= y and  y < 6 ))
      {
        cout << "error size " <<x<<":"<<y<< "\n";
        continue;
      }

    mat(x,y) = piece_color(colora);

  }

  cout << mat.t() << std::endl;


  while(waitKey(0)!=10);

  return 0;

}
