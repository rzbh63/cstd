#ifndef _GET_CANDIDATE_
#define _GET_CANDIDATE_
#include "headline.h"
#include <vector>
#include "Candidate.h"
#include "swt.h"
using namespace std;
using namespace cv;
using namespace cv::ml;

class GetCandidate{
public:
	std::vector<Candidate> run(cv::Mat& Image);
	void featureExtract();
	void swtprocess();
	void ExtractCCfeatures();
	void Filter();
	GetCandidate() {
		mser = cv::MSER::create();
	}

private:
	Ptr<cv::MSER> mser;
	Swt swt;
	Ptr<RTrees> CharacterClassifier;
	cv::Mat oriBgrImage_8UC3;
	cv::Mat gray_source_image;
	vector< vector<cv::Point2i> > strVectorStore;
	vector<Candidate> ccStore;
	vector<Candidate> candidateStore;
};
#endif
