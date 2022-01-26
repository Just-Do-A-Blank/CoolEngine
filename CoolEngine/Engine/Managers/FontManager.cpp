#include "FontManager.h"
#include "Engine/FileIO/FileIO.h"
#include  "Engine/Includes/json.hpp"

using namespace nlohmann;


void FontManager::LoadFonts()
{
	json file = FileIO::LoadJson("Engine/FileIO/FontData.json");
	json font = file["font"];

	json chars = font["char"]["_id"];

}
