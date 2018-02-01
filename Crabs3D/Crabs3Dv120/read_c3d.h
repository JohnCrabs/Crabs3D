#pragma once
#ifndef READ_C3D_H
#define READ_C3D_H

#include <iostream>
#include <QWidget>

/***********/
/* DEFINES */
/***********/

#define PROCESSOR_NONE  83
#define PROCESSOR_INTEL 84
#define PROCESSOR_DEC   85
#define PROCESSOR_MIPS  86

#define FORMAT_CHAR     -1
#define FORMAT_BYTE      1
#define FORMAT_INT_16    2
#define FORMAT_FLOAT     4

#define SIZE_8_BIT 1
#define SIZE_16_BIT 2
#define SIZE_32_BIT 4

#define LITTLE_ENDIAN_PROC 0
#define BIG_ENDIAN_PROC 1

#define SAME_ENDIAN 0
#define DIFF_ENDIAN 1

#define MAX_DIMENSIONS 7

/****************/
/*  STRUCTURES  */
/****************/

struct GeoPoint {
    float x;
    float y;
    float z;

    float dx;
    float dy;
    float dz;
    float dr;

    float residual;
};

struct GeoCentroid {
    float x;
    float y;
    float z;
};

/***********/
/* CLASSES */
/***********/

/******************************************/
/*               Header_c3d               */
/******************************************/

class Header_c3d
{
public:
    const static int futureBlock_1_size = 135;
    const static int eventTime_size = 18;
    const static int displayFlag_size = 9;
    const static int eventLabel_size = 18;
    const static int futureBlock_4_size = 22;

    Header_c3d() {} //Constructor

    inline short int ParameterBlock(void) {return parameter_block;}
    inline short int NumberID(void) {return id_number;}
    inline short int NumberOfPoints(void) {return points_number;}
    inline short int NumberOfAnalog(void) {return analog_number;}
    inline short int FirstFrame(void) {return first_frame;}
    inline short int LastFrame(void) {return last_frame;}
    inline short int MaxGap(void) {return maximum_interpolation_gap;}
    inline float ScaleFactor(void) {return scale_factor;}
    inline short int DataStart(void) {return data_start_block;}
    inline short int AnalogPerFrame(void) {return analog_per_frame;}
    inline float FrameRate(void) {return frame_rate;}

    inline short int FutureBlock_1(const int index) {
        if(index < 0 || index >= futureBlock_1_size)
            return (short int)NULL;
        else
            return future_use_block_1[index];
    }

    inline short int KeyValue_1(void) {return key_value_1;}
    inline short int FirstLabelRange(void) {return first_label_range_block;}
    inline short int KeyValue_2(void) {return key_value_2;}
    inline short int EventTime(void) {return event_time_num;}
    inline short int FutureBlock_2(void) {return future_use_block_2;}

    inline float EventTimeSec(const int index) {
        if(index < 0 || index >= eventTime_size)
            return (short int)NULL;
        else
            return event_times_in_sec[index];
    }

    inline short int EventDisplayFlags(const int index) {
        if(index < 0 || index >= displayFlag_size)
            return (short int)NULL;
        else
            return event_display_flags[index];
    }

    inline short int FutureBlock_3(void) {return future_use_block_3;}

    inline char* EventLabel(const int index) {
        if(index < 0 || index >= eventLabel_size)
            return (short int)NULL;
        else
            return event_labels[index];
    }

    inline short int FutureBlock_4(const int index) {
        if(index < 0 || index >= futureBlock_4_size)
            return (short int)NULL;
        else
            return future_use_block_4[index];
    }

    /*Read the 512-byte block of the header file*/
    void read_header_block(FILE* file, Header_c3d* header);

    /*If File Endian is different than System Endian then swap header values*/
    void swapHeader(Header_c3d* header);

    /*Print Header block to file*/
    void print_header_to_file(Header_c3d header, const std::string fileName);

    virtual ~Header_c3d() {} //Desturctor

protected:

private:
    short int parameter_block = 0;            //byte 1: points to the first block of the parameter section.
    short int id_number = 0;                  //byte 2: key value 0x50h indicating a C3D file.
    short int points_number = 0;              //number of 3D points in the C3D file (i.e. the number of the stored trajectories).
    short int analog_number = 0;              //total number of analog measurements per 3D frame, i.e. number of channels multiplied by the saples per channel.
    short int first_frame = 0;                //number of the first frame of 3D data (1 based, not 0).
    short int last_frame = 0;                 //number of the last frame of 3D frames.
    short int maximum_interpolation_gap = 0;  //maximum interpolation gap in 3D frames.
    float scale_factor = 0.0;                 //the 3D scale factor (floating-point) that converts signed integer 3D data to reference system measurement units. If this is negative then the file is scaled in floating-point.
    short int data_start_block = 0;           //DATA_START-the number of the first block of the 3D and analog data section.
    short int analog_per_frame = 0;           //the number of analog samples per 3D frame.
    float frame_rate = 0.0;                   //the 3D frame rate in Hz (floating-point).
    short int future_use_block_1[futureBlock_1_size];        //reserved for future use.
    short int key_value_1 = 0;                //a key value (12345 decimal) is written here if Label and Range data is present, otherwise write 0x00.
    short int first_label_range_block = 0;    //the first block of the Label and Range section (if present).
    short int key_value_2 = 0;                //a key value (12345 decimal) present if this file supports 4 char event labels. An older format supported only 2 character labels.
    short int event_time_num = 0;             //number of defined time events (0 to 18).
    short int future_use_block_2 = 0;         //reserved for future use.
    float event_times_in_sec[eventTime_size]; //event times (floating-point) in seconds (up to 18 events).
    short int event_display_flags[displayFlag_size];         //18 bytes - event display flags 0x00 = ON, 0x01 = OFF.
    short int future_use_block_3 = 0;         //reserved for future use.
    char event_labels[eventLabel_size][4];    //event labels. Each label is 4 character long.
    short int future_use_block_4[futureBlock_4_size];         //reserved for future use.

};

/*********************************************/
/*               Parameter_c3d               */
/*********************************************/

class Parameter_Header_C3D  {
public:
    inline short int ParameterBlock() {return parameter_block;} //return parameter_block
    inline short int NumberID() {return id_number;} // return id_number
    inline short int NumberOfParameterBlock() {return number_of_parameter_block;} // return number_of_parameter_block
    inline short int ProcessorType() {return processor_type;} //return processor_type


    void ReadHeaderBlock(FILE* file, Parameter_Header_C3D& header);


private:
    short int parameter_block; //The block number of the first block in the parameter section.
    short int id_number; //Key identifier of 80 decial.
    short int number_of_parameter_block; //Number of parameter blocks to follow (see below)
    /* 83 decimal + processor type.
     * Processor type 1 = Intel
     * Processor type 2 = DEC (VAX, PDP-11)
     * Processor type 3 = MIPS processor (SGI/MIPS)
     */
    short int processor_type;
};

class Parameter_Parameter_C3D {
public:
    inline short int NameSize(void) {return character_number;} //return character number
    inline bool IsLocked(void) {return locked;} //return locked
    inline short int ID(void) {return parameter_ID;} //return parameter_ID
    inline std::string Name(void) {return parameter_name;} //return parameter name
    inline short int NextStart(void) {return next_group_parameter_start;} //return next_group_parameter_start
    inline short int Format(void) {return byte_format;} //return byte format
    inline short int DimensionsSize(void) {return number_of_dimensions;} //return number_of_dimensions
    inline short int Dimension(const int index) {return parameter_dimensions[index];} //parameter_dimensions

    inline std::string ParameterChar(void) {return parameter_data_char;} //return parameter_data_char
    inline short int ParameterByte(const int index) {return parameter_data_byte[index];} //return parameter_data_byte
    inline short int ParameterInt16(const int index) {return parameter_data_16_int[index];} //return parameter_data_16_int
    inline float ParameterFloat(const int index) {return parameter_data_float[index];} //return parameter_data_float

    inline short int DescriptionSize(void) {return description_number;} //return description_number
    inline std::string Description(void) {return description;} //return description

    //Read Parameter Block
    bool ReadParameterBlock(FILE* file, Parameter_Parameter_C3D& parameter, const int endianFlag);

