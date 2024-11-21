/**
 * @file Common.hpp
 * @author Andrew Kerr
 * @brief A common file filled with common things needed by multiple hpp files.
 * @version 0.1
 * @date 2022-08-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <iostream>

#ifndef __AK_COMMON_HPP__
#define __AK_COMMON_HPP__

#define FATAL_MSG(msg) std::cerr << "[" << __FILE__ << ":" << __LINE__ << "|" << __FUNCTION__ << "] Fatal:" << std::endl << msg << std::endl << std::endl

#ifdef _DEBUG
#define ERROR_MSG(msg) std::cerr << "[" << __FILE__ << ":" << __LINE__ << "|" << __FUNCTION__ << "] Error:" << std::endl << msg << std::endl << std::endl
#define DEBUG_MSG(msg) std::cerr << "[" << __FILE__ << ":" << __LINE__ << "|" << __FUNCTION__ << "] Debug:" << std::endl << msg << std::endl << std::endl
#define VEBROSE_MSG(msg) std::cerr << "[" << __FILE__ << ":" << __LINE__ << "|" << __FUNCTION__ << "] Verbose:" << std::endl << msg << std::endl << std::endl
#define LOG_MSG(msg) std::cerr << "[" << __FILE__ << ":" << __LINE__ << "|" << __FUNCTION__ << "] Log:" << std::endl << msg << std::endl << std::endl
#else
#define ERROR_MSG(msg)
#define DEBUG_MSG(msg) 
#define VEBROSE_MSG(msg)
#define LOG_MSG(msg)
#endif

#endif//__AK_COMMON_HPP__