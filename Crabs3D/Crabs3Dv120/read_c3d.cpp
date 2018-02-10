#include "read_c3d.h"
#include <cmath>
#include <stdio.h>
#include <ctype.h>

#include <QMessageBox>

/*****************************************/
/*               Templates               */
/*****************************************/

/*
 * Finds out the system reading Endian.
 */
namespace sys_endian {

    const unsigned one = 1U;

    inline bool little_endian() {
        return reinterpret_cast<const char*>(&one) + sizeof(unsigned) - 1;
    }

    inline bool big_endian() {
        return !little_endian();
    }

} //sys_endian

//SwapEndian
template <typename T>
T swapEndian(const T numIn) {
    T numOut;
    int size = sizeof(T);

    char* numToConvert = (char*) & numIn;
    char* numToReturn = (char*) & numOut;

    for(int i = 0, j = size-1; j >= 0; i++, j--)
        numToReturn[i] = numToConvert[j];

    return numOut;
}

//SwapEndian
template <typename T>
T returnByte(const T numIn, const int bit) {
    T numOut;

    int size = sizeof(T);

    char* numToConvert = (char*) & numIn;
    char* numToReturn = (char*) & numOut;

    for(int i = 0; i < size; i++)
         numToReturn[i] = 0;

    numToReturn[0] = numToConvert[bit-1];

    return numOut;
}

//ReadWord
template <typename T>
void readWord(T* word, const int bytes, FILE* file) {
    char* wordToRead = (char*) word;

    if((int)sizeof(T) > bytes)
        for(int i = 0; i < bytes; i++)
            wordToRead[i+1] = 0;

    for(int i = 0; i < bytes; i++) {
        wordToRead[i] = 0;
        fread(&wordToRead[i], 1, 1, file);
    }
}

//ReadWordWithEndian
template <typename T>
void readWordWithEndian(T* word, const int bytes, FILE* file, const int endian_flag) {
    char* wordToRead = (char*) word;

    if((int)sizeof(T) >= bytes)
        for(int i = 0; i < bytes; i++)
            wordToRead[i+1] = 0;

    if(endian_flag == SAME_ENDIAN)
        for(int i = 0; i < bytes; i++) {
            fread(&wordToRead[i], 1, 1, file);
        }
    else if(endian_flag == DIFF_ENDIAN) {
        if(bytes > SIZE_8_BIT)
            for(int i = bytes-1; i >= 0; i--) {
                wordToRead[i] = 0;
                fread(&wordToRead[i], 1, 1, file);
            }
        else
            for(int i = 0; i < bytes; i++) {
                wordToRead[i] = 0;
                fread(&wordToRead[i], 1, 1, file);
            }
    }
}

/***********************/
/* Auxirialy Functions */
/***********************/

int wordCounter(const char* text) {
    int counter = 0;
    int i = 0;

    while(i < (int)strlen(text) || text[i] != '\0') {
        while(!isgraph(text[i]) && text[i] != '\0') {
            i++;
        }
        if(isgraph(text[i])) {
            counter++;
            while(isgraph(text[i]) && text[i] != '\0') {
                i++;
            }
        }
    }

    return counter;
}

std::string wordReturn (const char* text, const int wordToReturnIndex) {
    int counter = 0;
    int i = 0;
    std::string word = "";

    while(i < (int)strlen(text) || text[i] != '\0') {
        while(!isgraph(text[i]) && text[i] != '\0') {
            i++;
        }
        if(isgraph(text[i])) {
            counter++;
            if(counter == wordToReturnIndex) {
                while(isgraph(text[i]) && text[i] != '\0') {
                    word += text[i];
                    i++;
                }
                break;
            } else {
                while(isgraph(text[i]) && text[i] != '\0') {
                    i++;
                }
            }
        }
    }

    return word;
}

/******************************************/
/*               Header_c3d               */
/******************************************/

void Header_c3d::read_header_block(FILE* file, Header_c3d* header) {
    readWord(&header->parameter_block, SIZE_8_BIT, file);                   //1-byte
    if(header->parameter_block == 1) {
        fseek(file, 0, SEEK_SET);
        return ;
    }
    readWord(&header->id_number, SIZE_8_BIT, file);                         //1-byte
    readWord(&header->points_number, SIZE_16_BIT, file);                    //2-bytes
    readWord(&header->analog_number, SIZE_16_BIT, file);                    //2-bytes
    readWord(&header->first_frame, SIZE_16_BIT, file);                      //2-bytes
    readWord(&header->last_frame, SIZE_16_BIT, file);                       //2-bytes
    readWord(&header->maximum_interpolation_gap, SIZE_16_BIT, file);        //2-bytes

    readWord(&header->scale_factor, SIZE_32_BIT, file);                     //4-bytes
    readWord(&header->data_start_block, SIZE_16_BIT, file);                 //2-bytes
    readWord(&header->analog_per_frame, SIZE_16_BIT, file);                 //2-bytes
    readWord(&header->frame_rate, SIZE_32_BIT, file);                       //4-bytes

    for(int i=0; i<135; i++)
        readWord(&header->future_use_block_1[i], SIZE_16_BIT, file);        //270-bytes

    readWord(&header->key_value_1, SIZE_16_BIT, file);                      //2-bytes
    readWord(&header->first_label_range_block, SIZE_16_BIT, file);          //2-bytes
    readWord(&header->key_value_2, SIZE_16_BIT, file);                      //2-bytes
    readWord(&header->event_time_num, SIZE_16_BIT, file);                   //2-bytes
    readWord(&header->future_use_block_2, SIZE_16_BIT, file);               //2-bytes

    for(int i=0; i<18; i++)
        readWord(&header->event_times_in_sec[i], SIZE_32_BIT, file);            //72-bytes

    for(int i=0; i<9; i++)
        readWord(&header->event_display_flags[i], SIZE_16_BIT, file);       //18-bytes

    readWord(&header->future_use_block_3, SIZE_16_BIT, file);               //2-bytes

    for(int i=0; i<18; i++)
        readWord(&header->event_labels[i], SIZE_32_BIT, file);              //72-bytes

    for(int i=0; i<22; i++)
        readWord(&header->future_use_block_4[i], SIZE_16_BIT, file);        //44-bytes
}