    //Clean Memory
    void CleanUp(Parameter_Parameter_C3D& parameter);

private:
    /*number of characters in the Group name (1-127)
     *this value may be set to a negative number to indicate that the group is "locked"
     */
    short int character_number;
    bool locked;

    //group ID number (-1 to -127 ... always negative). parameter ID (+1 to +127 ... always positive)
    short int parameter_ID;

    //group name (ASCII characters-upper case A-Z, 0-9 and underscore _ only)
    char* parameter_name;

    //a signed integer offset in bytes pointing to the start of the next group/parameter.
    short int next_group_parameter_start;

    /* Length in bytes of each data element
     *-1 for character data
     * 1 for byte data
     * 2 for integer data (16-bit integers)
     * 4 for floating-point (REAL) data
     */
    short int byte_format;

    //number of dimensions (0-7) of the parameter - 0 if the parameter is scalar.
    short int number_of_dimensions;

    //parameter dimensions
    short int parameter_dimensions[MAX_DIMENSIONS];

    //Parameter Data Formats
    char* parameter_data_char;        //Character
    short int* parameter_data_byte;   //Byte
    short int* parameter_data_16_int; //16-bit integer
    float*     parameter_data_float;  //Real, floating-point

    //number of characters in the description
    short int description_number;

    //description
    char* description;
};

class Parameter_Group_C3D {
public:
    inline short int NameSize(void) {return character_number;} //return character number
    inline bool IsLocked(void) {return locked;} //return locked
    inline short int ID(void) {return group_ID;} //return parameter_ID
    inline std::string Name(void) {return group_name;} //return parameter name
    inline short int NextStart(void) {return next_group_parameter_start;} //return next_group_parameter_start
    inline short int DescriptionSize(void) {return description_number;} //return description_number
    inline std::string Description(void) {return description;} //return description

    inline short int ParameterSize(void) {return parameter_size;} //return parameter_size
    inline Parameter_Parameter_C3D Parameter(const int index) {
        if(index > 127 || index < 0) {
            return parameter[0];
        } else
            return parameter[index];
    }

    //Check if it's group or parameter, then if it's group read group
    bool ReadGroupBlock(FILE* file, Parameter_Group_C3D& group, const int endianFlag, const bool isGroup);

    //Clean Memory
    void CleanUp(Parameter_Group_C3D* group);

private:
    /*number of characters in the Group name (1-127)
     *this value may be set to a negative number to indicate that the group is "locked"
     */
    short int character_number;
    bool locked;

    //group ID number (-1 to -127 ... always negative). parameter ID (+1 to +127 ... always positive)
    short int group_ID;

    //group name (ASCII characters-upper case A-Z, 0-9 and underscore _ only)
    char* group_name;

    //a signed integer offset in bytes pointing to the start of the next group/parameter.
    short int next_group_parameter_start = 1;

    //number of characters in the description
    short int description_number;

    //description
    char* description;

    //Parameters
    short int parameter_size;
    Parameter_Parameter_C3D* parameter;
};

class Parameter_c3d {
public:
    inline Parameter_Header_C3D Header() {return header;}

    inline short int GroupSize(void) {return group_size;}
    inline Parameter_Group_C3D Group(const int index) {
        if(index > 127 || index < 0) {
            return group[0];
        } else {
            return group[index];
        }
    }

    //Read Header Parameter Block
    void ReadHeaderParameterBlock(FILE* file, Parameter_c3d* parameter);

    //Read Group Parameter Block
    void ReadGroupParameterBlock(FILE* file, Parameter_c3d* parameter, const int endianFlag);

    //Print Parameter Block
    void print_parameter_to_file(Parameter_c3d parameter, const std::string fileName);

    //Clean Memory
    void CleanUp(Parameter_c3d* parameter);

private:
    Parameter_Header_C3D header;

    short int group_size;
    Parameter_Group_C3D* group;
};

/*******************************/
/* PARAMETER STRUCTURE CLASSES */
/*******************************/

/***************/
/*   1.TRIAL   */
/***************/
/*The C3D file header contains the frame numbers of the first and last frames of trial
data stored in the C3D file while the total number of frames is stored in the
POINT:FRAMES parameter. The C3D format defines these as unsigned integer values
which limits these to a maximum value of 65535 .

When using high-speed video cameras, this equates to a trial of just over two minutes
at 240Hz. To escape from the existing 16-bit integer limits and allow more than
65535 frames to be stored, two parameters ( ACTUAL_START_FIELD and
ACTUAL_END_FIELD ) have been created as 32-bit values in a new TRIAL group. It is
proposed that C3D applications should, in future, read these parameters and only use
the header values and POINT:FRAMES parameter values if these new TRIAL
parameters are not present.

The choice of parameter name is unfortunate as many video systems sample in the
interlaced mode where a 60Hz sample rate is obtained by sampling odd and even
fields at 30Hz.*/

class Trial {
public:
    inline unsigned int ActualStartField(void) {return Actual_Start_Field;}
    inline unsigned int ActualEndField(void) {return Actual_End_Field;}
    inline unsigned int VideoRateDivider(void) {return Video_Rate_Divider;}
    inline float CameraRate(void) {return Camera_Rate;}
    inline unsigned int Year(void) {return Date[0];}
    inline unsigned int Month(void) {return Date[1];}
    inline unsigned int Day(void) {return Date[2];}
    inline unsigned int Hours(void) {return Time[0];}
    inline unsigned int Minutes(void) {return Time[1];}
    inline unsigned int Seconds(void) {return Time[2];}

    inline void SetTrial(Trial* trial, Parameter_c3d parameter);

private:

    /*The first frame number is stored in two unsigned integer values to form a 32-bit
    value. The first unsigned integer is the least significant word while the second is the
    most significant word.*/
    unsigned int Actual_Start_Field;

    /*The last frame number is stored in two unsigned integer values to form a 32-bit
    value. The first unsigned integer is the least significant word and the second is the
    most significant word.
    This implementation is compatible with the existing C3D specification so long as the
    old parameter and header values are maintained whenever possible. This will be
    compatible unless more than 65535 frames of data are recorded. Once this limit is
    exceeded only applications that can read these parameters will be able to read the
    additional data – although both old and new applications will be able to read all the
    data up to 65535 frames in unsigned C3D files. Note than many older software
    applications will treat the existing POINT:FRAMES parameter as a signed integer and
    will fail to read files that contain more than 32767 frames.*/
    unsigned int Actual_End_Field;

    /*Normally with a value of one ( 1 ) this, unsigned integer value, records any delay
    between successive 3D samples at the time of data collection. This allows data
    collection applications to record marker positions every “n” frames for slow moving
    points – such as occur in a tidal flow simulation for example. The actual data
    capture rate, in terms of frames, is determined by dividing the TRIAL:CAMERA_RATE
    parameter value by this value. A value of one ( 1 ) indicates that there is no delay
    between frames while a value of two ( 2 ) indicates that data is only recorded every
    second frame.*/
    unsigned int Video_Rate_Divider;

    /*This floating-point parameter records the original data collection rate, in Hertz. This
    value may be different to the value stored in the POINT:RATE parameter (and C3D
    header) due to data re-sampling or video rate division (see VIDEO_RATE_DIVIDER
    above).*/
    float Camera_Rate;

    /*This is an array of three unsigned integers that records the date of capture using the
    western calendar system. The date is stored as the year, month, and day, in that
    order.*/
    unsigned int Date[3];

    /*An array of three unsigned integers that records the time of capture stored as the
    hours, minutes and seconds, in that order using a 24 hour clock. This is normally
    recorded in the local time rather than Universal Time (UTC).*/
    unsigned int Time[3];

};

/******************/
/*   2.SUBJECTS   */
/******************/
/*The SUBJECTS group is used by an animation-modeling package and should not be
confused with the similarly named SUBJECT group. It is not a good idea to create a
new group name that is very similar to another name that has been in common use.
In addition, this group actually contains very little information about the actual
“subjects” of the test other than their name – it might have been better to have named
it “configuration”, “parameters”, or “data_sets” etc.*/

class Subjects {
public:

    //IsStatic
    inline short int IsStatic() {return is_static;}

    //Names
    inline int NamesSize() {return namesSize;}
    inline std::string Names(const int index) {return names[index];}

    //ModelParams
    inline int ModelParamsSize() {return model_paramsSize;}
    inline std::string ModelParams(const int index) {return model_params[index];}

