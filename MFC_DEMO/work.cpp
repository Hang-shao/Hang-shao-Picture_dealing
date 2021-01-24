#include "stdafx.h"
#include "MFC_DEMO.h"
#include "MFC_DEMODlg.h"
#include "afxdialogex.h"
#include "NewForm.h"
#include "newform2.h"
#include "newform3.h"
#include "newform6.h"
#include "CEncryptionMFCDlg.h"
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
#include <opencv2\tracking.hpp>
#include "videoio.hpp"

#include <ctype.h>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;
//��ȡͼ����

//����Ƶ��ͼ��
void CMFC_DEMODlg::open2()
{
	CFileDialog Openfiledlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL);
	if (Openfiledlg.DoModal() == IDOK) //����ģ̬�Ի���  
	{
		//CString����ת��Ϊstring����  
	
		CString  filepath;
		filepath = Openfiledlg.GetPathName();
		CStringA temp(filepath.GetBuffer(0));
		SetDlgItemText(IDC_ImagePath1, filepath);
		filepath.ReleaseBuffer();
		_Path = temp.GetBuffer(0);

		temp.ReleaseBuffer();
		string ext = strrchr(_Path.c_str(), '.') + 1;		
		if (ext == "mp4" || ext == "avi") 
		{
			AfxMessageBox("��ʽ����");
		}
		else
		{
			image = imread(_Path);
			ShowPic_1(image);
			AfxMessageBox("ͼ���ȡ���");
		}
	}
}

//���Ϊ
void CMFC_DEMODlg::save2()
{
	//TCHAR szFilter[] = _T("jpgͼ��(*.jpg)|*.jpg|png�ļ�(*.png*)|*.png*||");
	CFileDialog fileDlg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		TEXT("Supported Types (*.jpg;*.png;*.gif;*.bmp;...)|*.jpg;*.png;*.gif;*.bmp|Tiff(*.tiff;*.tif)|*.tiff;*.tif|All Files(*.*)|*.*||"), NULL);
	if (fileDlg.DoModal() == IDOK) //����ģ̬�Ի���  
	{
		CString  filepath;
		filepath = fileDlg.GetPathName();	
		char *s_path;
		s_path = filepath.GetBuffer(filepath.GetLength());
		imwrite(s_path, out);
		filepath.ReleaseBuffer();
	}
}

//��Ƶ����
Mat images;
bool backprojMode = false; //��ʾ�Ƿ�Ҫ���뷴��ͶӰģʽ��ture��ʾ׼�����뷴��ͶӰģʽ  
bool selectObject = false;//�����Ƿ���ѡҪ���ٵĳ�ʼĿ�꣬true��ʾ���������ѡ��  
int trackObject = 0; //�������Ŀ����Ŀ  
bool showHist = true;//�Ƿ���ʾֱ��ͼ  
Point origin;//���ڱ������ѡ���һ�ε���ʱ���λ��  
Rect selection;//���ڱ������ѡ��ľ��ο�  
int vmin = 10, vmax = 50, smin = 30;

 void CMFC_DEMODlg::onMouse(int event, int x, int y, int, void*)
{
	if (selectObject)
	{
		selection.x = MIN(x, origin.x);//�������ϽǶ�������  
		selection.y = MIN(y, origin.y);
		selection.width = std::abs(x - origin.x);//���ο�  
		selection.height = std::abs(y - origin.y);//���θ�  

		selection &= Rect(0, 0, images.cols, images.rows);//����ȷ����ѡ�ľ���������ͼƬ��Χ��
	}

	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:
		origin = Point(x, y);
		selection = Rect(x, y, 0, 0);
		selectObject = true;
		break;
	case CV_EVENT_LBUTTONUP:
		selectObject = false;
		if (selection.width > 0 && selection.height > 0)
			trackObject = -1;
		break;
	}
}
const char* keys =
{
	"{1|  | 0 | camera number}"
};

