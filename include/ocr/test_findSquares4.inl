#include <opencv2/opencv.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <WINSOCK2.H>
#include<iostream>
#include<thread>
#include <winsock2.h>
#include <stdio.h>
#include<string>
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")
#include<vector>
 
using namespace cv;
 
 
					//////////////////////////////////////////////////////////////////
					//�������ܣ�����������COS��=������֮��/������ģ�ĳ˻��������߶μн�
					//���룺   �߶�3��������pt1,pt2,pt0,���һ������Ϊ������
					//�����   �߶μнǣ���λΪ�Ƕ�
					//////////////////////////////////////////////////////////////////
double angle(CvPoint* pt1, CvPoint* pt2, CvPoint* pt0)
{
	double dx1 = pt1->x - pt0->x;
	double dy1 = pt1->y - pt0->y;
	double dx2 = pt2->x - pt0->x;
	double dy2 = pt2->y - pt0->y;
	double angle_line = (dx1*dx2 + dy1 * dy2) / sqrt((dx1*dx1 + dy1 * dy1)*(dx2*dx2 + dy2 * dy2) + 1e-10);//����ֵ
	return acos(angle_line) * 180 / 3.141592653;
}
//////////////////////////////////////////////////////////////////
//�������ܣ����ö���μ�⣬ͨ��Լ������Ѱ�Ҿ���
//���룺   img ԭͼ��
//          storage �洢
//          minarea��maxarea �����ε���С/������
//          minangle,maxangle �����α߼нǷ�Χ����λΪ�Ƕ�
//�����   ��������
//////////////////////////////////////////////////////////////////
CvSeq* findSquares4(IplImage* img, CvMemStorage* storage, int minarea, int maxarea, int minangle, int maxangle, int(&temp)[30])
{
	CvSeq* contours;//��Ե
	int N = 6;  //��ֵ�ּ�
	CvSize sz = cvSize(img->width & -2, img->height & -2);
	IplImage* timg = cvCloneImage(img);//����һ��img
	IplImage* gray = cvCreateImage(sz, 8, 1); //img�Ҷ�ͼ
	IplImage* pyr = cvCreateImage(cvSize(sz.width / 2, sz.height / 2), 8, 3);  //�������˲�3ͨ��ͼ���м����
	IplImage* tgray = cvCreateImage(sz, 8, 1); ;
	CvSeq* result;
	double s, t;
	int sk = 0;
	CvSeq* squares = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvPoint), storage);
 
	cvSetImageROI(timg, cvRect(0, 0, sz.width, sz.height));
	//�������˲� 
	cvPyrDown(timg, pyr, 7);
	cvPyrUp(pyr, timg, 7);
	//��3��ͨ����Ѱ�Ҿ��� 
	for (int c = 0; c < 3; c++) //��3��ͨ���ֱ���д��� 
	{
		cvSetImageCOI(timg, c + 1);
		cvCopy(timg, tgray, 0);  //���ν�BGRͨ������tgray         
		for (int l = 0; l < N; l++)
		{
			//��ͬ��ֵ�¶�ֵ��
			cvThreshold(tgray, gray, 75, 250, CV_THRESH_BINARY);
			cvShowImage("111", gray);
			cvFindContours(gray, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));
			while (contours)
			{ //����αƽ�             
				result = cvApproxPoly(contours, sizeof(CvContour), storage, CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0);
 
				//�����͹�ı��β�������ڷ�Χ�� 
				if (result->total == 4 && fabs(cvContourArea(result, CV_WHOLE_SEQ)) > minarea  && fabs(cvContourArea(result, CV_WHOLE_SEQ)) < maxarea &&  cvCheckContourConvexity(result))
				{
 
					s = 0;
					//�ж�ÿһ����
					for (int i = 0; i < 5; i++)
					{
						if (i >= 2)
						{   //�Ƕ�            
							t = fabs(angle((CvPoint*)cvGetSeqElem(result, i), (CvPoint*)cvGetSeqElem(result, i - 2), (CvPoint*)cvGetSeqElem(result, i - 1)));
							s = s > t ? s : t;
						}
					}
					//�����SΪֱ���ж����� ��λΪ�Ƕ�
					if (s > minangle && s < maxangle)
					{
						for (int i = 0; i < 4; i++)
							cvSeqPush(squares, (CvPoint*)cvGetSeqElem(result, i));
						CvRect rect = cvBoundingRect(contours, 1);       // ��ȡ���α߽�� 
						CvPoint p1;
						p1 = cvPoint(rect.x + rect.width / 2, rect.y + rect.height / 2);   //������������  
						std::cout << "X:" << p1.x << "Y��" << p1.y << std::endl;
					}
				}
				contours = contours->h_next;
			}
		}
		std::cout << "Բ��������"<<sk << std::endl;
		temp[26] = sk;
 
	
		sk = 0;
	}
	cvReleaseImage(&gray);
	cvReleaseImage(&pyr);
	cvReleaseImage(&tgray);
	cvReleaseImage(&timg);
 
	return squares;
}
//////////////////////////////////////////////////////////////////
//�������ܣ��������о���
//���룺   img ԭͼ��
//          squares ��������
//          wndname ��������
//�����   ͼ���б�Ǿ���
//////////////////////////////////////////////////////////////////
void drawSquares(IplImage* img, CvSeq* squares, const char* wndname)
{
	CvSeqReader reader;
	IplImage* cpy = cvCloneImage(img);
	CvPoint pt[4];
	int i;
	cvStartReadSeq(squares, &reader, 0);
	for (i = 0; i < squares->total; i += 4)
	{
		CvPoint* rect = pt;
		int count = 4;
		memcpy(pt, reader.ptr, squares->elem_size);
		CV_NEXT_SEQ_ELEM(squares->elem_size, reader);
		memcpy(pt + 1, reader.ptr, squares->elem_size);
		CV_NEXT_SEQ_ELEM(squares->elem_size, reader);
		memcpy(pt + 2, reader.ptr, squares->elem_size);
		CV_NEXT_SEQ_ELEM(squares->elem_size, reader);
		memcpy(pt + 3, reader.ptr, squares->elem_size);
		CV_NEXT_SEQ_ELEM(squares->elem_size, reader);
		//cvPolyLine( cpy, &rect, &count, 1, 1, CV_RGB(0,255,0), 3, CV_AA, 0 );
		cvPolyLine(cpy, &rect, &count, 1, 1, CV_RGB(rand() & 255, rand() & 255, rand() & 255), 1, CV_AA, 0);//��ɫ����
	}
	cvShowImage("22", cpy);
	cvReleaseImage(&cpy);
}