/*If File Endian is different than System Endian then swap header values*/
void Header_c3d::swapHeader(Header_c3d* header) {
    header->points_number = swapEndian(header->points_number);
    header->analog_number = swapEndian(header->analog_number);
    header->first_frame = swapEndian(header->first_frame);
    header->last_frame = swapEndian(header->last_frame);
    header->maximum_interpolation_gap = swapEndian(header->maximum_interpolation_gap);
    header->scale_factor = swapEndian(header->scale_factor);
    header->data_start_block = swapEndian(header->data_start_block);
    header->analog_per_frame = swapEndian(header->analog_per_frame);
    header->frame_rate = swapEndian(header->frame_rate);

    for(int i = 0; i < 135; i++)
        header->future_use_block_1[i] = swapEndian(header->future_use_block_1[i]);

    header->key_value_1 = swapEndian(header->key_value_1);
    header->first_label_range_block = swapEndian(header->first_label_range_block);
    header->key_value_2 = swapEndian(header->key_value_2);
    header->event_time_num = swapEndian(header->event_time_num);
    header->future_use_block_2 = swapEndian(header->future_use_block_2);

    for(int i = 0; i < 18; i++)
        header->event_times_in_sec[i] = swapEndian(header->event_times_in_sec[i]);

    for(int i = 0; i < 9; i++)
        header->event_display_flags[i] = swapEndian(header->event_display_flags[i]);

    header->future_use_block_3 = swapEndian(header->future_use_block_3);

    for(int i = 0; i < 22; i++)
        header->future_use_block_4[i] = swapEndian(header->future_use_block_4[i]);
}

/*Print Header block to file*/
void Header_c3d::print_header_to_file(Header_c3d header, const std::string fileName) {
    FILE* outputFile;

    outputFile = fopen(fileName.c_str(), "w");

    fprintf(outputFile, "parameter_block = %d\n", header.parameter_block);
    fprintf(outputFile, "ADTech_ID_number = %d\n", header.id_number);
    fprintf(outputFile, "Number_of_Points = %d\n", header.points_number);
    fprintf(outputFile, "Number_of_Analog_data = %d\n", header.analog_number);
    fprintf(outputFile, "First_Frame = %d\n", header.first_frame);
    fprintf(outputFile, "Last_Frame = %d\n", header.last_frame);
    fprintf(outputFile, "Max_Interpolation_Gap = %d\n", header.maximum_interpolation_gap);
    fprintf(outputFile, "Point_Scale_Factor = %.4f\n", header.scale_factor);
    fprintf(outputFile, "Data_Start_Block = %d\n", header.data_start_block);
    fprintf(outputFile, "Analog_Per_Frame = %d\n", header.analog_per_frame);
    fprintf(outputFile, "Frame_Rate = %.4f Hz\n", header.frame_rate);

    for(int i=0; i<135; i++)
        fprintf(outputFile, "Future_use_block_[%d] = %d\n", i+1, header.future_use_block_1[i]);

    fprintf(outputFile, "Key_Value_1 = %d\n", header.key_value_1);
    fprintf(outputFile, "First_label_Range_Block = %d\n", header.first_label_range_block);
    fprintf(outputFile, "Key_Value_2 = %d\n", header.key_value_2);
    fprintf(outputFile, "Event_Time_Num = %d\n", header.event_time_num);
    fprintf(outputFile, "Future_use_block = %d\n", header.future_use_block_2);

    for(int i=0; i<18; i++)
        fprintf(outputFile, "Event_Time = %f sec\n", header.event_times_in_sec[i]);

    for(int i=0; i<9; i++)
        fprintf(outputFile, "Event_Displat_Flag = %d\n", header.event_display_flags[i]);

    fprintf(outputFile, "Future_use_block = %d\n", header.future_use_block_3);

    for(int i=0; i<18; i++)
        fprintf(outputFile, "Event_Label_[%d] = %s\n", i+1, header.event_labels[i]);

    for(int i=0; i<22; i++)
        fprintf(outputFile, "Future_use_block_[%d] = %d\n", i+1, header.future_use_block_1[i]);

    fclose(outputFile);
}

