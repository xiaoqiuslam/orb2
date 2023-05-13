#include<iostream>
#include<algorithm>
#include<fstream>

#include<opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;

void LoadImages(const string &strPathLeft,
                const string &strPathRight,
                const string &strPathTimes,
                vector<string> &vstrImageLeft,
                vector<string> &vstrImageRight,
                vector<double> &vTimeStamps);

int main(int argc, char **argv)
{
    if(argc != 6)
    {
        cerr << endl << "Usage: ./stereo_euroc path_to_vocabulary path_to_settings path_to_left_folder path_to_right_folder path_to_times_file" << endl;
        return 1;
    }

    // Retrieve paths to images
    vector<string> vstrImageLeft;
    vector<string> vstrImageRight;
    vector<double> vTimeStamp;
    LoadImages(string(argv[3]), string(argv[4]), string(argv[5]), vstrImageLeft, vstrImageRight, vTimeStamp);

    if(vstrImageLeft.empty() || vstrImageRight.empty())
    {
        cerr << "ERROR: No images in provided path." << endl;
        return 1;
    }

    if(vstrImageLeft.size()!=vstrImageRight.size())
    {
        cerr << "ERROR: Different number of left and right images." << endl;
        return 1;
    }

    const int nImages = vstrImageLeft.size();

    // Vector for tracking time statistics
    vector<float> vTimesTrack;
    vTimesTrack.resize(nImages);

    cout << "Images in the sequence: " << nImages << endl << endl;

    // Main loop
    cv::Mat imLeft, imRight, imLeftRect, imRightRect;
    for(int ni=0; ni<nImages; ni++)
    {
        // Read left and right images from file
        imLeft = cv::imread(vstrImageLeft[ni],cv::IMREAD_UNCHANGED);
        imRight = cv::imread(vstrImageRight[ni],cv::IMREAD_UNCHANGED);

        if(imLeft.empty())
        {
            cerr << endl << "Failed to load image at: "
                 << string(vstrImageLeft[ni]) << endl;
            return 1;
        }

        if(imRight.empty())
        {
            cerr << endl << "Failed to load image at: "
                 << string(vstrImageRight[ni]) << endl;
            return 1;
        }


        // 创建一个足够大的图像来容纳两个输入图像
        cv::Mat combined_image(cv::max(imLeft.rows, imRight.rows), imLeft.cols + imRight.cols, imLeft.type());

        // 将第一幅图像放在新图像的左侧
        cv::Mat left_roi(combined_image, cv::Rect(0, 0, imLeft.cols, imLeft.rows));
        imLeft.copyTo(left_roi);

        // 将第二幅图像放在新图像的右侧
        cv::Mat right_roi(combined_image, cv::Rect(imLeft.cols, 0, imRight.cols, imRight.rows));
        imRight.copyTo(right_roi);

        // 显示合并后的图像
        cv::namedWindow("Combined Images", cv::WINDOW_AUTOSIZE);
        cv::imshow("Combined Images", combined_image);

        // 等待按键，按下任意键关闭窗口
        cv::waitKey(0);
    }
    return 0;
}

void LoadImages(const string &strPathLeft,
                const string &strPathRight,
                const string &strPathTimes,
                vector<string> &vstrImageLeft,
                vector<string> &vstrImageRight,
                vector<double> &vTimeStamps)
{
    ifstream fTimes;
    fTimes.open(strPathTimes.c_str());
    vTimeStamps.reserve(5000);
    vstrImageLeft.reserve(5000);
    vstrImageRight.reserve(5000);
    while(!fTimes.eof())
    {
        string s;
        getline(fTimes,s);
        if(!s.empty())
        {
            stringstream ss;
            ss << s;
            vstrImageLeft.push_back(strPathLeft + "/" + ss.str() + ".png");
            vstrImageRight.push_back(strPathRight + "/" + ss.str() + ".png");
            double t;
            ss >> t;
            vTimeStamps.push_back(t/1e9);

        }
    }
}
