#pragma once
//#include <Windows.h>
#include <map>
#include <unordered_map>
#include <string>
#include <dwrite.h>
#pragma comment(lib, "Dwrite")

using namespace std;

class FontController {
private:
	IDWriteFactory* writeFactory;
	unordered_map<string, IDWriteTextFormat*> fonts;
public:
	//FontController();
	FontController(IDWriteFactory* f);
	IDWriteFactory* getWriteFactory() const;
	void setWriteFactory(IDWriteFactory* f);

	HRESULT addFont(string key, wstring fn, float fsize);
	HRESULT setTextAlignment(string key, DWRITE_TEXT_ALIGNMENT dta);
	HRESULT setParagraphAlignment(string key, DWRITE_PARAGRAPH_ALIGNMENT dpa);
	IDWriteTextFormat* getFont(string key);
	void release();
};