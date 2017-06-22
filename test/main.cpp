/*
usage:
test <model> <weight> <mean> <video> [<video2> <camera matrix dir> <graph>]

output:
velocity.txt
*/

#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>

#include "Classifier.h"
#include "Codebook.h"
#include "NNTracker.h"
#include "Reconstruct.h"
#include "TrajPredict.h"
#include "ballprops.h"

using namespace cv;
using namespace std;

const int SCALE = 1;
const int MAX_NO_TRACK_CNT = 10;
const int TRAIN_BG_MODEL_ITER = 50;
const int CLEAR_STALE_PER_ITER = 50;
const int TRACK_MAX_PROPOSALS = 8;
const int PATCH_WIDTH = 100;
const int PATCH_HEIGHT = 100;
const int ORIGIN_HEIGHT = 1525;
const int ORIGIN_WIDTH = 2740;

bool double_view = false;

int frameCount = 0;
char frameCountStr[10];
int no_track_cnt = 0;

Mat temp, temp2;
Mat stitchDisplay, display, display2;

BgSubtractor *bg, *bg2;
NNTracker *track, *track2;

Reconstruct *reconstruct;
TrajPredict *trajPredict;
BallProps ballProps;

void stitchImages(Mat *crops, Mat &display, Mat *bboxes, Mat *probs, int numCrops){
	IplImage ipl = IplImage(display);
	cvZero(&ipl);
	int grid = sqrt(TRACK_MAX_PROPOSALS)+1;
	int k = 0;
	char scoreStr[10];
	for(int i=0;i<grid;++i){
		for(int j=0;j<grid;++j){
			if(k==numCrops)return;
			Mat roi = display(Rect(i*PATCH_WIDTH, j*PATCH_HEIGHT, PATCH_WIDTH, PATCH_HEIGHT));
			Mat crop;
			resize(crops[k], crop, cvSize(PATCH_WIDTH, PATCH_HEIGHT));
			crop.copyTo(roi);
			Rect bbox = Rect(bboxes[k].at<float>(0,0)*2,
				bboxes[k].at<float>(0,1)*2,
				bboxes[k].at<float>(0,2)*2 - bboxes[k].at<float>(0,0)*2,
				bboxes[k].at<float>(0,3)*2 - bboxes[k].at<float>(0,1)*2);
			rectangle(roi, cvPoint(bbox.x, bbox.y), cvPoint(bbox.x+bbox.width, bbox.y+bbox.height), CV_RGB(0x00, 0xff, 0x00), 1, CV_AA);
			
			float prob = probs[k].at<float>(0,0);
			sprintf(scoreStr, "%f", prob);
			putText(roi, scoreStr, cvPoint(0, roi.size().height), CV_FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(0xff, 0xff, 0xff));
			++k;
		}
	}
}

bool _track(cv::Mat frame, bool left, Point *pt, BgSubtractor *bg, NNTracker *track){
    cv::resize(frame, temp, temp.size());
    cvtColor(temp, temp, CV_BGR2RGB);
    cvtColor(temp, temp2, CV_BGR2YCrCb);
    bool success = false;
    float prob;
    Rect bbox;
	success = bg->process(temp2) &&
        track->track_vec(temp, bg->bboxes_vec, &prob, &bbox);
    if(success){
	    stitchImages(track->patches, stitchDisplay, track->bboxes, track->probs, track->num_patches);
	    if(left){
	    	imshow("score", stitchDisplay);
	    }else if(double_view){
	    	imshow("score2", stitchDisplay);
	    }
	    for(int i=0;i<track->num_patches;++i){
	    	rectangle(temp, track->contexts[i], CV_RGB(0xff, 0x00, 0x00), 1);
	    }
        *pt = Point(bbox.x*SCALE+bbox.width*SCALE/2, bbox.y*SCALE+bbox.height*SCALE/2);
        rectangle(temp, bbox, CV_RGB(0x00, 0xff, 0x00), 1);
    }
    for(int i=0;i<bg->bboxes_vec.size();++i){
    	rectangle(temp, bg->bboxes_vec[i], CV_RGB(0x00, 0x00, 0xff), 1);
    }
	//putText(temp, frameCountStr, cvPoint(0, temp.size().height), CV_FONT_HERSHEY_SIMPLEX, 1.0, CV_RGB(0xff, 0xff, 0xff));
	if(left)
    	temp.copyTo(display);//imshow("display", temp);
    else if(double_view)
    	temp.copyTo(display2);//imshow("display2", temp);
    return success;
}

