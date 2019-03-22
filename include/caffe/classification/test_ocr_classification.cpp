// ocr_test.cpp : �������̨Ӧ�ó������ڵ㡣

#include <map>
#include "public.h"

//#include "caffe/libcaffe.cpp"
#include "ICNNPredict.h"

//#define CPU_ONLY

#include "bktree.cpp"
#include "levenshtein.cpp"
#include "classification.cpp"

#include <time.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <set>
using namespace std;

#include "caffe/ctcpp.h"
#include <windows.h>

struct testdata_t {
	int n;
	int idx[20];
public:
};

int testdata_cmp(const struct testdata_t& a, const struct testdata_t& o) {
	if (a.n != o.n) {
		return a.n - o.n;
	}
	else {
		for (int i = 0; i < a.n; ++i) {
			if (a.idx[i] != o.idx[i]) {
				return a.idx[i] - o.idx[i];
			}
		}
	}
	return 0;
}
//�㷨
int ldistance(const struct testdata_t& a, const struct testdata_t& o)
{
	//step 1
	const int* source = a.idx;
	const int* target = o.idx;
	int n = a.n;
	int m = o.n;
	if (m == 0) return n;
	if (n == 0) return m;
	//Construct a matrix
	typedef vector< vector<int> >  Tmatrix;
	Tmatrix matrix(n + 1);
	for (int i = 0; i <= n; i++)  matrix[i].resize(m + 1);

	//step 2 Initialize

	for (int i = 1; i <= n; i++) matrix[i][0] = i;
	for (int i = 1; i <= m; i++) matrix[0][i] = i;

	//step 3
	for (int i = 1; i <= n; i++)
	{
		const char si = source[i - 1];
		//step 4
		for (int j = 1; j <= m; j++)
		{

			const char dj = target[j - 1];
			//step 5
			int cost;
			if (si == dj) {
				cost = 0;
			}
			else {
				cost = 1;
			}
			//step 6
			const int above = matrix[i - 1][j] + 1;
			const int left = matrix[i][j - 1] + 1;
			const int diag = matrix[i - 1][j - 1] + cost;
			matrix[i][j] = min(above, min(left, diag));

		}
	}//step7
	return matrix[n][m];
}

int GetUppercaseNum(const string& str)
{
	int n = 0;
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			n++;
	}
	return n;
}


std::wstring string2wstring(const string& str, bool bSrcIsUTF8 = true)
{
#ifdef _WIN32
	UINT srcCode = bSrcIsUTF8 ? CP_UTF8 : CP_ACP;
	int len = ::MultiByteToWideChar(srcCode,
		0,
		str.c_str(),
		-1,
		NULL,
		0);
	if (len == 0)
		return wstring();

	WCHAR* dst = new WCHAR[len];
	int nRet = ::MultiByteToWideChar(srcCode,
		0,
		str.c_str(),
		-1,
		dst,
		len);
#else
	//printf("=====str====%s,len=%lu\n", str.c_str(), str.size());
	wstring wstr = convert_mb2wc("utf-8", "ucs-2", str);
	// 	if (wstr.size() == 0)
	// 		wstr = convert_mb2wc("gb2312", "ucs-2", str);
	// 	if(wstr.size()==0)
	// 		wstr = convert_mb2wc("ascii", "ucs-2", str);

#endif

	wstring wstr = dst;
	delete[]dst;


	return wstr;
}



