
#include "Cv_c4_optim.hpp"
#include <iostream>
#include <functional>
#include <unistd.h>

Cv_c4_optim::Cv_c4_optim(Cv_c4& p_cv):cv(p_cv) {

}

struct Board_element_diff
{
  int x,y;
  CV_BOARD_STATE v1;
  CV_BOARD_STATE v2;
  unsigned n;
};

std::vector<Board_element_diff> diff(std::vector<CV_BOARD_STATE> a,std::vector<CV_BOARD_STATE> b)
{
  std::vector<Board_element_diff> list_diff;
  for (int x = 0; x < 7; x++) {
    for (int y = 0; y < 6; y++) {
      if(a[x+y*7] != b[x+y*7])
        {
          Board_element_diff e = {x,y,a[x+y*7], b[x+y*7],1};
          list_diff.push_back(e);
        }
    }
  }
  return list_diff;
}

void  merge(std::vector<Board_element_diff>& list , Board_element_diff el)
{
  bool insert = true;
  std::for_each(std::begin(list),std::end(list),
                [el,&insert](Board_element_diff& a)
                {
                  if(el.x == a.x && el.y == a.y)
                    {
                      a.n ++ ;
                      insert = false;
                    }
                });
  if(insert)
    list.push_back(el);
}

bool is_a_valid_evolution(Board_element_diff o,std::vector<CV_BOARD_STATE> b,CV_BOARD_STATE change_want)
{
  // range check X
  if(not (0 <= o.x  && o.x <= 6))
    return false;

  // range check Y
  if(not (0 <= o.y  && o.y <= 5))
    return false;

  // right color change
  if (not ((o.v1 == VIDE || o.v1 == UNDEFINED) && (o.v2 == change_want)))
    {
      return  false;
    }

  //  gravity
  // TODO:  gravity
  // not gravity for the bottom
  if(o.y != 5 )
    {
      if(b[o.x+(o.y+1)*7] == VIDE)
         return false;
    }

  return true;
}

std::vector<CV_BOARD_STATE> Cv_c4_optim::predict_next_board(std::vector<CV_BOARD_STATE> prev_game, unsigned int valid,unsigned int nb_echantillon, cv::VideoCapture& vc,CV_BOARD_STATE change_want) const {

  cv::Mat img;
  while(1){

    std::vector<CV_BOARD_STATE> b;
    std::vector<Board_element_diff> lapin;



    for (unsigned int i = 0; i < nb_echantillon; i++) {
      //      std::cout << "______ try ______ "<< i << "\n";

      std::vector<CV_BOARD_STATE> g;
      std::vector<Board_element_diff> d;
      std::vector<Board_element_diff> d_filter;
      auto f = std::bind(is_a_valid_evolution, std::placeholders::_1, prev_game,change_want);
      try{

        if(!vc.read(img)) {
          std::cout << "No frame" << std::endl;
          cv::waitKey();
          continue;
        }

        g = this->cv.predict_board(img);
        d = diff(prev_game,g);

        if(d.size() == 0) continue;

        auto it= std::find_if(std::begin(d),std::end(d),f);

        d_filter= std::vector<Board_element_diff>(it,std::end(d));

        if(d_filter.size() == 0) continue;

      }
      catch(char const * a)
        {
          std::cout << a << "\n";
        }
      catch(const std::string& a)
        {
          std::cout << a << "\n";
        }
      if(d_filter.size() == 1)
        {
          merge(lapin, d_filter[0]);
        }
      else
        {
          std::cout << "error df " << d_filter.size()<<std::endl;
        }

    }

    if(lapin.size()==0)
      {
        //        std::cout <<"not board evolution"<<std::endl;
        continue;
      }

    //    std::cout << "evolution choice "<< lapin.size() <<"\n";

    std::sort(std::begin(lapin),std::end(lapin),[](Board_element_diff a ,Board_element_diff b){return a.n > b.n; });

    // check if the best are in the valid condition
    if(valid > lapin[0].n)
      {
        //        std::cout << "not board with valid ";//+lapin[0].n+"/"+valid;
        continue;
      }

    // apply the evolution
    prev_game[lapin[0].x + lapin[0].y * 7] = lapin[0].v2;

    return prev_game;
  }
}


void add(std::vector<int>& r,
         std::vector<int>& g,
         const std::vector<CV_BOARD_STATE>& new_prediction)
{

  for (int x = 0; x < 6; x++) {
    for (int y = 0; y < 7; y++) {
      CV_BOARD_STATE v = new_prediction[x+y*7];
      if(v == RED)
        r[x+y*7] ++ ;
      if(v == GREEN)
        g[x+y*7] ++;
    }
  }
}




void decide(std::vector<CV_BOARD_STATE>& decide,
            const std::vector<int>& r,
            const std::vector<int>& g,
            unsigned int val_max_echantillon,
            unsigned int val_acceptable_echantillon
            )
{
  for (int x = 0; x < 6; x++) {
    for (int y = 0; y < 7; y++) {
      unsigned int vg = g[x+y*7];
      unsigned int vr = r[x+y*7];
      if (vg > val_acceptable_echantillon && vr > val_acceptable_echantillon )
        {
          if(vg>vr)
            {decide[x+y*7] = GREEN;}
          else
            {decide[x+y*7] = RED;}
        }
      else if(vg>val_acceptable_echantillon)
        {
        decide[x+y*7] = GREEN;
        }
      else if (vr > val_acceptable_echantillon)
        {
        decide[x+y*7] = RED;
        }
    }
  }
}


Cv_c4_optim::~Cv_c4_optim() {

}