int main(int argc, char **argv){

	namedWindow("display", WINDOW_AUTOSIZE);
	namedWindow("score", WINDOW_AUTOSIZE);

	ofstream velocity;
	velocity.open("velocity.txt", ios::out | ios::trunc);

	string model = string(argv[1]);
	string weights = string(argv[2]);
	string mean = string(argv[3]);

    Classifier classifier(model, weights, mean);

	VideoCapture capture, capture2;
	Mat frame, frame2;
    capture.open(argv[4]);

    capture >> frame;
    if(frame.empty()){
        return 0;
    }
    Size szSmall = frame.size();
    szSmall.width = szSmall.width/SCALE;
    szSmall.height = szSmall.height/SCALE;
    temp = Mat(szSmall, frame.type());
    temp2 = Mat(szSmall, frame.type());
    stitchDisplay = Mat(cvSize((sqrt(TRACK_MAX_PROPOSALS)+1)*PATCH_WIDTH, (sqrt(TRACK_MAX_PROPOSALS)+1)*PATCH_HEIGHT), frame.type());
    bg = new BgSubtractor(szSmall, TRAIN_BG_MODEL_ITER, TRACK_MAX_PROPOSALS);

    track = new NNTracker(classifier);
    track->setProbThresh(0.6f);
    track->setProposalRange(szSmall.width/4);
    track->setMaxProposals(TRACK_MAX_PROPOSALS);

    if(argc>5){
    	capture2.open(argv[5]);
    	capture2 >> frame2;
	    if(frame2.empty()){
	        double_view = false;
	    }else{
	    	double_view = true;
			namedWindow("display2", WINDOW_AUTOSIZE);
			namedWindow("score2", WINDOW_AUTOSIZE);
	    	bg2 = new BgSubtractor(szSmall, TRAIN_BG_MODEL_ITER, TRACK_MAX_PROPOSALS);
	    	track2 = new NNTracker(classifier);
		    track2->setProbThresh(0.6f);
		    track2->setProposalRange(szSmall.width/4);
    		track2->setMaxProposals(TRACK_MAX_PROPOSALS);
		    reconstruct = new Reconstruct(string(argv[6]));
		    trajPredict = new TrajPredict(string(argv[7]));
	    }
    }

    while(1){
		frameCount++;
		sprintf(frameCountStr, "%04d", frameCount);
    	capture >> frame;
    	if(frame.empty())break;
	    Point left, right;
	    bool success = _track(frame, true, &left, bg, track);
	    if(double_view){
	    	capture2 >> frame2;
	    	if(frame2.empty())break;
	    	success &= _track(frame2, false, &right, bg2, track2);
	    	if(ballProps.has_predict_point){
	    		cout<<ballProps.predictPoint().x<<' '<<ballProps.predictPoint().y<<' '<<ballProps.predictPoint().z<<endl;
	    		Point pred_left = reconstruct->xyz2uv(ballProps.predictPoint(), true);
	    		Point pred_right = reconstruct->xyz2uv(ballProps.predictPoint(), false);
	    		//TODO: draw predict point on 2d frames
	    		circle(display, pred_left, 20, CV_RGB(0x00, 0xff, 0xff));
	    		circle(display2, pred_right, 20, CV_RGB(0x00, 0xff, 0xff));
	    	}
	    	if(success){
		        no_track_cnt = 0;
		        CvPoint3D32f coord_world = reconstruct->uv2xyz(left, right);
	    		cout<<coord_world.x<<' '<<coord_world.y<<' '<<coord_world.z<<endl;
		        ballProps.feed(coord_world);
		        if(ballProps.isRebound()){
		            //table->setLandingPoint(ballProps.lastPoint());
		        }
		        if(ballProps.crossHalfCourt()){
		            cout<<"try to predict landing point"<<endl;
		            //_sampleUntilLanding(coord_world);
		        }
		        if(ballProps.is_hit){
		        	cout<<"hit!"<<endl;
		        	trajPredict->clearState();
		        }
		        ballProps.setPredictPoint(trajPredict->sample1(coord_world));
		    	velocity<<ballProps.velocityX<<' '<<ballProps.velocityY<<' '<<ballProps.velocityZ<<endl;
		    }else{
		    	track->fail();
		    	track2->fail();
		        no_track_cnt++;
		        if(no_track_cnt>MAX_NO_TRACK_CNT){
		            ballProps.clearState();
		            trajPredict->clearState();
		        }else{
		        	if(ballProps.has_predict_point){
		            	ballProps.feed(ballProps.predictPoint());
		        	}
		        	ballProps.setPredictPoint(trajPredict->sample1(ballProps.lastPoint()));
		        }
		    	velocity<<"0 0 0"<<endl;
		    }
	    	imshow("display2", display2);
	    }
	    imshow("display", display);
	    char c = cvWaitKey(20);
	    if(c==27)break;
	    else if(c==32)cvWaitKey(0);
    }
    velocity.close();
	return 0;
}