    //UsesPrefixes
    inline short int UsesPrefixes() {return uses_prefixes;}

    //LabelPrefixes
    inline int LabelPrefixesSize() {return label_prefixesSize;}
    inline std::string LabelPrefixes(const int index) {return label_prefixes[index];}

    //Used
    inline short int Used() {return used;}

    //MarkerSets
    inline int MarkerSetsSize() {return marker_setsSize;}
    inline std::string MarkerSets(const int index) {return marker_sets[index];}

    //DisplaySet
    inline int DisplaySetSize() {return display_setsSize;}
    inline std::string DisplaySets(const int index) {return display_sets[index];}

    //Models
    inline int ModelsSize() {return modelsSize;}
    inline std::string Models(const int index) {return models[index];}

    //Set Subject Values
    void SetSubjects(Subjects* subjects, Parameter_c3d parameter);

private:
    /*A single signed integer variable, this is set to 1 if the trial subjects were captured in a
    static pose for the purposes of calibration, otherwise 0 .
    Although this is compatible with the C3D specification, the parameter really should
    have been a logical parameter type since this is the way that the parameter is
    documented and used.*/
    short int is_static;

    /*The SUBJECTS:NAMES parameter is an array of ASCII character strings with the
    description “N subject names of length L”.

    The description really is not very helpful – especially since this description came
    from the manufacturers’ documentation... the actual parameter description string in
    the C3D file is empty. This parameter must serve some function but it is going to
    remain a mystery to anyone who opens the C3D file.*/
    int namesSize;
    std::string* names;

    /*The SUBJECTS:MODEL_PARAMS parameter is an array of ASCII character strings that
    contain the marker set names used in the trial. These identify the model parameter
    (usually .MP files) filenames for each subject ordered as in the NAMES parameter
    above. Each subject will typically have his or her own model parameters. Note that
    the path is excluded from the filename.

    According to the description, this parameter contains the filename of an external file
    that holds data about the trial that is relevant to the contents of this C3D file. This is
    an example an applications programmer failing to understand the rational for using
    the C3D file format. The implication of this parameter is that, if it is used, the
    resulting C3D file will probably not contain details of all of the parametric values
    needed to process or interpret the data.

    In this example data portability has been compromised because if this C3D file is
    ever separated from the required model parameter files there may no way to process
    the contents or validate the results stored in the file.

    The C3D format is flexible and can easily store complex information within the
    parameter section. Data stored as parameters is preserved and is accessible to
    anyone reading the file.*/
    int model_paramsSize;
    std::string* model_params;

    /*A single signed integer variable, this is set to 1 if the trial subjects are identified by
    prefixing the subject name to the point label, otherwise set to 0 . The manufacturers’
    documentation states that this is only used when labeling has not yet been performed
    and that the presence of any non-blank SUBJECT:LABEL_PREFIXES entries overrides
    this parameter.

    Although this is compatible with the C3D specification, the parameter really should
    have been a logical parameter type since this is the way that the parameter is
    documented and used. In addition, the documentation suggests that this is a
    temporary parameter that is used to pass information between an application that
    performs the photogrammetry calculations and a point labeling process.*/
    short int uses_prefixes;

    /*The SUBJECT:LABEL_PREFIXES parameter is an array of ASCII character strings that
    identify prefixes attached to the trajectory labels for each subject ordered as in
    SUBJECT:NAMES parameter. Each prefix is typically either blank or the same as the
    subject name but with a colon ‘:’ suffix e.g., “ FRED: ” where the NAMES parameter
    contains “ FRED ” without the colon.*/
    int label_prefixesSize;
    std::string* label_prefixes;

    /*A single signed integer variable that stores the number of named subjects in the trial.
    It is set to 0 in C3D files where specific subjects were not used or for trials that
    contain only subject calibration data.*/
    short int used;

    /*The SUBJECT:MARKER_SETS parameter is an array of ASCII character strings that
    identify filenames that contains the names used by the manufacturers’ trajectory
    identification application for each subject. The MARKER_SETS filenames are
    entered in the same order as SUBJECT:NAMES to allow each subject to use a different
    marker set to identify the 3D points. Note that the use of LABEL_PREFIXES allows
    the same marker names to by used on two subjects in the same C3D file.

    The parameter contains only the filename – both the file type (i.e. MKR etc) and the
    path or location of the file are omitted from the parameter.*/
    int marker_setsSize;
    std::string* marker_sets;

    /*This is an array of ASCII strings that identify the active display set within the marker
    set for each subject ordered as in the POINT:NAMES parameter. If this parameter is
    blank then the first display set should be used.*/
    int display_setsSize;
    std::string* display_sets;

    /*An array of ASCII strings containing marker set names. These identify the model
    filenames for each subject ordered as in NAMES. Each subject may use a different
    model. Note that the path is excluded from the model filename – model file names
    will normally end in .MOD – since the location of the path is not specified, any
    application attempting to locate the model file will need to know where to find it.

    This is another example of a poor choice of parameter storage as the parameter is
    providing information that is only useful if another file (the .MOD file) can be
    located. If the .MOD file becomes separated from the C3D file then it is possible
    that the data contained in the C3D file will become worthless. It would have been
    much better, if the model information is important, to include it within the C3D file –
    either as a group or a set of parameters.*/
    int modelsSize;
    std::string* models;

};

/***************/
/*   3.POINT   */
/***************/
/*(1)The POINT parameters group provides information about the 3D sample data
contained within a C3D file as well as some basic information about the data
environment. As a result, POINT parameters such as POINT:DATA_START,
POINT:FRAMES, and POINT:USED are required even if the C3D file contains only
analog information without any 3D information at all. The POINT:DATA_START
parameter is needed to provide a pointer to the start of the 3D point and analog
storage within the file. The POINT:USED parameter enabled any software application
to determine the number of 3D points recorded in the data area – thus it must be set
to zero to indicate that the 3D point and analog storage area does not contain any 3D
point data.

Other POINT parameters may be required by particular software applications – you
will need to consult your software or hardware manufacturers documentation for
details of application specific parameters and their use. It is worth noting here that
every parameter and group structure has an associated description string that should
be used to provide some basic information about each group and parameter.

(2)Although initially conceived as a group that provided information about the 3D data,
the POINT group also contains a number of parameters that may control the display
and presentation of the data to the user. Various manufacturers have added
parameters to this group that allow applications to store processed data within the 3D
data section so that C3D files may now contain the results of modeling calculations
in addition to marker positional information.*/

class Point {
public:
    //Used
    inline unsigned int Used(void) {return used;}

    //Scale
    inline float Scale(void) {return scale;}

    //Rate
    inline float Rate(void) {return rate;}

    //Data Start
    inline int DataStart(void) {return data_start;}

    //Frames
    inline int Frames(void) {return frames;}

    //Labels
    inline int LabelsSize(void) {return labelsSize;}
    inline std::string Labels(const int index) { if(index < labelsSize) return labels[index]; else return "";}

    //Descriptions
    inline int DescriptionsSize(void) {return descriptionsSize;}
    inline std::string Descriptions(const int index) {return descriptions[index];}

    //Units
    inline std::string Units(void) {return units[0];}

    //Initial Command
    inline std::string InitialCommand(void) {return initial_command[0];}

    //X Screen
    inline std::string X_Screen(void) {return x_screen[0];}

    //Y Screen
    inline std::string Y_Screen(void) {return y_screen[0];}

    //Movie Delay
    inline float MovieDelay(void) {return movie_delay;}

    //Labels2
    inline int Labels2Size(void) {return labels2Size;}
    inline std::string Labels2(const int index) {return labels2[index];}

    //Descriptions
    inline int Descritpions2Size(void) {return descriptions2Size;}
    inline std::string Descriptions2(const int index) {return descriptions2[index];}

    //Type Groups
    inline int TypeGroupsSize(void) {return type_groupsSize;}
    inline std::string TypeGroups(const int index) {return type_groups[index];}

    //Angles
    inline int AnglesSize(void) {return anglesSize;}
    inline std::string Angles(const int index) {return angles[index];}

    //Angle Units
    inline std::string AngleUnits(void) {return angle_units[0];}

    //Scalars
    inline int ScalarsSize(void) {return scalarsSize;}
    inline std::string Scalars(const int index) {return scalars[index];}

