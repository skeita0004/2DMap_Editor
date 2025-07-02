#pragma once
#include <string>
#include <vector>

/// <summary>
/// CSV�t�@�C����ǂ݁A�Z�����ƂɃf�[�^��Ԃ��܂�
/// �f�[�^�́Astring�^�Aint�^�Afloat�^�ŕԂ��܂�
/// 3�̉����т̃Z���ŁAVECTOR�^��Ԃ����Ƃ��ł��܂�
/// �͈͊O���w�肵���ꍇ�́Aassert���܂�
/// </summary>
class CsvReader {
public:
	/// <summary>
	/// ���̃N���X�́A�R���X�g���N�^�Ńt�@�C����ǂ݁A
	/// CSV�t�@�C����ǂ�ŁA�l��ێ����܂��B
	/// �R���X�g���N�^�I�����ɂ́A�t�@�C������Ă��܂��B
	/// </summary>
	/// <param name="filename">�t�@�C����</param>
	CsvReader(std::string filename);
	~CsvReader();

	/// <summary>
	/// CSV�̍s�����擾���܂�
	/// </summary>
	/// <returns></returns>
	int GetLines();

	/// <summary>
	/// �w�肵���s�̃J���������擾���܂�
	/// </summary>
	/// <param name="line">�s�ԍ�</param>
	/// <returns>�J������</returns>
	int GetColumns(int line);

	/// <summary>
	/// �w�肵���s�E��̃f�[�^�𕶎���ŕԂ��܂�
	/// </summary>
	/// <param name="line">�s�ԍ�</param>
	/// <param name="column">��ԍ�</param>
	/// <returns>������</returns>
	std::string GetString(int line, int column);

	/// <summary>
	/// �w�肵���s�E��̃f�[�^��int�^�̐��l�ŕԂ��܂�
	/// �f�[�^�������ĂȂ��ꍇ��0��Ԃ��܂�
	/// </summary>
	/// <param name="line">�s�ԍ�</param>
	/// <param name="column">��ԍ�</param>
	/// <returns>���l</returns>
	int GetInt(int line, int column);

	/// <summary>
	/// �w�肵���s�E��̃f�[�^��int�^�̐��l�ŕԂ��܂�
	/// �f�[�^�������ĂȂ��ꍇ��0��Ԃ��܂�
	/// </summary>
	/// <param name="line"></param>
	/// <param name="column"></param>
	/// <param name="line">�s�ԍ�</param>
	/// <param name="column">��ԍ�</param>
	/// <returns>�����l</returns>
	float GetFloat(int line, int column);
private:
	struct LINEREC {
		std::vector<std::string> record;
	};
	std::vector<LINEREC> all;
};
