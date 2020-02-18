#ifndef GAME_LIB_CSV_Reader_H_
#define GAME_LIB_CSV_Reader_H_

#include <string>
#include <vector>

using std::string;
using std::vector;

class CSVReader
{
public:
	/**
	*	@fn read
	*	@brief �}�b�v�\�����`����CSV�t�@�C����ǂݍ��݁A����2�����z��Ɋi�[����
	*	@param {file_path} CSV�t�@�C���̃p�X�i�v���W�F�N�g����̑��΃p�X�j
	*	@param {map_structure} �}�b�v�����i�[���邽�߂�2�����z��ւ̃|�C���^
	*/
	static void read(string file_path, vector<vector<int>>& map_structure);
};

using std::vector;
/**
*	@fn split
*	@brief �C�ӂ̕�������w�肳�ꂽ��؂蕶���ŕ�������
*	@param {input} �C�ӂ̕�����
*	@param {delimiter} ��؂蕶��
*	@return ��؂蕶���ŕ������ꂽ��������i�[�����ϒ��z��
*/
extern vector<string> split(string& input, char delimiter);

#endif // !GAME_LIB_CSV_Reader_H_