    //Scalar Units
    inline std::string ScalarUnits(void) {return scalar_units[0];}

    //Powers
    inline int PowersSize(void) {return powersSize;}
    inline std::string Powers(const int index) {return powers[index];}

    //Power Units
    inline std::string PowerUnits(void) {return power_units[0];}

    //Forces
    inline int ForcesSize(void) {return forcesSize;}
    inline std::string Forces(const int index) {return forces[index];}

    //Force Units
    inline std::string ForceUnits(void) {return force_units[0];}

    //Momets
    inline int MomentsSize(void) {return momentsSize;}
    inline std::string Moments(const int index) {return moments[index];}

    //Moment Units
    inline std::string MomentUnits(void) {return moment_units[0];}

    //Reactions
    inline int ReactionsSize(void) {return reactionsSize;}
    inline std::string Reactions(const int index) {return reactions[index];}

    //Set Point Values
    void SetPoint(Point* point, Parameter_c3d parameter);

    float MultiplierForMeters(Point point);
    GeoPoint CheckScreens(Point point, const float X, const float Y, const float Z, const float DR);

private:

    /*The POINT:USED parameter is a single signed integer value that contains the number
    of 3D point coordinates that are written to each frame of data in the C3D file data
    section. If it is wished to store coordinates for say ten 3D points, then POINTS:USED
    must be ten or greater, and every frame will have space for POINTS:USED number of
    3D points. Any unused point location should be filled with an “invalid point” having
    X,Y,Z = 0, and the fourth word equal to -1. POINTS:USED or the number of valid
    points stored in the frames is not dependent on the POINT:LABLES list, which may
    contain any number of labels. If the label list contains fewer labels than valid points
    stored, the application should be prepared to supply default label names.

    The importance of the USED parameter lies in the fact that an application that is
    reading the 3D data section directly must use this value to determine how many 3D
    co-ordinate points are stored in each frame. The points do not have to be valid, they
    just have to have storage allocated, – invalid points will be stored with “invalid”
    coordinates if no trajectory is assigned to the label. When an application has read
    USED number of 3D co-ordinate points then it has read the entire frame of 3D data.

    As a signed integer, this parameter has a possible range of –32768 to +32767 . Since
    there is no conceivable requirement to store negative values for this parameter, this
    range could be extended by interpreting this parameter as an unsigned integer with a
    range of 0 to +65535 . The use of an unsigned value for this parameter would be
    flagged if the parameter returned a negative value when read as a signed integer.

    The USED parameter value can also be found in word 2 of the C3D file header. The
    POINT:USED header value should always be identical to the parameter value.*/
    unsigned int used;

    /*The POINT:SCALE parameter is a single floating-point value that records the scaling
    factor that must be applied to convert the signed integer 3D point values into the
    reference coordinate system values recorded by the POINT:UNITS parameter. If the
    C3D file contains 3D points saved in floating-point format then the POINT:SCALE
    value must be set to a negative value. This value is the POINT:SCALE value that will
    be applied if the C3D file is converted to integer format.

    To retain the maximum resolution for integer data, the 3D scale factor should be
    about (max. absolute coordinate value used)/32000 . This will allow all of the 3D
    point coordinates to be expressed within the range of a 16-bit signed integer. Since
    the SCALE value is required to interpret the 3D residual it is important that a sensible
    SCALE value is calculated even if the 3D information stored in floating-point format.

    The SCALE parameter value can also be found stored in floating-point format in
    words 7-8 of the C3D file header. The POINT:SCALE header value should always be
    identical to the value stored in the parameter section.

    Note that if an integer formatted C3D file is converted to a floating-point C3D file
    then it is important to preserve the absolute POINT:SCALE value, as this will allow the
    file to be transparently converted back into an integer form if desired. The
    POINT:SCALE value is also used to scale the 3D residual information when a C3D file
    is stored in integer or floating-point formats.*/
    float scale;

    /*The POINT:RATE parameter is a single floating-point value that records the 3D
    sample rate of the data contained within the C3D file in samples per second. Note
    that this is not the necessarily the same as the original data sample rate. For instance
    if the 3D data points were recorded on a 60 Hz system, then RATE should be set to
    60. If the C3D file only contains 3D sample data for every fourth sample then the
    POINT:RATE value will be 15. This parameter is used to calculate times for the 3D
    data samples.

    The RATE parameter value can also be found stored in floating-point format in words
    11-12 of the C3D file header. The POINT:RATE header value should always be
    identical to the value stored in the parameter section.

    The same POINT:RATE value applies to all 3D samples – the C3D file format
    requires that all 3D points be recorded at a single rate. This means that if the C3D
    file is used to store 3D data from a variety of different sources, all 3D points (even
    fixed points) must be sampled at the rate required by the fastest moving 3D point.*/
    float rate;

    /*The POINT:DATA_START parameter is a single signed integer value. This value is a
    pointer to the first block of the 3D/analog data section within the C3D file and must
    always be used to determine start of the data section. A C3D block is always 512
    bytes long (256 sixteen-bit words). The first block is always block number one and
    contains data structures (header records etc.) that indicate the contents of the file.

    Since the POINT:DATA_START parameter is a 16-bit signed integer parameter, this
    limits the location of the first block of 3D data storage to within the first 16Mb of the
    C3D file. By interpreting this parameter as an unsigned integer with a range of 0 to
    +65535 which extends the position of the start of the 3D/Analog data storage section
    to anywhere within the first 32Mb of the start of the C3D file. The use of an
    unsigned value for this parameter would be flagged if the parameter returned a negative
    value when read as a signed integer.

    Although located in the POINT group, this parameter is required even when the C3D
    file only contains analog data, as analog data is stored in the 3D data section.

    A copy of the DATA_START parameter value can also be found stored in word 9 of
    the C3D file header to enable software applications to quickly locate the start of 3D
    data without requiring them to read the entire parameter section. The
    POINT:DATA_START header value should always be identical to the parameter value.*/
    int data_start;

    /*The POINT:FRAMES parameter is a single signed integer value that records the
    number of 3D data frames that are recorded in the C3D file. Note that when the 3D
    data has been derived from a video based system this value does not necessarily
    correspond to the number of video frames in the original recording.

    As a signed integer, this parameter has a possible range of –32768 to +32767 but
    since the FRAMES parameter is always a positive number, the maximum frame count
    is 32767 . Since there is no reason to store negative values for this parameter, the
    range can be extended by interpreting this parameter as an unsigned integer with a
    range of 0 to +65535 . The use of an unsigned value for the FRAMES parameter
    would be flagged if the parameter returned a negative value when read as a signed
    integer.

    The POINT:FRAMES parameter value is not stored in the C3D file header. However,
    the frame numbers of the first and last 3D frames are stored words 4 and 5 of the
    C3D file header – as a result, the POINT:FRAMES parameter value should always be
    identical to the value:

                               last_frame – first_frame +1

    Note that if the parameter POINT:FRAMES is interpreted as an unsigned integer than it
    will be necessary to interpret the header frame numbers as unsigned integers.*/
    int frames;

    /*The POINT:LABELS parameter is a character data array that consists of one unique
    four-character ASCII value for each 3D data point contained within the C3D file. By
    convention, the parameters are usually four characters of upper-case standard ASCII
    text (A-Z, 0-9). The contents of each LABEL (e.g. LASI, RASI, LTOE etc.) is
    referred to as the point label and is used to reference each 3D point contained within
    the C3D file data section.

    In the original C3D file description (signed C3D), arrays use a signed byte as an
    index. Signed bytes have a possible range of –128 to +127 but since the array index
    is always a positive number, the maximum number of array entries is 127 . Since
    negative array indexes are illegal, the range of the array storage can be extended by
    interpreting the index as an unsigned byte with a range of 0 to +255 . The use of an
    unsigned byte for the array index can be assumed if the array index appeared to be
    negative when read as a signed byte.

    The purpose of the LABELS parameter is to allow applications to search for a specific
    3D point or trajectory by referencing its LABEL value instead of looking for a specific
    trajectory number in a fixed list of trajectories. This allows applications to be written
    in a general manner so that they can process data by reference e.g., calculate the
    direction of progression from the 3D points identified as points LASI, RASI and SACR .
    An application written in this way will work in any environment, as it does not
    require that the 3D data is stored in any specific order within the C3D file.

    The POINT:LABELS parameter can refer to a maximum of 127 3D data points in an
    “signed C3D file” or a maximum of 255 3D data points in a “unsigned” C3D data
    file. Note that a C3D file may contain more or less than the number of trajectories
    described by this parameter. If the C3D file contains more trajectories (read the
    parameter POINT:USED to determine the actual number stored in the 3D/analog data
    section) than are described by POINT:LABELS parameters then the additional
    trajectories must be referenced by number.

    Note that while the POINT:LABELS are typically four upper case characters, some
    applications may create labels that are larger. It is recommended that the
    POINT:LABELS values are consistent within a set of data files.*/
    int labelsSize;
    std::string* labels;

