#include "Cv_c4.hpp"
#include <opencv2/opencv.hpp>


class Cv_c4_optim
{
  Cv_c4& cv;
public:
  Cv_c4_optim(Cv_c4& p_cv);

  std::vector<CV_BOARD_STATE> predict_next_board(std::vector<CV_BOARD_STATE> prev_game,unsigned int valid,unsigned int nb_echantillon, cv::VideoCapture& vc,CV_BOARD_STATE change_want) const;

  virtual ~Cv_c4_optim();
};
