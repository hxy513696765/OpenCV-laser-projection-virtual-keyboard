//#include "stdafx.h"
#include <cv.h> 
#include <highgui.h>  
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
//#include<WinUser.h>
#include<Windows.h>

using namespace cv;
using namespace std;

Mat src; Mat src_gray;
IplImage* bin_mimage = NULL;

//int Get_Form_thresh = 20;
//int Get_Form_max_thresh = 255;


int thresh = 60;
int max_thresh = 255;
RNG rng(12345);
char Keyboard_String_table[5][15]=
{
	{     '1'   ,   '2'  ,   '3'   , '4'  , '5'  ,    '6'   ,   '7'    ,   '8'    ,   '9'    ,    '0'   , VK_SUBTRACT  , VK_ADD   ,   NULL    ,    NULL   ,   NULL    },
	{VK_ESCAPE  ,   'Q'  ,   'W'   , 'E'  , 'R'  ,    'T'   ,   'Y'    ,    'U'   ,   'I'    ,    'O'   ,      'P'     , VK_BACK  ,   NULL    ,    NULL   ,   NULL    },
	{VK_CAPITAL , VK_TAB ,   'A'   , 'S'  , 'D'  ,    'F'   ,   'G'    ,    'H'   ,   'J'    ,    'K'   ,      'L'     ,    ';'   , VK_RETURN ,    NULL   ,   NULL    },
	{VK_SHIFT   ,   '`'  ,   '|'   , 'Z'  , 'X'  ,    'C'   ,   'V'    ,    'B'   ,   'N'    ,    'M'   ,      ','     ,    '.'   ,   VK_UP   ,    '/'    , VK_SHIFT  },
	{VK_CONTROL , VK_MENU, VK_LWIN , '['  , ']'  , VK_SPACE , VK_SPACE , VK_SPACE , VK_SPACE , VK_SPACE ,      ','     , VK_LEFT  , VK_DOWN   , VK_RIGHT  , VK_DELETE },
};

UINT OLD_FLAG_ROW4 = 0xFFFFF;
UINT NEW_FLAG_ROW4 = 0;
UCHAR Read_key_count = 0;
/// 函数声明
void thresh_callback(int, void* );
void Get_Form(int,void*);
int main( int argc, char** argv )
{
		//int count = 4;
		//CvCapture* capture;
		//for (count = 4;count!=-1;count--)
		//{
		//	capture =cvCaptureFromCAM(count) ;
		//	printf("%d\n",capture);
		//	//if (capture)
		//	//{
		//		printf("No: %d No Came \n",count);
		//	//}
		//}
		CvCapture* capture =cvCaptureFromCAM(0) ;

		//cvNamedWindow("MyOpenCV",0);   
/*		printf("%d",*capture);*/
		if(!capture)   
			return -1;  
		//else
		//	printf("No Came");
		cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH,480);   
		cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT,640);   
		int pressKey;   
		IplImage* frame=NULL; 
		IplImage* g_pGrayImage=NULL; 
		IplImage* Undistor_Image = NULL;

		IplImage* mapx;
		IplImage* mapy;

		CvMat *intrinsic = (CvMat*)cvLoad("Intrinsics.xml");
		CvMat *distortion= (CvMat*)cvLoad("Distortion.xml");

		int flag = 0;
		
		
		while(true)   
		{   
			frame = cvQueryFrame(capture);
			
	
	
			//rectangle(img,Temp_Rect,Scalar(0,0,255),2);
			//cvtColor( img, edges,CV_BGR2GRAY);
			if (frame)
			{
				/// 载入原图像, 返回3通道图像
				//Mat img(frame);
				//src = img;//imread( "boundrect.jpg" );

				Undistor_Image = cvCloneImage(frame);
				cvUndistort2(frame, Undistor_Image, intrinsic, distortion);

				Mat img(Undistor_Image);
				src = img;//imread( "boundrect.jpg" );

				//cvShowImage( "Undistort", Undistor_Image); // Show corrected image
				//cvReleaseImage(&Undistor_Image);
				
				g_pGrayImage =  cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);  
				cvCvtColor(frame, g_pGrayImage, CV_BGR2GRAY);  
				bin_mimage   =  cvCreateImage(cvGetSize(g_pGrayImage), IPL_DEPTH_8U, 1); 

				cvThreshold(g_pGrayImage, bin_mimage, thresh, 255, CV_THRESH_BINARY);  

				// 显示二值图   

				//cvtColor( src, src_gray, CV_BGR2GRAY );
				////src_gray(bin_mimage);//bin_mimage;

				//blur( src_gray, src_gray, Size(2,2) );

				cvShowImage("BinaryWindows", bin_mimage);
				Get_Form(0,0);				


				///*Mat mtx(bin_mimage);
				//src_gray = mtx;*/

				/// 转化成灰度图像并进行平滑
				cvtColor( src, src_gray, CV_BGR2GRAY );
				//src_gray(bin_mimage);//bin_mimage;
	
				blur( src_gray, src_gray, Size(2,2) );


				
				/// 创建窗口
				char* source_window = "Source";
				namedWindow( source_window, CV_WINDOW_AUTOSIZE );
				imshow( source_window, src );



				createTrackbar( " Threshold:", "Source", &thresh, max_thresh, thresh_callback );
				thresh_callback( 0, 0 );
				img.release();

				//bin_mimage_output.release();
				//mtx.release();
				cvReleaseImage(&g_pGrayImage);
				cvReleaseImage(&bin_mimage);
				cvReleaseImage(&Undistor_Image);


			} 
			else
			{
				std::cout <<  "访问异常"  <<std::endl;
				flag = 1;
			}
	
	
			//imshow("edges", img);
			pressKey = cvWaitKey(50);   
			if(pressKey == 's') //保存图片   
			{   
				cvSaveImage("../mine2.jpg",frame);//cvSave函数不行   
			}   
			else if(pressKey == 27)   
			{   
				src.release();
				src_gray.release();
				break;   
			}   
	
	
	
		}   
		cvReleaseCapture(&capture);   
		//cvDestroyWindow("MyOpenCV");   
		//cvDestroyWindow("BinaryWindows");   
		//cvDestroyWindow("edges");   
		return 0;   


}


