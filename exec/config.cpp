
#include <iostream>
#include "Cv_c4_option_helper.hpp"

int main(int argc, char *argv[])
{
  if(not (3 == argc || argc == 4))
    {
      std::cout << "cv_c4_config img_path conf_file"  << "\n";
      std::cout << "\timg_path:  path to the image to do the initialisation\n";
      std::cout << "\tconf_file_path_out:  path to save the configuration\n";
      std::cout << "\tconf_file_path_in:  (option) path to load the configuration\n";
      return 0;
    }

  std::string answer;

  std::string path_img(argv[1]);
  // default conversation
  Cv_c4_option opt;

  if(argc == 4 )
    opt.load(std::string(argv[3]));


  Cv_c4_option_helper cv_h(opt);



  do
  {
    cv_h.chose_blue(path_img);
    cv_h.chose_red(path_img);
    cv_h.chose_green(path_img);
    cv_h.chose_distance(path_img);

    std::vector<CV_BOARD_STATE> a = cv_h.cv.predict_board(path_img);


    std::cout << "board predict:\n"
              << a << "\n";

    std::cout << "The result are ok ?(YES/no)";

    std::cin >> answer ;
    std::cout << "-"<<answer<<"-";
  } while (answer != "yes");


  std::cout << "config save to "<< argv[2] << "\n";
  cv_h.cv.opt.save(argv[2]);

  return 0;
}