//void CMFC_DEMODlg::videoG()
//{
//	//int argc; const char** argv;
//	string out = _Path;
//	capture.open(out);
//
//	//help();
//
//	//VideoCapture cap;//ʵ����
//	Rect trackWindow;
//	int hsize = 16;
//	//hranges�ں���ļ���ֱ��ͼ������Ҫ�õ�
//	float hranges[] = { 0,180 };
//	const float* phranges = hranges;
//
//	//�����Ӧ����
//	setMouseCallback("view2", onMouse, 0);
//
//	Mat frame, hsv, hue, mask, hist, histimg = Mat::zeros(200, 320, CV_8UC3), backproj;
//	bool paused = false;
//
//	for (;;)
//	{
//		if (!paused)//û����ͣ
//		{
//			//������ͷץȡһ֡ͼ�������frame�� 
//			capture >> frame;
//			if (frame.empty())
//				break;
//		}
//
//		frame.copyTo(images);
//
//		if (!paused)//û�а���ͣ��
//		{
//			//����ɫ�ռ�ת����HSV
//			cvtColor(images, hsv, COLOR_BGR2HSV);
//			//trackObject��ʼ��Ϊ0,���߰�����̵�'c'����ҲΪ0������굥���ɿ���Ϊ-1 
//			if (trackObject)
//			{
//				int _vmin = vmin, _vmax = vmax;
//				//inRange()���������������ÿ��Ԫ�ش�С�Ƿ���2��������ֵ֮��
//				inRange(hsv, Scalar(0, smin, MIN(_vmin, _vmax)),
//					Scalar(180, 256, MAX(_vmin, _vmax)), mask);
//				int ch[] = { 0, 0 };
//				//hue��ʼ��Ϊ��hsv��С���һ���ľ���ɫ���Ķ������ýǶȱ�ʾ�ģ�
//				//������֮�����120�ȣ���ɫ���180�� 
//				hue.create(hsv.size(), hsv.depth());
//				//ͨ�����
//				mixChannels(&hsv, 1, &hue, 1, ch, 1);
//				//���ѡ�������ɿ��󣬸ú����ڲ��ֽ��丳ֵ1
//				if (trackObject < 0)
//				{
//					//�˴��Ĺ��캯��roi�õ���Mat hue�ľ���ͷ����roi������ָ��ָ��hue��
//					//��������ͬ�����ݣ�selectΪ�����Ȥ������
//					Mat roi(hue, selection), maskroi(mask, selection);
//					//calcHist����ֱ��ͼ
//					calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);
//					//��һ��
//					normalize(hist, hist, 0, 255, CV_MINMAX);
//
//					trackWindow = selection;
//					trackObject = 1;
//
//					histimg = Scalar::all(0);
//					int binW = histimg.cols / hsize;
//					Mat buf(1, hsize, CV_8UC3);
//					for (int i = 0; i < hsize; i++)
//						buf.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>(i*180. / hsize), 255, 255);
//					cvtColor(buf, buf, CV_HSV2BGR);
//
//					for (int i = 0; i < hsize; i++)
//					{
//						int val = saturate_cast<int>(hist.at<float>(i)*histimg.rows / 255);
//						//��һ������ͼ���ϻ�һ���򵥳�ľ��Σ�ָ�����ϽǺ����½ǣ���������ɫ����С�����͵�
//						rectangle(histimg, Point(i*binW, histimg.rows),
//							Point((i + 1)*binW, histimg.rows - val),
//							Scalar(buf.at<Vec3b>(i)), -1, 8);
//					}
//				}
//				//����ֱ��ͼ�ķ���ͶӰ������hueͼ��0ͨ��ֱ��ͼhist�ķ���ͶӰ��
//				//������backproj��
//				calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
//				backproj &= mask;
//				//trackWindowΪ���ѡ�������TermCriteriaΪȷ��������ֹ��׼��
//				RotatedRect trackBox = CamShift(backproj, trackWindow,
//					//CV_TERMCRIT_EPS��ͨ��forest_accuracy,CV_TERMCRIT_ITER
//					TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1));
//				if (trackWindow.area() <= 1)
//				{
//					int cols = backproj.cols, rows = backproj.rows, r = (MIN(cols, rows) + 5) / 6;
//					trackWindow = Rect(trackWindow.x - r, trackWindow.y - r,
//						trackWindow.x + r, trackWindow.y + r) &
//						Rect(0, 0, cols, rows);
//					//Rect����Ϊ�����ƫ�ƺʹ�С������һ��������Ϊ��������Ͻǵ����꣬�����ĸ�����Ϊ����Ŀ�͸�
//				}
//
//				if (backprojMode)
//					//���ͶӰģʽ����ʾ��Ҳ��rgbͼ
//					cvtColor(backproj, images, COLOR_GRAY2BGR);
//				//���ٵ�ʱ������ԲΪ����Ŀ��
//				ellipse(images, trackBox, Scalar(0, 0, 255), 3, CV_AA);
//			}
//		}
//		else if (trackObject < 0)
//			paused = false;
//
//		if (selectObject && selection.width > 0 && selection.height > 0)
//		{
//			Mat roi(images, selection);
//			bitwise_not(roi, roi);
//		}
//
//		ShowPic_2(images);
//
//		char c = (char)waitKey(70);
//		if (c == 27)
//			break;
//		switch (c)
//		{
//		case 'b':
//			backprojMode = !backprojMode;
//			break;
//		case 'c':
//			trackObject = 0;
//			histimg = Scalar::all(0);
//			break;
//		case 'h':
//			showHist = !showHist;
//			if (!showHist)
//				destroyWindow("Histogram");
//			else
//				namedWindow("Histogram", 1);
//			break;
//		case 'p':
//			paused = !paused;
//			break;
//		default:
//			;
//		}
//	}
//}

//ת���ɻҶ�ͼ��
Mat CMFC_DEMODlg::ImageGray1()
{
	Mat huidu, out2;
	cvtColor(image, huidu, COLOR_BGR2GRAY);
	return huidu;
}

//��ֵ������
void CMFC_DEMODlg::ImageEZH1()
{
	Mat huidu;
	newform2  Dl;
	if (Dl.DoModal() == IDOK) {
		int k = Dl.canshu;
		cvtColor(image, huidu, COLOR_BGR2GRAY);
		threshold(huidu, out, k, 255, CV_THRESH_BINARY);
		ShowPic_2(out);
	}
}

//��ֵ�˲�
void CMFC_DEMODlg::ImageJZ1()
{
	newform6  D;
	if (D.DoModal() == IDOK) {
		int k = D.n;
		//Mat junzhi,out2;
		out.create(image.size(), image.type());
		blur(image, out, Size(k, k));
		ShowPic_2(out);
	}
}
//��˹�˲�
void CMFC_DEMODlg::ImageGS1()
{
	newform6  D;
	if (D.DoModal() == IDOK) {
		int k = D.n;
		GaussianBlur(image, out, Size(k,k), 0, 0); 
		ShowPic_2(out);
	}
}
//������˹
Mat CMFC_DEMODlg::ImageLPLS1()
{
	Mat lpls;
	Mat K = (Mat_<double>(3, 3) << -1, -1, -1, -1, 9, -1, -1, -1, -1);
	filter2D(image, lpls, image.depth(), K);
	return lpls;
}

//canny����
void CMFC_DEMODlg::ImageCANNY1()
{
	//Mat huidu;
	NewForm  Dl;
	if (Dl.DoModal() == IDOK) {
		//NewForm q;
		int m= Dl.pos;
		cvtColor(image, out, COLOR_BGR2GRAY);
		Canny(out, out, m, m*3, 3);
		ShowPic_2(out);
	}
}

//sobel����
void CMFC_DEMODlg::ImageSOBEL()
{
	newform6  Dl;
	if (Dl.DoModal() == IDOK) {
		int m = Dl.n;
		Sobel(image, out, CV_16S, 1, 0, m);
		ShowPic_2(out);
	}
}

