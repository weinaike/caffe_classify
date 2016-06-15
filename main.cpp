#include <QCoreApplication>
#include <opencv.hpp>
#include <classifier.h>
#include <ctrain.h>
#include <gflags/gflags.h>
//#include <glog/logging.h>
#include <string>
using namespace cv;
using namespace std;
void prep_treat(Mat & src,Mat & dst);
void class_one()
{
    string path="/home/joyoung/qtwork/caffe_classify/demo-google/";
    string model_file=path+"deploy.prototxt";
    string mean_file=path+"mean.binaryproto";
    string trained_file=path+"snapshot_iter.caffemodel";
    string label_file=path+"labels.txt";
    Classifier classifier(model_file, trained_file, mean_file, label_file);

    VideoCapture cap;
    cap.open(0);
    //Mat src,dst,src_org,dst_org,mean;
    Mat src,dst;
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,1200);
    cap.set(CV_CAP_PROP_FRAME_WIDTH,1600);
    while(1)
    {
        double t=(double)getTickCount();
        cap>>src;
        /*
        for(int i=0;i<10;i++)
        {
            cap>>src_org;
            src_org.convertTo(dst_org,CV_32FC3,1.0/255.0);
            if(i==0)
            {
                mean=dst_org/10;
            }
            mean+=dst_org/10;
        }
        mean.convertTo(src,CV_8UC3,255.0);
        */
        prep_treat(src,dst);
        std::vector<Prediction> predictions = classifier.Classify(dst,1);
        t=((double)getTickCount()-t)/getTickFrequency();
        Prediction p;
        for (size_t i = 0; i < predictions.size(); ++i) {
          p = predictions[i];
          std::cout << std::fixed << std::setprecision(4) << p.second << " - \""
                    << p.first << "\"" <<"     time: "<<t<<"s"<<std::endl;
        }
        putText(dst,p.first,Point(20,30),FONT_HERSHEY_SIMPLEX,1,Scalar(255,255,255),2);
        imshow("bean",dst);
        waitKey(1);

    }

}
void prep_treat(Mat & src, Mat & dst)
{
    Size m_size=src.size();
    int len=0;
    if(m_size.height>m_size.width)
        len=m_size.width;
    else
        len=m_size.height;
    int left,top;
    left=(m_size.width-len)/2;
    top=(m_size.height-len)/2;
    Rect m_rect;
    m_rect=Rect(left,top,len,len);
    dst=src(m_rect);
    cv::resize(dst,dst,Size(256,256));
}

void train_test()
{
    string solverfile="cifar10_full_solver.prototxt";
    CTrain cifar_train(solverfile);
    cifar_train.train();

}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    class_one();
    //train_test();
    return a.exec();
}