    /*The POINT:DESCRIPTIONS parameter is a character data array that usually consists
    of a short description of each 3D data point referenced by the POINT:LABELS
    parameter. There should always be a one to one relationship between the number of
    LABELS and the number of DESCRIPTIONS . By convention, these entries usually
    contain upper and lower case ASCII characters and are typically 32 characters in
    length but may be up to 127 characters long in a signed C3D file or 255 characters in
    an unsigned C3D file.

    Although it is not strictly required, it is good practice to include a DESCRIPTIONS
    parameter for each point with a LABELS entry. Since this is an array of character
    strings, the comments in the LABELS parameter description regarding the maximum
    number of array entries also apply to this parameter. Signed C3D files cannot
    contain more than 127 DESCRIPTIONS while unsigned C3D files may contain up to
    255 DESCRIPTIONS .

    This parameter exists to provide documentation about each of the individual 3D
    POINT:LABELS , which are generally short abbreviations of anatomical or other
    “landmarks” such as LASI, RKNE etc. These names generally have longer
    POINT:DESCRIPTIONS such as Left ASIS Marker and Right Knee Marker.*/
    int descriptionsSize;
    std::string* descriptions;

    /*The POINT:UNITS parameter is a single four-character value that records the units of
    distance measurement used by the 3D data e.g. mm, cm, m etc. POINT:UNITS is
    typically four ASCII characters and may be upper or lower case. The common value
    for this parameter is mm (millimeters).

    Note that this parameter only records the units of measurement – it does not control
    them and is not used in the 3D scaling calculations. Changing the value of
    POINT:UNITS from “ mm ” to “ cm ” will not re-scale the coordinate system used to
    generate the 3D data points unless this is a feature that is specifically implemented in
    your software application.*/
    std::string* units;

    /*The POINT:INITIAL_COMMAND parameter is a single ASCII character string
    (character data type) that contains an optional command string that can be read when
    a C3D file is opened. This string can be up to 127 characters long and could
    configure the C3D application to present a particular view or perform some
    predetermined analysis.

    This parameter does not affect any of the default C3D parameters and is completely
    compatible with the C3D file format. This is a nice way to pass a command or set of
    commands to a program for initial configuration, analysis or simply to run a software
    demo function for example.

    Since any application or user can access this parameter, it would be a good idea if the
    program that utilizes the values performed a thorough syntax checks on contents to
    make sure that they are correct.*/
    std::string* initial_command;

    /*This is a two-character ASCII string containing a sign together with a single
    character ( +X, +Y, +Z, -X, -Y, -Z ) that indicates which axis of the reference
    coordinate system will be displayed left-to-right across the screen. This parameter
    provides information and is compatible with the C3D file format.

    While this parameter (and its companion Y_SCREEN , below) and commonly found in
    C3D files it seems that most software applications ignore them. Remember that
    setting a C3D parameter to a particular value will only be effective if the software
    application reading the C3D file implements the parameter.*/
    std::string* x_screen;

    /*Like the X_SCREEN above , this is an ASCII string containing a sign together with a
    single character ( +X, +Y, +Z, -X, -Y, -Z ). This is used by software applications to
    indicate which axis of the reference coordinate system should be displayed bottom-
    to-top up the screen when the application initially opens the file.

    A companion to the X_SCREEN parameter above, this parameter is also compatible
    with the C3D file format. Note that the programmer could have chosen to implement
    the parameter as an array, e.g., SCREEN(1,2 ). However, this might not have been as
    intuitive for a casual user to edit or use. Creating two separate parameters was a
    good decision as it makes the function of both values clear.*/
    std::string* y_screen;

    /*This is a single floating-point value that records the synchronization offset, in
    seconds and fractions of a second, between frame 1 of the trial and the start of
    additional movie data. Exactly how this would work is not clear from the Vicon
    documentation – does a positive value indicated that the C#D data precedes the
    additional movie data or lags behind the movie data?

    The capability exists for this additional movie data to be stored within the C3D file
    although the intent of this parameter to synchronize data that has been stored in an
    external video file (such as AVI or MPEG etc). The storage of additional data such
    as video records in a separate file can potentially cause a problem if the external
    video file is overwritten, edited, or changed in some way as this could invalidate the
    stored MOVIE_DELAY value. Ideally the video information should be stored in a
    separate data block within the C3D file – see page 119 for a discussion of methods to
    create additional data blocks within the C3D file to store information such as video
    frame images.*/
    float movie_delay;

    /*This is an array of up to 255 character strings. Some software applications can
    generate a great many 3D trajectories. Since the C3D parameter arrays (used to store
    the POINT:LABEL names) have a maximum dimension of 255, the use of a single
    label array would limit the number of 3D markers that could be stored in a C3D file.
    The solution here is to create additional LABEL parameters by adding a number e.g.,
    LABEL2 . This new parameter is used to store additional ASCII labels beyond the 255
    limit in the default POINT:LABELS group and expands the maximum number of labels
    to 510. If required, additional parameters like this could exist such as LABELS3,
    LABELS4 , etc. to store even more 3D point labels.

    However, many common C3D applications will be unable to access the labels from
    trajectory numbers greater than 127 although they will have no trouble reading the
    3D data points. Since the parameter index is stored as a signed byte many
    applications may have problems interpreting these labels unless they treat the
    parameter index as unsigned.*/
    int labels2Size;
    std::string* labels2;

    /*This is another array of character strings with an entry to match each LABELS2 value.
    This parameter is synchronized with the LABELS2 parameter and contains additional
    ASCII description strings beyond the 127 array size limit of the standard
    POINT:DESCRIPTIONS parameter. This parameter will track the existence of the
    LABELSn parameters – there should be a corresponding DESCRIPTIONSn parameter
    for each LABELSn parameter.

    The same comments apply to this parameter as the previous LABELS2 – most
    common C3D applications will be unable to access these new descriptions although
    they will have no trouble reading the 3D data points. Since the additional
    information is stored as a parameter it should be quite easy to update other
    applications to handle this at the same time that support was added for the LABELS2
    parameter.*/
    int descriptions2Size;
    std::string* descriptions2;

    /*This parameter is one of a suite of parameters that are added to C3D files by
    applications that store the results of kinematic calculations in C3D files by storing
    the results as additional POINT values. While this method appears rather untidy, in
    that it can add large numbers of very “odd” data values to the 3D point group, it has
    the major advantage that it is compatible with virtually all existing C3D software
    applications. Thus, kinematic data that has been generated and stored this way is
    accessible to virtually any other user of software application.

    An array of ASCII character strings that identify the POINT parameter names that are
    associated with different data types such as measured points, calculated virtual
    points, angles or forces, etc. For each entry in this table, there should be a
    corresponding POINT parameter listing the marker labels that correspond to that type
    group.*/
    int type_groupsSize;
    std::string* type_groups;

    /*This is an array of ASCII character string labels that match strings used in
    POINT:LABELS and are used to identify trajectories stored in the 3D data section. 3D
    trajectories that match strings in POINT:ANGLES should be treated as three-
    dimensional angles measured in degrees.*/
    int anglesSize;
    std::string* angles;

    std::string* angle_units;

    /*An array of ASCII character labels. 3D point trajectories with labels matching those
    in POINT:SCALARS are to be treated as scalars rather than 3D co-ordinates. The
    magnitude of the scalar is stored in the Z component with X and Y both set to zero.
    The units (if any) depend on the meaning of each scalar according to the model that
    produced them and are recorded in the parameter POINT:SCALAR_UNITS .*/
    int scalarsSize;
    std::string* scalars;

