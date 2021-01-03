#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/calib3d.hpp>

#include <sstream>
#include <iostream>
#include <fstream>

const float calibrationSquareDimension = 0.01905f;
const float arucoSquareDimension = 0.05f;
const cv::Size chessBoardDimensions = cv::Size(9, 6);

void createArucoMarkers() {
	cv::Mat outputMarker;
	cv::Ptr<cv::aruco::Dictionary> markerDictionary = cv::aruco::getPredefinedDictionary(cv::aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_50);
    
    for (int i = 0; i < 50; i++) {
        cv::aruco::drawMarker(markerDictionary, i, 500, outputMarker, 1);
        std::ostringstream convert;
        std::string imageName = "4x4marker_";
        convert << imageName << i << ".jpg";
        cv::imwrite(convert.str(), outputMarker);
    }
}

void createKnownBoardPositions(cv::Size boardSize, float SquareEdgeLength, std::vector<cv::Point3f>& corners) {

    for (int i = 0; i < boardSize.height; i++) {
        for (int j = 0; j < boardSize.width; j++) {
            corners.push_back(cv::Point3f(j * SquareEdgeLength, i * SquareEdgeLength, 0.0f));
        }
    }
}

void getChessBoardCorners(std::vector<cv::Mat> images, std::vector<std::vector<cv::Point2f>>& allFoundCorners, bool showResults = false) {

    for (std::vector<cv::Mat>::iterator iter = images.begin(); iter != images.end(); iter++) {
        std::vector<cv::Point2f> pointBuf;
        bool found = cv::findChessboardCorners(*iter, cv::Size(9, 6), pointBuf, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);
        if (found) {
            allFoundCorners.push_back(pointBuf);
        }

        if (showResults) {
            cv::drawChessboardCorners(*iter, cv::Size(9, 6), pointBuf, found);
            cv::imshow("Looking for Corners", *iter);
            cv::waitKey(0);
        }
    }
}

void camerCalibration(std::vector<cv::Mat> calibrationImages, cv::Size boardSize, float squareEdgeLenght, cv::Mat& cameraMatrix, cv::Mat& distanceCoefficient) {

    std::vector<std::vector<cv::Point2f>> checkboardImageSpacePoints;
    getChessBoardCorners(calibrationImages, checkboardImageSpacePoints, false);

    std::vector<std::vector<cv::Point3f>> worldSpaceCornerPoints(1);
    createKnownBoardPositions(boardSize, squareEdgeLenght, worldSpaceCornerPoints[0]);
    worldSpaceCornerPoints.resize(checkboardImageSpacePoints.size(), worldSpaceCornerPoints[0]);

    std::vector<cv::Mat> rVectors, tVectors;
    distanceCoefficient = cv::Mat::zeros(8, 1, CV_64F);

    cv::calibrateCamera(worldSpaceCornerPoints, checkboardImageSpacePoints, boardSize, cameraMatrix, distanceCoefficient, rVectors, tVectors);
     
}

bool saveCameraCalibration(std::string name, cv::Mat cameraMatrix, cv::Mat distanceCoefficient) {
    std::ofstream outstream(name);
    if (outstream) {
        uint16_t rows = cameraMatrix.rows;
        uint16_t columns = cameraMatrix.cols;

        outstream << rows << std::endl;
        outstream << columns << std::endl;

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < columns; c++) {
                double value = cameraMatrix.at<double>(r, c);
                outstream << value << std::endl;
            }
        }

        rows = distanceCoefficient.rows;
        columns = distanceCoefficient.cols;

        outstream << rows << std::endl;
        outstream << columns << std::endl;

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < columns; c++) {
                double value = distanceCoefficient.at<double>(r, c);
                outstream << value << std::endl;
            }
        }

        outstream.close();
        return true;
    }
    return false;
}


bool loadCameraCalibration(std::string name, cv::Mat& cameraMatrix, cv::Mat& distanceCoefficients) {
    std::ifstream inStream(name);
    if (inStream) {
        uint16_t rows;
        uint16_t columns;

        inStream >> rows;
        inStream >> columns;

        cameraMatrix = cv::Mat(cv::Size(columns, rows), CV_64F);

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < columns; c++) {
                double read = 0.0f;
                inStream >> read;
                cameraMatrix.at<double>(r, c) = read;
                std::cout << cameraMatrix.at<double>(r, c) << "\n";
            }
        }
        //Distance Coefficients
        inStream >> rows;
        inStream >> columns;

        distanceCoefficients = cv::Mat::zeros(rows, columns, CV_64F);

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < columns; c++) {
                double read = 0.0f;
                inStream >> read;
                distanceCoefficients.at<double>(r, c) = read;
                std::cout << distanceCoefficients.at<double>(r, c) << "\n";
            }
        }
        inStream.close();
        return true;
    }
    return false;
}

