#include <iostream>
#include <fstream>

enum encoder_mode {code, decode};

class encoder 
{
private:
	unsigned char* s;
public:
	encoder (unsigned char const * key, size_t size_of_key);
	~encoder ();
	void mutator(unsigned char const* key, size_t size_of_key);
	void encode(char const* input_file_name, char const* output_file_name, bool mode);

	void KSA(unsigned char const* key, size_t size_of_key);

	void print_s() {
		for (int i = 0; i < 256; ++i) 
		{
			std::cout << (unsigned int)s[i] << " ";
		}
		std::cout << "\n" << std::endl;
	}

};

void swap(unsigned char &a, unsigned char &b) 
{
	unsigned char temp_char = a;
	a = b;
	b = temp_char;
}

void encoder::KSA(unsigned char const* key, size_t size_of_key) 
{
	int i = 0, j = 0;
	if (key == nullptr || size_of_key == 0) 
	{
		throw std::invalid_argument("Null pointer");
	}
	//std::cout << "KSA is run... (key = " << key << ")\n";
	
	// initialaize S-block
	for (i = 0; i < 256; i++) 
	{
		s[i] = i;
	}
	
	// mix S-block
	for (i = 0; i < 256; i++) 
	{
		j = (j + s[i] + key[i % (int)(size_of_key)]) % 256;
		swap(s[i], s[j]);
	}
}

encoder::encoder (unsigned char const* key, size_t size_of_key) 
{
	s = new unsigned char[256];
	if (s == nullptr) 
	{
		throw std::bad_alloc();
	}
	KSA(key, size_of_key);
}

encoder::~encoder () 
{
	delete[] s;
	s = nullptr;
}

void encoder::mutator(unsigned char const* key, size_t size_of_key) 
{
	KSA(key, size_of_key);
}

void encoder::encode(char const* input_file_name, char const* output_file_name, bool mode = code) 
{
	int i = 0, j = 0;
	std::ifstream input_file(input_file_name, std::ios::binary);
	std::ofstream output_file(output_file_name, std::ios::binary);

	if ((!input_file) || (!output_file)) 
	{
		throw std::runtime_error("Open file error");
	}

	char byte;
	while (input_file.read(&byte, sizeof(unsigned char))) 
	{
		i = (i + 1) % 256;
		j = (j + s[i]) % 256;
		swap(s[i], s[j]);
		unsigned char t = (s[i] + s[j]) % 256;
		unsigned char k = s[t];
		char resbyte = (byte ^ k);
		output_file.write(&resbyte, sizeof(unsigned char));
	}

	input_file.close();
	output_file.close();
}

int program_1_main(int argc, char* argv[]) 
{
	unsigned char const kkey_1[] = "helloworld";
	unsigned char const kkey_2[] = "Ya Luchshiy codirovshik!";

	try
	{
		encoder super_codirovshik(kkey_1, 10);
		super_codirovshik.encode("task_1.txt", "task_1_res.txt", code);
		super_codirovshik.mutator(kkey_1, 10);
		super_codirovshik.encode("task_1_res.txt", "task_1_decoded.txt", decode);

		super_codirovshik.mutator(kkey_2, strlen((const char*)kkey_2));
		super_codirovshik.encode("task_1.jpg", "task_1_res.jpg", code);
		super_codirovshik.mutator(kkey_2, strlen((const char*)kkey_2));
		super_codirovshik.encode("task_1_res.jpg", "task_1_decoded.jpg", decode);
	}
	catch (const std::exception&)
	{
		std::cout << "Oh! You have some error... x_x" << std::endl;
	}

    return 0;
}