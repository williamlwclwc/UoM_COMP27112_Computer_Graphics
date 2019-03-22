#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
Mat image, image_greyscale, image_blurred, image_difference, image_difference_hist;

Mat drawHist(Mat hist, int scale)
{
    double mx = 0;
    minMaxLoc(hist, 0, &mx, 0, 0);
    Mat result = Mat::zeros(256*scale, 256*scale, CV_8UC1);
    for(int i = 0; i < 255; i++)
    {
        // Get the histogram values
        float histValue = hist.at<float>(i, 0);
        float nextValue = hist.at<float>(i+1, 0);

        //Create 4 points for the poly
        Point p1 = Point(i*scale, 256*scale);
        Point p2 = Point(i*scale + scale, 256*scale);
        Point p3 = Point(i*scale + scale, (256-nextValue*256/mx)*scale);
        Point p4 = Point(i*scale, (256-nextValue*256/mx)*scale);

        //Draw the poly (Ending in p1)
        int numPoints = 5;
        Point points[] = {p1, p2, p3, p4, p1};
        fillConvexPoly(result, points, numPoints, Scalar::all(256), 0, 0);
    }
    return result;
}

void on_trackbar(int slider, void* userdata) 
{
    if(slider % 2 == 0) 
    {
        slider++;
    }
    // update image
    GaussianBlur(image_greyscale, image_blurred, Size(slider, slider),0);
    imshow("Blurred Image", image_blurred);
    image_difference = image_greyscale - image_blurred + 128;
    imshow("Difference Image", image_difference);
}

int main(int argc, char *argv[])
{
    printf("OpenCV version: %d.%d\n", CV_MAJOR_VERSION, CV_MINOR_VERSION);
    image = imread("source.jpg" , CV_LOAD_IMAGE_COLOR);

    if(! image.data ) {
        printf("Could not open or find the image\n");
        return -1;
    }

    cvtColor(image, image_greyscale, COLOR_BGR2GRAY);

    int slider = 5; // initial slider value
    namedWindow( "Original Image", WINDOW_AUTOSIZE );
    namedWindow( "Blurred Image", WINDOW_AUTOSIZE);
    namedWindow( "Difference Image", WINDOW_AUTOSIZE);
    imshow( "Original Image", image); // show original image
    GaussianBlur(image_greyscale, image_blurred, Size(slider, slider),0);
    imshow("Blurred Image", image_blurred); // show initial blurred image
    image_difference = image_greyscale - image_blurred + 128; // initial noise image
    createTrackbar("Kernel Size", "Blurred Image", &slider, 100, on_trackbar);
    imshow("Difference Image", image_difference); // show initial noise image

    // show histogram
    // namedWindow( "Histogram of difference image");
    // image_difference_hist = drawHist(image_difference, 1);
    // imshow( "Histogram of difference image", image_difference_hist);

    waitKey(0);
    return 0;
}