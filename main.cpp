#include <QCoreApplication>
#include <opencv.hpp>
#include <classifier.h>
#include <ctrain.h>
#include <gflags/gflags.h>
//#include <glog/logging.h>
#include <string>
using namespace cv;
using namespace std;
void class_one()
{
    string model_file="/home/joyoung/qtwork/caffe_classify/demo/deploy.prototxt";
    string mean_file="/home/joyoung/qtwork/caffe_classify/demo/mean.binaryproto";
    string trained_file="/home/joyoung/qtwork/caffe_classify/demo/snapshot_iter_980.caffemodel";
    string label_file="/home/joyoung/qtwork/caffe_classify/demo/labels.txt";
    Classifier classifier(model_file, trained_file, mean_file, label_file);
    //Mat src=imread("/home/joyoung/caffe-master/examples/images/cat.jpg");
    VideoCapture cap;
    cap.open(0);
    Mat src;
    while(1)
    {
        cap>>src;
        std::vector<Prediction> predictions = classifier.Classify(src,1);
        for (size_t i = 0; i < predictions.size(); ++i) {
          Prediction p = predictions[i];
          std::cout << std::fixed << std::setprecision(4) << p.second << " - \""
                    << p.first << "\"" << std::endl;
        }
        imshow("test",src);
        waitKey(10);
    }

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

