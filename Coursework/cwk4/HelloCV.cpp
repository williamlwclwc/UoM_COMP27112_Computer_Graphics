#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(int argc, char *argv[])
{
    printf("OpenCV version: %d.%d\n", CV_MAJOR_VERSION, CV_MINOR_VERSION);
    cv::Mat image;
    image = cv::imread("sickle.jpg" , CV_LOAD_IMAGE_COLOR);

    if(! image.data ) {
        printf("Could not open or find the image\n");
        return -1;
    }

    cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );
    cv::imshow( "Display window", image );

    cv::waitKey(0);
    return 0;
}
