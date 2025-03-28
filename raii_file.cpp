#include<iostream>
#include<memory>
#include<fstream>

class FileManager{
public:
	FileManager(const std::string& path)
	    :file_( std::make_unique<std::fstream>(path,std::ios::out|std::ios::app)){
	    if(!file_->is_open())throw std::runtime_error("无法创建/打开文件: " + path);
	}

	~FileManager(){
	     if(file_&&file_->is_open()){
		file_->close();
		std::cout<<"文件已自动关闭\n";
	     }
	}

	void write(const std::string& text){
	    if(file_)*file_<<text;
	}

private:
    std::unique_ptr<std::fstream>file_;
};

int main(){
    try{
        std::string file_path="/home/yyz/Cpp-Practice/test.txt";
        std::cout<<"正在尝试创建文件: "<<file_path<<std::endl;
    
	FileManager fm(file_path);
	fm.write("RAII实战成功！\n");
	std::cout<< "文件写入成功！\n";
   }catch(const std::exception& e){
	std::cerr<<"错误："<<e.what()<<std::endl;
        // 检查目录权限
        system("ls -ld /home/yyz/Cpp-Practice");
   }
   return 0;
}