//��ɫͼ����⻯
Mat CMFC_DEMODlg::ImageJHH1()
{
	Mat out2;
	Mat P;
	vector<Mat>V;  //����Mat�������������ֺ������
	Mat B;
	Mat G;
	Mat R;
	split(image, V);//ͨ������
	B = V.at(0);//��ɫͨ��
	G = V.at(1);//��ɫͨ��
	R = V.at(2);//��ɫͨ��
	//ֱ��ͼ���⻯
	equalizeHist(B, B);
	equalizeHist(G, G);
	equalizeHist(R, R);

	merge(V, P);
	return P;
}
// ���⻯ǰ�Ĳ�ɫֱ��ͼ
Mat  CMFC_DEMODlg::ImageJHH2()
{
	Mat out2;

	MatND BHist, GHist, RHist; //��ά����     
	int dims = 1;//һάֱ��ͼ
				 //int dims = 3;
	float hranges[] = { 0, 255 };
	const float *ranges[] = { hranges };   // ������ҪΪconst����
	int size = 256;
	//int channels = 0;
	int channels_b[] = { 0 };
	int channels_g[] = { 1 };
	int channels_r[] = { 2 };

	calcHist(&image, 1, channels_b, Mat(), BHist, dims, &size, ranges);
	calcHist(&image, 1, channels_g, Mat(), GHist, dims, &size, ranges);
	calcHist(&image, 1, channels_r, Mat(), RHist, dims, &size, ranges);

	int scale = 1;
	//��ȡ���ֵ����Сֵ
	//double minValue = 0;
	double maxValue_b, maxValue_g, maxValue_r;
	//���������ҵ�ȫ����Сֵ�����ֵ
	//������Сֵ�����ֵ
	minMaxLoc(BHist, 0, &maxValue_b, 0, 0);
	minMaxLoc(GHist, 0, &maxValue_g, 0, 0);
	minMaxLoc(RHist, 0, &maxValue_r, 0, 0);
	//����ֱ��ͼ�ľ���
	//static MatExpr Mat::zeros(int rows, int cols, int type)
	Mat New = Mat::zeros(256, 256 * 3, CV_8UC3);

	for (int i = 0; i < 256; i++)
	{
		//���Ҷȼ�Ƶ����ע��hist����float����
		float binValue_b = BHist.at<float>(i);
		float binValue_g = GHist.at<float>(i);
		float binValue_r = RHist.at<float>(i);
		//Ҫ���Ƶ���ʵ�߶�
		int realValue_b = saturate_cast<int>(binValue_b * 256 / maxValue_b);
		int realValue_g = saturate_cast<int>(binValue_g * 256 / maxValue_g);
		int realValue_r = saturate_cast<int>(binValue_r * 256 / maxValue_r);
		//���λử��New����
		//Point(i*scale, size - 1)���ζ���
		// Point((i+1)*scale - 1, size - realValue)ǰһ���ĶԽǶ���
		//��ɫ����
		rectangle(New, Point(i*scale, size - 1), Point((i + 1)*scale - 1, size - realValue_b), CV_RGB(0, 0, 255));
		rectangle(New, Point((i + 256)*scale, size - 1), Point((i + 256 + 1)*scale - 1, size - realValue_g), CV_RGB(0, 255, 0));
		rectangle(New, Point((i + 256 * 2)*scale, size - 1), Point((i + 256 * 2 + 1)*scale - 1, size - realValue_r), CV_RGB(255, 0, 0));
	}
	return New;
}
// ������⻯��Ĳ�ɫֱ��ͼ
Mat CMFC_DEMODlg::ImageJHH3()
{
	Mat out2;
	Mat P;
	vector<Mat>V;  //����Mat�������������ֺ������
	Mat B;
	Mat G;
	Mat R;
	split(image, V);//ͨ������
	B = V.at(0);//��ɫͨ��
	G = V.at(1);//��ɫͨ��
	R = V.at(2);//��ɫͨ��
				//ֱ��ͼ���⻯
	equalizeHist(B, B);
	equalizeHist(G, G);
	equalizeHist(R, R);
	merge(V, P);
	MatND BHist, GHist, RHist; //��ά����     
	int dims = 1;//һάֱ��ͼ
				 //int dims = 3;
	float hranges[] = { 0, 255 };
	const float *ranges[] = { hranges };   // ������ҪΪconst����
	int size = 256;
	//int channels = 0;
	int channels_b[] = { 0 };
	int channels_g[] = { 1 };
	int channels_r[] = { 2 };
	//����ͼ���ֱ��ͼ
	//&yuan�������飬1ͼ������� &channels��Ҫͳ�Ƶ�ͨ��������Mat()��OutArray���͵�hist����õ���ֱ��ͼ
	//int���͵�dims����Ҫ�����ֱ��ͼ��ά��
	//&sizeÿ��ά��ֱ��ͼ������������������� bin�ĸ���
	//const float**���͵�ranges����һ����ά���飬����ָ��ÿ������ķ�Χ
	calcHist(&P, 1, channels_b, Mat(), BHist, dims, &size, ranges);
	calcHist(&P, 1, channels_g, Mat(), GHist, dims, &size, ranges);
	calcHist(&P, 1, channels_r, Mat(), RHist, dims, &size, ranges);
	int scale_2 = 1;
	//��ȡ���ֵ����Сֵ
	//double minValue = 0;
	double maxValue_b2, maxValue_g2, maxValue_r2;
	//���������ҵ�ȫ����Сֵ�����ֵ
	//������Сֵ�����ֵ
	minMaxLoc(BHist, 0, &maxValue_b2, 0, 0);
	minMaxLoc(GHist, 0, &maxValue_g2, 0, 0);
	minMaxLoc(RHist, 0, &maxValue_r2, 0, 0);
	//����ֱ��ͼ�ľ���
	//static MatExpr Mat::zeros(int rows, int cols, int type)
	Mat New_2 = Mat::zeros(256, 256 * 3, CV_8UC3);

	//int hpt = saturate_cast<int>(0.9 * size);
	for (int i = 0; i < 256; i++)
	{
		//���Ҷȼ�Ƶ����ע��hist����float����
		float binValue_b2 = BHist.at<float>(i);
		float binValue_g2 = GHist.at<float>(i);
		float binValue_r2 = RHist.at<float>(i);
		//Ҫ���Ƶ���ʵ�߶�
		int realValue_b2 = saturate_cast<int>(binValue_b2 * 256 / maxValue_b2);
		int realValue_g2 = saturate_cast<int>(binValue_g2 * 256 / maxValue_g2);
		int realValue_r2 = saturate_cast<int>(binValue_r2 * 256 / maxValue_r2);
		//���λử��New����
		//Point(i*scale, size - 1)���ζ���
		// Point((i+1)*scale - 1, size - realValue)ǰһ���ĶԽǶ���
		//��ɫ����
		rectangle(New_2, Point(i*scale_2, size - 1), Point((i + 1)*scale_2 - 1, size - realValue_b2), CV_RGB(0, 0, 255));
		rectangle(New_2, Point((i + 256)*scale_2, size - 1), Point((i + 256 + 1)*scale_2 - 1, size - realValue_g2), CV_RGB(0, 255, 0));
		rectangle(New_2, Point((i + 256 * 2)*scale_2, size - 1), Point((i + 256 * 2 + 1)*scale_2 - 1, size - realValue_r2), CV_RGB(255, 0, 0));
	}
	return New_2;
}