    /*A single ASCII string that stores the measurement units used by the scalar values
    (e.g. mm, M etc.,) stored in the C3D file.*/
    std::string* scalar_units;

    /*This is an array of ASCII character labels. 3D trajectories with labels matching
    those in this list are to be treated as powers rather than 3D points. Since powers are
    scalars rather than vectors, the value is held in the Z component with X and Y both
    set to zero. See the parameter POINT:POWER_UNITS parameter for units used to
    store the powers.*/
    int powersSize;
    std::string* powers;

    /*This is a single ASCII string that stores the measurement units used by the power
    values, e.g. mW, W, kW etc., and stored in the C3D file.*/
    std::string* power_units;

    /*This is an array of ASCII character labels. Trajectories with labels matching those in
    this list are to be treated as forces rather than 3D coordinates. See the parameter
    POINT:FORCE_UNITS for the units used to store the forces.*/
    int forcesSize;
    std::string* forces;

    /*A single ASCII string that stores the measurement units used by the force values, e.g.
    N, kN, mN, etc.*/
    std::string* force_units;

    /*An array of ASCII character labels. Trajectories with labels matching those in this
    list are to be treated as moments rather than 3D coordinates. See the parameter
    POINT:MOMENT_UNITS for the units used to store the moments.*/
    int momentsSize;
    std::string* moments;

    /*A single ASCII string that stores the measurement units used by the moment values,
    e.g. Nmm, Nm, etc.*/
    std::string* moment_units;

    /*An array of ASCII character labels. These labels are used as a base name for
    identifying three trajectories each that represent the force, moment and point
    components of a reaction. The corresponding force trajectories have an “ .F ” suffix,
    the moment trajectories an “ .M ” suffix and the point trajectories a “ .P ” suffix. For
    example, “ LKNEE ” would correspond to “ LNKEE.F ”, “ LKNEE.M ”, and “ LNKEE.P ”
    trajectories. Note that force and moment trajectories listed in REACTIONS should not
    appear in the FORCE and MOMENTS lists.*/
    int reactionsSize;
    std::string* reactions;

};

/****************/
/*   4.ANALOG   */
/****************/

class Analog {
public:

private:
    /*The ANALOG:USED parameter is a single signed integer value that records the
    number of analog channels that are contained within the C3D file. The value stored
    in ANALOG:USED is used to compute the analog data frame rate from the total
    number of analog data words collected during each 3D frame. The total number of
    ADC samples stored per 3D sample frame must be an integer multiple of
    ANALOG:USED .

    The value of the ANALOG:USED parameter is not stored in the C3D file header but
    can be calculated from two values that are stored in the C3D file header. The
    ANALOG:USED parameter value is equal to C3D header word 3 divided by C3D
    header Word 10.

    As a signed integer, the ANALOG:USED parameter has a possible range of –32768 to
    +32767 . Since there is no conceivable requirement to store negative values for this
    parameter, this range could be extended by interpreting this parameter as an unsigned
    integer with a range of 0 to +65535 . The use of an unsigned value for this parameter
    would be flagged if the parameter returned a negative value when read as a signed
    integer.

    This means that in theory, C3D files that contain more than 32,767analog channels
    may not be readable by some older software applications. In practice, it is unusual to
    find analog hardware systems collecting more than 64 channels of analog data.*/
    int used;

    /*The ANALOG:LABELS parameter is a character data array that consists of a unique
    four-character ASCII (A-Z, 0-9) string for each analog channel contained within the
    C3D file. This is referred to as the analog channel label and is used to reference
    each channel of data contained within the C3D file data section in the order in which
    the channels are stored. Labels are typically 4-30 characters long (4 upper case
    characters is the default).

    The purpose of the LABELS parameter is to allow applications to search for a specific
    channel of data by referencing its LABEL value instead of looking for a specific
    analog channel number. This allows applications to be written in a general manner
    so that they can process data by reference e.g. analyze all the EMG channels where
    they are identified as channels EM01 through EM32 . An application written in this
    way will work in any environment, as it does not require that the EMG signals be
    stored on specific numbered ADC channels.

    Note that while ANALOG:LABELS are typically four upper case characters, some
    applications may create labels that are longer. If compatibility with older
    FORTRAN based applications is required then LABELS should be only four
    characters long.

    As described in the original (signed) C3D specifications, this parameter can refer to a
    maximum of 127 analog channels because the array index uses a signed byte.
    However, unsigned C3D files may use an unsigned byte as an array index to extend
    the number of LABELS to 255. Any application that reads a negative array index for
    LABELS should assume that the index is an unsigned byte.

    Note that a C3D file may contain more or less analog channels than described by this
    parameter. If the C3D file contains more analog channels than are described by
    ANALOG:LABELS parameters then the additional analog channels must be referenced
    by number.*/
    int labelsSize;
    std::string* labels;

    /*The ANALOG:DESCRIPTIONS parameter is a character data array that usually consists
    of a short description of each analog channel referenced by the ANALOG:LABELS
    parameter. There should always be a one to one relationship between the number of
    LABELS and the number of DESCRIPTIONS . Although it is not essential, there should
    generally be a one to one relationship between the number of LABELS and the number
    of DESCRIPTIONS . By convention, these entries usually contain upper and lower
    case ASCII characters and are typically 32 characters in length but may be up to 127
    characters long in a signed C3D file or 255 characters in an unsigned C3D file.

    This parameter exists to provide documentation about each of the individual analog
    channels. The ANALOG:LABELS parameter generally stores a short abbreviation of
    each analog channel name such as 1FX, EM05 etc. Each of the channels referenced
    by these LABELS generally has a longer ANALOG:DESCRIPTIONS such as Force Plate
    One – Fx channel and Left Extensor Hallucis Longus etc.

    Note that, like the POINTS:DESCRIPTIONS , the ANALOG:DESCRIPTIONS are provided
    simply as a means of providing a human readable description or documentation of
    the analog channel. Software applications that need to access individual analog
    channels should access each channel by use of the ANALOG:LABEL , not the
    ANALOG:DESCRIPTION parameter.*/
    int descriptionsSize;
    std::string* descriptions;

    /*The ANALOG:GEN_SCALE parameter is a single floating-point value that is used as a
    universal analog scaling factor. It is applied in addition to the individual analog
    channel scaling factors and acts on all channels. Common values for GEN_SCALE
    are:

    • 0.0048828 – the value of a single bit of data from a 12-bit ADC that is
    measuring a ± 10V input range. An individual channel ANALOG:SCALE
    value would then be 1.00 to obtain the analog data scaled in Volts.

    • 0.0024414 – the value of a single bit of data from a 12-bit ADC that is
    measuring a ±5 V input range. An individual channel ANALOG:SCALE
    value would then be 1.00 to obtain the analog data scaled in Volts.

    • 1.00 – individual channel ANALOG:SCALE values must be set to
    0.0048828 to obtain analog data scaled in Volts when sampled by a 12-
    bit ADC that is measuring a ± 10V input range, or 0.0024414 when
    measuring with a ±5 V ADC input range.

    • 0.062500 – upgrading a 12-bit data collection system to use a 16-bit
    ADC requires only that the ANALOG:GEN_SCALE parameter change to
    reflect the new resolution while retaining the original . If the system
    used a value of 1.00 with a 12-bit ADC then changing the
    ANALOG:GEN_SCALE parameter by a factor of 16 is all that is required
    when the ADC card is upgraded to continue using the original 12-bit
    ANALOG:SCALE values unchanged.

    Since the value of the ANALOG:GEN_SCALE parameter is used with the individual
    ANALOG:SCALE values to calculate the correct value of each analog channel signal, it
    is critically important that the ANALOG:GEN_SCALE value is not changed without
    considering its effect on the individual ANALOG:GEN_SCALE values.
    It is important to take into account the possible scaling ranges when selecting scaling
    values. C3D files using an ANALOG:GEN_SCALE value of 1.000 will require
    individual ANALOG:SCALE values of 0.0048828 to scale the output data in Volts, an
    EMG application might require scaling in microvolts with corresponding
    ANALOG:SCALE value in the range of 0.0000048828 to 0.0000000048828, while the
    force plate, scaled in Newtons would use values of 100 – 300 .*/
    float gen_scale;