using namespace std;

int LoadTextFileList(const string& testfile, std::vector<string>& imgs) {
	FILE* input = NULL;
	input = fopen(testfile.c_str(), "rb");
	if (input) {
		char buf[1024];
		char fname[256];
		for (; fgets(buf, 1024, input) > 0; ) {
			sscanf(buf, "%s", fname);
			imgs.push_back(fname);
		}
		fclose(input);
	}
	return imgs.size();
}

int test_findSquares4()
{
	Mat edges;  //����ת���ĻҶ�ͼ
	const char* winn = "1111";

	std::vector<string> flist;
	LoadTextFileList("E:/OCR_Line/demo_images/list.txt", flist);
	CvMemStorage* storage = 0;
	storage = cvCreateMemStorage(0);
	for (int j = 0; j < flist.size(); ++j) {
		int Y = 0, J = 0;
		Mat frame;
		frame = imread(flist[j], IMREAD_COLOR);
		resize(frame, frame, Size(), 0.5, 0.5, INTER_LANCZOS4);
		IplImage img0 = frame;
		int a[30];
		drawSquares(&img0, findSquares4(&img0, storage, 100, 2000, 80, 100, a), winn);

		Mat E = frame(Range(1, 320), Range(1, 240));
		cvtColor(frame, edges, CV_BGR2GRAY);

		imshow("��Ч��ͼ��", frame);
		waitKey(-1);
	}
	return 0;
}

int test_findSquares4_cam()
{
	//��������ͷ
	VideoCapture capture;
	capture.open(0);
	Mat edges;  //����ת���ĻҶ�ͼ
	if (!capture.isOpened())
		namedWindow("��Ч��ͼ��", CV_WINDOW_NORMAL);
	const char* winn = "1111";
	if (!capture.isOpened())
		//namedWindow(winn, CV_WINDOW_NORMAL);
		CvMemStorage* storage = 0;
	CvMemStorage* storage = 0;
	storage = cvCreateMemStorage(0);
	while (1)
	{
		int Y = 0, J = 0;
		Mat frame;
		capture >> frame;
		IplImage img0 = frame;
		//drawSquares(&img0, findSquares4(&img0, storage, 100, 2000, 80, 100, a), winn);
		//cvClearMemStorage(storage);  //��մ洢
		Mat E = frame(Range(1, 320), Range(1, 240));
		cvtColor(frame, edges, CV_BGR2GRAY);
		//��˹�˲�
		GaussianBlur(edges, edges, Size(7, 7), 2, 2);
		std::vector<Vec3f> circles;//�洢ÿ��Բ��λ����Ϣ
								   //����Բ
		HoughCircles(edges, circles, CV_HOUGH_GRADIENT, 1.5, 10, 100, 100, 0, 50);
		for (size_t i = 0; i < circles.size(); i++)
		{
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			//std::cout << "Բ��X��" << circles[i][0] << "Բ��Y��" << circles[i][1] << std:: endl;
			//����Բ����  
			circle(frame, center, radius, Scalar(155, 50, 255), 3, 8, 0);
			int R = frame.at<Vec3b>(cvRound(circles[i][1]), cvRound(circles[i][0]))[2];//R
			int G = frame.at<Vec3b>(cvRound(circles[i][1]), cvRound(circles[i][0]))[1];//G
			int B = frame.at<Vec3b>(cvRound(circles[i][1]), cvRound(circles[i][0]))[0];//B
			int num = R + G + B;
			std::cout << "Բ����ɫ��" << num << std::endl;
		}

		imshow("��Ч��ͼ��", frame);
		waitKey(30);
	}
	return 0;
}
 
