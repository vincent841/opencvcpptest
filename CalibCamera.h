
#ifndef __CALIBCAMER_H__
#define __CALIBCAMER_H__

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


namespace aidoop {

  class Calibration 
  {
    public:
      Calibration (int board_width, int board_height, float square_width, float square_height);

      void initialize();
      int findChessboard(Mat &view, bool reg = false);
      bool runCalibration();
      bool loadCalibrationData ();

    private:
      const Size boardSize;
      const Size2f squareSize;
      vector<vector<Point2f>> imagePoints;
      Size imageSize;
  };

}


#endif // __CALIBCAMER_H__