void FindAllImages(const char *folder, std::vector<std::string>& vImgPaths, bool bSubFolder)
{

#ifdef WIN32
	char szPathName[MAX_PATH];
	strcpy_s(szPathName, folder);
	if (szPathName[strlen(szPathName) - 1] != '\\')
		strcat_s(szPathName, "\\");

	char szFileName[256];
	strcpy_s(szFileName, szPathName);
	strcat_s(szFileName, "*.*");

	int ret = 0;
	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile(szFileName, &wfd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (strcmp(wfd.cFileName, ".") == 0 || strcmp(wfd.cFileName, "..") == 0)
				continue;

			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (bSubFolder)
				{
					strcpy_s(szFileName, szPathName);
					strcat_s(szFileName, wfd.cFileName);
					FindAllImages(szFileName, vImgPaths, bSubFolder);
				}
			}
			else
			{
				if (strlen(wfd.cFileName) >= 5)
				{
					char *ext3 = wfd.cFileName + strlen(wfd.cFileName) - 4;
					char *ext4 = ext3 - 1;
					if (_stricmp(ext3, ".bmp") == 0
						|| _stricmp(ext3, ".jpg") == 0
						|| _stricmp(ext3, ".JPG") == 0
						|| _stricmp(ext4, ".jpeg") == 0
						|| _stricmp(ext4, ".JPEG") == 0
						|| _stricmp(ext3, ".png") == 0
						|| _stricmp(ext3, ".gif") == 0)
					{
						//printf("%s\n", wfd.cFileName);

						char filename[256];
						sprintf_s(filename, "%s%s", szPathName, wfd.cFileName);
						vImgPaths.push_back(filename);
					}
				}
			}
		} while (FindNextFile(hFind, &wfd) != 0);
	}
#else

	DIR* pDir = NULL;
	struct dirent* ent = NULL;

	pDir = opendir(folder);
	if (pDir == 0)
	{
		printf("open folder(%s) FAIL\n", folder);
		return;
	}

	std::string strFolder = folder;
	if (strFolder[strFolder.size() - 1] != '/')
		strFolder += "/";

	while (ent = readdir(pDir))
	{
		if (ent->d_type & DT_DIR)
			continue;

		int len = (int)strlen(ent->d_name);
		if (len >= 5 && strcmp(ent->d_name + len - 4, ".jpg") == 0)
		{
			char filepath[256];
			snprintf(filepath, 256, "%s%s", strFolder.c_str(), ent->d_name);
			vImgPaths.push_back(filepath);
		}
	}

	closedir(pDir);

#endif
}

string GetPredictString(const vector<float>& fm, int idxBlank, const vector<string>& labels, testdata_t& td, bool is_seq)
{
	string str;
	td.n = 0;
  if (is_seq) {
    for (int t = 0; t < (int)fm.size(); t++)
    {
      int idx = t;
      int label = (int)(fm[idx] + 0.5f);
      if (label >= 0 && label != idxBlank)
      {
        str += labels[label];
        if (td.n < 19) {
          td.idx[td.n++] = label;
        }
      }
    }
  }
  else {
    int idx = 0;
    for (int t = 1; t < (int)fm.size(); t++)
    {
      if (fm[idx] < fm[t])
      {
        idx = t;
      }
    }
    str = labels[idx];
  }
	return str;
}


float GetCTCLoss(float*activations, int timesteps, int alphabet_size, int blank_index_,
	const string& strlabel, const std::map<wchar_t, int>& mapLabel2Idx)
{
	size_t workspace_alloc_bytes_;

	ctcOptions options;
	options.loc = CTC_CPU;
	options.num_threads = 8;
	options.blank_label = blank_index_;

	int len = (int)strlabel.size();
	ctcStatus_t status = CTC::get_workspace_size<float>(&len,
		&timesteps,
		alphabet_size,
		1,
		options,
		&workspace_alloc_bytes_);
	//CHECK_EQ(status, CTC_STATUS_SUCCESS) << "CTC Error: " << ctcGetStatusString(status);
	vector<float> workspace_(workspace_alloc_bytes_);

	vector<int> flat_labels;
	for (size_t i = 0; i < strlabel.size(); i++)
	{
		map<wchar_t, int>::const_iterator it = mapLabel2Idx.find(strlabel[i]);
		if (it != mapLabel2Idx.end())
			flat_labels.push_back(it->second);
	}
	if (flat_labels.size() != strlabel.size())
		return 0;
	float cost = 0;
	status = CTC::compute_ctc_loss_cpu<float>(activations,
		0,
		flat_labels.data(),
		&len,
		&timesteps,
		alphabet_size,
		1,
		&cost,
		workspace_.data(),
		options
		);
	return cost;
}