void drawSquare(
    cv::InputOutputArray image, cv::InputArray cameraMatrix,
    cv::InputArray distCoeffs, cv::InputArray rvec, cv::InputArray tvec,
    float l0
)
{
    float l = l0 * 1.05;  // new square is 5% larger than the aruco marker
    float half_l = l / 2.0;

    // Define the square on the camera frame (this is 3D since the camera
    // frame is 3D).
    std::vector<cv::Point3f> squarePoints;
    squarePoints.push_back(cv::Point3f(half_l, half_l, 0));
    squarePoints.push_back(cv::Point3f(half_l, -half_l, 0));
    squarePoints.push_back(cv::Point3f(-half_l, -half_l, 0));
    squarePoints.push_back(cv::Point3f(-half_l, half_l, 0));

    // Project the square to the image.
    std::vector<cv::Point2f> imagePoints;
    projectPoints(
        squarePoints, rvec, tvec, cameraMatrix, distCoeffs, imagePoints
    );

    // Draw the square on the image.
    cv::line(image, imagePoints[0], imagePoints[1], cv::Scalar(255, 0, 0), 3);
    cv::line(image, imagePoints[1], imagePoints[2], cv::Scalar(255, 0, 0), 3);
    cv::line(image, imagePoints[2], imagePoints[3], cv::Scalar(255, 0, 0), 3);
    cv::line(image, imagePoints[3], imagePoints[0], cv::Scalar(255, 0, 0), 3);
}

int startWebcamMonitoring(const cv::Mat& cameraMatrix, const cv::Mat& distanceCoefficients, float arucoSquareDimensions) {
    cv::Mat frame;
    std::vector<int> markerIds;
    std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;
    cv::aruco::DetectorParameters parameters;
    cv::Ptr<cv::aruco::Dictionary> markerDictionary = cv::aruco::getPredefinedDictionary(cv::aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_50);
    
    cv::VideoCapture vid(0);
    
    if (!vid.isOpened()) {
        return -1;
    }

    cv::namedWindow("Webcam", cv::WindowFlags::WINDOW_AUTOSIZE);

    std::vector<cv::Vec3d> rotationVectors, translationVectors;

    while (true) {
        if (!vid.read(frame))
            break;
        cv::aruco::detectMarkers(frame, markerDictionary, markerCorners, markerIds);
        cv::aruco::estimatePoseSingleMarkers(markerCorners, arucoSquareDimension, cameraMatrix, distanceCoefficients, rotationVectors, translationVectors);
        for (int i = 0; i < markerIds.size(); i++) {
            drawSquare(frame, cameraMatrix, distanceCoefficients, rotationVectors[i], translationVectors[i], 0.05f);
            //cv::aruco::drawAxis(frame, cameraMatrix, distanceCoefficients, rotationVectors[i], translationVectors[i], 0.1f);
        }
        cv::imshow("Webcam", frame);
        if (cv::waitKey(30) >= 0) break;

    }
    return 1;
}

void cameraCalibrationProcess(cv::Mat& cameraMatrix, cv::Mat& distanceCoefficients) {
    cv::Mat frame;
    cv::Mat drawToFrame;

    std::vector<cv::Mat> savedImages;

    std::vector<std::vector<cv::Point2f>> makeCorners, rejectedCandidates;

    cv::VideoCapture vid(0);

    if (!vid.isOpened()) {
        return;
    }

    int framePerSecond = 20;

    cv::namedWindow("Webcam", cv::WindowFlags::WINDOW_AUTOSIZE);

    while (true) {
        if (!vid.read(frame))
            break;
        std::vector<cv::Vec2f> foundPoints;
        bool found = false;

        found = cv::findChessboardCorners(frame, chessBoardDimensions, foundPoints, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);
        frame.copyTo(drawToFrame);
        cv::drawChessboardCorners(drawToFrame, chessBoardDimensions, foundPoints, found);
        if (found)
            cv::imshow("Webcam", drawToFrame);
        else
            cv::imshow("Webcam", frame);
        char character = cv::waitKey(1000 / framePerSecond);

        switch (character)
        {
        case ' ':
            //saving image
            if (found) {
                cv::Mat temp;
                frame.copyTo(temp);
                savedImages.push_back(temp);
            }
            std::cout << "Image Saved" << std::endl;
            break;
        case 13:
            //start calibration
            if (savedImages.size() > 15) {
                camerCalibration(savedImages, chessBoardDimensions, calibrationSquareDimension, cameraMatrix, distanceCoefficients);
                saveCameraCalibration("CAMARA", cameraMatrix, distanceCoefficients);
                std::cout << "Well, it enters the if" << std::endl;
            }
            std::cout << "Calibration Started" << std::endl;
            break;
        case 27:
            //exit
            std::cout << "Finished" << std::endl;
            return;
            break;
        default:
            break;
        }
    }
}

int main(int argv, char** argc) {
    cv::Mat cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
    cv::Mat distanceCoefficients;
    /**ChessBoard Calibration**/
    //cameraCalibrationProcess(cameraMatrix, distanceCoefficients);
    //startWebcamMonitoring(cameraMatrix, distanceCoefficients, 0.05f);
    /**Draw over the marker**/
    loadCameraCalibration("CAMARA", cameraMatrix, distanceCoefficients);
    startWebcamMonitoring(cameraMatrix, distanceCoefficients, arucoSquareDimension);

    return 0;
}