#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "Cv_c4_option_helper.hpp"
#include "Cv_c4_optim.hpp"
#include <ctime>



void optim_run(CV_BOARD_STATE expect, std::vector<CV_BOARD_STATE> old_board, Cv_c4 cv, cv::VideoCapture vcap) {
  Cv_c4_optim  optim_cv(cv);
  std::cout <<"run optim:"  << "\n";
  while(1){
    std::cout << ((expect==CV_RED)?"RED":"GREEN") << " TURN"  << "\n";
    old_board = optim_cv.predict_next_board(old_board,8,10,vcap,expect);
    std::cout<<old_board<<std::endl;
    expect = (expect==CV_RED)? CV_GREEN:CV_RED;
  }

}

void base_run(Cv_c4 cv, cv::VideoCapture vcap) {

  cv::Mat image;

  cv::namedWindow("Output Window", cv::WINDOW_NORMAL );



  for(;;) {

    if(!vcap.read(image)) {
      std::cout << "No frame" << std::endl;
      cv::waitKey();
    }

    clock_t begin = std::clock();
    try {
            std::cout << cv.predict_board(image) << "\n";
            //cv.predict_board(image);

    }
    catch (const char * a) {
      std::cout << a << "\n";
    }
    catch (const std::string a) {
      std::cout << a << "\n";
    }


    clock_t end = std::clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    std::cout << elapsed_secs << "\n";

    cv::imshow("Output Window", image);
    auto c = cv::waitKey(1);

    if(c == 10) break;

  }
}

void config_run(Cv_c4_option_helper& cvh,  cv::VideoCapture vcap) {
  cv::Mat image;
  vcap.read(image);
  cvh.chose_blue(image);
  cvh.chose_red(image);
  cvh.chose_green(image);
  std::cout << cvh.cv.opt<<std::endl;
}

int main(int argc, char* argv[]) {
  cv::VideoCapture vcap;

  if(argc!=4)
    {
      std::cout << "need 4 arg" << "\n";
    }
  // This works on a D-Link CDS-932L
  std::cout << argv[1] << "\n";
  const std::string videoStreamAddress(argv[1]);

  //open the video stream and make sure it's opened
  if(!vcap.open(videoStreamAddress)) {
    std::cout << "Error opening video stream or file" << std::endl;
    return -1;
  }

  Cv_c4_option opt;
  opt.load(argv[2]);



  if(std::string(argv[3]) == "config")
    {
      Cv_c4_option_helper cvh(opt);
      config_run(cvh, vcap);
      cvh.cv.opt.save(std::string(argv[2]));

      return 0;
    }



  if(std::string(argv[3]) == "optim")
    {
      Cv_c4 cv(opt);
      std::vector<CV_BOARD_STATE> old_board(6*7,CV_VIDE);
      CV_BOARD_STATE expect = CV_RED;

      optim_run(expect, old_board,  cv, vcap);
      return 0;
    }

  if(std::string(argv[3]) == "Base")
    {
      Cv_c4 cv(opt);
      base_run(cv, vcap);
      return 0;
    }
}