/*********************************************/
/*               Parameter_c3d               */
/*********************************************/

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~ Parameter_Header_C3D ~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void Parameter_Header_C3D ::ReadHeaderBlock(FILE* file, Parameter_Header_C3D& header) {
    readWord(&header.parameter_block, SIZE_8_BIT, file); //1-byte
    readWord(&header.id_number, SIZE_8_BIT, file); //1-byte
    readWord(&header.number_of_parameter_block, SIZE_8_BIT, file); //1-byte
    readWord(&header.processor_type, SIZE_8_BIT, file); //1-byte
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~ Parameter_Parameter_C3D ~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
bool Parameter_Parameter_C3D::ReadParameterBlock(FILE* file, Parameter_Parameter_C3D& parameter, const int endianFlag) {
    bool loop;

    readWordWithEndian(&parameter.character_number, SIZE_8_BIT, file, endianFlag); //Read character number
    if(parameter.character_number < 0) {
        parameter.character_number *= -1;
        parameter.locked = true;
    } else {
        parameter.locked = false;
    }

    readWordWithEndian(&parameter.parameter_ID, SIZE_8_BIT, file, endianFlag); //Read parameter ID

    //Read Parameter Name
    parameter.parameter_name = (char*)malloc(parameter.character_number*sizeof(char));
    for(int i = 0; i < parameter.character_number; i++) {
        readWordWithEndian(&parameter.parameter_name[i] , SIZE_8_BIT, file, endianFlag);
        parameter.parameter_name[i+1] = '\0';
    }

    //Read next group pointer
    readWordWithEndian(&parameter.next_group_parameter_start, SIZE_16_BIT, file, endianFlag);
    //Check if this is the last group
    if(parameter.next_group_parameter_start == 0) {
        loop = false;
    } else {
        loop = true;
    }

    //Read byte format
    readWordWithEndian(&parameter.byte_format, SIZE_8_BIT, file, endianFlag);
    if(parameter.byte_format == 255)
        parameter.byte_format = -1;

    //Read dimensions size
    readWordWithEndian(&parameter.number_of_dimensions, SIZE_8_BIT, file, endianFlag);

    //Read dimensions
    int dim = 1;
    if(parameter.number_of_dimensions == 0) {
        parameter.parameter_dimensions[0] = 0;
    }
    for(int i = 0; i < parameter.number_of_dimensions; i++) {
        readWordWithEndian(&parameter.parameter_dimensions[i], SIZE_8_BIT, file, endianFlag);
        dim *= parameter.parameter_dimensions[i];
    }

    //Read the correct data type
    switch(parameter.byte_format) {
        case FORMAT_CHAR: {
                parameter.parameter_data_char = (char*)malloc((dim+1)*sizeof(char));
                parameter.parameter_data_char[0] = '\0';
                for(int i = 0; i < dim; i++) {
                    readWordWithEndian(&parameter.parameter_data_char[i], SIZE_8_BIT, file, endianFlag);
                    parameter.parameter_data_char[i+1] = '\0';
            }
            break;
        } case FORMAT_BYTE: {
            parameter.parameter_data_byte = (short int*)malloc((dim+1)*sizeof(short int));
            for(int i = 0; i < dim; i++) {
                readWordWithEndian(&parameter.parameter_data_byte[i], SIZE_8_BIT, file, endianFlag);
            }
            break;
        } case FORMAT_INT_16: {
            parameter.parameter_data_16_int = (short int*)malloc((dim+1)*sizeof(short int));
            for(int i = 0; i < dim; i++) {
                readWordWithEndian(&parameter.parameter_data_16_int[i], SIZE_16_BIT, file, endianFlag);
            }
            break;
        } case FORMAT_FLOAT: {
            parameter.parameter_data_float = (float*)malloc((dim+1)*sizeof(float));
            for(int i = 0; i < dim; i++) {
                readWordWithEndian(&parameter.parameter_data_float[i], SIZE_32_BIT, file, endianFlag);
            }
            break;
        }
    }

    //Read number description
    readWordWithEndian(&parameter.description_number, SIZE_8_BIT, file, endianFlag);

    //Read Description
    //Read Description
    if(parameter.description_number <= 0) {
        parameter.description = (char*)malloc(1*sizeof(char));
        parameter.description[0] = '\0';
    } else {
        parameter.description = (char*)malloc(parameter.description_number*sizeof(char));
        parameter.description[0] = '\0';
        for(int i = 0; i < parameter.description_number; i++) {
            readWordWithEndian(&parameter.description[i], SIZE_8_BIT, file, endianFlag);
            parameter.description[i+1] = '\0';
        }
    }

    return loop;
}

void Parameter_Parameter_C3D::CleanUp(Parameter_Parameter_C3D& parameter) {
    switch(parameter.byte_format) {
        case FORMAT_CHAR: {
            free(parameter.parameter_data_char);
            break;
        } case FORMAT_BYTE: {
            free(parameter.parameter_data_byte);
            break;
        } case FORMAT_INT_16: {
            free(parameter.parameter_data_16_int);
            break;
        } case FORMAT_FLOAT: {
            free(parameter.parameter_data_float);
            break;
        }
    }

    free(parameter.parameter_name);
    free(parameter.description);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~ Parameter_Group_C3D ~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~ */
bool Parameter_Group_C3D::ReadGroupBlock(FILE* file, Parameter_Group_C3D& group, const int endianFlag, const bool isGroup) {

    bool loop;

    if(isGroup == true) {
        group.parameter_size = 0; //Set parameter size for this group to 0
        group.parameter = (Parameter_Parameter_C3D*)malloc((127)*sizeof(Parameter_Parameter_C3D));

        readWordWithEndian(&group.character_number, SIZE_8_BIT, file, endianFlag); //Read character number
        if(group.character_number < 0) {
            group.character_number *= -1;
            group.locked = true;
        } else {
            group.locked = false;
        }

        readWordWithEndian(&group.group_ID , SIZE_8_BIT, file, endianFlag); //Read group ID
        if(group.group_ID > 128) {
            group.group_ID -= 256;
        }
        //Read Group Name
        group.group_name = (char*)malloc(group.character_number*sizeof(char));
        for(int i = 0; i < group.character_number; i++) {
            readWordWithEndian(&group.group_name[i], SIZE_8_BIT, file, endianFlag);
            group.group_name[i+1] = '\0';
        }

        //Read next group pointer
        group.next_group_parameter_start = 0;
        readWordWithEndian(&group.next_group_parameter_start, SIZE_16_BIT, file, endianFlag);

        //Read number description
        readWordWithEndian(&group.description_number, SIZE_8_BIT, file, endianFlag);

        //Read Description
        if(group.description_number <= 0) {
            group.description = (char*)malloc(1*sizeof(char));
            group.description[0] = '\0';
        } else {
            group.description = (char*)malloc(group.description_number*sizeof(char));
            for(int i = 0; i < group.description_number; i++) {
                readWordWithEndian(&group.description[i], SIZE_8_BIT, file, endianFlag);
                group.description[i+1] = '\0';
            }
        }

        //Check if this is the last group
        if(group.next_group_parameter_start == 0) {
            loop = false;
        } else {
            loop = true;
        }

    } else {
       loop = group.parameter->ReadParameterBlock(file, group.parameter[group.parameter_size], endianFlag);
       group.parameter_size++;
       //group.parameter = (Parameter_Parameter_C3D*)realloc(group.parameter, (group.parameter_size+1)*sizeof(Parameter_Parameter_C3D));
    }

    return loop;
}

void Parameter_Group_C3D::CleanUp(Parameter_Group_C3D* group) {
    for(int i = 0; i < group->parameter_size; i++)
        group->parameter->CleanUp(group->parameter[i]);

    free(group->parameter);
    free(group->group_name);
    free(group->description);
}

/* ~~~~~~~~~~~~~~~~~~~ */
/* ~~ Parameter_C3D ~~ */
/* ~~~~~~~~~~~~~~~~~~~ */
void Parameter_c3d::ReadHeaderParameterBlock(FILE* file, Parameter_c3d* parameter) {
    parameter->header.ReadHeaderBlock(file, parameter->header);
}

void Parameter_c3d::ReadGroupParameterBlock(FILE* file, Parameter_c3d* parameter, const int endianFlag) {
    parameter->group_size = 0;
    short int num_char;
    short int par_id;

    parameter->group = (Parameter_Group_C3D*)malloc((parameter->group_size+1)*sizeof(Parameter_Group_C3D));
    bool loop = true;
    while(loop) {
        readWordWithEndian(&num_char, SIZE_8_BIT, file, endianFlag); //Read number of character
        readWordWithEndian(&par_id, SIZE_8_BIT, file, endianFlag); //Read ID number
        if(par_id > 128) {
            par_id -= 256;
        }

        fseek(file, -2, SEEK_CUR); //Return back to char number (correct the reading procedure)

        //If is group
        if(par_id < 0) {
            loop = parameter->group[parameter->group_size].ReadGroupBlock(file, parameter->group[parameter->group_size], endianFlag, true);
            parameter->group_size++;
            parameter->group = (Parameter_Group_C3D*)realloc(parameter->group, (parameter->group_size+1)*sizeof(Parameter_Group_C3D));
        } else if(par_id > 0) {
            loop = parameter->group[par_id-1].ReadGroupBlock(file, parameter->group[par_id-1], endianFlag, false);
        } else if(num_char == 0 || par_id == 0) {
            loop = false;
        }
    }
    short int buf = 0;
    while(buf == 0) {
        readWordWithEndian(&buf, SIZE_8_BIT, file, endianFlag);
    }
    fseek(file, -1, SEEK_CUR);
}

void Parameter_c3d::print_parameter_to_file(Parameter_c3d parameter, const std::string fileName) {
    FILE* outputFile;

    outputFile = fopen(fileName.c_str(), "w");

    //Print Parameter Header
    fprintf(outputFile, "Parameter_Block_Start = %d\n", parameter.Header().ParameterBlock());
    fprintf(outputFile, "ADTech_ID_Number = %d\n", parameter.Header().NumberID());
    fprintf(outputFile, "Parameter_512_byte_Block = %d\n", parameter.Header().NumberOfParameterBlock());
    fprintf(outputFile, "Processor_Type = %d\n", parameter.Header().ProcessorType());
    fprintf(outputFile, "\n\n");

    for (int i = 0; i < parameter.GroupSize(); i++) {
        fprintf(outputFile, "Group_Name = %s\n", parameter.Group(i).Name().c_str());
        fprintf(outputFile, "Group_ID = %d\n", parameter.Group(i).ID());
        if(parameter.Group(i).IsLocked()) {
            fprintf(outputFile, "IsLocked = TRUE\n");
        } else {
            fprintf(outputFile, "IsLocked = FALSE\n");
        }
        fprintf(outputFile, "Description = %s\n\n", parameter.Group(i).Description().c_str()); //Group Description

        for(int j = 0; j < parameter.Group(i).ParameterSize(); j++) {
            fprintf(outputFile, "\t\tParameter_Name = %s\n", parameter.Group(i).Parameter(j).Name().c_str()); //Parameter Name
            fprintf(outputFile, "\t\tParameter_ID = %d\n", parameter.Group(i).Parameter(j).ID()); //Parameter ID

            //Check if Parameter is Lockes
            if(parameter.Group(i).Parameter(j).IsLocked()) {
                fprintf(outputFile, "\t\tIsLocked = TRUE\n");
            } else {
                fprintf(outputFile, "\t\tIsLocked = FALSE\n");
            }

            //Parameter Format
            switch(parameter.Group(i).Parameter(j).Format()) {
            case FORMAT_CHAR: {
                fprintf(outputFile, "\t\tByte_Format = CHAR\n");
                break;
            }
            case FORMAT_BYTE: {
                fprintf(outputFile, "\t\tByte_Format = BYTE\n");
                break;
            }
            case FORMAT_INT_16: {
                fprintf(outputFile, "\t\tByte_Format = INT_16_BIT\n");
                break;
            }
            case FORMAT_FLOAT: {
                fprintf(outputFile, "\t\tByte_Format = FLOAT (REAL)\n");
                break;
            }
            default:
                break;
            }

            //Number of Dimensions
            fprintf(outputFile, "\t\tNumber_Of_Dimensions = %d\n", parameter.Group(i).Parameter(j).DimensionsSize());

            //Dimensions Size
            int dim_size = parameter.Group(i).Parameter(j).Dimension(0);
            fprintf(outputFile, "\t\tDimension_Sizes = %d", parameter.Group(i).Parameter(j).Dimension(0));
            for(int dim = 1; dim <  parameter.Group(i).Parameter(j).DimensionsSize(); dim++) {
                fprintf(outputFile, " x %d", parameter.Group(i).Parameter(j).Dimension(dim));
                dim_size *= parameter.Group(i).Parameter(j).Dimension(dim);
            }
            fprintf(outputFile, "\n");

            //Parameter Data
            fprintf(outputFile, "\t\tParameter_Data: ");
            switch(parameter.Group(i).Parameter(j).Format()) {
            case FORMAT_CHAR: {
                fprintf(outputFile, "%s\n", parameter.Group(i).Parameter(j).ParameterChar().c_str());
                if(parameter.Group(i).Parameter(j).ParameterChar() == " ") {
                    fprintf(outputFile, "\n");
                }
                break;
            }
            case FORMAT_BYTE: {
                for(int dim = 0; dim < dim_size; dim++) {
                    if(dim == 0) {
                        fprintf(outputFile,"%5d\n", parameter.Group(i).Parameter(j).ParameterByte(dim));
                    } else {
                       fprintf(outputFile, "\t\t                %5d\n", parameter.Group(i).Parameter(j).ParameterByte(dim));
                    }
                }
                break;
            }
            case FORMAT_INT_16: {
                for(int dim = 0; dim < dim_size; dim++) {
                    if(dim == 0) {
                        fprintf(outputFile,"%5d\n", parameter.Group(i).Parameter(j).ParameterInt16(dim));
                    } else {
                       fprintf(outputFile, "\t\t                %5d\n", parameter.Group(i).Parameter(j).ParameterInt16(dim));
                    }
                }
                break;
            }
            case FORMAT_FLOAT: {
                for(int dim = 0; dim < dim_size; dim++) {
                    if(dim == 0) {
                        fprintf(outputFile,"%8.4f\n", parameter.Group(i).Parameter(j).ParameterFloat(dim));
                    } else {
                       fprintf(outputFile, "\t\t                %8.4f\n", parameter.Group(i).Parameter(j).ParameterFloat(dim));
                    }
                }
                break;
            }
            default:
                fprintf(outputFile, "\n");
                break;
            }

            fprintf(outputFile, "\t\tDescription = %s\n\n", parameter.Group(i).Parameter(j).Description().c_str()); //Parameter Description

            //fprintf(outputFile, "\n\n");
        }

        //fprintf(outputFile, "\n\n");
    }

    fclose(outputFile);
}

void Parameter_c3d::CleanUp(Parameter_c3d* parameter) {
    for(int i = 0; i < parameter->group_size; i++)
        parameter->group[0].CleanUp(&parameter->group[i]);
    free(parameter->group);
}

/*******************************/
/* PARAMETER STRUCTURE CLASSES */
/*******************************/

/***************/
/*   1.TRIAL   */
/***************/

void Trial::SetTrial(Trial* trial, Parameter_c3d parameter) {
    //Set Default Values
    trial->Actual_Start_Field = 0;
    trial->Actual_Start_Field = 0;
    trial->Video_Rate_Divider = 0;
    trial->Camera_Rate = 0.0;

    trial->Date[0] = 0;
    trial->Date[1] = 0;
    trial->Date[2] = 0;

    trial->Time[0] = 0;
    trial->Time[1] = 0;
    trial->Time[2] = 0;

    for(int i = 0; i < parameter.GroupSize(); i++) {
        if(parameter.Group(i).Name() == "TRIAL") {
            for(int j = 0; j < parameter.Group(i).ParameterSize(); j++) {

                if(parameter.Group(i).Parameter(j).Name() == "ACTUAL_START_FIELD") {
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_INT_16)
                            trial->Actual_Start_Field = parameter.Group(i).Parameter(j).ParameterInt16(0);
                } else if(parameter.Group(i).Parameter(j).Name() == "ACTUAL_END_FIELD"){
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_INT_16)
                            trial->Actual_End_Field = parameter.Group(i).Parameter(j).ParameterInt16(0);
                } else if(parameter.Group(i).Parameter(j).Name() == "VIDEO_RATE_DIVIDER"){
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_INT_16)
                            trial->Video_Rate_Divider = parameter.Group(i).Parameter(j).ParameterInt16(0);
                } else if(parameter.Group(i).Parameter(j).Name() == "CAMERA_RATE"){
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_FLOAT)
                            trial->Camera_Rate = parameter.Group(i).Parameter(j).ParameterFloat(0);
                } else if(parameter.Group(i).Parameter(j).Name() == "DATE"){
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 3)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_INT_16) {
                            trial->Date[0] = parameter.Group(i).Parameter(j).ParameterFloat(0);
                            trial->Date[1] = parameter.Group(i).Parameter(j).ParameterFloat(1);
                            trial->Date[2] = parameter.Group(i).Parameter(j).ParameterFloat(2);
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "TIME"){
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 3)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_INT_16) {
                            trial->Time[0] = parameter.Group(i).Parameter(j).ParameterFloat(0);
                            trial->Time[1] = parameter.Group(i).Parameter(j).ParameterFloat(1);
                            trial->Time[2] = parameter.Group(i).Parameter(j).ParameterFloat(2);
                        }
                }

            }
        }
    }
}

