#include "megahal_misc.h"

using namespace std;

long file_size(string filename)
{
	ifstream Fichier(filename.c_str(), ios::in);
    long pos = Fichier.tellg();
    Fichier.seekg(0, std::ios_base::end);
    long size = Fichier.tellg();
    Fichier.seekg(pos, std::ios_base::beg);
    Fichier.close();
    return size;
}
