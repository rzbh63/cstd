#include <opencv2/features2d.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>      //for imshow
#include <vector>
#include <iostream>
#include <iomanip>

#include "stats.h" // Stats structure definition
#include "utils.h" // Drawing and printing functions

using namespace std;
using namespace cv;

const double akaze_thresh = 3e-4; // AKAZE detection threshold set to locate about 1000 keypoints
const double ransac_thresh = 2.5f; // RANSAC inlier threshold
const double nn_match_ratio = 0.8f; // Nearest-neighbour matching ratio
const int bb_min_inliers = 100; // Minimal number of inliers to draw bounding box
const int stats_update_period = 10; // On-screen statistics are updated every 10 frames

namespace example {
class Tracker
{
public:
    Tracker(Ptr<Feature2D> _detector, Ptr<DescriptorMatcher> _matcher) :
        detector(_detector),
        matcher(_matcher)
    {}

    void setFirstFrame(const CvMat frame, vector<Point2f> bb, string title, Stats& stats);
    CvMat process(const CvMat frame, Stats& stats);
    Ptr<Feature2D> getDetector() {
        return detector;
    }
protected:
    Ptr<Feature2D> detector;
    Ptr<DescriptorMatcher> matcher;
    CvMat first_frame, first_desc;
    vector<KeyPoint> first_kp;
    vector<Point2f> object_bb;
};

void Tracker::setFirstFrame(const CvMat frame, vector<Point2f> bb, string title, Stats& stats)
{
    Point *ptMask = new Point[bb.size()];
    const Point* ptContain = { &ptMask[0] };
    int iSize = static_cast<int>(bb.size());
    for (size_t i=0; i<bb.size(); i++) {
        ptMask[i].x = static_cast<int>(bb[i].x);
        ptMask[i].y = static_cast<int>(bb[i].y);
    }
    first_frame = frame.clone();
    CvMat matMask = CvMat::zeros(frame.size(), CC_8UC1);
    fillPoly(matMask, &ptContain, &iSize, 1, Scalar::all(255));
    detector->detectAndCompute(first_frame, matMask, first_kp, first_desc);
    stats.keypoints = (int)first_kp.size();
    drawBoundingBox(first_frame, bb);
    putText(first_frame, title, Point(0, 60), FONT_HERSHEY_PLAIN, 5, Scalar::all(0), 4);
    object_bb = bb;
    delete[] ptMask;
}

CvMat Tracker::process(const CvMat frame, Stats& stats)
{
    vector<KeyPoint> kp;
    CvMat desc;
    detector->detectAndCompute(frame, noArray(), kp, desc);
    stats.keypoints = (int)kp.size();

    vector< vector<DMatch> > matches;
    vector<KeyPoint> matched1, matched2;
    matcher->knnMatch(first_desc, desc, matches, 2);
    for(unsigned i = 0; i < matches.size(); i++) {
        if(matches[i][0].distance < nn_match_ratio * matches[i][1].distance) {
            matched1.push_back(first_kp[matches[i][0].queryIdx]);
            matched2.push_back(      kp[matches[i][0].trainIdx]);
        }
    }
    stats.matches = (int)matched1.size();

    CvMat inlier_mask, homography;
    vector<KeyPoint> inliers1, inliers2;
    vector<DMatch> inlier_matches;
    if(matched1.size() >= 4) {
        homography = findHomography(Points(matched1), Points(matched2),
                                    RANSAC, ransac_thresh, inlier_mask);
    }

    if(matched1.size() < 4 || homography.empty()) {
        CvMat res;
        hconcat(first_frame, frame, res);
        stats.inliers = 0;
        stats.ratio = 0;
        return res;
    }
    for(unsigned i = 0; i < matched1.size(); i++) {
        if(inlier_mask.at<uchar>(i)) {
            int new_i = static_cast<int>(inliers1.size());
            inliers1.push_back(matched1[i]);
            inliers2.push_back(matched2[i]);
            inlier_matches.push_back(DMatch(new_i, new_i, 0));
        }
    }
    stats.inliers = (int)inliers1.size();
    stats.ratio = stats.inliers * 1.0 / stats.matches;

    vector<Point2f> new_bb;
    perspectiveTransform(object_bb, new_bb, homography);
    CvMat frame_with_bb = frame.clone();
    if(stats.inliers >= bb_min_inliers) {
        drawBoundingBox(frame_with_bb, new_bb);
    }
    CvMat res;
    drawMatches(first_frame, inliers1, frame_with_bb, inliers2,
                inlier_matches, res,
                Scalar(255, 0, 0), Scalar(255, 0, 0));
    return res;
}
}

