/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
 */

#include "interrupts.hpp"

int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/
    int current_time = 0;
    int context_save_time = 30;// will range from 10,20,30 for further testing
    int ISR_activity_time = 200;

    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/
        if (activity == "CPU"){
            execution += std::to_string(current_time) + ", " + std::to_string(duration_intr) + ", CPU execution.\n";
            current_time += duration_intr;
        }
        else if (activity =="SYSCALL"){
            auto[execution2,current_time2] = intr_boilerplate(current_time,duration_intr,context_save_time,vectors);
            execution += execution2;
            current_time = current_time2;
        
            execution += std::to_string(current_time) + ", " + std::to_string(ISR_activity_time) + ", SYSCALL: run the ISR (device driver)\n";
            current_time += ISR_activity_time;
            execution += std::to_string(current_time) + ", " + std::to_string(ISR_activity_time) + ", transfer data from device to memory\n";
            current_time += ISR_activity_time;
            
            int error_check_timer = delays.at(duration_intr) - (2*ISR_activity_time);
            

            execution += std::to_string(current_time) + ", " + std::to_string(error_check_timer) + ", check for errors\n";
            current_time += error_check_timer;
        }
        else if (activity == "END_IO"){
            auto[execution2,current_time2] = intr_boilerplate(current_time,duration_intr,context_save_time,vectors);
            execution += execution2;
            current_time = current_time2;

            int error_device_status = delays.at(duration_intr)- (ISR_activity_time);
            
            execution += std::to_string(current_time) + ", " + std::to_string(40) + ", ENDIO: run the ISR (device driver)\n";
            current_time += 40;
            execution += std::to_string(current_time) + ", " + std::to_string(error_device_status) + ", check device status\n";
            current_time += error_device_status;

        }

        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}
