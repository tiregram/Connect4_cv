#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "Cv_c4_option_helper.hpp"
#include "Cv_c4_optim.hpp"

int main(int argc, char* argv[]) {
  cv::VideoCapture vcap;
  cv::Mat image;

  // This works on a D-Link CDS-932L
  std::cout << argv[1] << "\n";
  const std::string videoStreamAddress(argv[1]);

  //open the video stream and make sure it's opened
  if(!vcap.open(videoStreamAddress)) {
    std::cout << "Error opening video stream or file" << std::endl;
    return -1;
  }

  Cv_c4_option opt;

  if(argc > 2)
    opt.load(argv[2]);

  Cv_c4_option_helper cvh(opt);

  if(std::string(argv[3]) == "config")
    {
      vcap.read(image);
      cvh.chose_blue(image);
      cvh.chose_red(image);
      cvh.chose_green(image);
    }



  Cv_c4 cv(opt);

  if(std::string(argv[4]) == "optim")
    {
      Cv_c4_optim  optim_cv(cv);
      std::vector<CV_BOARD_STATE> old_board(6*7,VIDE);
      CV_BOARD_STATE expect = RED;

      while(1){
        std::cout << ((expect==RED)?"RED":"GREEN") << " TURN"  << "\n";
        old_board = optim_cv.predict_next_board(old_board,5,10,vcap,expect);
        std::cout<<old_board<<std::endl;
        expect = (expect==RED)? GREEN:RED;
    }
      return 0;
    }

  cv::namedWindow("Output Window", CV_WINDOW_NORMAL );

  for(;;) {
    if(!vcap.read(image)) {
      std::cout << "No frame" << std::endl;
      cv::waitKey();
    }

    try {
      std::cout << cv.predict_board(image) << "\n";
    }
    catch (const char * a) {
      std::cout << a << "\n";
    }
    catch (const std::string a) {
      std::cout << a << "\n";
    }



    cv::imshow("Output Window", image);
    auto c = cv::waitKey(1);

    if(c == 10) break;

  }
}
