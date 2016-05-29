#include <QCoreApplication>
#include <opencv.hpp>
#include <classifier.h>
#include <ctrain.h>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <string>
using namespace cv;
using namespace std;
void class_one()
{
    string model_file="/home/wnk/caffe-master/models/bvlc_reference_caffenet/deploy.prototxt";
    string mean_file="/home/wnk/caffe-master/data/ilsvrc12/imagenet_mean.binaryproto";
    string trained_file="/home/wnk/caffe-master/models/bvlc_reference_caffenet/bvlc_reference_caffenet.caffemodel";
    string label_file="/home/wnk/caffe-master/data/ilsvrc12/synset_words.txt";
    Classifier classifier(model_file, trained_file, mean_file, label_file);
    Mat src=imread("/home/wnk/caffe-master/examples/images/cat.jpg");
    std::vector<Prediction> predictions = classifier.Classify(src,1);
    for (size_t i = 0; i < predictions.size(); ++i) {
      Prediction p = predictions[i];
      std::cout << std::fixed << std::setprecision(4) << p.second << " - \""
                << p.first << "\"" << std::endl;
    }
    imshow("test",src);
    waitKey(300);
/*
 *  VideoCapture cap;
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
        waitKey(300);
    }
*/
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