    /*The ANALOG:SCALE parameter is an array of floating-point values that must be
    applied (together with the ANALOG:GEN_SCALE parameter value) to convert the raw
    analog data to real world values – normally the units described in the ANALOG:UNITS
    parameter. As a result, it is essential that each analog channel have an associated
    SCALE parameter together with an OFFSET parameter so that the correctly scaled
    analog values can be calculated.

    In the original C3D file description (signed C3D), arrays, such as ANALOG:SCALE ,
    use a signed byte as an index. Signed bytes have a possible range of –128 to +127
    but since the array index is always a positive number, the maximum number of array
    entries for ANALOG:SCALE is 127 . Since negative array indexes are illegal, the range
    of the array storage can be extended by interpreting the index as an unsigned byte
    with a range of 0 to +255 . The use of an unsigned byte for the array index can be
    assumed if the array index appeared to be negative when read as a signed byte.
    To convert the analog signal to volts measured at the ADC inputs, the necessary
    scale factor is given by the following expression:

    ANALOG : SCALE = ADC _ range / ADC _ resolution

    The ANALOG:GEN_SCALE parameter may be used to apply an additional uniform
    scale factor to all analog channels. In these discussions it will be assumed that
    ANALOG:GEN_SCALE = 1.0 and therefore has no effect on the results although we will
    show it in the calculations thus:

    ANALOG : GEN _ SCALE * ANALOG : SCALE = ADC _ range / ADC _ resolution

    Since the two C3D file variables are both in the ANALOG group, this can be simply
    stated thus:

    SCALE = (ADC _ range / ADC _ resolution) / GEN _ SCALE


    The ADC_range is the actual input range of the ADC card that is used to collect the
    data. This is normally ±10Volts, which yields an actual ADC_range of 20 – that is
    to say; the ADC card can record signals as over the range of 10 volts negative to 10
    volts positive magnitude, a total range of 20 Volts.*/
    float* scale;

    /*The ANALOG:OFFSET parameter is an array of integer values that are subtracted from
    each analog measurement before the individual ANALOG:SCALE scaling factors are
    applied. By default a signed integer, the ANALOG:OFFSET values may be either
    positive or negative numbers in the range of –32768 to +32767 and can include the
    value of zero. However, if the ANALOG:FORMAT parameter is “ UNSIGNED ” then the
    ANALOG:OFFSET parameter must be interpreted as unsigned integer numbers in the
    range of 0 to +65537 .

    There must always be a one to one correspondence between the ANALOG:SCALE and
    ANALOG:OFFSET parameters. Both the SCALE and OFFSET parameters must exist for
    every analog channel up to the value stored in the ANALOG:USED parameter.

    The sampled analog data is normally stored in the C3D file as signed integer values
    within the range of -32768 to +32767. It’s worth noting at this point that the precise
    binary encoding method for analog data is not specified within the C3D format
    specification and, so long as the scaled results are correct, analog data can be stored
    anywhere within the range of the integer data type.

    In general, the analog data is encoded over a symmetrical range (from a value of +v
    to –v) but this is not an absolute requirement. Several software applications write the
    raw analog data samples as unsigned values and use the OFFSET parameter to
    convert them to back to signed values when the data is scaled into real-world values.

    Offset Binary is a simple binary count that is offset in order to represent equal
    magnitude over the positive and negative ranges – the maximum negative range
    being all zeros while all ones represents the maximum positive range. The mid-
    range or zero is represented by setting the most significant bit, with all other bits
    cleared. Two’s Complement Binary uses a simple binary count to represent all
    positive values while all negative values are stored with the most significant bit set.

    The Two’s Complement format simplifies the interface at a machine code level but
    offers no advantages within the C3D format or within high-level languages. It is a
    common output option for many Analog to Digital Converter (ADC) devices.
    Software applications must always use the OFFSET and SCALE parameters to
    determine data magnitude and must not assume that either OFFSET or SCALE will
    take any particular value.

    Typically, an analog-to-digital converter (ADC) has 12 to 16 bits of resolution, and
    can capture and store analog data using signed integer values from -32768 to +32767
    representing both positive and negative input signal excursions. In order for
    software applications to correctly translate the analog data recorded in the C3D file
    into real world values, the ANALOG:OFFSET and ANALOG:SCALE parameters must
    contain appropriate values. These are applied as shown:

    real world value = (data value – ANALOG:OFFSET) * ANALOG:SCALE

    For example, a ±5 volt ADC with 12-bits of resolution can produce 4096 discreet
    samples values – these may be mapped as unsigned values using the range of 0 to
    +4095 (in which case the OFFSET would be +2047 for a symmetrical +5 to –5 volt
    range, translating the ADC samples into the signed integers). They could equally
    well be mapped directly as signed integers in the range of –2048 to +2047 in which
    case the OFFSET would be 0. If the ANALOG:SCALE and OFFSET values are applied
    correctly, both configurations will return identical values covering the range of +5 to
    –5 volts.

    One application of the ANALOG:OFFSET is to adjust the zero baselines for devices
    such as force plates that should return a zero when unloaded. In practice, force
    plates are notorious for drifting away from an unloaded zero value, which can lead to
    measurement errors in use. There are two common methods for “zeroing” these
    devices, both involve determining the measurement error during some period of
    unloaded sampling, by subtracting the sampled data values from the recorded
    ANALOG:OFFSET value. This result can then be used to reset the ANALOG:OFFSET
    parameters to new values (each analog channel will have a different “error” value
    here) or, can be used to adjust the sampled analog data values or correct the original
    offset measurement error. Both methods are in common use; both methods may run
    into problems if either the analog data or OFFSET parameters are close to their limits.*/
    short int* offset;

    /*The ANALOG:UNITS parameter is an array of character data values (normally each
    value is 4 characters in length). This parameter records the analog measurement
    units for each channel (e.g. V, N, Nm). The units should describe the quantities after
    the scaling factors are applied – as a result, there should always be one
    ANALOG:UNITS entry for a total of ANALOG:USED channels.
    Note that changing the ANALOG:UNITS parameter does not automatically affect the
    calculated analog values, as it is not used in the analog scaling calculations. You
    must change the ANALOG:SCALE parameter to re-scale the analog data.*/
    std::string* units;

    /*The ANALOG:RATE parameter is a single floating-point value that records the sample
    rate at which the analog data was collected in samples per second. This indicates the
    number of analog samples that exist in each analog channel for the given
    POINT:RATE value. THUS, an ANALOG:RATE value of 600 for a C3D file that
    contains data with a POINT:RATE of 60.00 has 10 analog samples per 3D sample ( 60
    x 10 ).

    The RATE parameter value is not stored in the C3D file header. However, the header
    does record the 3D sample frame rate in words 11-12 as well as the number of
    analog samples per 3D frame in word 10. The ANALOG:RATE parameter value
    should always be identical to the value:
    3D_frame_rate * analog samples per frame

    THUS, an ANALOG:RATE will have a value of 600 in a C3D file with a POINT:RATE
    value of 60 that contains 10 samples of analog data per 3D frame. Note that
    although the C3D format specified that the number of analog samples per 3D frame
    must be an integer number, the actual 3D frame rate is a floating-point value since it
    may not be exact. Therefore, the ANALOG:RATE (from the above calculation) must
    also be stored as a floating-point value.

    Note that a single ANALOG:RATE value applies to all analog channels – the C3D file
    format requires that all analog channels be recorded at a single rate. This means that
    if the C3D file is used to store analog data from a variety of different sources, all
    analog signals must be sampled at the rate required by the source with the highest
    frequency components.*/
    float rate;

    /*The ANALOG:FORMAT parameter is a character data array that consists of a single
    ASCII (A-Z, 0-9) string that documents the analog data format used within the C3D
    file. The parameter has two possible values: SIGNED or UNSIGNED . This specifies
    whether the 'data' format (rather than the 'storage' format) is 2's compliment or offset
    binary respectively. This parameter applies to all analog data values within the 3D
    and Analog data section. It should normally be “locked”.

    If the ANALOG:FORMAT parameter contains the string “ SIGNED” then the C3D
    'storage' format for both the data samples and the ANALOG:OFFSET parameters must
    also be “ SIGNED ”. This is the default storage method for all analog data values,
    irrespective of resolution and allows data to be stored using signed integer values
    from -32768 to +32767 representing both positive and negative input signal
    excursions.

    If the ANALOG:FORMAT parameter contains the string “ UNSIGNED ” then the
    ANALOG:OFFSET parameters must also be treated as “UN SIGNED ” values.
    If the ANALOG:FORMAT parameter does not exist the it should be assumed that its
    value is SIGNED unless the analog data contains 16-bit values, in which case
    UNSIGNED is a possibility.*/
    std::string* format;

