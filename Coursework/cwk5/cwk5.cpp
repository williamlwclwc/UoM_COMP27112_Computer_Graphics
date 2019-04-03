#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
Mat image, image_grayscale, image_threshold;
int thresh = 5; // initial slider value
double maxVal = 200;

void on_trackbar(int slider, void* userdata) 
{
    // update image
    threshold(image_grayscale, image_threshold, thresh, maxVal, CV_THRESH_BINARY);
    imshow("Threshold Image", image_threshold); // show image with threshold
}

int main(int argc, char *argv[])
{
    printf("OpenCV version: %d.%d\n", CV_MAJOR_VERSION, CV_MINOR_VERSION);
    image = imread("source.jpg" , CV_LOAD_IMAGE_COLOR);

    if(! image.data ) {
        printf("Could not open or find the image\n");
        return -1;
    }

    namedWindow("Original Image", WINDOW_AUTOSIZE );
    namedWindow("Threshold Image", WINDOW_AUTOSIZE);
    imshow("Original Image", image); // show original image
    cvtColor(image, image_grayscale, COLOR_RGB2GRAY);
    threshold(image_grayscale, image_threshold, thresh, maxVal, CV_THRESH_BINARY);
    imshow("Threshold Image", image_threshold); // show image with threshold
    createTrackbar("Thresh value:", "Threshold Image", &thresh, 200, on_trackbar);

    waitKey(0);
    imwrite("threshold.jpg", image_threshold);
    return 0;
}