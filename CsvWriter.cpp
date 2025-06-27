#include "CsvWriter.h"
#include <cassert>

CsvWriter::CsvWriter(string _path) :
	filePath_(_path),
	ofs_(filePath_)
{
}

CsvWriter::~CsvWriter()
{
}

void CsvWriter::WriteString(string _str, int _col)
{
	
	ofs_ << "\n";
}