void test_ocr_english(const string& imgfolder, const string& modelfolder, const string& lexiconfile, bool is_seq)
{
#ifdef CPU_ONLY
	bool usegpu = false;
#else
	bool usegpu = true;
#endif

	//load model
	ICNNPredict* pCNN = CreatePredictInstance(modelfolder.c_str(), usegpu);
	int wstd = 0, hstd = 0;
	pCNN->GetInputImageSize(wstd, hstd);

	//get alphabet
	vector<string> alphabets = pCNN->GetLabels();

	int idxBlank = 0;
	vector<string>::const_iterator it = find(alphabets.begin(), alphabets.end(), "blank");
	if (it != alphabets.end())
		idxBlank = (int)(it - alphabets.begin());


	map<wchar_t, int> mapLabel2IDs;
	for (int i = 0; i < (int)alphabets.size(); i++)
	{
		wchar_t c = 0;
		if (alphabets[i] == "blank")
			continue;
		wstring wlabel = string2wstring(alphabets[i], true);
		mapLabel2IDs.insert(make_pair(wlabel[0], i));
	}


	//init BK-tree of lexicon
	printf("init BK-tree of lexicon\n");
	BKTree* pBKtree = bktree_new(levenshtein_distance);
	ifstream fslexicon(lexiconfile);

	int n = 0;
	int caseoffset = 'A' - 'a';
	string line;
	while (getline(fslexicon, line))
	{
		if (line.size() == 0)
			continue;
		//if(line[line.size()-1]=='\t')
		bktree_add(pBKtree, const_cast<char*>(line.c_str()), (int)line.size());
		n++;
		if (GetUppercaseNum(line) == 0)//ȫ����Сд�ģ�ת�ɴ�д����ӣ�ת������ĸ��д�����
		{
			line[0] += caseoffset;
			bktree_add(pBKtree, const_cast<char*>(line.c_str()), (int)line.size());
			n++;
			if (line.size() > 1)
			{
				for (size_t i = 1; i < line.size(); i++)
				{
					if (line[i] >= 'a' && line[i] <= 'z')
						line[i] += caseoffset;
				}
				bktree_add(pBKtree, const_cast<char*>(line.c_str()), (int)line.size());
				n++;
			}
		}
		printf("\r%d", n);
	}
	printf("\n");


	int sumspend = 0;
	int nok_lexicon = 0;
	int nok_nolexicon = 0;

	vector<string> imgs;
	FindAllImages(imgfolder.c_str(), imgs, false);
	testdata_t td;
	for (int i=0;i<(int)imgs.size();i++)
	{
		string imgfile = imgs[i];
		cv::Mat img = cv::imread(imgfile, CV_LOAD_IMAGE_COLOR);
		int w = img.cols, h = img.rows;
		if (2 * w <= h)
		{
			cv::transpose(img, img);
			cv::flip(img, img, 1);
			w = img.cols, h = img.rows;
		}


		int w1 = hstd*w / h;
		if (w1 != w && h != hstd)
			cv::resize(img, img, cv::Size(w1, hstd));

		int start = clock();

		vector<int> shape;
		vector<float> pred = pCNN->GetOutputFeatureMap(img, shape);

		int end = clock();
		sumspend += (end - start);

		string strpredict0 = GetPredictString(pred, idxBlank, alphabets, td, is_seq);

		printf("[%d/%d]%s\n\torig result: %s\n",i+1,(int)imgs.size(),imgs[i].c_str(), strpredict0.c_str());

		string strpredict = strpredict0;


		int dist = MIN(2, (int)strpredict0.size() / 3);
		vector< BKResult> ress = bktree_query(pBKtree, const_cast<char*>(strpredict0.c_str()), strpredict0.size(), dist);

		float min_ctc_loss = 1000;
		vector<int> outshape;
		vector<float> activitas = pCNN->GetLayerFeatureMaps("fc1x", outshape);;
		int timesteps = outshape[0];
		int min_ctc_idx = -1;

		for (size_t j = 0; j < ress.size(); j++)
		{
			float ctcloss = GetCTCLoss(activitas.data(), timesteps, alphabets.size(), idxBlank, ress[j].str, mapLabel2IDs);
#ifdef _DEBUG
			printf("%s, ctc loss=%f\n", ress[j].str.c_str(), ctcloss);
#endif
			if (ctcloss < min_ctc_loss)
			{
				min_ctc_loss = ctcloss;
				min_ctc_idx = (int)j;
			}
		}

		if (ress.size()>0 && min_ctc_idx >= 0)
			printf("\tdic result: %s\n", ress[min_ctc_idx].str.c_str());

		//printf("%d, mean spend=%.2f",	i+1, sumspend / (float)(i+1));

	}

	bktree_destroy(pBKtree);
}