//�Ҷ�ͼ����⻯
Mat CMFC_DEMODlg::ImageZFT1()
{
	Mat yuan, out2;
	cvtColor(image, yuan, COLOR_BGR2GRAY); //�Ƚ��лҶȻ�
	Mat yuan2;
	equalizeHist(yuan, yuan2);//ֱ��ͼ���⻯
	return yuan2;
}
// ���⻯ǰ�Ĳ�ɫֱ��ͼ
Mat CMFC_DEMODlg::ImageZFT2()
{
	Mat yuan,out2;
	cvtColor(image, yuan, COLOR_BGR2GRAY);
									
	MatND dstHist; //��ά����     
	int dims = 1;//һάֱ��ͼ
	float hranges[] = { 0, 255 };
	const float *ranges[] = { hranges };   // ������ҪΪconst����
	int size = 256;
	int channels = 0;
	//����ͼ���ֱ��ͼ
	//&yuan�������飬1ͼ������� &channels��Ҫͳ�Ƶ�ͨ��������Mat()��OutArray���͵�hist����õ���ֱ��ͼ
	//int���͵�dims����Ҫ�����ֱ��ͼ��ά��
	//&sizeÿ��ά��ֱ��ͼ������������������� bin�ĸ���
	//const float**���͵�ranges����һ����ά���飬����ָ��ÿ������ķ�Χ
	calcHist(&yuan, 1, &channels, Mat(), dstHist, dims, &size, ranges);
	int scale = 1;
	Mat New(size, size * scale, CV_8U, Scalar(0));
	//��ȡ���ֵ����Сֵ
	double minValue = 0;
	double maxValue = 0;
	//���������ҵ�ȫ����Сֵ�����ֵ
	//������Сֵ�����ֵ
	minMaxLoc(dstHist, &minValue, &maxValue, 0, 0);
	//���Ƴ�ֱ��ͼ
	int hpt = saturate_cast<int>(0.9 * size);
	for (int i = 0; i < 256; i++)
	{
		float binValue = dstHist.at<float>(i); //���Ҷȼ�Ƶ����ע��hist����float����  
		int realValue = saturate_cast<int>(binValue * hpt / maxValue);
		//���λử��New����
		//Point(i*scale, size - 1)���ζ���
		// Point((i+1)*scale - 1, size - realValue)ǰһ���ĶԽǶ���
		// Scalar(255)��ɫ
		rectangle(New, Point(i*scale, size - 1), Point((i + 1)*scale - 1, size - realValue), Scalar(255));
	}
	return New;
}
// ���⻯��Ĳ�ɫֱ��ͼ
Mat CMFC_DEMODlg::ImageZFT3()
{
	Mat yuan, out2;
	cvtColor(image, yuan, COLOR_BGR2GRAY);
	MatND dstHist; //��ά����     
	int dims = 1;//һάֱ��ͼ
	float hranges[] = { 0, 255 };
	const float *ranges[] = { hranges };   // ������ҪΪconst����
	int size = 256;
	int channels = 0;
	calcHist(&yuan, 1, &channels, Mat(), dstHist, dims, &size, ranges);
	Mat yuan2;
	equalizeHist(yuan, yuan2);//ֱ��ͼ���⻯

	MatND dstHist_2;
	calcHist(&yuan2, 1, &channels, Mat(), dstHist_2, dims, &size, ranges);
	int scale_2 = 1;
	Mat New2(size, size * scale_2, CV_8U, Scalar(0));
	//��ȡ���ֵ����Сֵ
	double minValue_2 = 0;
	double maxValue_2 = 0;
	//���������ҵ�ȫ����Сֵ�����ֵ
	//������Сֵ�����ֵ
	minMaxLoc(dstHist_2, &minValue_2, &maxValue_2, 0, 0);
	//���Ƴ�ֱ��ͼ
	int hpt_2 = saturate_cast<int>(0.9 * size);
	for (int i = 0; i < 256; i++)
	{
		float binValue = dstHist_2.at<float>(i); //���Ҷȼ�Ƶ����ע��hist����float����  
		int realValue = saturate_cast<int>(binValue * hpt_2 / maxValue_2);
		//���λử��New����
		//Point(i*scale, size - 1)���ζ���
		// Point((i+1)*scale - 1, size - realValue)ǰһ���ĶԽǶ���
		// Scalar(255)��ɫ
		rectangle(New2, Point(i*scale_2, size - 1), Point((i + 1)*scale_2 - 1, size - realValue), Scalar(255));	
	}
	return New2;
}

