#include <opencv.hpp>
#include <classifier.h>
#include <ctrain.h>
#include <gflags/gflags.h>
#include <string>
#include <cdirfile.h>
#include <fstream>
using namespace cv;
using namespace std;
void prep_treat(Mat & src,Mat & dst);
void class_one()
{
    string path="/home/joyoung/qtwork/caffe_classify/soymilk18-finetune/";
    string model_file=path+"deploy.prototxt";
    string mean_file=path+"mean.binaryproto";
    string trained_file=path+"snapshot_iter_1k.caffemodel";
    string label_file=path+"labels.txt";
    Classifier classifier(model_file, trained_file, mean_file, label_file);

    VideoCapture cap;
    cap.open(0);
    Mat src,dst;
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,1600);
    cap.set(CV_CAP_PROP_FRAME_WIDTH,1200);
    Mat mean,dst2;
    int mean_num=1;
    while(1)
    {
        double t=(double)getTickCount();
        cap>>src;

        prep_treat(src,dst);
        /*
        for(int i=0;i<mean_num;i++)
        {
            cap>>src;
            prep_treat(src,dst);
            dst.convertTo(dst2,CV_32FC3,1.0/255.0);
            if(i==0)
                mean=dst2/mean_num;
            else
                mean+=dst2/mean_num;
        }
        mean.convertTo(dst,CV_8UC3,255.0);
        */
        std::vector<Prediction> predictions = classifier.Classify(dst,1);
        t=((double)getTickCount()-t)/getTickFrequency();
        Prediction p;
        for (size_t i = 0; i < predictions.size(); ++i) {
          p = predictions[i];
          std::cout << std::fixed << std::setprecision(4) << p.second << " - \""
                    << p.first << "\"" <<"     time: "<<t<<"s"<<std::endl;
        }
        putText(dst,p.first,Point(20,30),FONT_HERSHEY_SIMPLEX,1,Scalar(255,255,255),2);

        imshow("test",dst);
        waitKey(100);
    }

}
void prep_treat(Mat & src, Mat & dst)
{
    Size m_size=src.size();
    //GaussianBlur(src,src,Size(5,5),1.5);
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
    Mat temp=src(m_rect);
    cv::resize(temp,temp,Size(256,256));
    Rect ROI=Rect(6,6,224,224);
    dst=temp(ROI);
}

void train_test()
{
    string solverfile="cifar10_full_solver.prototxt";
    CTrain cifar_train(solverfile);
    cifar_train.train();

}
void class_many()
{
    string path="/home/joyoung/qtwork/caffe_classify/soymilk18-google2/";
    string model_file=path+"deploy.prototxt";
    string mean_file=path+"mean.binaryproto";
    string trained_file=path+"snapshot_iter_2k.caffemodel";
    string label_file=path+"labels.txt";
    Classifier classifier(model_file, trained_file, mean_file, label_file);
    ifstream fin(label_file.c_str());

    string test_dir="/home/joyoung/digits/data/soymilk_test/";

    string result_file=path+"result2k.txt";
    ofstream outfile(result_file.c_str());//save file of result

    vector<string> label_vec;
    char label[200]={0};
    while(fin.getline(label,sizeof(label)))
    {
        label_vec.push_back(label);
    }
    int result[18][19]={0};// save result of predict, result[0] the max of image; result[1] the mount of correct predict
    float percent[18];
    for(int num_label=0;num_label<label_vec.size();num_label++)
    {
        int abc=0;

        string img_dir=test_dir+label_vec[num_label]+".txt";
        ifstream fin_image(img_dir.c_str());

        //cout<<img_dir<<endl;
        //CDirfile dirfile(img_dir.c_str());
        string filename;
        Mat src,dst;
        char im_file[200]={0};
        while(fin_image.getline(im_file,sizeof(im_file)))
        {

            filename=test_dir+im_file;
            //string filename_save=test_dir+"256_256/"+im_file;
            src=imread(filename.c_str());            
            double t=(double)getTickCount();
            prep_treat(src,dst);
            //imwrite(filename_save,dst);
            std::vector<Prediction> predictions = classifier.Classify(dst,1);
            t=((double)getTickCount()-t)/getTickFrequency();

            result[num_label][18]++;
            Prediction p;
            for (size_t i = 0; i < predictions.size(); ++i) {
                p = predictions[i];
                cout<<"src: "<<label_vec[num_label]<<"  predict: "<< p.first<<"  "<<std::fixed << std::setprecision(4)
                       << p.second <<"   time: "<<t<<"s"<<std::endl;

                if(label_vec[num_label]==p.first)
                    result[num_label][num_label]++;
                else
                    for(int ss=0;ss<label_vec.size();ss++)
                    {
                        if(label_vec[ss]==p.first)
                        {
                            result[num_label][ss]++;
                            //cout<<label_vec[num_label]<<"   "<<p.first<<"    "<<num_label<<" "<<ss<<endl;
                        }
                    }
            }
            abc++;
            //if(abc>10)break;

        }
        percent[num_label]=result[num_label][num_label]/(result[num_label][18]+0.00001);

        cout<<"percent:"<<percent[num_label]<<"      ";
        cout<<"Max:"<<result[num_label][18]<<"        "<<"Yes:"<<result[num_label][num_label]
              <<"       "<<label_vec[num_label]<<endl;

    }

    for (int j=0;j<18;j++)
    {
        for (int i=0;i<18;i++)
        {
            cout.width(5);
            cout<<result[j][i];
            outfile.width(5);
            outfile<<result[j][i];
        }
        cout<<endl;
        outfile<<endl;
    }
    float sum=0;
    for (int j=0;j<18;j++)
    {
        sum+=percent[j];
        cout<<result[j][j]/(result[j][18]+0.00001);
        cout<<"     Max: "<<result[j][18];
        cout<<"     Yes: "<<result[j][j];
        cout<<"     "<<label_vec[j]<<endl;

        outfile<<result[j][j]/(result[j][18]+0.00001);
        outfile<<"      Max: "<<result[j][18];
        outfile<<"      Yes: "<<result[j][j];
        outfile<<"      "<<label_vec[j]<<endl;
    }

    outfile<<"the mean percent : " <<sum/18<<endl;
    cout<<"predict is over."<<"  the mean percent:"<<sum/18<<endl;
    outfile.close();
}

int main()
{
    //class_one();
    class_many();
    //train_test();
    return(0);
}

