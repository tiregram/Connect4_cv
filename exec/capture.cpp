#include <iostream>
#include <vector>
#include "Cv_c4_option_helper.hpp"
#include "Cv_c4.hpp"

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

cv::Mat take_picture(cv::VideoCapture& vcap)
{
  cv::Mat m;
  while(true)
    {
      // capture en boucle
      while(!vcap.read(m))
        {
          std::cout << "No frame retry..." << std::endl;
        }


      cv::imshow("Al",m);
      cv::waitKey(1);

      std::cout << "valid? (Y,n)" << std::endl;

      char c;
      std::cin >> c;
      if(c == 'N' || c == 'n')
        {
          continue;
        }
      else
        {
          return m;
        }
    }
}

std::vector<CV_BOARD_STATE> take_board(const cv::Mat& m, Cv_c4& cv)
{
  std::vector<CV_BOARD_STATE> b;

  while(true)
    {
      try {
        b = cv.predict_board(m);
        std::cout << b  << "\n";
        std::cout << "valid? (Y,n)" << std::endl;
        char c;
        std::cin >> c;
        if(c == 'N' || c == 'n')
          {
            continue;
          }
        else
          {
            return b;
          }

      }
      catch (const char * a) {
        std::cout << a << "\n";
        continue;
      }
      catch (const std::string a) {
        std::cout << a << "\n";
        continue;
      }
      break;
    }


}


void create_set(const std::string& name, unsigned int v,char* video_IP)
{
  cv::Mat m ;
  std::vector<CV_BOARD_STATE> b;
  cv::VideoCapture vcap;

  const std::string videoStreamAddress(video_IP);

  //open the video stream and make sure it's opened
  if(!vcap.open(videoStreamAddress)) {
    std::cout << "Error opening video stream or file" << std::endl;
    return;
  }

  // loop
  while(true)
    {
      m = take_picture(vcap);
      b = 
      //take_board();

      std::cout << "exit (y:N)" << "\n";
      char answer;
      std::cin >> answer;

      if(answer=='y' || answer=='Y' )
        break;
    }
  // take picture
  // take position

  // validate

  // end loop

}


int main(int argc, char *argv[])
{
  cv::namedWindow("Al", CV_WINDOW_NORMAL );

  if(argc != 2 )
    {
      std::cout << "error command exec IPWEBCAM" << "\n";
      return -1;
    }

  cv::Mat image;



  std::string name;
  unsigned int v;
  std::cout << "name of the set" << "\n";
  std::cin  >> name;
  std::cout << "start id"  << "\n";
  std::cin >> v;


  char chose;
  std::cout << "config cam y/N" << "\n";
  std::cin >> chose;
  if( chose=='Y' || chose=='y' )
    {
      std::cout <<" configuration!"  << "\n";
      // TODO: 
    }

  create_set(name,v,argv[1]);

  return 0;
}


