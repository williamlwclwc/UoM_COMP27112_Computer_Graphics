#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;
Mat image, image_grayscale, image_edges, image_horizon, image_threshold;

int thresh_min = 0, thresh_max = 0, thresh = 0;
int kernel_size = 9, hough_thresh = 100;

// Polynomial regression function: referenced from lab manual
vector<double> fitPoly(vector<Point> points, int n)
{
    // Number of points
    int nPoints = points.size();

    // Vectors for all the points' xs and ys
    vector<float> xValues = vector<float>();
    vector<float> yValues = vector<float>();

    // Split the points into two vectors for x and y values
    for(int i = 0; i < nPoints; i++)
    {
        xValues.push_back(points[i].x);
        yValues.push_back(points[i].y);
    }

    // Augmented matrix
    double matrixSystem[n+1][n+2];
    for(int row = 0; row < n+1; row++)
    {
        for(int col = 0; col < n+1; col++)
        {
            matrixSystem[row][col] = 0;
            for(int i = 0; i < nPoints; i++)
            {
                matrixSystem[row][col] += pow(xValues[i], row+col);
            }
        }
        matrixSystem[row][n+1] = 0;
        for(int i = 0; i < nPoints; i++)
        {
            matrixSystem[row][n+1] += pow(xValues[i], row) * yValues[i];
        }
    }

    // Array that holds all the coefficients
    double coeffVec[n+2];

    // Gauss reduction
    for(int i = 0; i <= n-1; i++)
    {
        for(int k = i+1; k <= n; k++)
        {
            double t = matrixSystem[k][i] / matrixSystem[i][i];
            for(int j = 0; j <= n+1; j++)
            {
                matrixSystem[k][j] = matrixSystem[k][j] - t*matrixSystem[i][j];
            }
        }
    }

    // Back-substitution
    for(int i = n; i >= 0; i--)
    {
        coeffVec[i] = matrixSystem[i][n+1];
        for(int j=0; j<=n+1; j++)
        {
            if(j != i)
            {
                coeffVec[i] = coeffVec[i] - matrixSystem[i][j] * coeffVec[j];
            }
        }
        coeffVec[i] = coeffVec[i] / matrixSystem[i][i];
    }

    // Construct the cv vector and return it
    vector<double> result = vector<double>();
    for(int i = 0; i < n+1; i++)
    {
        result.push_back(coeffVec[i]);
    }
    return result;
}

// Returns the point for the equation determined 
// by a vector of coefficients, at a certain x location
Point pointAtX(vector<double> coeff, double x)
{
    double y = 0;
    for(int i = 0; i < coeff.size(); i++)
    {
        y += pow(x, i) * coeff[i];
    }
    return Point(x, y);
}

// track bar call back for canny filter
void canny_on_track(int, void*)
{
    // apply threshold
    threshold(image_grayscale, image_threshold, thresh, 200, CV_THRESH_BINARY);
    imshow("Threshold Image", image_threshold);
    if(kernel_size % 2 == 0) 
    {
        kernel_size++;
    }
    GaussianBlur(image_threshold, image_edges, Size(kernel_size, kernel_size),0, 0);
    Canny(image_edges, image_edges, thresh_min, thresh_max);
    imshow("Canny filter Image", image_edges);
    // apply Hough transfermation
    vector<Vec4i> hough_out;
    HoughLinesP(image_edges, hough_out, 1, CV_PI/180, hough_thresh);
    // polynomial regression
    image_horizon = image.clone();
    vector<Point> hough_lines;
    for(int i = 0; i < hough_out.size(); i++)
    {
        Vec4i l = hough_out[i];
        hough_lines.push_back(Point(l[0], l[1]));
        hough_lines.push_back(Point(l[2], l[3]));
    }
    vector<double> coeff;
    coeff = fitPoly(hough_lines, 2);
    for(int i = 0; i < image.cols; i++)
    {
        circle(image_horizon, pointAtX(coeff, i), 2, Scalar(0, 0, 255));
    }
    imshow("Horizon Image", image_horizon);
}

int main(int argc, char *argv[])
{
    char filename[30];
    printf("OpenCV version: %d.%d\n", CV_MAJOR_VERSION, CV_MINOR_VERSION);
    printf("Please input the filename of your image: ");
    scanf("%s", filename);
    image = imread(filename , CV_LOAD_IMAGE_COLOR);

    if(! image.data ) {
        printf("Could not open or find the image\n");
        return -1;
    }

    // namedWindow("Original Image", WINDOW_AUTOSIZE);
    namedWindow("Grayscale Image", WINDOW_NORMAL);
    cvResizeWindow("Grayscale Image", 500, 500);
    namedWindow("Canny filter Image", WINDOW_NORMAL);
    cvResizeWindow("Canny filter Image", 500, 500);
    namedWindow("Horizon Image", WINDOW_NORMAL);
    cvResizeWindow("Horizon Image", 500, 500);
    namedWindow("Threshold Image", WINDOW_NORMAL);
    cvResizeWindow("Threshold Image", 500, 500);
    // imshow("Original Image", image); // show original image

    // convert into grayscale
    cvtColor(image, image_grayscale, COLOR_RGB2GRAY);
    imshow("Grayscale Image", image_grayscale); // show grayscale image
    // apply threshold
    threshold(image_grayscale, image_threshold, thresh, 200, CV_THRESH_BINARY);
    imshow("Threshold Image", image_threshold);
    createTrackbar("Thresh value:", "Threshold Image", &thresh, 200, canny_on_track);
    // apply Canny filter
    if(kernel_size % 2 == 0) 
    {
        kernel_size++;
    }
    GaussianBlur(image_threshold, image_edges, Size(kernel_size, kernel_size),0, 0);
    Canny(image_edges, image_edges, thresh_min, thresh_max);
    imshow("Canny filter Image", image_edges);
    createTrackbar("Gaussian blur kernel size", "Canny filter Image", &kernel_size, 200, canny_on_track);
    createTrackbar("Threshold min", "Canny filter Image", &thresh_min, 500, canny_on_track);
    createTrackbar("Threshold max", "Canny filter Image", &thresh_max, 500, canny_on_track);
    // apply Hough transfermation
    vector<Vec4i> hough_out;
    HoughLinesP(image_edges, hough_out, 1, CV_PI/180, hough_thresh);
    // polynomial regression
    image_horizon = image.clone();
    vector<Point> hough_lines;
    for(int i = 0; i < hough_out.size(); i++)
    {
        Vec4i l = hough_out[i];
        hough_lines.push_back(Point(l[0], l[1]));
        hough_lines.push_back(Point(l[2], l[3]));
    }
    vector<double> coeff;
    coeff = fitPoly(hough_lines, 2);
    for(int i = 0; i < image.cols; i++)
    {
        circle(image_horizon, pointAtX(coeff, i), 2, Scalar(0, 0, 255));
    }
    imshow("Horizon Image", image_horizon);
    createTrackbar("Threshold", "Horizon Image", &hough_thresh, 500, canny_on_track);
    waitKey(0);
    imwrite("edge image.jpg", image_edges);
    imwrite("horizon image.jpg", image_horizon);
    return 0;
}