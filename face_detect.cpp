#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
 
#include <iostream>
#include <stdio.h>
 
using namespace std;
using namespace cv;
 
#define CAM_WIDTH 1280
#define CAM_HEIGHT 720
 
/** Function Headers */
void detectAndDisplay(Mat frame);
 
/** Global variables */
String face_cascade_name;
CascadeClassifier face_cascade;
String window_name = "Face detection";
 
/** @function main */
int main(int argc, const char** argv)
{
   // 정면 얼굴을 인식하기 위해, haarcascade_frontalface_alt.xml 파일을 불러오기
    face_cascade_name = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
    if (!face_cascade.load(face_cascade_name)) { 
        printf("--(!)Error loading face cascade\n"); 
        return -1; 
    };
 
    //웹캠을 연결하고, 영상을 받아오기
    VideoCapture cam(0); 
    Mat frame;
 
    cam.set(CV_CAP_PROP_FRAME_WIDTH, CAM_WIDTH);
    cam.set(CV_CAP_PROP_FRAME_HEIGHT, CAM_HEIGHT);
 
    if (!cam.isOpened()) { 
        printf("--(!)Error opening video cam\n"); 
        return -1; 
    }
    /*
        ESC를 눌러 종료할 때 까지, 웹캠으로부터 계속 프레임을 받아오고
        매 프레임마다 얼굴을 디텍팅하는 detectAndDisplay함수를 실행
     */
    while (cam.read(frame))
    {
        if (frame.empty())
        {
            printf(" --(!) No camd frame -- Break!");
            break;
        }
 
        detectAndDisplay(frame);
        char c = (char)waitKey(10);
        if (c == 27) { break; } 
    }
    return 0;
}
 
/** @function detectAndDisplay */
/*
detectMultiScale 함수를 이용해 얼굴을 검출합니다. 우선, 함수를 살펴보면 아래와 같습니다.
각 파라미터에 대한 상세한 설명은, opencv docs에서 확인하실 수 있습니다.
마지막의 minSize와 maxSize를 설정해두면, 일정 크기 이하나 이상의 얼굴은 처리하지 않고 무시하게 됩니다.
검출한 얼굴을 전부 주황색의 원 모양으로 표시해줍니다.
윈도우를 띄워서 결과를 보여줍니다. 
앞서 설정한 대로 1280*720의 Face detection이란 이름을 가진 윈도우가 보여지게 됩니다
*/
void detectAndDisplay(Mat frame)
{
    std::vector<Rect> faces;
    Mat frame_gray;
 
    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);
 
    face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
 
    for (size_t i = 0; i < faces.size(); i++)
    {
        Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
        ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 
                0, 0, 360, Scalar(0, 0, 255), 4, 8, 0);
    }
 
    imshow(window_name, frame);
}
