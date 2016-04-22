
/////////////////////////
// DATA SPECIFICATION  //
/////////////////////////

#define LFPW

#ifdef LFPW
#define NUM_TRAINING_IMAGE 871
#define NUM_TESTING_IMAGE 507
#define NUM_LAND_MARK 68
#define MODEL_FILE_NAME "./data/myModel_LFPW.txt"
#define TEST_IMAGE_FOLDER_NAME  "./data/lfpw/testset/"
#define TRAIN_IMAGE_FOLDER_NAME "./data/lfpw/trainset/"
#define TEST_BBOX_FILE_NAME  "./data/lfpw/bbox_lfpw_test.txt"
#define TRAIN_BBOX_FILE_NAME "./data/lfpw/bbox_lfpw_train.txt"
#endif

//////////////////////////
// FACE DETECTION MODEL //
//////////////////////////
#define FACE_DETECTION_MODEL_FILE_NAME "data/haarcascade_frontalface_alt.xml"

////////////////////
// MODEL SETTING  //
////////////////////

#define NUM_EXTERNAL_REGRESSOR 10 
#define NUM_INTERNAL_REGRESSOR 500
#define NUM_FERN_FEATURE 5
#define NUM_CANDIDATE_PIXEL 400

//////////////////
// HOW TO TEST  //
//////////////////

#define USE_TEST_IMAGE
//#define USE_WEBCAM