//��ɫͼ�����ԻҶȱ任
void CMFC_DEMODlg::ImageHUIDU()
{
	//��ȡMat��Ϣ
	out = Mat::zeros(image.size(), image.type());
	CV_Assert(image.depth() != sizeof(uchar));
	int channels = image.channels();
	int nRows = image.rows;
	int nCols = image.cols * channels;
	uchar *p, *q;
	newform3  Dl;   // ��ȡ�������
	if (Dl.DoModal() == IDOK) {
		int k = Dl.liangdu;   // ����
		int b= Dl.duibidu;    // �Աȶ�
		for (int i = 0; i < nRows; i++)
		{
			p = image.ptr<uchar>(i);
			q = out.ptr<uchar>(i);
			for (int j = 0; j < nCols; j++)
				q[j] = saturate_cast<uchar> (k * p[j] * 0.01 + b - 100);
		}
		ShowPic_2(out);
	}
}

//�Ҷ�ͼ��ֶ�����
struct coordinate
{
	uchar x;
	uchar y;
};
coordinate v1{ 0,50 };
coordinate v2{ 205,255 };
coordinate p1{ 20,20 };
coordinate p2{ 125,120 };
coordinate p3{ 230,230 };
void CMFC_DEMODlg::ImageHUIDU_2()
{
	Mat image_2 = Mat::zeros(image.size(), CV_8UC1);
	//Mat image_2;
	cvtColor(image, image_2, COLOR_BGR2GRAY);
	//�ж�p1�ĺ������Ƿ�С��p2�ĺ�����
	if (p1.x >= p2.x)
		return ;

	//����һ����ԭͼƬͬ����С��Mat
	Mat out = Mat::zeros(image.size(), CV_8UC1);
	//Mat out;
	//p1,p2Ϊ�յ��ӳ�����
	uchar table[256];
	for (int i = 0; i < p1.x; i++)
	{
		table[i] = i * p1.y / p1.x;
	}
	for (int i = p1.x; i < p2.x; i++)
	{
		table[i] = (i - p1.x) * (p2.y - p1.y) / (p2.x - p1.x) + p1.y;
	}
	for (int i = p2.x; i < 256; i++)
	{
		table[i] = (i - p2.x) * (255 - p2.y) / (255 - p2.x) + p2.y;
	}

	//��ȡMat��Ϣ
	CV_Assert(image_2.depth() != sizeof(uchar));
	int channels = image_2.channels();
	int nRows = image_2.rows;
	int nCols = image_2.cols * channels;

	//����table��ѭ�������ҵ���Ӧ��ֵ��ֵ��newImg��
	uchar *p, *q;
	for (int i = 0; i < nRows; i++)
	{
		p = image_2.ptr<uchar>(i);
		q = out.ptr<uchar>(i);
		for (int j = 0; j < nCols; j++)
			q[j] = table[p[j]];
		ShowPic_2(out);
	}
}
//�Ҷ�ͼ�������
void CMFC_DEMODlg::ImageHUIDU_3()
{
	newform2  Dl;
	if (Dl.DoModal() == IDOK) {
		int o = Dl.canshu;
		Mat image_2 = Mat::zeros(image.size(), CV_8UC1);
		cvtColor(image, image_2, COLOR_BGR2GRAY);
		out = Mat::zeros(image_2.size(), CV_8UC1);
		int channels = image_2.channels();
		int nRows = image_2.rows;
		int nCols = image_2.cols* channels;
		uchar* p;

		int k;
		double g = 0;
		for (int x = 0; x < nRows; x++) {
			for (int y = 0; y < nCols; y++) {
				g = image_2.ptr(x)[y];
				/*
				saturate_cast<uchar>��Ҫ��Ϊ�˷�ֹ��ɫ�������
				*/
				g = 1 * saturate_cast<uchar>(pow((double)g / 255., o)*255.0f);
				out.ptr(x)[y] = g;
			}
		}
		ShowPic_2(out);
	}
}

//newform1 *pe;
//Ƶ��ͼ
Mat CMFC_DEMODlg::ImageDCT()
{
	// ����ͼ��
	cvtColor(image, img, COLOR_BGR2GRAY);
	TZ();
	ZXH();
	//Ϊ����Ҷ�任�Ľ��(ʵ�����鲿)����洢�ռ�
	//�����ǻ�ȡ������mat��һ�����ڴ��dft�任��ʵ����һ�����ڴ���鲿����ʼ��ʱ��ʵ������ͼ�����鲿ȫΪ0
	Mat plane[] = { tz_img, Mat::zeros(tz_img.size(), CV_32F) };
	Mat complexImg;
	merge(plane, 2, complexImg); //��plane�ںϺϲ���һ����ͨ������complexI
								 //��������ͨ����mat�ںϳ�һ����ͨ����mat�������ںϵ�complexImg����ʵ���������鲿
								 //���ٸ���Ҷ�任��Ƶ����ʾ
	dft(complexImg, complexImg);
	Mat outs = ShowSpectrum(complexImg);
	return outs;
}
//�����ͨ�˲���Ƶ��ͼ
Mat CMFC_DEMODlg::ImageDCT_1()
{
	// ����ͼ��
	cvtColor(image, img, COLOR_BGR2GRAY);
	TZ();
	ZXH();
	Mat plane[] = { tz_img, Mat::zeros(tz_img.size(), CV_32F) };
	Mat complexImg;
	merge(plane, 2, complexImg); //��plane�ںϺϲ���һ����ͨ������complexI
								 //��������ͨ����mat�ںϳ�һ����ͨ����mat�������ںϵ�complexImg����ʵ���������鲿
								 //���ٸ���Ҷ�任��Ƶ����ʾ
	dft(complexImg, complexImg);
	Mat complexFilter = complexImg.clone();
	////////�����/��ͨ�˲���/////////////
	IdealFilter(complexFilter, 100, true);//true��ͨ
	Mat outs=ShowSpectrum(complexFilter);
	//pe->ShowPic2(out);
	return outs;
}
//�����ͨ�˲���Ƶ��ͼ
Mat CMFC_DEMODlg::ImageDCT_2()
{
	cvtColor(image, img, COLOR_BGR2GRAY);
	TZ();
	ZXH();
	Mat plane[] = { tz_img, Mat::zeros(tz_img.size(), CV_32F) };
	Mat complexImg;
	merge(plane, 2, complexImg); //��plane�ںϺϲ���һ����ͨ������complexI
								 //��������ͨ����mat�ںϳ�һ����ͨ����mat�������ںϵ�complexImg����ʵ���������鲿
								 //���ٸ���Ҷ�任��Ƶ����ʾ
	dft(complexImg, complexImg);
	Mat complexFilter = complexImg.clone();
	////////�����/��ͨ�˲���/////////////
	IdealFilter(complexFilter, 100, false);//false��ͨ
	Mat outs = ShowSpectrum(complexFilter);
	return outs;
}
//�����ͨƵ���˲�����ԭͼ
Mat CMFC_DEMODlg::ImageLIX_1()
{
	cvtColor(image, img, COLOR_BGR2GRAY);
	TZ();
	ZXH();
	Mat plane[] = { tz_img, Mat::zeros(tz_img.size(), CV_32F) };
	Mat complexImg;
	merge(plane, 2, complexImg); //��plane�ںϺϲ���һ����ͨ������complexI
								 //��������ͨ����mat�ںϳ�һ����ͨ����mat�������ںϵ�complexImg����ʵ���������鲿
								 //���ٸ���Ҷ�任��Ƶ����ʾ
	dft(complexImg, complexImg);

	Mat complexFilter = complexImg.clone();
	////////�����/��ͨ�˲���/////////////
	IdealFilter(complexFilter, 100, true);//true��ͨ
	idft(complexFilter, complexFilter);
	Mat dd=ShowImage(complexFilter);//�˲������ͼ����ʾ
	return dd;
}
//�����ͨƵ���˲�����ԭͼ
Mat CMFC_DEMODlg::ImageLIX_2()
{
	// ����ͼ��
	cvtColor(image, img, COLOR_BGR2GRAY);
	//img = imread("input.jpg", 0);
	TZ();
	ZXH();
	Mat plane[] = { tz_img, Mat::zeros(tz_img.size(), CV_32F) };
	Mat complexImg;
	merge(plane, 2, complexImg); //��plane�ںϺϲ���һ����ͨ������complexI
								 //��������ͨ����mat�ںϳ�һ����ͨ����mat�������ںϵ�complexImg����ʵ���������鲿
								 //���ٸ���Ҷ�任��Ƶ����ʾ
	dft(complexImg, complexImg);

	Mat complexFilter = complexImg.clone();
	////////�����/��ͨ�˲���/////////////
	//IdealFilter(complexFilter, 100, true);//true��ͨ
										  IdealFilter(complexFilter, 100, false);//false��ͨ

	idft(complexFilter, complexFilter);
	Mat dd_2 = ShowImage(complexFilter);//�˲������ͼ����ʾ
	return dd_2;
}

