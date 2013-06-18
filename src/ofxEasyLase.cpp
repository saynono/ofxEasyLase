#include "ofxEasyLase.h"

//--------------------------------------------------------------
void ofxEasyLase::setup(bool bStartThread) {
//    etherdream_lib_start();
    
    setPPS(8000);
    setWaitBeforeSend(false);
    
	/* Sleep for a bit over a second, to ensure that we see broadcasts
	 * from all available DACs. */
//	usleep(100000);
//    init();
    
    if(bStartThread) start();
}


//--------------------------------------------------------------
bool ofxEasyLase::stateIsFound() {
    return state == EASYLASE_FOUND;
}

//--------------------------------------------------------------
bool ofxEasyLase::checkConnection() {
    return easyLaseGetCardNum() > 0;
}

//--------------------------------------------------------------
void ofxEasyLase::init() {
    
    int device_num = getDeviceCount();
	if (device_num < 1) {
		ofLogWarning() << "ofxEasyLase::init - No EasyLase devices found";
        deviceID = -1;
        state = EASYLASE_NOTFOUND;
		return 0;
	}
    
    deviceID = 0;
    ofLogNotice() << "ofxEasyLase::init - Found " << device_num << " EasyLase Devices ";
    ofLogNotice() << "ofxEasyLase::init - Connected";
    state = EASYLASE_FOUND;
    
}

//--------------------------------------------------------------
void ofxEasyLase::threadedFunction() {
//    init();
    ofLog() << "ofxEasyLase::threadedFunction start : " << state;
    while (isThreadRunning() != 0) {

        switch (state) {
            case EASYLASE_NOTFOUND:
                if(lock()) {
                    init();
                    unlock();
                }
                break;
//                return;
                break;
            case EASYLASE_FOUND:
                
                if(lock()) {
                    send();
                    unlock();
                }
                break;
        }
//        sleep(100);
    }
    ofLog() << "ofxEasyLase::threadedFunction end ";
}

//--------------------------------------------------------------
void ofxEasyLase::start() {
    startThread(true, false);  // TODO: blocking or nonblocking?
}

//--------------------------------------------------------------
void ofxEasyLase::stop() {
    stopThread();
}

//--------------------------------------------------------------
void ofxEasyLase::send() {
//    if(pointsEL.empty()) sendBlank();
    if(pointsEL.empty()) return;
    int status = easyLaseGetStatus(0);
    // ADD bWaitBeforeSend
    if(status == EASYLASE_GET_STATUS_READY){
        int pointCount = pointsEL.size();
        status = easyLaseWriteFrame(deviceID, (unsigned char *)pointsEL.data(), pointCount*8, pps );
    }
    pointsEL.clear();
}

//--------------------------------------------------------------
void ofxEasyLase::sendBlank() {
    //    points.clear();
    pointsEL.clear();
    for(int i=0;i<64;i++){
        EasyLaseData pOut;
        pOut.x = 2048 + i;
        pOut.y = 2048;
        pOut.r = 0;
        pOut.g = 0;
        pOut.b = 0;
        pOut.i = 0;
        pointsEL.push_back(pOut);
    }
}

//--------------------------------------------------------------
void ofxEasyLase::clear() {
    if(lock()) {
//        points.clear();
        pointsEL.clear();
        unlock();
    }
}

//--------------------------------------------------------------
void ofxEasyLase::addPoints(const vector<ofxIlda::Point>& _points) {
    if(lock()) {
        if(!_points.empty()) {
            convertAllPointsToEasyLase(_points);
//            points.insert(points.end(), _points.begin(), _points.end());
        }
        unlock();
    }
}


//--------------------------------------------------------------
void ofxEasyLase::addPoints(const ofxIlda::Frame &ildaFrame) {
    addPoints(ildaFrame.getPoints());
}


//--------------------------------------------------------------
void ofxEasyLase::setPoints(const vector<ofxIlda::Point>& _points) {
    if(lock()) {
//        points = _points;
        pointsEL.clear();
        convertAllPointsToEasyLase(_points);
        unlock();
    }
}


//--------------------------------------------------------------
void ofxEasyLase::setPoints(const ofxIlda::Frame &ildaFrame) {
    setPoints(ildaFrame.getPoints());
}

//--------------------------------------------------------------
void ofxEasyLase::setWaitBeforeSend(bool b) {
    if(lock()) {
        bWaitBeforeSend = b;
        unlock();
    }
}

//--------------------------------------------------------------
bool ofxEasyLase::getWaitBeforeSend() const {
    return bWaitBeforeSend;
}


//--------------------------------------------------------------
void ofxEasyLase::setPPS(int i) {
    if(lock()) {
        pps = i;
        unlock();
    }
}

//--------------------------------------------------------------
int ofxEasyLase::getPPS() const {
    return pps;
}

//--------------------------------------------------------------
bool ofxEasyLase::selectDevice(int dID){
    if(dID>=getDeviceCount()) return false;
    deviceID = dID;
    return true;
}

//--------------------------------------------------------------
int ofxEasyLase::getDeviceCount(){
    return easyLaseGetCardNum();
}

void ofxEasyLase::convertAllPointsToEasyLase(const vector<ofxIlda::Point>& _points){
//    pointsEL.clear();
    for(int i=0;i<_points.size();i++){
        pointsEL.push_back(convertOFXIldaPointToEasyLase(_points[i]));
    }    
}
