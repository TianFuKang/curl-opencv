#include "curl/curl.h" // has to go before opencv headers
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

size_t write_data(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    vector<uchar> *stream = (vector<uchar>*)userdata;
    size_t count = size * nmemb;
    stream->insert(stream->end(), ptr, ptr + count);
    return count;
}

//function to retrieve the image as cv::Mat data type
cv::Mat curlImg(const char *img_url, int timeout=10)
{
    vector<uchar> stream;
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, img_url); //the img url
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); // pass the writefunction
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stream); // pass the stream ptr to the writefunction
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout); // timeout if curl_easy hangs, 
    CURLcode res = curl_easy_perform(curl); // start curl
    curl_easy_cleanup(curl); // cleanup
    return imdecode(stream, -1); // 'keep-as-is'
}

int main(int argc, char** argv)
{
	cout<<"Usage:  ./main URL\n";
    Mat image = curlImg(argv[1]);
    if (image.empty())
    {
    	cout<<"Could not load image\n";
    	return -1; // load fail
    }
    namedWindow( "Image output", CV_WINDOW_AUTOSIZE );
    imshow("Image output",image); 
    waitKey(0); 
}