void Get_Form(int,void*)
{
	/*---------------------------------------------------------------------------------------------------------*/

	Read_key_count = ++Read_key_count;
	Read_key_count = Read_key_count % 2;
	Mat bin_mimage_output(bin_mimage);
	IplImage* key_draw = cvLoadImage("keyNull.bmp",-1);
	//IplImage* key_draw = cvLoadImage("keybaord.bmp",-1);
	Mat draw_key_point(key_draw);
	vector<vector<Point> > bin_mimage_contours;    //轮廓数组（非矩形数组），每个轮廓是一个Point型的vector
	vector<Vec4i> bin_mimage_hierarchy;                 //见下面findContours的解释
	int Point_count = 0;
	int Key_X_axisPosion = NULL;
	unsigned char Row_count = 0;
	int Key_Press_Posion = NULL;
	int subkeypress;
	int linewhith = -1;
	//cvThreshold(g_pGrayImage, bin_mimage, thresh, 255, CV_THRESH_BINARY); 
	/// 找到轮廓
	//contours参数为检测的轮廓数组，每一个轮廓用一个point类型的vector表示
	//hiararchy参数和轮廓个数相同，每个轮廓contours[ i ]对应4个hierarchy元素hierarchy[ i ][ 0 ] ~hierarchy[ i ][ 3 ]，
	//分别表示后一个轮廓、前一个轮廓、父轮廓、内嵌轮廓的索引编号，如果没有对应项，该值设置为负数。
	//CV_RETR_TREE：建立一个等级树结构的轮廓
	//

	findContours( bin_mimage_output, bin_mimage_contours, bin_mimage_hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(10, 10) );

	/// 多边形逼近轮廓 + 获取矩形和圆形边界框
	vector<vector<Point> > contours_poly( bin_mimage_contours.size() );          //近似后的轮廓点集
	vector<Rect> boundRect( bin_mimage_contours.size() );                           //包围点集的最小矩形vector
	vector<Point2f>center( bin_mimage_contours.size() );                               //包围点集的最小圆形vector
	vector<float>radius( bin_mimage_contours.size() );                                   //包围点集的最小圆形半径vector

	for( int i = 0; i < bin_mimage_contours.size(); i++ )
	{
		approxPolyDP( Mat(bin_mimage_contours[i]), contours_poly[i], 3, true );      //对多边形曲线做适当近似，contours_poly[i]是输出的近似点集
		boundRect[i] = boundingRect( Mat(contours_poly[i]) );         //计算并返回包围轮廓点集的最小矩形
		minEnclosingCircle( contours_poly[i], center[i], radius[i] );     //计算并返回包围轮廓点集的最小圆形及其半径
	}


	/// 画多边形轮廓 + 包围的矩形框
	Mat drawing = Mat::zeros( bin_mimage_output.size(), CV_8UC3 );
	
	for( int i = 0; i< bin_mimage_contours.size(); i++ )
	{
			if (((boundRect[i].br().x  - boundRect[i].tl().x) > 10) || ((boundRect[i].br().y  - boundRect[i].tl().y) > 10))
			{
				string dis_string;
				int Num_Count = 0;	
				
				/*dis_string = format("x=%d,y=%d", Num_Count);*/

				Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );   //随机颜色

				drawContours( drawing, contours_poly, i, color, 1, 8, bin_mimage_hierarchy, 0, Point() );         //根据轮廓点集contours_poly和轮廓结构hierarchy画出轮廓

				rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );              //画矩形，tl矩形左上角，br右上角
				//rectangle( draw_key_point, boundRect[i].tl(), boundRect[i].br(), CV_RGB(255,0,0), 2, 8, 0 );              //画矩形，tl矩形左上角，br右上角
				

				Point Row_start_point,Row_end_point,Col_start_point,Col_end_point,Word_point,key_circle_center;
				Row_start_point  = boundRect[i].tl();
				Row_end_point    = boundRect[i].br();
				Row_start_point.y = (boundRect[i].br().y  + boundRect[i].tl().y) / 2;
				Row_end_point.y   = Row_start_point.y;

				line(drawing, Row_start_point, Row_end_point,CV_RGB(255,0,0),1.5,8,0);
				//line(draw_key_point, Row_start_point, Row_end_point,CV_RGB(255,0,0),2,8,0);

				Col_start_point  = boundRect[i].tl();
				Col_end_point    = boundRect[i].br();
				Col_start_point.x = (boundRect[i].br().x  + boundRect[i].tl().x) / 2;
				Col_end_point.x   = Col_start_point.x;

				line(drawing, Col_start_point, Col_end_point,CV_RGB(255,0,0),1.5,8,0);
				//line(draw_key_point, Col_start_point, Col_end_point,CV_RGB(255,0,0),2,8,0);

				key_circle_center.x = Col_start_point.x;
				key_circle_center.y = Row_start_point.y;
				Point Row_Start_axis,Row_End_axis,Col_Start_axis,Col_End_axis;

				Row_Start_axis = key_circle_center;
				Row_End_axis = key_circle_center;
				Row_Start_axis.x = Row_Start_axis.x - 10;
				Row_End_axis.x = Row_End_axis.x + 10;

				Col_Start_axis = key_circle_center;
				Col_End_axis = key_circle_center;
				Col_Start_axis.y = Col_Start_axis.y - 10;
				Col_End_axis.y = Col_End_axis.y + 10;


				//line(draw_key_point, Row_Start_axis, Row_End_axis,CV_RGB(255,0,0),2,8,0);
				//line(draw_key_point, Col_Start_axis, Col_End_axis,CV_RGB(255,0,0),2,8,0);
				//circle( draw_key_point, key_circle_center, 20, CV_RGB(255,0,0), 8, 8, 0 );
				//rectangle( draw_key_point, Point(593,181), Point(629,211), color, -1, 8, 0 ); 
				

				Word_point.x = Col_start_point.x;
				Word_point.y = Row_start_point.y;

				if ((Word_point.y <= 377) && (Word_point.y > 332))
				{
					Row_count = Row_count | 0x01;
				} 
				else if((Word_point.y <= 332) && (Word_point.y > 286))
				{
					Row_count = Row_count | 0x02;
				}
				else if((Word_point.y <= 286) && (Word_point.y > 249))
				{
					Row_count = Row_count | 0x04;
				}
				else if((Word_point.y <= 249) && (Word_point.y > 211))
				{
					Row_count = Row_count | 0x08;
				}
				else if((Word_point.y <= 211) && (Word_point.y > 171))
				{
					Row_count = Row_count | 0x10;
				}

				if (Row_count)
				{
					if (Row_count & 0x10)
					{
						if((Word_point.x > 22) && (Word_point.x < 639) && (Word_point.y <= 211) && (Word_point.y > 171))
						{
							Key_Press_Posion =15 - (Word_point.x )/40;
							if (Key_Press_Posion == 0)
							{
								rectangle( draw_key_point, Point(15 + (Key_Press_Posion * 38),265), Point(53 + (Key_Press_Posion * 38),299), CV_RGB(255,255,255), linewhith, 8, 0 );
							} 
							else if((Key_Press_Posion >= 1) && (Key_Press_Posion <= 4))
							{
								rectangle( draw_key_point, Point(19 + (Key_Press_Posion * 38),265), Point(53 + (Key_Press_Posion * 38),299), CV_RGB(255,255,255), linewhith, 8, 0 );
							}
							else if ((Key_Press_Posion >= 10) && (Key_Press_Posion <= 14))
							{
								Key_Press_Posion = Key_Press_Posion - 4;
								rectangle( draw_key_point, Point(201 + (Key_Press_Posion * 38),265), Point(235 + (Key_Press_Posion * 38),299), CV_RGB(255,255,255), linewhith, 8, 0 );
							}
							else
							{
								rectangle( draw_key_point, Point(209,265), Point(425,299), CV_RGB(255,255,255), linewhith, 8, 0 );
							}
						    
							//keybd_event(Keyboard_String_table[4][Key_Press_Posion],0,0,0);
							//keybd_event(Keyboard_String_table[4][Key_Press_Posion],0,KEYEVENTF_KEYUP,0);
							putText(drawing, format("%C",Keyboard_String_table[4][Key_Press_Posion]), Point(100,100), FONT_HERSHEY_SIMPLEX, 1.5, CV_RGB(255,255,255), 0);
							//putText(drawing, format("%d",Key_Press_Posion), Point(150,150), FONT_HERSHEY_SIMPLEX, 1.5, CV_RGB(255,255,255), 0);
						}
						else
						{
							break;
						}
					}

					if (Row_count & 0x08)
					{
						if ((Word_point.x > 589) && (Word_point.x < 638)) 
						{
								//Key_X_axisPosion  Keyboard_String_table[5][15]
								//Key_Press_Posion =0;
								rectangle( draw_key_point, Point(15 ,227), Point(66 ,261), CV_RGB(255,255,255), linewhith, 8, 0 );
								//keybd_event(Keyboard_String_table[1][0],0,0,0);
								//keybd_event(Keyboard_String_table[1][0],0,KEYEVENTF_KEYUP,0);
								putText(drawing, format("%C",Keyboard_String_table[1][0]), Point(100,100), FONT_HERSHEY_SIMPLEX, 1.5, CV_RGB(0,0,0), 0);
				
						} 
						else if((Word_point.x > 40) && (Word_point.x < 590) && (Word_point.y <= 249) && (Word_point.y > 211))
						{
							Key_Press_Posion =15 - (Word_point.x )/40;

								//if (Key_Press_Posion == 1)
								//{
								//	rectangle( draw_key_point, Point(15 + (Key_Press_Posion * 38),227), Point(66 + (Key_Press_Posion * 38),261), CV_RGB(0,0,0), linewhith, 8, 0 );
								//} 
								if (Key_Press_Posion == 14)
								{
									rectangle( draw_key_point, Point(32 + (Key_Press_Posion * 38),227), Point(83 + (Key_Press_Posion * 38),261), CV_RGB(255,255,255), linewhith, 8, 0 );
								}
								else
								{
									rectangle( draw_key_point, Point(32 + (Key_Press_Posion * 38),227), Point(66 + (Key_Press_Posion * 38),261), CV_RGB(255,255,255), linewhith, 8, 0 );
								}

							//rectangle( draw_key_point, Point(71,227), Point(105,261), color, -1, 8, 0 ); 
							//rectangle( draw_key_point, Point(109,227), Point(143,261), color, -1, 8, 0 ); 
							//rectangle( draw_key_point, Point(147,227), Point(181,261), color, -1, 8, 0 );

							//rectangle( draw_key_point, Point(71 + ((Key_Press_Posion - 1) * 38),227), Point(105 + ((Key_Press_Posion - 1) * 38),261), CV_RGB(0,0,0), 1, 8, 0 );
							//rectangle( draw_key_point, Point(500 - ((Key_Press_Posion - 1) * 40),220), Point(540 - ((Key_Press_Posion - 1) * 40),245), color, -1, 8, 0 );
							
								//subkeypress = cvWaitKey(50);   
							//if(subkeypress == 'k') //保存图片   
							//{   
							//	cvSaveImage("../temp.bmp",key_draw);//cvSave函数不行   
							//}
							//rectangle( draw_key_point, Point(568 - (32 * Key_Press_Posion) - ((Key_Press_Posion - 1) * 6),220), Point(600 - (32 * Key_Press_Posion) - ((Key_Press_Posion - 1) * 6),252), color, -1, 8, 0 ); 
							//putText(drawing, format("%d",Key_Press_Posion), Point(150,150), FONT_HERSHEY_SIMPLEX, 1.5, CV_RGB(255,255,255), 0);
							//keybd_event(Keyboard_String_table[3][14 - (Word_point.x - 15)/41],0,0,0);
							//keybd_event(Keyboard_String_table[3][14 - (Word_point.x - 15)/41],0,KEYEVENTF_KEYUP,0);
							putText(drawing, format("%C",Keyboard_String_table[3][14 - (Word_point.x - 15)/41]), Point(100,100), FONT_HERSHEY_SIMPLEX, 1.5, CV_RGB(255,255,255), 0);
						}
						else
						{
							break;
						}
					}

					if (Row_count & 0x04)
					{


						if (((Word_point.x > 576) && (Word_point.x < 620)) || ((Word_point.x > 44) && (Word_point.x < 105)) && (Word_point.y <= 286) && (Word_point.y > 249))
						{
							if (((Word_point.x > 576) && (Word_point.x < 620)))
							{
								//Key_X_axisPosion  Keyboard_String_table[5][15]
								
								rectangle( draw_key_point, Point(34 ,189), Point(85 ,223), CV_RGB(255,255,255), linewhith, 8, 0 );
								//keybd_event(Keyboard_String_table[1][0],0,0,0);
								//keybd_event(Keyboard_String_table[1][0],0,KEYEVENTF_KEYUP,0);
								putText(drawing, format("%C",Keyboard_String_table[1][0]), Point(100,100), FONT_HERSHEY_SIMPLEX, 1.5, CV_RGB(255,255,255), 0);
							} 
							else if((Word_point.x > 45) && (Word_point.x < 106))
							{
								rectangle( draw_key_point, Point(507,189), Point(592,223), CV_RGB(255,255,255), linewhith, 8, 0 );
								//keybd_event(Keyboard_String_table[1][11],0,0,0);
								//keybd_event(Keyboard_String_table[1][11],0,KEYEVENTF_KEYUP,0);
								putText(drawing, format("%C",Keyboard_String_table[1][11]), Point(100,100), FONT_HERSHEY_SIMPLEX, 1.5, CV_RGB(255,255,255), 0);
							}
							else
							{
								break;
							}
						} 
						else if((Word_point.x > 45) && (Word_point.x < 577) && (Word_point.y <= 286) && (Word_point.y > 249))
						{
							Key_Press_Posion =14 - (Word_point.x )/40;
							if (Key_Press_Posion > 0 && Key_Press_Posion < 12 )
							{
								rectangle( draw_key_point, Point(51 + (Key_Press_Posion * 38),189), Point(85 + (Key_Press_Posion * 38),223), CV_RGB(255,255,255), linewhith, 8, 0 );
							}	
							//keybd_event(Keyboard_String_table[2][11 - (Word_point.x - 105)/43],0,0,0);
							//keybd_event(Keyboard_String_table[2][11 - (Word_point.x - 105)/43],0,KEYEVENTF_KEYUP,0);
							putText(drawing, format("%C",Keyboard_String_table[2][11 - (Word_point.x - 105)/43]), Point(100,100), FONT_HERSHEY_SIMPLEX, 1.5, CV_RGB(255,255,255), 0);
							//putText(drawing, format("%d",Key_Press_Posion), Point(150,150), FONT_HERSHEY_SIMPLEX, 1.5, CV_RGB(255,255,255), 0);
						}
						else
						{
							break;
						}
					}

					if (Row_count & 0x02)
					{

						if (((Word_point.x > 552) && (Word_point.x < 600)) || ((Word_point.x > 65) && (Word_point.x < 110)) && (Word_point.y <= 332) && (Word_point.y > 286))
						{
							if (((Word_point.x > 552) && (Word_point.x < 600)))
							{
								//Key_X_axisPosion  Keyboard_String_table[5][15]
								rectangle( draw_key_point, Point(53 ,151), Point(104 ,185), CV_RGB(255,255,255), linewhith, 8, 0 );
								//keybd_event(Keyboard_String_table[1][0],0,0,0);
								//keybd_event(Keyboard_String_table[1][0],0,KEYEVENTF_KEYUP,0);
								putText(drawing, format("%C",Keyboard_String_table[1][0]), Point(100,100), FONT_HERSHEY_SIMPLEX, 1.5, CV_RGB(255,255,255), 0);
							} 
							else if((Word_point.x > 65) && (Word_point.x < 110))
							{
								rectangle( draw_key_point, Point(488,151), Point(573,185), CV_RGB(255,255,255), linewhith, 8, 0 );
								//keybd_event(Keyboard_String_table[1][11],0,0,0);
								//keybd_event(Keyboard_String_table[1][11],0,KEYEVENTF_KEYUP,0);
								putText(drawing, format("%C",Keyboard_String_table[1][11]), Point(100,100), FONT_HERSHEY_SIMPLEX, 1.5, CV_RGB(255,255,255), 0);
							}
							else
							{
								break;
							}
						} 
						else if((Word_point.x > 111) && (Word_point.x < 553) && (Word_point.y <= 332) && (Word_point.y > 286))
						{
							Key_Press_Posion =10 - (Word_point.x - 109)/44;
							rectangle( draw_key_point, Point(70 + Key_Press_Posion * 38,151), Point(104 + Key_Press_Posion * 38,185), CV_RGB(255,255,255), linewhith, 8, 0 );
							//keybd_event(Keyboard_String_table[1][10 - (Word_point.x - 109)/44],0,0,0);
							//keybd_event(Keyboard_String_table[1][10 - (Word_point.x - 109)/44],0,KEYEVENTF_KEYUP,0);
							putText(drawing, format("%C",Keyboard_String_table[1][10 - (Word_point.x - 109)/44]), Point(100,100), FONT_HERSHEY_SIMPLEX, 1.5, CV_RGB(255,255,255), 0);
						}
						else
						{
							break;
						}
					}

					if (Row_count & 0x01)
					{
						NEW_FLAG_ROW4 = 0X00000;
						//NEW_FLAG_ROW4 = 0;
/*						keybd_event(VK_SHIFT,0,0,0);   
						keybd_event(vkey,0,0,0);   
						keybd_event(vkey,0,KEYEVENTF_KEYUP,0);  */ 

						//keybd_event('A',0,0,0);
						//keybd_event('A',0,KEYEVENTF_KEYUP,0);

						//keybd_event( 'M ',   0,   0   ,0); 
						//keybd_event( 'M ',   0,   KEYEVENTF_KEYUP   ,0); 

						if (((Word_point.x > 551) && (Word_point.x < 586)) || ((Word_point.x > 77) && (Word_point.x < 116)) && (Word_point.y <= 377) && (Word_point.y > 332))
						{
							if (((Word_point.x > 551) && (Word_point.x < 586)) && (Word_point.y <= 377) && (Word_point.y > 332))
							{
								//Key_X_axisPosion  Keyboard_String_table[5][15]
								rectangle( draw_key_point, Point(89 ,113), Point(123 ,147), CV_RGB(255,255,255), linewhith, 8, 0 );
							//	keybd_event(Keyboard_String_table[0][0],0,0,0);
								NEW_FLAG_ROW4 = NEW_FLAG_ROW4 | 0x00001;
								//keybd_event(Keyboard_String_table[0][0],0,KEYEVENTF_KEYUP,0);
								putText(drawing, format("%C",Keyboard_String_table[0][0]), Point(100,100), FONT_HERSHEY_SIMPLEX, 1.5, CV_RGB(255,255,255), 0);
							} 
							else if((Word_point.x > 77) && (Word_point.x < 116) && (Word_point.y <= 377) && (Word_point.y > 332))
							{
								rectangle( draw_key_point, Point(507 ,113), Point(541 ,147), CV_RGB(255,255,255), linewhith, 8, 0 );
							//	keybd_event(Keyboard_String_table[0][11],0,0,0);
								NEW_FLAG_ROW4 = NEW_FLAG_ROW4 | 0x00800;

								//keybd_event(Keyboard_String_table[0][11],0,KEYEVENTF_KEYUP,0);
								putText(drawing, format("%C",Keyboard_String_table[0][11]), Point(100,100), FONT_HERSHEY_SIMPLEX, 1.5, CV_RGB(255,255,255), 0);
							}
							else
							{
								break;
							}
						} 
						else if((Word_point.x > 117) && (Word_point.x < 552) && (Word_point.y <= 377) && (Word_point.y > 332))
						{
							Key_Press_Posion =10 - (Word_point.x - 109)/45;
							rectangle( draw_key_point, Point(89 + Key_Press_Posion * 38,113), Point(123 + Key_Press_Posion * 38,147), CV_RGB(255,255,255), linewhith, 8, 0 );
							//keybd_event(Keyboard_String_table[0][10 - (Word_point.x - 109)/45],0,0,0);
							NEW_FLAG_ROW4 = NEW_FLAG_ROW4 | (0x00001 << Key_Press_Posion);
							//keybd_event(Keyboard_String_table[0][10 - (Word_point.x - 109)/45],0,KEYEVENTF_KEYUP,0);
							putText(drawing, format("%C",Keyboard_String_table[0][10 - (Word_point.x - 109)/45]), Point(100,100), FONT_HERSHEY_SIMPLEX, 1.5, CV_RGB(255,255,255), 0);
						}
						else
						{
							OLD_FLAG_ROW4 = 0x00000;
							break;							
						}

						OLD_FLAG_ROW4 = OLD_FLAG_ROW4 & NEW_FLAG_ROW4;
						putText(drawing, dis_string, Word_point, FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(255,255,255), 0);

					}

					dis_string = format("x=%d,y=%d", Word_point.x,Word_point.y);	
					putText(drawing, dis_string, Word_point, FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(255,255,255), 0);

				}

				if (NEW_FLAG_ROW4 && (OLD_FLAG_ROW4 != 0xFFFFF) && (1 == Read_key_count))
				{
					if (OLD_FLAG_ROW4)
					{
						for (UINT LOOP_COUNT = 0;LOOP_COUNT < 16;LOOP_COUNT++)
						{
							if ((OLD_FLAG_ROW4 >> LOOP_COUNT) & 0x00001)
							{
								keybd_event(Keyboard_String_table[0][LOOP_COUNT],0,0,0);
								keybd_event(Keyboard_String_table[0][LOOP_COUNT],0,KEYEVENTF_KEYUP,0);

							}
						}
					} 

					OLD_FLAG_ROW4 = 0xFFFFF;

				} 

				//dis_string = format("x=%d,y=%d", Word_point.x,Word_point.y);	
				//putText(drawing, dis_string, Word_point, FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(255,255,255), 0);
				++Point_count;

			} 
			else
			{
				//keybd_event(Keyboard_String_table[0][11],0,KEYEVENTF_KEYUP,0);
				NEW_FLAG_ROW4 = 0;
				continue;
			}

			//putText(drawing, format("new=%d",NEW_FLAG_ROW4), Point(150,150), FONT_HERSHEY_SIMPLEX, 1.5, CV_RGB(255,255,255), 0);

			

			//line(drawing, boundRect[i].tl(), boundRect[i].br(),color,1,16,0);
	
	}


	/*---------------------------------------------------------------------------------------------------------*/
	//putText(drawing, dis_string, Point(50, 50), FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(255,0,0), 0);
	string dis_point_count;
	dis_point_count = format("Point count: %d Row :%d",Point_count,Row_count);
	putText(drawing, dis_point_count, Point(20, 30), FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(255,0,0), 0);

	namedWindow( "BinaryWindows_Contours", CV_WINDOW_AUTOSIZE );
	imshow( "BinaryWindows_Contours", drawing );

	//Mat key_draw = Mat::zeros( bin_mimage_output.size(), CV_8UC3 );
	
	//namedWindow( "Keyboard", CV_WINDOW_AUTOSIZE );
	//cvShowImage("Keyboard",key_draw);
	//Mat rotateMat; 
	//Point2f imgcenter = cv::Point2f(draw_key_point.cols / 2, draw_key_point.rows / 2);  // 旋转中心
	//double angle = 180;  // 旋转角度
	//double scale = 1; // 缩放尺度

	///*cv::Mat rotateMat;*/
	//rotateMat = cv::getRotationMatrix2D(imgcenter, angle, scale);

	//cv::Mat rotateImg;
	//cv::warpAffine(draw_key_point, rotateImg, rotateMat, draw_key_point.size());


	namedWindow( "Keyboard", CV_WINDOW_AUTOSIZE );
	//imshow( "Keyboard",  draw_key_point );
	imshow( "Keyboard",  draw_key_point );

	cvReleaseImage(&key_draw);
	 draw_key_point.release();
	drawing.release();
	bin_mimage_output.release();
	//rotateMat.release();
	//rotateImg.release();
	//imshow( "Keyboard", key_draw );

	//Num_Count++;
	//Num_Count = Num_Count % 1000;
}


