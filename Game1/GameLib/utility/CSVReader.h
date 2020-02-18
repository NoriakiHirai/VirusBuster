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
	*	@brief マップ構造を定義したCSVファイルを読み込み、情報を2次元配列に格納する
	*	@param {file_path} CSVファイルのパス（プロジェクトからの相対パス）
	*	@param {map_structure} マップ情報を格納するための2次元配列へのポインタ
	*/
	static void read(string file_path, vector<vector<int>>& map_structure);
};

using std::vector;
/**
*	@fn split
*	@brief 任意の文字列を指定された区切り文字で分割する
*	@param {input} 任意の文字列
*	@param {delimiter} 区切り文字
*	@return 区切り文字で分割された文字列を格納した可変長配列
*/
extern vector<string> split(string& input, char delimiter);

#endif // !GAME_LIB_CSV_Reader_H_
