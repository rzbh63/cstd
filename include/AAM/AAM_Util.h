/****************************************************************************
* 
* Copyright (c) 2008 by Yao Wei, all rights reserved.
* Copyright (c) 2008 by Magic: http://magic.nju.edu.cn/
*
* Author:      	Yao Wei
* Contact:     	njustyw@gmail.com
* 
* This software is partly based on the following open source: 
*  
*		- OpenCV 
* 
****************************************************************************/

#ifndef AAM_UTIL_H
#define AAM_UTIL_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "cv.h"
#include <highgui.h>


std::ostream& operator<<(std::ostream &os, const CvMat* mat);

std::istream& operator>>(std::istream &is, CvMat* mat);

std::vector<std::string> ScanNSortDirectory(const std::string &path, 
											const std::string &extension);
class AAM_Shape;

class AAM
{
public:
	AAM();
	virtual ~AAM() = 0;

	//Fit the image using aam 
	virtual int Fit(const IplImage* image, AAM_Shape& Shape, 
		int max_iter = 30, bool showprocess = false) = 0;

	//Draw the image according search result
	virtual void Draw(IplImage* image, int type) = 0;

	// Read data from stream 
	virtual void Read(std::ifstream& is) = 0;

	// write data to stream
	virtual void Write(std::ofstream& os) = 0;

	//Get Mean Shape of model
	virtual const AAM_Shape GetMeanShape()const = 0;
};

#endif // AAM_UTIL_H