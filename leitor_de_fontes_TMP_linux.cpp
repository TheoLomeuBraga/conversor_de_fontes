//============================================================================
// Name        : TMP_FONT_READER_LINUX.cpp
// Author      : Theo
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================




#include <iostream>
using namespace std;


#include  <fstream>
#include <sstream>
#include <filesystem>
#include <map>
#include <vector>

#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include <ft2build.h>
#include FT_FREETYPE_H


vector<string> argumentos;

string entrada, saida;
int qualidade, pixel_perfeito;

#define NUM_CARACTERES 128

int main(int argc, char** argv)
{
	cout << "iniciando\n";

	for (int i = 0; i < argc; i++) {
			argumentos.push_back(argv[i]);
		}

		entrada = argumentos[1];
		cout << "local entrada: " << entrada << endl;
		saida = argumentos[2];
		cout << "local saida: " << saida << endl;
		qualidade = stoi(argumentos[3]);
		cout << "local qualidade: " << qualidade << endl;
		pixel_perfeito = stoi(argumentos[4]);
		cout << "local pixel perfeito: " << pixel_perfeito << endl;



	    //carregar
	    FT_Library ft;
	    FT_Face face;
	    if (FT_Init_FreeType(&ft))
	    {
	        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	    }
	    const char* entradacc = entrada.c_str();
	    if (FT_New_Face(ft, entradacc, 0, &face))
	    {
	        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	    }
	    FT_Set_Pixel_Sizes(face, 0, qualidade);
	    if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
	    {
	        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
	    }


	    vector<json>  caracteres;

		//cada letra
	    for (int i = 0; i < NUM_CARACTERES; i++) {

	        FT_Load_Char(face, i, FT_LOAD_RENDER);

	        vector<unsigned int> res = { face->glyph->bitmap.width, face->glyph->bitmap.rows };

	        vector<int> pos_sca = { face->glyph->bitmap_left, face->glyph->bitmap_top, (int)face->glyph->bitmap.width, (int)face->glyph->bitmap.rows };
	        vector<unsigned int> data;
	        for (int i = 0; i < res[0] * res[1]; i++) {
	            data.push_back(face->glyph->bitmap.buffer[i]);
	        }
	        //cout << "a\n";

	        json caractere = {
	            {"char",i},
	            {"resolution",res},
	            {"position_scale",pos_sca},
	            {"adivancement",(float)face->glyph->advance.x / 64},
	            {"data",data},


	        };
	        caracteres.push_back(caractere);

	    }



	    json fonte = {
	        {"pixel_perfect",pixel_perfeito},
	        {"Chars",caracteres},
	        {"quality",qualidade},
	    };




	    ofstream file(saida);

	    string conteudo = fonte.dump();
	    file << conteudo;
	    cout << conteudo << endl;
	    file.close();




	return 0;
}