int main(int argc, char **argv)
{
    if(argc < 2) {
        cerr << "Usage: " << endl
             << "akaze_track input_path" << endl
             << "  (input_path can be a camera id, like 0,1,2 or a video filename)" << endl;
        return 1;
    }

    std::string video_name = argv[1];
    std::stringstream ssFormat;
    ssFormat << atoi(argv[1]);

    VideoCapture video_in;
    if (video_name.compare(ssFormat.str())==0) {    //test str==str(num)
        video_in.open(atoi(argv[1]));
    }
    else {
        video_in.open(video_name);
    }

    if(!video_in.isOpened()) {
        cerr << "Couldn't open " << argv[1] << endl;
        return 1;
    }

    Stats stats, akaze_stats, orb_stats;
    Ptr<AKAZE> akaze = AKAZE::create();
    akaze->setThreshold(akaze_thresh);
    Ptr<ORB> orb = ORB::create();
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
    example::Tracker akaze_tracker(akaze, matcher);
    example::Tracker orb_tracker(orb, matcher);

    CvMat frame;
    video_in >> frame;
    namedWindow(video_name, WINDOW_NORMAL);
    resizeWindow(video_name, frame.cols, frame.rows);

    cout << "Please select a bounding box, and press any key to continue." << endl;
    vector<Point2f> bb;
    Rect2d uBox = selectROI(video_name, frame);
    bb.push_back(Point2f(static_cast<float>(uBox.x), static_cast<float>(uBox.y)));
    bb.push_back(Point2f(static_cast<float>(uBox.x+uBox.width), static_cast<float>(uBox.y)));
    bb.push_back(Point2f(static_cast<float>(uBox.x+uBox.width), static_cast<float>(uBox.y+uBox.height)));
    bb.push_back(Point2f(static_cast<float>(uBox.x), static_cast<float>(uBox.y+uBox.height)));

    akaze_tracker.setFirstFrame(frame, bb, "AKAZE", stats);
    orb_tracker.setFirstFrame(frame, bb, "ORB", stats);

    Stats akaze_draw_stats, orb_draw_stats;
    CvMat akaze_res, orb_res, res_frame;
    int i = 0;
    for(;;) {
        i++;
        bool update_stats = (i % stats_update_period == 0);
        video_in >> frame;
        // stop the program if no more images
        if(frame.empty()) break;

        akaze_res = akaze_tracker.process(frame, stats);
        akaze_stats += stats;
        if(update_stats) {
            akaze_draw_stats = stats;
        }

        orb->setMaxFeatures(stats.keypoints);
        orb_res = orb_tracker.process(frame, stats);
        orb_stats += stats;
        if(update_stats) {
            orb_draw_stats = stats;
        }

        drawStatistics(akaze_res, akaze_draw_stats);
        drawStatistics(orb_res, orb_draw_stats);
        vconcat(akaze_res, orb_res, res_frame);
        imshow(video_name, res_frame);
        if(waitKey(1)==27) break; //quit on ESC button
    }
    akaze_stats /= i - 1;
    orb_stats /= i - 1;
    printStatistics("AKAZE", akaze_stats);
    printStatistics("ORB", orb_stats);
    return 0;
}