void thresh_callback(int, void* )
{
	Mat threshold_output;
	vector<vector<Point> > contours;    //轮廓数组（非矩形数组），每个轮廓是一个Point型的vector
	vector<Vec4i> hierarchy;                 //见下面findContours的解释


	/// 使用Threshold二值
	threshold( src_gray, threshold_output, thresh, 255, THRESH_BINARY );



	/// 找到轮廓
	//contours参数为检测的轮廓数组，每一个轮廓用一个point类型的vector表示
	//hiararchy参数和轮廓个数相同，每个轮廓contours[ i ]对应4个hierarchy元素hierarchy[ i ][ 0 ] ~hierarchy[ i ][ 3 ]，
	//分别表示后一个轮廓、前一个轮廓、父轮廓、内嵌轮廓的索引编号，如果没有对应项，该值设置为负数。
	//CV_RETR_TREE：建立一个等级树结构的轮廓
	//
	findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );



	/// 多边形逼近轮廓 + 获取矩形和圆形边界框
	vector<vector<Point> > contours_poly( contours.size() );          //近似后的轮廓点集
	vector<Rect> boundRect( contours.size() );                           //包围点集的最小矩形vector
	vector<Point2f>center( contours.size() );                               //包围点集的最小圆形vector
	vector<float>radius( contours.size() );                                   //包围点集的最小圆形半径vector



	for( int i = 0; i < contours.size(); i++ )
	{
		approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );      //对多边形曲线做适当近似，contours_poly[i]是输出的近似点集
		boundRect[i] = boundingRect( Mat(contours_poly[i]) );         //计算并返回包围轮廓点集的最小矩形
		minEnclosingCircle( contours_poly[i], center[i], radius[i] );     //计算并返回包围轮廓点集的最小圆形及其半径
	}


	/// 画多边形轮廓 + 包围的矩形框 + 圆形框
	Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
	for( int i = 0; i< contours.size(); i++ )
	{

			Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );   //随机颜色
			//Scalar color = Scalar( 255, 255, 255 );   //随机颜色

			//   drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
			drawContours( drawing, contours_poly, i, color, 1, 8, hierarchy, 0, Point() );         //根据轮廓点集contours_poly和轮廓结构hierarchy画出轮廓

			rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );              //画矩形，tl矩形左上角，br右上角
			circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );                                        //画圆形
	

	}

	/// 显示在一个窗口
	//namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
	//imshow( "Contours", drawing );

	drawing.release();
	threshold_output.release();
	///// 显示在一个窗口
	//namedWindow( "Bin_img", CV_WINDOW_AUTOSIZE );
	//imshow( "Bin_img", src_gray );
}
