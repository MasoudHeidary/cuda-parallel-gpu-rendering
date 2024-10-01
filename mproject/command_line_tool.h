#pragma once

#define STRINGIFY(x) #x
#define TO_STRING(x) STRINGIFY(x)

#include <iostream>
#include <string>
#include <cstdlib>
#include <thread>

#include "multi_thread_tool.h"

namespace cArg {
    struct CommandLineArgs {
        std::string scene_file = "";
        std::string obj_file = "";
        int num_threads = 0;
        int width = 0;
        int height = 0;
        bool shadow = false;
    };

    std::string __str__(cArg::CommandLineArgs args) {
        return \
            "scene file: " + args.scene_file + "\n" + \
            "obj file: " + args.obj_file + "\n" + \
            "num threads: " + std::to_string(args.num_threads) + "\n" + \
            "width: " + std::to_string(args.width) + "\n" +\
            "height: " + std::to_string(args.height) + "\n" +\
            "shadow: " + (args.shadow ? "true" : "false") + "\n" \
            ;
    }

    enum ErrorCode {
        SUCCESS = 0,
        INCOMPLETE_ARGUMENT,
        NO_SCENE_FILE_DEFINED, 
        INVALID_THREAD_NUM,
        INVALID_WIDTH,
        INVALID_HEIGHT,
        UNKNOWN_ERROR,
    };

    std::string get_error_description(ErrorCode error_code) {
        switch (error_code) {
        case ErrorCode::INCOMPLETE_ARGUMENT:
            return TO_STRING(ErrorCode::INCOMPLETE_ARGUMENT) + std::string(", one of flags provided has no valid value");
        case ErrorCode::NO_SCENE_FILE_DEFINED:
            return TO_STRING(ErrorCode::NO_SCENE_FILE_DEFINED);
        case ErrorCode::INVALID_THREAD_NUM:
            return TO_STRING(ErrorCode::INVALID_THREAD_NUM);
        case ErrorCode::INVALID_WIDTH:
            return TO_STRING(ErrorCode::INVALID_WIDTH);
        case ErrorCode::INVALID_HEIGHT:
            return TO_STRING(ErrorCode::INVALID_HEIGHT);
        
        case ErrorCode::UNKNOWN_ERROR:
        default:
            return TO_STRING(ErrorCode::UNKNOWN_ERROR);
        }
    }


    // default values
    int default_width = 1000;
    int default_height = 1000;
    int default_num_thread(void) {
        int device_num_thread = std::thread::hardware_concurrency();
        if (device_num_thread == 0)
            return 4;
        return device_num_thread;
    }

    ErrorCode validate_arguments(CommandLineArgs args) {
        if (args.scene_file == "")
            return ErrorCode::NO_SCENE_FILE_DEFINED;
        if (args.num_threads <= 0)
            return ErrorCode::INVALID_THREAD_NUM;
        if (args.height <= 0)
            return ErrorCode::INVALID_HEIGHT;
        if (args.width <= 0)
            return ErrorCode::INVALID_WIDTH;
        return ErrorCode::SUCCESS;
    }

    ErrorCode parse_arguments(CommandLineArgs* args, int argc, char* argv[]) {
        args->num_threads = default_num_thread();
        args->width = default_width;
        args->height = default_height;
        
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];

            if (arg == "--scene" && i + 1 < argc) {
                args->scene_file = argv[++i];
            }
            else if (arg == "--obj" && i + 1 < argc) {
                args->obj_file = argv[++i];
            }
            else if (arg == "--threads" && i + 1 < argc) {
                args->num_threads = std::atoi(argv[++i]);
            }
            else if (arg == "--width" && ((i + 1) < argc)) {
                args->width = std::atoi(argv[++i]);
            }
            else if (arg == "--height" && ((i + 1) < argc)) {
                args->height == std::atoi(argv[++i]);
            }
            else if (arg == "--shadow") {
                args->shadow = true;
            }
            else {
                /*std::cerr << "Unknown or incomplete argument: " << arg << std::endl;
                exit(1);*/
                return ErrorCode::INCOMPLETE_ARGUMENT;
            }
        }

        return validate_arguments(*args);
    }

}