//��˹��ͨ�˲���Ƶ��ͼ
Mat CMFC_DEMODlg::ImageDCT_3()
{
	// ����ͼ��
	cvtColor(image, img, COLOR_BGR2GRAY);
	TZ();
	ZXH();
	Mat plane[] = { tz_img, Mat::zeros(tz_img.size(), CV_32F) };
	Mat complexImg;
	merge(plane, 2, complexImg); //��plane�ںϺϲ���һ����ͨ������complexI
								 //��������ͨ����mat�ںϳ�һ����ͨ����mat�������ںϵ�complexImg����ʵ���������鲿
								 //���ٸ���Ҷ�任��Ƶ����ʾ
	dft(complexImg, complexImg);
	Mat complexFilter = complexImg.clone();
										  ///////////��˹��/��ͨ�˲���//////////
	GaussianFilter(complexFilter, 100, true);//true��ͨ
									
	Mat outs = ShowSpectrum(complexFilter);
	return outs;
}
//��˹��ͨ�˲���Ƶ��ͼ
Mat CMFC_DEMODlg::ImageDCT_4()
{
	// ����ͼ��
	cvtColor(image, img, COLOR_BGR2GRAY);
	TZ();
	ZXH();
	//Ϊ����Ҷ�任�Ľ��(ʵ�����鲿)����洢�ռ�
	//�����ǻ�ȡ������mat��һ�����ڴ��dft�任��ʵ����һ�����ڴ���鲿����ʼ��ʱ��ʵ������ͼ�����鲿ȫΪ0
	Mat plane[] = { tz_img, Mat::zeros(tz_img.size(), CV_32F) };
	Mat complexImg;
	merge(plane, 2, complexImg); //��plane�ںϺϲ���һ����ͨ������complexI
								 //��������ͨ����mat�ںϳ�һ����ͨ����mat�������ںϵ�complexImg����ʵ���������鲿
								 //���ٸ���Ҷ�任��Ƶ����ʾ
	dft(complexImg, complexImg);
	Mat complexFilter = complexImg.clone();
	GaussianFilter(complexFilter, 100, false);//false��ͨ								
	Mat outs = ShowSpectrum(complexFilter);
	return outs;
}

//��˹��ͨƵ���˲�����ԭͼ
Mat CMFC_DEMODlg::ImageLIX_3()
{
	// ����ͼ��
	cvtColor(image, img, COLOR_BGR2GRAY);
	TZ();
	ZXH();

	Mat plane[] = { tz_img, Mat::zeros(tz_img.size(), CV_32F) };
	Mat complexImg;
	merge(plane, 2, complexImg); //��plane�ںϺϲ���һ����ͨ������complexI
								 //��������ͨ����mat�ںϳ�һ����ͨ����mat�������ںϵ�complexImg����ʵ���������鲿
								 //���ٸ���Ҷ�任��Ƶ����ʾ
	dft(complexImg, complexImg);
	Mat complexFilter = complexImg.clone();
	///////////��˹��/��ͨ�˲���//////////
	GaussianFilter(complexFilter, 100, true);//true��ͨ								
	idft(complexFilter, complexFilter);
	Mat dd_3 = ShowImage(complexFilter);//�˲������ͼ����ʾ
	return dd_3;
	
}
//��˹��ͨƵ���˲�����ԭͼ
Mat CMFC_DEMODlg::ImageLIX_4()
{
	// ����ͼ��
	cvtColor(image, img, COLOR_BGR2GRAY);
	TZ();
	ZXH();
	//Ϊ����Ҷ�任�Ľ��(ʵ�����鲿)����洢�ռ�
	//�����ǻ�ȡ������mat��һ�����ڴ��dft�任��ʵ����һ�����ڴ���鲿����ʼ��ʱ��ʵ������ͼ�����鲿ȫΪ0
	Mat plane[] = { tz_img, Mat::zeros(tz_img.size(), CV_32F) };
	Mat complexImg;
	merge(plane, 2, complexImg); //��plane�ںϺϲ���һ����ͨ������complexI
								 //��������ͨ����mat�ںϳ�һ����ͨ����mat�������ںϵ�complexImg����ʵ���������鲿
								 //���ٸ���Ҷ�任��Ƶ����ʾ
	dft(complexImg, complexImg);
	Mat complexFilter = complexImg.clone();
	///////////��˹��/��ͨ�˲���//////////
	//GaussianFilter(complexFilter, 100, true);//true��ͨ
	GaussianFilter(complexFilter, 100, false);//false��ͨ
	idft(complexFilter, complexFilter);
	Mat dd = ShowImage(complexFilter);//�˲������ͼ����ʾ
	return dd;
}

