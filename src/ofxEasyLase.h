//
//  ofxEasyLase.h
//  ofxILDA
//
//  Modifications by say nono
//
//  This is super based on
//
//  Created by Daito Manabe + Yusuke Tomoto (Rhizomatiks)
//  Mods by Memo Akten
//
//


#include "ofMain.h"
#include "easyLase.h"
#include "ofxIldaFrame.h"

#define kEasyLaseIldaMinPoint -32768
#define kEasyLaseIldaMaxPoint 32767
#define kEasyLaseIldaDimension (kIldaMaxPoint - kIldaMinPoint)
#define kEasyLaseIldaMaxIntensity 65535



class ofxEasyLase : public ofThread {
public:
    ofxEasyLase():state(EASYLASE_NOTFOUND) {}
    
    ~ofxEasyLase() {
        kill();
    }
    
    bool stateIsFound();
    
    void kill() {
        clear();
        stop();
        easyLaseClose();
    }
    
    void setup(bool bStartThread = true);
    virtual void threadedFunction();
    
    
    // check if the device has shutdown (weird bug in etherdream driver) and reconnect if nessecary
    bool checkConnection();
    
    void clear();
    void start();
    void stop();

    void addPoints(const vector<ofxIlda::Point>& _points);
    void addPoints(const ofxIlda::Frame &ildaFrame);
    
    void setPoints(const vector<ofxIlda::Point>& _points);
    void setPoints(const ofxIlda::Frame &ildaFrame);
    
    void send();
    
    void setPPS(int i);
    int getPPS() const;
    
    void setWaitBeforeSend(bool b);
    bool getWaitBeforeSend() const;

    bool selectDevice(int deviceID);
    int getDeviceCount();
    
private:
    void init();
    void sendBlank();
    void convertAllPointsToEasyLase(const vector<ofxIlda::Point>& _points);
    
private:
    enum {
        EASYLASE_NOTFOUND = 0,
        EASYLASE_FOUND
    } state;
    
    int pps;
    bool bWaitBeforeSend;
    bool bSendBlank;
    
    int deviceID;
//    vector<ofxIlda::Point> points;
    vector<EasyLaseData> pointsEL;
/*
    typedef struct {
		unsigned short x;  // 2 Bytes  Value 0 - 4095  X-Coordinate
		unsigned short y;  // 2 Bytes  Value 0 - 4095  Y-coordinate
		unsigned char  r;  // 1 Byte   Value 0 - 255   Red
		unsigned char  g;  // 1 Byte   Value 0 - 255   Green
		unsigned char  b;  // 1 Byte   Value 0 - 255   Blue
		unsigned char  i;  // 1 Byte   Value 0 - 255   Intensity
	} __attribute__((packed)) EasyLaseData, *EasyLasePoint;
*/
    inline EasyLaseData convertOFXIldaPointToEasyLase(const ofxIlda::Point& pIn) {
        EasyLaseData pOut;
        pOut.x = (pIn.x / 32) + 2048;
        pOut.y = (pIn.y / 32) + 2048;
        pOut.r = pIn.r / 256;
        pOut.g = pIn.g / 256;
        pOut.b = pIn.b / 256;
        pOut.i = pIn.a / 256;
        return pOut;
    };
    
};