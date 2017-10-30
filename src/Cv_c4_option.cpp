#include "Cv_c4_option.hpp"
#include <fstream>
#include <iostream>

Cv_c4_option::Cv_c4_option(std::pair<cv::Vec3b,cv::Vec3b> p_board_selector,
                           std::pair<cv::Vec3b,cv::Vec3b> p_red_selector,
                           std::pair<cv::Vec3b,cv::Vec3b> p_green_selector,
                           int p_distance_value,
                           int p_blur,
                           cv::Size p_size_remap,
                           cv::Size p_size_remap_margin)
  :blue_board(p_board_selector),
   red_piece(p_red_selector),
   green_piece(p_green_selector),
   distance_value(p_distance_value),
   blur(p_blur),
   size_remap(p_size_remap),
   size_remap_margin(p_size_remap_margin)
{

}


Cv_c4_option::Cv_c4_option()
  :Cv_c4_option(std::make_pair<cv::Vec3b,cv::Vec3b>(cv::Vec3b(3,10,70),cv::Vec3b(15,255,200)),
                std::make_pair<cv::Vec3b,cv::Vec3b>(cv::Vec3b(30,75,70),cv::Vec3b(80,255,200)),
                std::make_pair<cv::Vec3b,cv::Vec3b>(cv::Vec3b(100,0,0),cv::Vec3b(140,255,255)),
                100,
                5,
                cv::Size(200,200),
                cv::Size(5,5))
{

}


Cv_c4_option::Cv_c4_option(std::string file_path) {
  throw "error not done";
}

Cv_c4_option::~Cv_c4_option() {

}

void Cv_c4_option::save(std::string file_path) {
  std::fstream fs;
  fs.open (file_path,  std::fstream::out );
  fs << *(this);
  fs.close();
}

// vector<string> split(std::string s, string delimiter){
//   vector<string> list;
//   size_t pos = 0;
//   string token;

//   while ((pos = s.find(delimiter)) != string::npos) {
//     token = s.substr(0, pos);
//     list.push_back(token);
//     s.erase(0, pos + delimiter.length());
//   }

//   return list;
// }

void Cv_c4_option::load(std::string file_path) {
  std::ifstream fs;

  char vide[50];
  char name[50];

  fs.open (file_path,  std::fstream::in );

  do
    {
      fs.getline(vide, 50,'"');
      fs.getline(name, 50,'"');

      std::string name_s(name);

      if(name_s == "board" )
        {
          int x,y,z;
          fs.ignore(100,'[');
          fs >> x;
          fs.ignore(100,',');
          fs >> y;
          fs.ignore(100,',');
          fs >> z;

          int u,v,w;
          fs.ignore(100,'[');
          fs >> u;
          fs.ignore(100,',');
          fs >> v;
          fs.ignore(100,',');
          fs >> w;

          auto board = std::make_pair<cv::Vec3b,cv::Vec3b>(cv::Vec3b(x,y,z),cv::Vec3b(u,v,w));
          this->set_blue_board(board);
        }
      else if(name_s == "piece_1")
        {
          int x,y,z;
          fs.ignore(100,'[');
          fs >> x;
          fs.ignore(100,',');
          fs >> y;
          fs.ignore(100,',');
          fs >> z;

          int u,v,w;
          fs.ignore(100,'[');
          fs >> u;
          fs.ignore(100,',');
          fs >> v;
          fs.ignore(100,',');
          fs >> w;

          auto p1 = std::make_pair<cv::Vec3b,cv::Vec3b>(cv::Vec3b(x,y,z),cv::Vec3b(u,v,w));
          this->set_red_piece(p1);
        }
      else if(name_s == "piece_2")
        {
          int x,y,z;
          fs.ignore(100,'[');
          fs >> x;
          fs.ignore(100,',');
          fs >> y;
          fs.ignore(100,',');
          fs >> z;

          int u,v,w;
          fs.ignore(100,'[');
          fs >> u;
          fs.ignore(100,',');
          fs >> v;
          fs.ignore(100,',');
          fs >> w;

          auto p2 = std::make_pair<cv::Vec3b,cv::Vec3b>(cv::Vec3b(x,y,z),cv::Vec3b(u,v,w));
          this->set_green_piece(p2);
        }
      else if(name_s == "distance")
        {
          int v;
          fs.ignore(100,':');
          fs >> v;
          this->set_distance_value(v);
        }
      else if(name_s == "blur")
        {
          int v;
          fs.ignore(100,':');
          fs >> v;
          this->set_blur(v);
        }
      else if(name_s == "size_remap")
        {
          int h,w;
          fs.ignore(100,'[');
          fs >> w ;
          fs.ignore(100,'x');
          fs >> h ;
          fs.ignore(100,']');
          this->set_size_remap(cv::Size(w,h));
        }
      else if(name_s == "size_remap_margin")
        {
          int h,w;
          fs.ignore(100,'[');
          fs >> w ;
          fs.ignore(100,'x');
          fs >> h ;
          fs.ignore(100,']');
          this->set_size_remap_margin(cv::Size(w,h));
        }
      else if (name_s == "")
        {
          std::string vide;
          fs >> vide;
        }
      else
        {
          throw "Erreur name not implemented";
        }

      std::cout << name_s << " [x]"<< "\n";

    } while (! fs.eof());


  std::cout << "fin de fichier"  << "\n";

  fs.close();



}


bool Cv_c4_option::check() {
  return check_color();
  // c1 && c2
}

bool Cv_c4_option::check_color() {
  return check_color_collision() && check_color_value();

}

bool Cv_c4_option::check_color_collision() {
  // no colition on red green blue
  return true;

}

bool Cv_c4_option::check_color_value() {
  // hsv range and min < max
  return true;
}


namespace std {
  ostream& operator <<(ostream& flx, const pair<cv::Vec3b,cv::Vec3b>& v)
  {
    flx << "{"
        << v.first
        << ","
        << v.second
        << "}";

    return flx;
  }

  ostream& operator <<(ostream& c, const Cv_c4_option& t )
  {
    c << "{"
      // blue
      << "\"board\":"
      << t.get_blue_board()
      << ","
      // red
      << "\"piece_1\":"
      << t.get_red_piece()
      << ","
      // green
      << "\"piece_2\":"
      << t.get_green_piece()
      << ","
      // distance
      << "\"distance\":"
      << t.get_distance_value()
      << ","
      // blur
      << "\"blur\":"
      << t.get_blur()
      << ","
      // remap
      << "\"size_remap\":"
      << t.get_size_remap()
      << ","
      //remap margin
      << "\"size_remap_margin\":"
      << t.get_size_remap_margin()
      <<"}";

    return c;
  }
}