//������˹��ͨ�˲���Ƶ��ͼ
Mat CMFC_DEMODlg::ImageDCT_5()
{
	// ����ͼ��
	cvtColor(image, img, COLOR_BGR2GRAY);
	TZ();
	ZXH();
	//Ϊ����Ҷ�任�Ľ��(ʵ�����鲿)����洢�ռ�
	//�����ǻ�ȡ������mat��һ�����ڴ��dft�任��ʵ����һ�����ڴ���鲿����ʼ��ʱ��ʵ������ͼ�����鲿ȫΪ0
	Mat plane[] = { tz_img, Mat::zeros(tz_img.size(), CV_32F) };
	Mat complexImg;
	merge(plane, 2, complexImg); //��plane�ںϺϲ���һ����ͨ������complexI
								 //��������ͨ����mat�ںϳ�һ����ͨ����mat�������ںϵ�complexImg����ʵ���������鲿
								 //���ٸ���Ҷ�任��Ƶ����ʾ
	dft(complexImg, complexImg);
	Mat complexFilter = complexImg.clone();

//������˹��/��ͨ�˲���
	ButterworthFilter(complexFilter, 50, 2, true);
    //ButterworthFilter(complexFilter, 100, 2, false);

	Mat outs = ShowSpectrum(complexFilter);
	return outs;
}
//������˹��ͨ�˲���Ƶ��ͼ
Mat CMFC_DEMODlg::ImageDCT_6()
{
	// ����ͼ��
	cvtColor(image, img, COLOR_BGR2GRAY);
	TZ();
	ZXH();
	//Ϊ����Ҷ�任�Ľ��(ʵ�����鲿)����洢�ռ�
	//�����ǻ�ȡ������mat��һ�����ڴ��dft�任��ʵ����һ�����ڴ���鲿����ʼ��ʱ��ʵ������ͼ�����鲿ȫΪ0
	Mat plane[] = { tz_img, Mat::zeros(tz_img.size(), CV_32F) };
	Mat complexImg;
	merge(plane, 2, complexImg); //��plane�ںϺϲ���һ����ͨ������complexI
								 //��������ͨ����mat�ںϳ�һ����ͨ����mat�������ںϵ�complexImg����ʵ���������鲿
								 //���ٸ���Ҷ�任��Ƶ����ʾ
	dft(complexImg, complexImg);
	Mat complexFilter = complexImg.clone();

	/////////������˹��/��ͨ�˲���///////////
	ButterworthFilter(complexFilter, 50, 2, false);

	Mat outs = ShowSpectrum(complexFilter);
	return outs;
}
//������˹��ͨƵ���˲�����ԭͼ
Mat CMFC_DEMODlg::ImageLIX_5()
{
	// ����ͼ��
	cvtColor(image, img, COLOR_BGR2GRAY);
	TZ();
	ZXH();
	//Ϊ����Ҷ�任�Ľ��(ʵ�����鲿)����洢�ռ�
	//�����ǻ�ȡ������mat��һ�����ڴ��dft�任��ʵ����һ�����ڴ���鲿����ʼ��ʱ��ʵ������ͼ�����鲿ȫΪ0
	Mat plane[] = { tz_img, Mat::zeros(tz_img.size(), CV_32F) };
	Mat complexImg;
	merge(plane, 2, complexImg); //��plane�ںϺϲ���һ����ͨ������complexI
								 //��������ͨ����mat�ںϳ�һ����ͨ����mat�������ںϵ�complexImg����ʵ���������鲿
								 //���ٸ���Ҷ�任��Ƶ����ʾ
	dft(complexImg, complexImg);
	Mat complexFilter = complexImg.clone();
	/////////������˹��/��ͨ�˲���///////////
	ButterworthFilter(complexFilter, 50, 2, true);
	idft(complexFilter, complexFilter);
	Mat dd = ShowImage(complexFilter);//�˲������ͼ����ʾ
	return dd;
}
//������˹��ͨƵ���˲�����ԭͼ
Mat CMFC_DEMODlg::ImageLIX_6()
{
	cvtColor(image, img, COLOR_BGR2GRAY);
	TZ();
	ZXH();
	//Ϊ����Ҷ�任�Ľ��(ʵ�����鲿)����洢�ռ�
	//�����ǻ�ȡ������mat��һ�����ڴ��dft�任��ʵ����һ�����ڴ���鲿����ʼ��ʱ��ʵ������ͼ�����鲿ȫΪ0
	Mat plane[] = { tz_img, Mat::zeros(tz_img.size(), CV_32F) };
	Mat complexImg;
	merge(plane, 2, complexImg); //��plane�ںϺϲ���һ����ͨ������complexI
								 //��������ͨ����mat�ںϳ�һ����ͨ����mat�������ںϵ�complexImg����ʵ���������鲿
								 //���ٸ���Ҷ�任��Ƶ����ʾ
	dft(complexImg, complexImg);
	Mat complexFilter = complexImg.clone();
	/////////������˹��/��ͨ�˲���///////////
	ButterworthFilter(complexFilter, 50, 2, false);
	idft(complexFilter, complexFilter);
	Mat dd = ShowImage(complexFilter);//�˲������ͼ����ʾ
	return dd;
}