/******************/
/*   2.SUBJECTS   */
/******************/

void Subjects::SetSubjects(Subjects* subjects, Parameter_c3d parameter) {
    //integers
    subjects->is_static = 0;
    subjects->namesSize = 0;
    subjects->model_paramsSize = 0;
    subjects->uses_prefixes = 0;
    subjects->label_prefixesSize = 0;
    subjects->used = 0;
    subjects->marker_setsSize = 0;
    subjects->display_setsSize = 0;
    subjects->modelsSize = 0;

    //strings
    subjects->names = new std::string[1];
    subjects->names[0] = "";

    subjects->model_params = new std::string[1];
    subjects->model_params[0] = "";

    subjects->label_prefixes = new std::string[1];
    subjects->label_prefixes[0] = "";

    subjects->marker_sets = new std::string[1];
    subjects->marker_sets[0] = "";

    subjects->display_sets = new std::string[1];
    subjects->display_sets[0] = "";

    subjects->models = new std::string[1];
    subjects->models[0] = "";

    for(int i = 0; i < parameter.GroupSize(); i++) {
        if(parameter.Group(i).Name() == "SUBJECTS") {
            for(int j = 0; j < parameter.Group(i).ParameterSize(); j++) {

                if(parameter.Group(i).Parameter(j).Name() == "IS_STATIC") {
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_INT_16)
                            subjects->is_static = parameter.Group(i).Parameter(j).ParameterInt16(0);
                } else if(parameter.Group(i).Parameter(j).Name() == "NAMES"){
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            subjects->namesSize = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            subjects->names = new std::string[subjects->namesSize];

                            for(int k = 0; k < namesSize; k++)
                                subjects->names[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "MODEL_PARAMS"){
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            subjects->model_paramsSize = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            subjects->model_params = new std::string[subjects->model_paramsSize];

                            for(int k = 0; k < subjects->model_paramsSize; k++)
                                subjects->model_params[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "USES_PREFIXES") {
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_INT_16)
                            subjects->uses_prefixes = parameter.Group(i).Parameter(j).ParameterInt16(0);
                } else if(parameter.Group(i).Parameter(j).Name() == "LABEL_PREFIXES"){
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            subjects->label_prefixesSize = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            subjects->label_prefixes = new std::string[subjects->label_prefixesSize];

                            for(int k = 0; k < subjects->label_prefixesSize; k++)
                                subjects->label_prefixes[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "USED") {
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_INT_16)
                            subjects->used = parameter.Group(i).Parameter(j).ParameterInt16(0);
                } else if(parameter.Group(i).Parameter(j).Name() == "MARKER_SETS"){
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            subjects->marker_setsSize = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            subjects->marker_sets = new std::string[subjects->marker_setsSize];

                            for(int k = 0; k < subjects->marker_setsSize; k++)
                                subjects->marker_sets[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "DISPLAY_SETS"){
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            subjects->display_setsSize = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            subjects->display_sets = new std::string[subjects->display_setsSize];

                            for(int k = 0; k < subjects->marker_setsSize; k++)
                                subjects->display_sets[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "MODELS"){
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            subjects->modelsSize = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            subjects->models = new std::string[subjects->modelsSize];

                            for(int k = 0; k < subjects->modelsSize; k++)
                                subjects->models[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                }
            }
        }
    }


}

/***************/
/*   3.POINT   */
/***************/

void Point::SetPoint(Point* point, Parameter_c3d parameter) {
    //Used
    point->used = 0;
    //Scale
    point->scale = 0.0;
    //Rate
    point->rate = 0.0;
    //Data Start
    point->data_start = 0;
    //Frames
    point->frames = 0;

    //Labels
    point->labelsSize = 0;
    point->labels = new std::string[1];
    point->labels[0] = "";

    //Description
    point->descriptionsSize = 0;
    point->descriptions = new std::string[1];
    point->descriptions[0] = "";

    //Units
    point->units = new std::string[1];
    point->units[0] = "";

    //Initial Command
    point->initial_command = new std::string[1];
    point->initial_command[0] = "";

    //X Screen
    point->x_screen = new std::string[1];
    point->x_screen[0] = "";

    //Y Screen
    point->y_screen = new std::string[1];
    point->y_screen[0] = "";

    //Movie_Delay
    point->movie_delay = 0.0;

    //Labels2
    point->labels2Size = 0;
    point->labels2 = new std::string[1];
    point->labels2[0] = "";

    //Description2
    point->descriptions2Size = 0;
    point->descriptions2 = new std::string[1];
    point->descriptions2[0] = "";

    //Type Groups
    point->type_groupsSize = 0;
    point->type_groups = new std::string[1];
    point->type_groups[0] = "";

    //Angles
    point->anglesSize = 0;
    point->angles = new std::string[1];
    point->angles[0] = "";

    //Angles Units
    point->angle_units = new std::string[1];
    point->angle_units[0] = "";

    //Scalars
    point->scalarsSize = 0;
    point->scalars = new std::string[1];
    point->scalars[0] = "";

    //Scalar Units
    point->scalar_units = new std::string[1];
    point->scalar_units[0] = "";

    //Powers
    point->powersSize = 0;
    point->powers = new std::string[1];
    point->powers[0] = "";

    //Power Units
    point->power_units = new std::string[1];
    point->power_units[0] = "";

    //Forces
    point->forcesSize = 0;
    point->forces = new std::string[1];
    point->forces[0] = "";

    //Force Units
    point->force_units = new std::string[1];
    point->force_units[0] = "";

    //Moments
    point->momentsSize = 0;
    point->moments = new std::string[1];
    point->moments[0] = "";

    //Moment Units
    point->moment_units = new std::string[1];
    point->moment_units[0] = "";

    //Reactions
    point->reactionsSize = 0;
    point->reactions = new std::string[1];
    point->reactions[0] = "";/**/

    for(int i = 0; i < parameter.GroupSize(); i++) {
        if(parameter.Group(i).Name() == "POINT") {
            for(int j = 0; j < parameter.Group(i).ParameterSize(); j++) {

                if(parameter.Group(i).Parameter(j).Name() == "USED") {
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_INT_16)
                            point->used = parameter.Group(i).Parameter(j).ParameterInt16(0);
                } else if(parameter.Group(i).Parameter(j).Name() == "SCALE") {
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_FLOAT)
                            point->scale = parameter.Group(i).Parameter(j).ParameterFloat(0);
                } else if(parameter.Group(i).Parameter(j).Name() == "RATE") {
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_FLOAT)
                            point->rate = parameter.Group(i).Parameter(j).ParameterFloat(0);
                } else if(parameter.Group(i).Parameter(j).Name() == "DATA_START") {
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_INT_16)
                            point->data_start = parameter.Group(i).Parameter(j).ParameterInt16(0);
                } else if(parameter.Group(i).Parameter(j).Name() == "FRAMES") {
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_INT_16)
                            point->frames = parameter.Group(i).Parameter(j).ParameterInt16(0);
                } else if(parameter.Group(i).Parameter(j).Name() == "LABELS"){
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            point->labelsSize = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            point->labels = new std::string[point->labelsSize];

                            for(int k = 0; k < point->labelsSize; k++)
                                point->labels[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "DESCRIPTIONS"){
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            point->descriptionsSize = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            point->descriptions = new std::string[point->descriptionsSize];

                            for(int k = 0; k < point->descriptionsSize; k++)
                                point->descriptions[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "UNITS"){
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {
                            point->units[0] = parameter.Group(i).Parameter(j).ParameterChar();
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "INITIAL_COMMAND"){
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {
                            point->initial_command[0] = parameter.Group(i).Parameter(j).ParameterChar();
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "X_SCREEN"){
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {
                            point->x_screen[0] = parameter.Group(i).Parameter(j).ParameterChar();
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "Y_SCREEN"){
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {
                            point->y_screen[0] = parameter.Group(i).Parameter(j).ParameterChar();
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "MOVIE_DELAY") {
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_FLOAT)
                            point->movie_delay = parameter.Group(i).Parameter(j).ParameterFloat(0);
                } else if(parameter.Group(i).Parameter(j).Name() == "LABELS2"){
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            point->labels2Size = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            point->labels2 = new std::string[point->labels2Size];

                            for(int k = 0; k < point->labels2Size; k++)
                                point->labels2[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "DESCRIPTIONS2"){
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            point->descriptions2Size = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            point->descriptions2 = new std::string[point->descriptions2Size];

                            for(int k = 0; k < point->descriptions2Size; k++)
                                point->descriptions2[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "TYPE_GROUPS"){
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            point->type_groupsSize = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            point->type_groups = new std::string[point->type_groupsSize];

                            for(int k = 0; k < point->type_groupsSize; k++)
                                point->type_groups[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "ANGLES"){
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            point->anglesSize = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            point->angles = new std::string[point->anglesSize];

                            for(int k = 0; k < point->anglesSize; k++)
                                point->angles[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "ANGLE_UNITS"){
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {
                            point->angle_units[0] = parameter.Group(i).Parameter(j).ParameterChar();
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "SCALARS"){
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            point->scalarsSize = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            point->scalars = new std::string[point->scalarsSize];

                            for(int k = 0; k < point->scalarsSize; k++)
                                point->scalars[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "SCALAR_UNITS"){
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {
                            point->scalar_units[0] = parameter.Group(i).Parameter(j).ParameterChar();
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "POWERS"){
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            point->powersSize = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            point->powers = new std::string[point->powersSize];

                            for(int k = 0; k < point->powersSize; k++)
                                point->powers[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "POWER_UNITS"){
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {
                            point->power_units[0] = parameter.Group(i).Parameter(j).ParameterChar();
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "FORCES"){
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            point->forcesSize = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            point->forces = new std::string[point->forcesSize];

                            for(int k = 0; k < point->forcesSize; k++)
                                point->forces[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "FORCE_UNITS"){
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {
                            point->force_units[0] = parameter.Group(i).Parameter(j).ParameterChar();
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "MOMENTS"){
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            point->momentsSize = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            point->moments = new std::string[point->momentsSize];

                            for(int k = 0; k < point->momentsSize; k++)
                                point->moments[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "MOMENT_UNITS"){
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {
                            point->moment_units[0] = parameter.Group(i).Parameter(j).ParameterChar();
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "REACTIONS"){
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            point->reactionsSize = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            point->reactions = new std::string[point->reactionsSize];

                            for(int k = 0; k < point->reactionsSize; k++)
                                point->reactions[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                }
            }
        }
    }


}

float Point::MultiplierForMeters(Point point) {

    if(point.Units() == "mm" || point.Units() == "MM" || point.Units() == "Mm" || point.Units() == "mM") {
        return 0.001;
    } else if(point.Units() == "cm" || point.Units() == "CM" || point.Units() == "Cm" || point.Units() == "cM") {
        return 0.01;
    } else if (point.Units() == "dm" || point.Units() == "Dm" || point.Units() == "DM" || point.Units() == "dM") {
        return 0.1;
    } else if (point.Units() == "m" || point.Units() == "M") {
        return 1.0;
    } else if (point.Units() == "km" || point.Units() == "Km" || point.Units() == "KM" || point.Units() == "kM") {
        return 1000.0;
    }

    return 1.0;

}

GeoPoint Point::CheckScreens(Point point, const float X, const float Y, const float Z, const float DR) {

    GeoPoint geo;
    geo.x = X;
    geo.y = Y;
    geo.z = Z;
    geo.dr = DR;

    float sign_x = 1.0;
    float sign_y = 1.0;

    if(point.X_Screen() != "" && point.Y_Screen() != "") {
        if(point.X_Screen() == "-X" || point.X_Screen() == "-Y" || point.X_Screen() == "-Z") {
            sign_x = -1.0;
        }

        if(point.Y_Screen() == "-X" || point.Y_Screen() == "-Y" || point.Y_Screen() == "-Z") {
            sign_y = -1.0;
        }

        if(point.X_Screen() == "-X" || point.X_Screen() == "+X") {
            geo.x = sign_x * X;

            if(point.Y_Screen() == "-Y" || point.Y_Screen() == "+Y") {
                geo.y = sign_y * Y;
                geo.z = -Z;
            } else if(point.Y_Screen() == "-Z" || point.Y_Screen() == "+Z") {
                geo.y = sign_y * Z;
                geo.z = -Y;
            }
        } else if(point.X_Screen() == "-Y" || point.X_Screen() == "+Y") {
            geo.x = sign_x * Y;

            if(point.Y_Screen() == "-X" || point.Y_Screen() == "+X") {
                geo.y = sign_y * X;
                geo.z = -Z;
            } else if(point.Y_Screen() == "-Z" || point.Y_Screen() == "+Z") {
                geo.y = sign_y * Z;
                geo.z = -X;
            }
        } else if(point.X_Screen() == "-Z" || point.X_Screen() == "+Z") {
            geo.x = sign_x * Z;

            if(point.Y_Screen() == "-X" || point.Y_Screen() == "+X") {
                geo.y = sign_y * X;
                geo.z = -Y;
            } else if(point.Y_Screen() == "-Y" || point.Y_Screen() == "+Y") {
                geo.y = sign_y * Y;
                geo.z = -X;
            }
        }

        return geo;
    } else {
        return geo;
    }

}

/**********************/
/*   8.MANUFACTURER   */
/**********************/

void Manufacturer::SetManufacturer(Manufacturer* manufacturer, Parameter_c3d parameter) {
    manufacturer->company = new std::string[1];
    manufacturer->software = new std::string[1];
    manufacturer->version = new std::string[1];

    manufacturer->company[0] = "";
    manufacturer->software[0] = "";
    manufacturer->version[0] = "";

    for(int i = 0; i < parameter.GroupSize(); i++) {
        if(parameter.Group(i).Name() == "MANUFACTURER") {
            for(int j = 0; j < parameter.Group(i).ParameterSize(); j++) {

                if(parameter.Group(i).Parameter(j).Name() == "COMPANY") {
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR)
                            manufacturer->company[0] = parameter.Group(i).Parameter(j).ParameterChar();
                } else if(parameter.Group(i).Parameter(j).Name() == "SOFTWARE"){
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR)
                            manufacturer->software[0] = parameter.Group(i).Parameter(j).ParameterChar();
                } else if(parameter.Group(i).Parameter(j).Name() == "VERSION" || parameter.Group(i).Parameter(j).Name() == "VERSION_LABEL"){
                    if(parameter.Group(i).Parameter(j).DimensionsSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR)
                            manufacturer->version[0] = parameter.Group(i).Parameter(j).ParameterChar();
                }
            }

        }
    }
}

/****************************************/
/*               Data_c3d               */
/****************************************/

void Relocation_C3D::SetRelocation(Relocation_C3D* rel, Frames_C3D frame1, Frames_C3D frame2, const int pointSize) {
    rel->dx = (float*)malloc(pointSize*sizeof(float));
    rel->dy = (float*)malloc(pointSize*sizeof(float));
    rel->dz = (float*)malloc(pointSize*sizeof(float));
    rel->dr = (float*)malloc(pointSize*sizeof(float));

    for(int i = 0; i < pointSize; i++) {
        rel->dx[i] = frame2.Point(i).X() - frame1.Point(i).X();
        rel->dy[i] = frame2.Point(i).Y() - frame1.Point(i).Y();
        rel->dz[i] = frame2.Point(i).Z() - frame1.Point(i).Z();

        rel->dr[i] = sqrt(rel->dx[i]*rel->dx[i] + rel->dy[i]*rel->dy[i] + rel->dz[i]*rel->dz[i]);
        //std::cout << rel->dr[i] << std::endl;
    }
}

void Frames_C3D::ReadFrame(Frames_C3D* frame, FILE* file, const int pointSize, const int analogSize, const float pointScale, const int endianFlag) {
    frame->points = (Points_C3D*)malloc(pointSize*sizeof(Points_C3D));
    frame->analog = (Analog_C3D*)malloc(analogSize*sizeof(Analog_C3D));

    int index;
    for(int i = 0; i < pointSize; i++) {
        index = i;
        if(pointScale < 0) {
            float buf_x = 0.0;
            float buf_y = 0.0;
            float buf_z = 0.0;
            float buf_word_4_f = 0.0;
            short int buf_cam = 0.0;
            short int buf_res = 0.0;

            readWordWithEndian(&buf_x, SIZE_32_BIT, file, endianFlag);
            readWordWithEndian(&buf_y, SIZE_32_BIT, file, endianFlag);
            readWordWithEndian(&buf_z, SIZE_32_BIT, file, endianFlag);
            readWordWithEndian(&buf_word_4_f, SIZE_32_BIT, file, endianFlag);

            buf_cam = returnByte((short int)buf_word_4_f, 1);
            buf_res = returnByte((short int)buf_word_4_f, 2);

            frame->points[0].SetPoint(&frame->points[index], buf_x, buf_y, buf_z, (float) buf_cam, buf_res*(-pointScale));
        } else {
            short int buf_x = 0;
            short int buf_y = 0;
            short int buf_z = 0;
            short int buf_cam = 0;
            short int buf_res = 0;

            readWordWithEndian(&buf_x, SIZE_16_BIT, file, endianFlag);
            readWordWithEndian(&buf_y, SIZE_16_BIT, file, endianFlag);
            readWordWithEndian(&buf_z, SIZE_16_BIT, file, endianFlag);
            readWordWithEndian(&buf_cam, SIZE_8_BIT, file, endianFlag);
            readWordWithEndian(&buf_res, SIZE_8_BIT, file, endianFlag);

            frame->points[0].SetPoint(&frame->points[i], (float) buf_x*pointScale, (float) buf_y*pointScale, (float) buf_z*pointScale, (float) buf_cam, (float) buf_res*pointScale);
        }
        i = index;
    }

    for(int i = 0; i < analogSize; i++) {
        if(pointScale < 0) {
            float buf_analog = 0.0;

            readWordWithEndian(&buf_analog, SIZE_32_BIT, file, endianFlag);

            frame->analog[0].SetAnalog(&frame->analog[i], buf_analog);
        } else {
            short int buf_analog = 0.0;

            readWordWithEndian(&buf_analog, SIZE_16_BIT, file, endianFlag);

            frame->analog[0].SetAnalog(&frame->analog[i], buf_analog/pointScale);
        }
    }
}

void Frames_C3D::CleanUp(Frames_C3D* frame) {
    free(frame->points);
    free(frame->analog);
}

void Data_c3d::ReadData(Data_c3d* data, FILE* file, const int frameSize, const int pointSize, const int analogSize, const float pointScale, const int endianFlag) {
    data->frames = (Frames_C3D*)malloc(frameSize*sizeof(Frames_C3D));
    data->relocation = (Relocation_C3D*)malloc(1*sizeof(Relocation_C3D));

    for(int i = 0; i < frameSize; i++) {
        data->frames[0].ReadFrame(&data->frames[i], file, pointSize, analogSize, pointScale, endianFlag);
    }

    data->relocation[0].SetRelocation(&data->relocation[0], frames[0], frames[frameSize-1], pointSize);

}

void Data_c3d::print_point_data_to_file(Data_c3d data, const std::string fileName, const int frameSize, const int pointSize) {
    FILE* outputFile;

    outputFile = fopen(fileName.c_str(), "w");

    fprintf(outputFile, "FrameNumber");
    for(int i = 0; i < pointSize; i++) {
        fprintf(outputFile, ";X_%d;Y_%d;Z_%d;Camera_%d;Residual_%d", i+1, i+1, i+1, i+1, i+1);
    }
    fprintf(outputFile, "\n");
    for(int i = 0; i < frameSize; i++) {
        fprintf(outputFile, "%d", i+1);
        for(int j = 0; j < pointSize; j++) {
            fprintf(outputFile, ";%f;%f;%f;%f;%f", data.Frame(i).Point(j).X(), data.Frame(i).Point(j).Y(), data.Frame(i).Point(j).Z(), data.Frame(i).Point(j).Camera(), data.Frame(i).Point(j).Residual());
        }
        fprintf(outputFile, "\n");
    }

    fclose(outputFile);
}

void Data_c3d::print_analog_data_to_file(Data_c3d data, const std::string fileName, const int frameSize, const int analogSize) {
    FILE* outputFile;

    outputFile = fopen(fileName.c_str(), "w");

    fprintf(outputFile, "FrameNumber");
    for(int i = 0; i < analogSize; i++) {
        fprintf(outputFile, ";Analog_%d", i+1);
    }
    fprintf(outputFile, "\n");
    for(int i = 0; i < frameSize; i++) {
        fprintf(outputFile, "%d", i+1);
        for(int j = 0; j < analogSize; j++) {
            fprintf(outputFile, ";%f", data.Frame(i).Analog(j).AnalogPoint());
        }
        fprintf(outputFile, "\n");
    }

    fclose(outputFile);
}

void Data_c3d::CleanUp(Data_c3d* data, const int frameSize) {
    for(int i = 0; i < frameSize; i++)
        data->frames[0].CleanUp(&data->frames[i]);
    free(data->frames);
}

/****************************************/
/*               Read_c3d               */
/****************************************/

void Read_C3D::Import(std::string fileName, QWidget* widget, Read_C3D* c3d_f)
{
    FILE* openFile = fopen(fileName.c_str(), "rb"); // Open C3D File
    // Error Checking
    if(openFile == NULL) {
        QMessageBox::warning(widget, "Error", "File cannot open!");
        fclose(openFile);
        return ;
    }

    c3d_f->isOpen = true;

    //Read Header Block
    c3d_f->headerBlock.read_header_block(openFile, &c3d_f->headerBlock);
    //Error Checking
    if(c3d_f->headerBlock.NumberID() != 80 && c3d_f->headerBlock.ParameterBlock() > 1) {
        QMessageBox::warning(widget, "Error", "This file isn't ADTech format.");
        fclose(openFile);
        return ;
    }

    //c3d_f->parameterBlock = (Parameter_c3d*)malloc(1*sizeof(Parameter_c3d));
    //Read Parameter Header Block
    c3d_f->parameterBlock.ReadHeaderParameterBlock(openFile, &c3d_f->parameterBlock);
    //Set Endians
    if(sys_endian::little_endian())
            system_endian = LITTLE_ENDIAN_PROC;
    file_endian = LITTLE_ENDIAN_PROC;
    if(parameterBlock.Header().ProcessorType() == PROCESSOR_MIPS) {
        file_endian = BIG_ENDIAN_PROC;
    }

    //Check Endians
    int endian_flag = SAME_ENDIAN;
    if(system_endian != file_endian) {
        c3d_f->headerBlock.swapHeader(&c3d_f->headerBlock);
        endian_flag = DIFF_ENDIAN;
    }

    c3d_f->parameterBlock.ReadGroupParameterBlock(openFile, &c3d_f->parameterBlock, endian_flag);

    c3d_f->trial.SetTrial(&c3d_f->trial, c3d_f->parameterBlock);
    c3d_f->subjects.SetSubjects(&c3d_f->subjects, c3d_f->parameterBlock);
    c3d_f->point.SetPoint(&c3d_f->point, c3d_f->parameterBlock);

    c3d_f->manufacturer.SetManufacturer(&c3d_f->manufacturer, c3d_f->parameterBlock);


    int pointSize = c3d_f->Header().NumberOfPoints();
    float pointScale = c3d_f->Header().ScaleFactor();

    int analogSize = c3d_f->Header().NumberOfAnalog();

    int frameSize = c3d_f->Header().LastFrame() - c3d_f->Header().FirstFrame() + 1;

    c3d_f->dataBlock.ReadData(&c3d_f->dataBlock, openFile, frameSize, pointSize, analogSize, pointScale, endian_flag);

    fclose(openFile);
}

void Read_C3D::CleanUp(Read_C3D* c3d_f) {
    int frameSize = c3d_f->Header().LastFrame() - c3d_f->Header().FirstFrame() + 1;
    c3d_f->dataBlock.CleanUp(&c3d_f->dataBlock, frameSize);

    c3d_f->parameterBlock.CleanUp(&c3d_f->parameterBlock);

    c3d_f->isOpen = false;
}