int LoadTextFile(const string& folder, const string& testfile, std::vector<testdata_t>& testdata, std::vector<string>& imgs) {
	FILE* input = NULL;
	input = fopen(testfile.c_str(), "rb");
	if (input) {
		char buf[1024];
		char fname[256];
		for (; fgets(buf, 1024, input) > 0; ) {
			sscanf(buf, "%s", fname);
			char* p = buf;
			testdata_t td = { 0 };
			for (; p = strchr(p, ' '); ) {
				while (' ' == *p) ++p;
				int k = atoi(p);
				if (k>0) {
					td.idx[td.n++] = k;
				}
			}
			testdata.push_back(td);
			imgs.push_back(folder + fname);
		}
		fclose(input);
	}
	return testdata.size();
}
using namespace cv;

struct ocr_classification {
  ICNNPredict* pCNN = NULL;
  int wstd = 0, hstd = 0;
  vector<string> alphabets;
  ocr_classification(const string& modelfolder, double scale, bool usegpu) {
    pCNN = CreatePredictInstance(modelfolder.c_str(), usegpu);
    pCNN->GetInputImageSize(wstd, hstd);
    pCNN->SetScale(scale);
    alphabets  = pCNN->GetLabels();
  }

  string ocr(const cv::Mat& img1, std::vector<float>& pred, std::vector<int>& shape, bool is_seq) {
    Mat sample;
    Mat img;
    testdata_t td = { 0 };
    int idxBlank = -1;
    if (is_seq) {
      Mat img = img1;
      int w = img.cols, h = img.rows;
      int w1 = hstd*w / h;
      if (h != hstd)
        cv::resize(img, img, cv::Size(w1, hstd));

      pred = pCNN->GetOutputFeatureMap(img, shape);
      idxBlank = 0;
    }
    else {
      int num_channels_ = 1;
      cv::resize(img1, img, cv::Size(wstd, hstd));

      if (img.channels() == 3 && num_channels_ == 1)
        cv::cvtColor(img, sample, CV_BGR2GRAY);
      else if (img.channels() == 4 && num_channels_ == 1)
        cv::cvtColor(img, sample, CV_BGRA2GRAY);
      else if (img.channels() == 4 && num_channels_ == 3)
        cv::cvtColor(img, sample, CV_BGRA2BGR);
      else if (img.channels() == 1 && num_channels_ == 3)
        cv::cvtColor(img, sample, CV_GRAY2BGR);
      else
        sample = img;

      cv::Mat bin1;
      threshold(sample, bin1, 0, 255, CV_THRESH_OTSU | CV_THRESH_BINARY_INV); //��򷨶�ֵ��
      imshow("bin1", bin1); waitKey();
      pred = pCNN->GetOutputFeatureMap(bin1, shape);
    }
    string strpredict0 = GetPredictString(pred, idxBlank, alphabets, td, is_seq);
    return strpredict0;
  }
};

