#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/calib3d/calib3d_c.h>

#include <opencv2/imgproc/types_c.h>

#include "CalibCamera.h"


using namespace aidoop;

Calibration::Calibration (int board_width, int board_height, float square_width, float square_height)
: boardSize(board_width, board_height), squareSize(square_width, square_height)
{
}

void Calibration::initialize()
{
	imagePoints.clear();
}

int Calibration::findChessboard(Mat &view, bool reg)
{
	imageSize = view.size();

    Mat viewGray;
	cvtColor (view, viewGray, CV_BGR2GRAY); 

	vector<Point2f> pointbuf;
	bool found = findChessboardCorners (view, boardSize, pointbuf,
		CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FAST_CHECK | CV_CALIB_CB_NORMALIZE_IMAGE);

	if (found) {
		// improve the found corners' coordinate accuracy
		cornerSubPix (viewGray, pointbuf, Size(1,1), Size(-1,-1), TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));

		if (reg) {
			imagePoints.push_back (pointbuf);
		}
		drawChessboardCorners (view, boardSize, Mat(pointbuf), found);
	}
	return imagePoints.size();
}

bool Calibration::runCalibration()
{
	vector<vector<Point3f> > objectPoints(1);

	for( int i = 0; i < boardSize.height; i++ ) {
		for( int j = 0; j < boardSize.width; j++ ) {
			objectPoints[0].push_back(Point3f (j*squareSize.width, i*squareSize.height, 0.f));
		}
	}
	objectPoints.resize(imagePoints.size(),objectPoints[0]);

	int flags = 0;
	// flags |= CV_CALIB_ZERO_TANGENT_DIST;
	// flags |= CV_CALIB_FIX_PRINCIPAL_POINT;

	Mat cameraMatrix = Mat::eye(3, 3, CV_64F);
	Mat distCoeffs   = Mat::zeros(8, 1, CV_64F);

	vector<Mat> rvecs, tvecs;
	double rms = calibrateCamera (objectPoints, imagePoints, imageSize, cameraMatrix,distCoeffs, rvecs, tvecs, flags|CV_CALIB_FIX_K4|CV_CALIB_FIX_K5);

	bool ok = checkRange(cameraMatrix) && checkRange(distCoeffs);
	if (ok) {
		FileStorage fs("calibration_data.yml", FileStorage::WRITE );

		fs << "camera_matrix" << cameraMatrix;
		fs << "distortion_coefficients" << distCoeffs;
		fs << "imageSize" << imageSize;
	}

	return ok;
}

bool Calibration::loadCalibrationData()
{
	FileStorage fs("calibration_data.yml", FileStorage::READ);
	if (fs.isOpened ()) {
		Mat cameraMatrix = Mat::eye(3, 3, CV_64F);
		Mat distCoeffs   = Mat::zeros(8, 1, CV_64F);

		fs["camera_matrix"] >> cameraMatrix;
		fs["distortion_coefficients"] >> distCoeffs;
		FileNode is = fs["imageSize"];
		imageSize.width = is[0];
		imageSize.height = is[1];

		return true;
	}
	return false;
}


