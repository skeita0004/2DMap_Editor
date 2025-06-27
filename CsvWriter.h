#include <string>
#include <fstream>

using std::string;
using std::ofstream;

class CsvWriter
{
public:
	CsvWriter(string _path);
	~CsvWriter();

	void WriteString(string _str, int _col);
private:
	string filePath_;
	ofstream ofs_;
};