    /*The ANALOG:BITS parameter is a single integer value that describes the analog data
    sample resolution and will normally contain one of three values, 12, 14 or 16. As
    this value directly affects the interpretation of the analog data it should normally be
    “locked”. If the parameter does not exist then it is usually safe to assume that its
    value is 12. Alternatively, its value can be measured by reading every analog sample
    contained in the 3D/Analog data section and determining the effective resolution
    from the highest analog data value found.

    Software applications that change the resolution of analog data values for
    compatibility (i.e. down sampling 16-bit data to 12-bits) should always update this
    parameter to indicate the resolution of the data stored within the C3D file as it can be
    used to allow software applications to recalculate the ANALOG:SCALE parameter
    values.*/
    int bits;

    /*The ANALOG:GAIN parameter is an array of signed integer values – one entry per
    USED analog channel – that record the voltage ranges of the individual analog
    channels. The implementers specified the following values:

    0 = unknown
    1 = +/- 10Volts
    2 = +/- 5Volts
    3 = +/- 2.5Volts
    4 = +/- 1.25Volts.

    This is a useful addition to the ANALOG parameters because is allows an application
    to record the gain or voltage range associated with individual analog channels. This
    allows applications to modify the ANALOG:SCALE values to be adjusted when any
    particular channel gain is changed. This is particularly useful since the gains applied
    to each analog channel are used in the calculations for the ANALOG:SCALE values
    associated with the channels – application software that can determine individual
    channel gains can modify the analog scale values safely.

    This is compatible with the C3D file format although software applications may need
    to be modified to take advantage of the additional information.*/
    int* gain;

};

/************************/
/*   5.FORCE_PLATFORM   */
/************************/

class Force_Platform {
public:

private:

};

/***********************/
/*   6.Event_Context   */
/***********************/

class Event_Context {
public:

private:

};

/***************/
/*   7.EVENT   */
/***************/

class Event {
public:

private:

};

/**********************/
/*   8.MANUFACTURER   */
/**********************/
/*The MANUFACTURER group can be used to record information about the software
or hardware used to create or modify the C3D file. This group is intended to simply
provide information that can be used to identify the source of the data later. There
are no requirements that this group exist in a C3D file or that it contains any specific
parameters but the following parameters are common.*/

class Manufacturer {
public:
    inline std::string Company() {return company[0];}
    inline std::string Software() {return software[0];}
    inline std::string Version() {return version[0];}

    inline void SetManufacturer(Manufacturer* manufacturer, Parameter_c3d parameter);

private:
    /*An ASCII character string, the COMPANY parameter will identify the name of the
    company whose software was the original source of the C3D file. If this parameter
    exists then it should be locked and should not be changed by other software
    applications if they edit or modify the C3D file.*/
    std::string* company;

    /*An ASCII character string, the SOFTWARE parameter will identify the name of the
    software application that created the C3D file. If this parameter exists then it should
    be locked and should not be changed by other software applications if they edit or
    modify the C3D file.*/
    std::string* software;

    /*Stored as an ASCII character string, the VERSION parameter is intended to identify
    the version of the software that created the C3D file. If this parameter exists then it
    should be locked and should not be changed by other software applications if they
    edit or modify the C3D file.*/
    std::string* version;
};

/******************/
/*   9.ANALYSIS   */
/******************/

class Analysis {
public:

private:

};

/*********************/
/*   10.PROCESSING   */
/*********************/

class Processing {
public:

private:

};

/****************************************/
/*               Data_c3d               */
/****************************************/

class Points_C3D {
public:
    inline float X(void) {return coord_x;}
    inline float Y(void) {return coord_y;}
    inline float Z(void) {return coord_z;}

    inline float Camera(void) {return camera;}
    inline float Residual(void) {return residual;}

    inline void SetPoint(Points_C3D* point, const float x, const float y, const float z, const  float cam, const float res) {
        point->coord_x = x;
        point->coord_y = y;
        point->coord_z = z;

        point->camera = cam;
        point->residual = res;
    }

private:
    float coord_x;
    float coord_y;
    float coord_z;

    float camera;
    float residual;
};

class Analog_C3D {
public:
    inline float AnalogPoint(void) {return analog;}

    inline void SetAnalog(Analog_C3D* analog, float value) {analog->analog = value;}

private:
    float analog;
};

class Frames_C3D {
public:
    inline Points_C3D Point(const int index) {return points[index];}
    inline Analog_C3D Analog(const int index) {return analog[index];}

    void ReadFrame(Frames_C3D* frame, FILE* file, const int pointSize,
                   const int analogSize, const float pointScale, const int endianFlag);

    void CleanUp(Frames_C3D* frame);
private:
    Points_C3D *points;
    Analog_C3D *analog;
};

class Relocation_C3D {
public:

    float DX(const int index) {return dx[index];}
    float DY(const int index) {return dy[index];}
    float DZ(const int index) {return dz[index];}

    float DR(const int index) {return dr[index];}

    void SetRelocation(Relocation_C3D* rel, Frames_C3D frame1, Frames_C3D frame2, const int pointSize);

private:

    float* dx;
    float* dy;
    float* dz;

    float* dr;


};

class Data_c3d {
public:
    inline Frames_C3D Frame(const int index) {return frames[index];}
    inline Relocation_C3D Relocation(const int index) {return relocation[index];}

    void ReadData(Data_c3d* data, FILE* file, const int frameSize, const int pointSize,
                  const int analogSize, const float pointScale, const int endianFlag);

    void print_point_data_to_file(Data_c3d data, const std::string fileName, const int frameSize, const int pointSize);

    void print_analog_data_to_file(Data_c3d data, const std::string fileName, const int frameSize, const int analogSize);

    void CleanUp(Data_c3d* data, const int frameSize);

private:
    Frames_C3D* frames;
    Relocation_C3D* relocation;
};

/****************************************/
/*               Read_c3d               */
/****************************************/

class Read_C3D {
public:
    Read_C3D() { isOpen = false;}

    void Import(std::string fileName, QWidget* widget, Read_C3D* c3d_f); //Import C3D File

    void CleanUp(Read_C3D* c3d_f); //Clean memory

    inline bool IsOpen(void) {return isOpen;}
    //inline void SetIsOpen(bool state) {isOpen = state};

    inline Header_c3d Header(void) {return headerBlock;}
    inline Parameter_c3d Parameter(void) {return parameterBlock;}
    inline Data_c3d Data(void) {return dataBlock;}

    inline Trial TRIAL(void) {return trial;}
    inline Subjects SUBJECTS(void) {return subjects;}
    inline Manufacturer MANUFACTURER(void) {return manufacturer;}
    inline Point POINT(void) {return point;}

    //Print Header File to a File
    void printHeaderFile(const std::string fileName) {headerBlock.print_header_to_file(headerBlock, fileName);}

    //Print Parameter File to a File
    void printParameterFile(const std::string fileName) {parameterBlock.print_parameter_to_file(parameterBlock, fileName);}

    //Print Point Data to a CSV Type File
    void printPointFile(const std::string fileName) {
      int frameSize = headerBlock.LastFrame() - headerBlock.FirstFrame() + 1;
      int pointSize = headerBlock.NumberOfPoints();

      dataBlock.print_point_data_to_file(dataBlock, fileName, frameSize, pointSize);
    }

    //Print Analog Data to a CSV Type File
    void printAnalogFile(const std::string fileName) {
      int frameSize = headerBlock.LastFrame() - headerBlock.FirstFrame() + 1;
      int analogSize = headerBlock.NumberOfAnalog();

      dataBlock.print_analog_data_to_file(dataBlock, fileName, frameSize, analogSize);
    }

    ~Read_C3D() {}

private:
    short int system_endian;
    short int file_endian;
    bool isOpen;

    Header_c3d headerBlock;
    Parameter_c3d parameterBlock;
    Data_c3d dataBlock;

    Trial trial;
    Subjects subjects;
    Point point;

    Manufacturer manufacturer;

};

#endif // READ_C3D_H
