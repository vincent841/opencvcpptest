#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/calib3d/calib3d_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgcodecs/legacy/constants_c.h>

#include <stdio.h>
#include <iostream>
#include <sys/stat.h>
#include <string>
#include <unistd.h>
#include <dirent.h>

#include <time.h>

using namespace std;
using namespace cv;

struct stat statbuf;

bool checkDirectoryExists( const char* pzPath )
{
    if ( pzPath == NULL) return false;

    DIR *pDir;
    bool bExists = false;

    pDir = opendir (pzPath);

    if (pDir != NULL)
    {
        bExists = true;    
        (void) closedir (pDir);
    }

    return bExists;
}


string makeImageDirectory()
{
  char buf[256] = {0,};
  time_t now = time(0);
  struct tm tstruct;
  tstruct = *localtime(&now);
  strftime(buf, sizeof(buf), "%Y%m%d%H%M%S", &tstruct);
  string dirName(buf);

  getcwd(buf, sizeof(buf));
  string strDir("/Captured/");
  string strDest = string(buf) + strDir + dirName;

  try
  {
    bool bDirExists = checkDirectoryExists(strDest.c_str());
    if( bDirExists == false)
    {
      mkdir(strDest.c_str(), 0777);
    }
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }

  return strDest;
}

int main(int, char**)  
{
  VideoCapture cap(4);
  if (!cap.isOpened())  
    cout << "can't open this camera" << endl;

  string destDir = makeImageDirectory();

  Mat frame;

  int idxFile = 0;

  for(;;)  
  {  
    cap >> frame;

    imshow("camera1", frame);

    unsigned int key = waitKey(1);
    if( (key == 'q') || (key == 'Q') )
      break;

    switch(key)
    {
      case 'c':
      case 'C':
      {
        // make a directory name using the current time
        char buf[256];
        string fname = to_string(idxFile++);
        fname += ".jpg";
        fname  = destDir + "/" + fname;
        imwrite(fname.c_str(), frame);
        break;
      }
      
      default:
        break;

    }
  }  

  return 0;  
} 

