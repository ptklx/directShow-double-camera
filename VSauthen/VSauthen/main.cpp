#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>
#include <opencv2/opencv.hpp>
#include <functional>
#include <io.h>
#include <direct.h>
#include <string> 
//#include <vector>
#include <string.h> 
#include <cstring>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#include "camerads.h"
//#include <highgui.h>
cv::Directory dir;

using namespace cv;

const char *g_szTitle = "Camera";

using namespace cv;

using namespace std;
CvCapture *capture;                              //视频采集设备
IplImage *frame;                                 //视频采集图像

int createFolder(const string path)
{
    if (0 != ::_access(path.c_str(), 0))
    {
        // this folder not exist
        if (0 != ::_mkdir(path.c_str()))
        {
            printf("create folder fail !");
            return -1;
        }
    }
    else
    {
        printf("%s  folder already exists !", path.c_str());
    }

    return 0;
}


int _tmain(int argc, _TCHAR* argv[])
{
    int m_iCamCount = CCameraDS::CameraCount();
    printf("There are %d cameras.\n", m_iCamCount);

    if (m_iCamCount == 0)
    {
        return (-1);
    }

    CCameraDS m_CamDS, m_CamDS1;

    //获取所有摄像头的名称

    char szCamName[1024];

    int retval = m_CamDS.CameraName(0, szCamName, sizeof(szCamName));
    if (retval >0)
    {
        printf("Camera #%d's Name is '%s'.\n", 0, szCamName);
    }
    else
    {
        printf("Can not get Camera #%d's name.\n", 0);
    }

    retval = m_CamDS1.CameraName(1, szCamName, sizeof(szCamName));

    if (retval >0)
    {
        printf("Camera #%d's Name is '%s'.\n", 1, szCamName);
    }
    else
    {
        printf("Can not get Camera #%d's name.\n", 1);
    }
    string foldpath = "..//..//collectImage";

    /*CString path = "../../../STL/stl2";
    if (!PathIsDirectory(path))
    {
        ::CreateDirectory(path, 0);
    }*/
    if (createFolder(foldpath) == -1)
    {
        return false;
    }
    

    string exten2 = "*";//"Image*";//"*"  
    bool addPath2 = true;//false  
    vector<string> foldernames = dir.GetListFolders(foldpath, exten2, addPath2);

    int filenum = foldernames.size();


    //
    //string path1 = "E:/data/image";
    //string exten1 = "*.bmp";//"*"  
    //bool addPath1 = false;//true; 


    //vector<string> filenames = dir.GetListFiles(path1, exten1, addPath1);

    //string path3  = "E:/data/image";
    //string exten3  = "*";
    //bool addPath3  = true;//false  

    //vector<string> allfilenames  = dir.GetListFilesR(path3, exten3, addPath3);


    int m_iCamNum = 0; // 摄像头编号

    IplImage *pFrame = NULL, *pFrame1 = NULL;

    //1280x 960   960 x 720  640x 480  and so on
    //1280 x 720    

    if ((!m_CamDS.OpenCamera(1, false, 1920, 1080)) || ((pFrame = m_CamDS.QueryFrame()) == NULL))
    {
        printf("Can not open Camera 0 .\n");
        cin >> m_iCamNum;
        return -1;

    }

    if ((!m_CamDS1.OpenCamera(0, false, 1920, 1080)) || ((pFrame1 = m_CamDS1.QueryFrame()) == NULL))
    {
        printf("Can not open Camera 0 .\n");
        cin >> m_iCamNum;
        return -1;
    }

    //cvNamedWindow(g_szTitle);
    string fatherpath;
    string colorpath; 
    string graypath;
    int picNum = 0;
    cvNamedWindow(g_szTitle);
    while (true)
    {
        if (picNum>600||picNum == 0)
        {
            fatherpath = foldpath + "//" + to_string(filenum);
            if (createFolder(fatherpath) == -1)
            {
                break;
            }
            colorpath = fatherpath + "//color";
            if (createFolder(colorpath) == -1)
            {
                break;
            }
            graypath = fatherpath + "//gray";
            if (createFolder(graypath) == -1)
            {
                break;
            }
            picNum = 0;
            filenum += 1;
        }


        pFrame = m_CamDS.QueryFrame();
        pFrame1 = m_CamDS1.QueryFrame();
        if (pFrame == NULL || pFrame1 == NULL)
            break;

        string imgsave = colorpath +"//"+ to_string(picNum) + ".jpg";
        cvSaveImage(imgsave.c_str(), pFrame);
        string imgsave1 = graypath + "//" + to_string(picNum) + ".jpg";
        cvSaveImage(imgsave1.c_str(), pFrame1);
        picNum += 1;
        

        cvShowImage(g_szTitle, pFrame);
        cvShowImage("camera1", pFrame1);

        if (cvWaitKey(10) == 'q')
        {
            break;
        }
        
    }
    
    m_CamDS.CloseCamera();
    m_CamDS1.CloseCamera();

    return 0;
}