#ifndef FILEIO_HELPER_H
#define	FILEIO_HELPER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace util
{

	template<typename T>
	void save_data(const T* data, size_t n, const char* filename, bool quiet = false)
	{
		if (!quiet)
			std::cout << __FUNCTION__ << " : " << filename << std::endl;
			
		std::ofstream os(filename, std::ios::binary);
		if(os.is_open())
		{
			os.write(reinterpret_cast<const char*>(&data[0]), sizeof(T) * n);
			os.close();
		}
		else
			std::cout << __FUNCTION__ << " : Error. Failed to open file " << filename << std::endl;
	}

	template<typename T>
	void save_data(std::vector<T> data, const char* filename, bool quiet = false)
	{
		save_data(&data[0], data.size(), filename, quiet);
	}
	
	static size_t get_filesize_bytes(const char* filename)
	{
		std::ifstream is(filename, std::ios::binary | std::ios::ate);
		if (is.is_open())
		{
			size_t size = (size_t) is.tellg();
			is.seekg(0, std::ios::beg);
			is.close();
			return size;
		}
		else
			std::cout << __FUNCTION__ << " : Error. Failed to open file " << filename << std::endl;
			
		return 0;
	}
	
	template <typename T>
	void load_data(T* data, size_t n, const char* filename)
	{
		std::ifstream is(filename, std::ios::binary);
		if (is.is_open())
		{
			is.read((char*) data, sizeof(T) * n);
			is.close();
		}
		else
			std::cout << __FUNCTION__ << " : Error. Failed to open file " << filename << std::endl;
	}
	
	template <typename T>
	std::vector<T> load_file(const char* filename, bool quiet = false)
	{
		if (!quiet)
			std::cout << __FUNCTION__ << " : " << filename << std::endl;
		
		// Load entire file
		std::vector<T> data(get_filesize_bytes(filename) / sizeof(T));
		load_data(&data[0], data.size(), filename);
		return data;
	}
	
	static void create_empty_file(const char* filename, bool quiet = false)
	{
		if (!quiet)
			std::cout << __FUNCTION__ << " : " << filename << std::endl;
			
		std::ofstream os(filename, std::ios::binary);
		if (os.is_open())
			os.close();
		else
			std::cout << __FUNCTION__ << " : Error. Failed to open file " << filename << std::endl;
	}
	
	template <typename T>
	void append_to_file(T* data, size_t n, const char* filename, bool quiet = true)
	{
		if (!quiet)
			std::cout << __FUNCTION__ << " : " << filename << std::endl;
	
		// Open to append in binary format
		std::ofstream os(filename, std::ios::app | std::ios::binary);
		if (os.is_open())
		{
			os.write(reinterpret_cast<char *>(&data[0]), sizeof(T) * n);
			os.close();
		}
		else
			std::cout << __FUNCTION__ << " : Error. Failed to open file " << filename << std::endl;
	}
	
	template <typename T>
	void append_to_file(std::vector<T> data, const char* filename, bool quiet = true)
	{
		append_to_file(&data[0], data.size(), filename, quiet);
	}
	
    static std::string file2string(const char* filename)
    {
        std::ifstream is(filename);
        if (is.is_open())
        {
            std::stringstream buffer;
            buffer << is.rdbuf();
            return buffer.str(); 
        }
        else
			std::cout << __FUNCTION__ << " : Error. Failed to open file " << filename << std::endl;
		
        return(""); 
    }
	
} // namespace util

#endif	/* FILEIO_HELPER_H */