//��չͼ���Խ��и�Ч��DFT
void CMFC_DEMODlg::TZ()
{
	int h = getOptimalDFTSize(img.rows);//����DFT���ųߴ��С
	int w = getOptimalDFTSize(img.cols);
	if (w > img.cols || h > img.rows)
		copyMakeBorder(img, tz_img, 0, h - img.rows, 0, w - img.cols, BORDER_CONSTANT, Scalar::all(0));
	else
		img.copyTo(tz_img);
}
//�׵����Ļ�
void CMFC_DEMODlg::ZXH()
{
	tz_img.convertTo(tz_img, CV_32FC1);
	for (int i = 0; i<tz_img.rows; i++)
	{
		float *ptr = tz_img.ptr<float>(i);
		for (int j = 0; j < tz_img.cols; j++)
			ptr[j] *= pow(-1.0, i + j);//��(-1)^(x+y)������չ���ͼ��
	}
}

// ��ʾDFT���Ƶ��
Mat CMFC_DEMODlg::ShowSpectrum(Mat input)
{
	Mat plane[2];
	//��complexImg���²�ֳ�����mat��һ����ʵ����һ�����鲿
	split(input, plane); //��plane[0]Ϊʵ��,planes[1]Ϊ�鲿
						 //������mat��Ӧλ�����
	magnitude(plane[0], plane[1], plane[0]);//�����άʸ���ķ�ֵ
	plane[0] += Scalar::all(1);
	log(plane[0], plane[0]);//�Ը���Ҷ��ִ�ж����任����ǿϸ��
	normalize(plane[0], plane[0], 0, 1, CV_MINMAX);//��һ��
	Mat tmp = plane[0];
	return tmp;
}
// ��ʾIDFT��ĸ�ԭͼ��
Mat CMFC_DEMODlg::ShowImage(Mat input)
{
	Mat plane[2];
	split(input, plane);//��complexImg���²�ֳ�����mat��һ����ʵ����һ�����鲿
	for (int i = 0; i<plane[0].rows; i++)//ȡ������ͼ��ĳ���
	{
		float *ptr = plane[0].ptr<float>(i);
		for (int j = 0; j < plane[0].cols; j++)
			ptr[j] *= pow((double)(-1), (double)(i + j));
	}
	normalize(plane[0], plane[0], 0, 1, CV_MINMAX);//��һ��
	Mat tmp = plane[0];
	return tmp;
}
//��˹��/��ͨ�˲���
void CMFC_DEMODlg::GaussianFilter(Mat& input, float D0, bool flag)
{
	Mat GaussianKernel(input.size(), CV_32FC2);
	float d0 = 2 * pow(double(D0), 2.0);
	for (int i = 0; i<input.rows; i++)
	{
		float*p = GaussianKernel.ptr<float>(i);
		for (int j = 0; j<input.cols; j++)
		{
			float d2 = pow(i - input.rows / 2, 2.0) + pow(j - input.cols / 2, 2.0);
			if (flag)//trueΪ��ͨ
			{
				p[2 * j] = expf(-d2 / d0);
				p[2 * j + 1] = expf(-d2 / d0);
			}
			else//��ͨ
			{
				p[2 * j] = 1 - expf(-d2 / d0);
				p[2 * j + 1] = 1 - expf(-d2 / d0);
			}
		}
	}
	multiply(input, GaussianKernel, input);
}
//�����/��ͨ�˲���
void CMFC_DEMODlg::IdealFilter(Mat& input, float D0, bool flag)
{
	Mat IdealKernel(input.size(), CV_32FC2);

	for (int i = 0; i<input.rows; i++)
	{
		float*p = IdealKernel.ptr<float>(i);
		for (int j = 0; j<input.cols; j++)
		{
			float d2 = pow(i - input.rows / 2, 2.0) + pow(j - input.cols / 2, 2.0);
			float d3 = sqrt(d2);
			if (flag)//trueΪ��ͨ
			{
				if (d3 <= D0)
				{//ָ������֮�ڵĵ�Ƶ�ɷ�ȫ������
					p[2 * j] = 1;
					p[2 * j + 1] = 1;
				}
				else
				{//ָ������֮��ĸ�Ƶ�ɷ�ȫ�����ض�
					p[2 * j] = 0;
					p[2 * j + 1] = 0;
				}
			}
			else//��ͨ
			{
				if (d3 <= D0)
				{//ָ������֮�ڵĵ�Ƶ�ɷ�ȫ�����ض�
					p[2 * j] = 0;
					p[2 * j + 1] = 0;
				}
				else
				{//ָ������֮��ĸ�Ƶ�ɷ�ȫ������
					p[2 * j] = 1;
					p[2 * j + 1] = 1;
				}
			}
		}
	}
	multiply(input, IdealKernel, input);
}

//������˹��/��ͨ�˲���
void CMFC_DEMODlg::ButterworthFilter(Mat& input, float D0, float n, bool flag)
{
	Mat ButterworthKernel(input.size(), CV_32FC2);
	for (int i = 0; i<input.rows; i++)
	{
		float*p = ButterworthKernel.ptr<float>(i);
		for (int j = 0; j<input.cols; j++)
		{
			float d2 = pow(i - input.rows / 2, 2.0) + pow(j - input.cols / 2, 2.0);
			float d3 = sqrt(d2);
			if (flag)//true��ͨ
			{
				p[2 * j] = 1 / (pow(d3 / D0, (2 * n)) + 1);
				p[2 * j + 1] = 1 / (pow(d3 / D0, (2 * n)) + 1);
			}
			else//false��ͨ
			{
				p[2 * j] = 1 / (pow(D0 / d3, (2 * n)) + 1);
				p[2 * j + 1] = 1 / (pow(D0 / d3, (2 * n)) + 1);
			}
		}
	}
	multiply(input, ButterworthKernel, input);
}