void test_ocr_chinese(const string& imgfolder, const string& modelfolder, const string& outfile, const char* inputlistfile, double scale, bool is_seq)
{
#ifdef CPU_ONLY
	bool usegpu = false;
#else
	bool usegpu = true;
#endif

	//load model
	ICNNPredict* pCNN = CreatePredictInstance(modelfolder.c_str(), usegpu);
	int wstd = 0, hstd = 0;
	pCNN->GetInputImageSize(wstd, hstd);
  pCNN->SetScale(scale);
	//get alphabet
	vector<string> alphabets = pCNN->GetLabels();

	int idxBlank = -1;
	vector<string>::const_iterator it = find(alphabets.begin(), alphabets.end(), "blank");
	if (it != alphabets.end())
		idxBlank = (int)(it - alphabets.begin());

	map<wchar_t, int> mapLabel2IDs;
	for (int i = 0; i < (int)alphabets.size(); i++)
	{
		wchar_t c = 0;
		if (alphabets[i] == "blank")
			continue;
		wstring wlabel = string2wstring(alphabets[i], true);
		mapLabel2IDs.insert(make_pair(wlabel[0], i));
	}


	int sumspend = 0;
	int nok_lexicon = 0;
	int nok_nolexicon = 0;

	vector<string> imgs;
	std::vector<int> label_idx;
	std::vector<testdata_t> testdata;
	testdata_t td;
	FILE* input = NULL;
	if (inputlistfile) {
		LoadTextFile(imgfolder, inputlistfile, testdata, imgs);
	}
	else {
		FindAllImages(imgfolder.c_str(), imgs, false);
	}

	FILE* pf = fopen(outfile.c_str(), "wb");
	int errcnt = 0;
	int edtdistcnt = 0;
	for (int i = 0; i < imgs.size(); ++i)
	{
		string imgfile = imgs[i];
		cv::Mat img = cv::imread(imgfile, CV_LOAD_IMAGE_COLOR);
		int w = img.cols, h = img.rows;
		if (2 * w <= h)
		{
			cv::transpose(img, img);
			cv::flip(img, img, 1);
			w = img.cols, h = img.rows;
		}


		int w1 = hstd*w / h;
		if (w1 != w && h != hstd)
			cv::resize(img, img, cv::Size(w1, hstd));

		int start = clock();

		vector<int> shape;
		vector<float> pred = pCNN->GetOutputFeatureMap(img, shape);

		int end = clock();
		sumspend += (end - start);

		string strpredict0 = GetPredictString(pred, idxBlank, alphabets, td, is_seq);

		testdata_t td1 = testdata[i];
		errcnt += !!testdata_cmp(td, td1);
		edtdistcnt += ldistance(td, td1);
		const char* fn = strrchr(imgs[i].c_str(), '\\');
		++fn;
		printf("%d[%d/%d/%d]%s: %s\n", edtdistcnt, errcnt, i + 1, (int)imgs.size(), fn, strpredict0.c_str());

		if (1) {
			fprintf(pf, "%s ", fn);
			for (int iChar = 0; iChar < label_idx.size(); ++iChar) {
				fprintf(pf, "%d ", label_idx[iChar]);
			}
			fprintf(pf, "\n");
		}
	}
	fprintf(pf, "%d %d %d\n", edtdistcnt, errcnt, (int)imgs.size());
	fclose(pf);// 13418 11435 364400
	return;
}

int test_ocr_classification()
{
  string imgfolder, modelfolder, outfile, testfile;
  double scale = 0;
  bool is_seq = true;
  if (0) {
    string imgfolder = "I:\\OCR_Line\\synth_english\\db_read_test\\";
    string modelfolder = "I:\\OCR_Line\\synth_english\\crnn\\crnn_256\\";
    string lexiconfile = "I:\\OCR_Line\\synth_english\\lexicon.txt";
    test_ocr_english(imgfolder, modelfolder, lexiconfile, is_seq);
  }
  imgfolder = "D:/code/pudn/ocr/AstarLight/CPS-OCR-Engine/ocr/dataset/test/";
  modelfolder = "E:/OCR_Line/model/densenet-no-blstm/";
  outfile = "E:/OCR_Line/out.txt";
  testfile = "E:/OCR_Line/test.txt";

  if (0) {
    imgfolder = "D:/code/pudn/ocr/AstarLight/CPS-OCR-Engine/ocr/dataset/test/";
    imgfolder = "";
    modelfolder = "E:/OCR_Line/model/sss/";
    outfile = modelfolder + "out.txt";
    testfile = modelfolder + "test.txt";
    scale = 0.00390625;
    is_seq = false;
  }

  if (1) {
    imgfolder = "";
    modelfolder = "E:/OCR_Line/model/densenet-no-blstm/";
    outfile = modelfolder + "out.txt";
    testfile = modelfolder + "test.txt";
    scale = 0;
    is_seq = true;
  }

	test_ocr_chinese(imgfolder, modelfolder, outfile, testfile.c_str(), scale, is_seq);

	return 0;
}

